/* Exercise: [MGS] EPR: Game Of Life; with focus on performance
* Author: David Pertiller
* Date: 21.12.2012
* Version: 2, added OpenMP and OpenCL
*/

// OpenCL includes
//#include <CL/cl.h>
#include <vector>
#include "Change.hpp"
#define __CL_ENABLE_EXCEPTIONS
#include "cl.hpp"

#define MSTRINGIFY(A) #A
char* stringifiedSourceCL = 
#include "KernelOpenCL.cl"

using namespace std;

//the computation method will iterate over the data for the given number of generations
static inline void ComputeCL(vector<vector<int>> &dataVec2, const int generations, const int &width, const int height, unsigned __int64 deviceType, int platformId = -1, int deviceID = -1)
{	
	const int ELEMENTS = (width*height);   // elements in each vector
	size_t datasize = sizeof(cl_int)*ELEMENTS;

	vector<cl_int> data(ELEMENTS);

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			data[y*width+x] = dataVec2[y][x] ;
		}
	}

	//pre-calculate field indexes
	static const cl_int lastRow = height - 1; //this avoids the computing expensive modulo which should only be called if necessary
	static const cl_int secondLastRow = lastRow - 1; //this would crash if the height of the board would be below 2, but than GOF would not be possible anyway ;)
	static const cl_int lastColumn = width - 1;
	static const cl_int secondLastColumn = lastColumn - 1;


	//calculate generation and use result for calculation of next generation and so on
	
	try {
		// Get list of OpenCL platforms.
		vector<cl::Platform> platform;
		cl::Platform::get(&platform);

		//fulfilling additional command line parameters of exercise 2
		if (platformId > -1 && platformId < platform.size())
		{
			cl::Platform userSpecifiedPlatform = platform[platformId];
			platform.clear();
			platform.push_back(userSpecifiedPlatform);
		}

		if (platform.empty()) {
			cerr << "OpenCL platforms not found." << endl;
			exit(-1);
		}

		// Get first available GPU device which supports double precision.
		cl::Context context;
		vector<cl::Device> device;
		for(auto p = platform.begin(); device.empty() && p != platform.end(); p++) {
			vector<cl::Device> pldev;

			try {
				p->getDevices(deviceType, &pldev);
				
				for(auto d = pldev.begin(); device.empty() && d != pldev.end(); d++) {
					if (!d->getInfo<CL_DEVICE_AVAILABLE>()) continue;
					
					device.push_back(*d);
					context = cl::Context(device);
				}
			} catch(...) {
				device.clear();
			}
		}

		if (device.empty()) {
			cerr << "No devices or no device with specified type found!" << endl;
			exit(-2);
		}

		//cout << device[0].getInfo<CL_DEVICE_NAME>() << endl;

		//std::ifstream programFile("KernelOpenCL.cl");
		std::string programString(stringifiedSourceCL);
		cl::Program::Sources source(1, std::make_pair(programString.c_str(), programString.length()+1));
		cl::Program program(context, source);

		try 
		{
			program.build(device);

		} catch (const cl::Error&) 
		{
			cerr << "OpenCL compilation error" << endl << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device[0]) << endl;
			exit(-3);
		}

		cl::Kernel gol(program, "Gol_All");
		//cl::Kernel gol(program, "Gol_Inner");


		// Allocate device buffers and transfer input data to device.

		//cl::Buffer Params(context, CL_MEM_READ_ONLY, sizeof(Info), &info);
		//cl::Buffer Params(context, CL_MEM_READ_ONLY, sizeof(int), (void*)&width);

		cl::Buffer SwapBuffer(context, CL_MEM_WRITE_ONLY, datasize, NULL);

		//write the params struct to GPU memory as a buffer
		//queue.enqueueWriteBuffer(params, CL_TRUE, 0, sizeof(Info), &params);


		// Set kernel parameters.
		cl_int clWidth = width;
		cl_int clHeight = height;

		gol.setArg(1, SwapBuffer);
		gol.setArg(2, clWidth);
		gol.setArg(3, clHeight);

		// Create command queue.
		cl::CommandQueue queue(context, device[0]);
		//queue.enqueueTask(add);
		//queue.enqueueWriteBuffer(Data, CL_TRUE, 0, datasize, data.data());
		//queue.enqueueReadBuffer(SwapData, CL_TRUE, 0, datasize, swapData.data());
		// Launch kernel on the compute device.

		cl::Buffer Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, datasize, data.data());
		gol.setArg(0, Buffer);


		queue.enqueueWriteBuffer(Buffer, CL_TRUE, 0, datasize, data.data());

		//till N-1 just copy output to input buffer (inexpensive way to read buffer during iterations)
		for (int gen = 0; gen < generations-1; ++gen)
		{
			//cl::Buffer Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, datasize, data.data());
			
			queue.enqueueNDRangeKernel(gol, cl::NullRange, ELEMENTS, cl::NullRange);
			// Get result back to host.
				queue.enqueueCopyBuffer(SwapBuffer, Buffer, 0, 0, datasize);
		}
		
		//in the last iteration, we read the buffer back to data
		queue.enqueueNDRangeKernel(gol, cl::NullRange, ELEMENTS, cl::NullRange);
		queue.enqueueReadBuffer(SwapBuffer, CL_TRUE, 0, datasize, data.data());

		for (int row = 0; row < height; ++row)
		{
			for (int col = 0; col < width; ++col)
			{
				dataVec2[row][col] = data[row*width+col];
			}
		}
	}
	catch (const cl::Error &err)
	{
		cerr << "OpenCL error: " << err.what() << "(" << err.err() << ")" << endl;
		exit(-4);
	}
}
