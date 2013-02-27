Conway-s-Game-of-Life-Parallelized
==================================

Conway's genetic laws computed in parallel with OpenMP and OpenCL.
With benchmarks for OpenMP, OpenCL (CPU &amp; GPU) as well as sequential execution.

What is it?
-----------
On the basis of a field with living and dead cells, Conway's genetic laws are applied to each cell and its next generation (state) is calculated.
The rules are:
* Birth: A dead cell with exactly three live neighbors becomes a live cell.
* Survival: A live cell with two or three live neighbors stays alive.
* Death: A live cell with four or more neighbors dies from overpopulation, with one or none neighbors dies from isolation.
* Stable: In all other cases a cell remains unchanged.

The [Moore Neighborhood](http://en.wikipedia.org/wiki/Moore_neighborhood) is used, resulting in 8 neighbours to each cell.
Furthermore, the borders of the field are wrapped-around, so that even the cell at the North-West position of the field has a neighbour North-West to ist and so on.

The application is built to compute fields with huge data and over many generations. Input is read and output written to text files. Example input files are provided.

How to use?
------------
Examples:

| Key             | Action  |
| --------------: | :-------|
| Sequential Computation             | gol.exe --load "random10000_in.gol" --generations 250 --save  "random10000_step250_out.gol" |
| Parallel Computation on CPU with OpenMP      | gol.exe --mode omp --load "random10000_in.gol" --generations 250 --save  "random10000_step250_out_OMP.gol" |
| Parallel Computation on CPU with OpenMP using a specific number of threads (e.g. 4) | gol.exe --mode omp --threads 4 --load "random10000_in.gol" --generations 250 --save "random10000_step250_out_OMP4.gol" |
| Parallel Computation on GPU with OpenCL      | gol.exe --mode --device gpu --load "random10000_in.gol" --generations 250 --save  "random10000_step250_out_OCL_GPU.gol" |
| Parallel Computation on CPU with OpenCL      | gol.exe --mode --device cpu --load "random10000_in.gol" --generations 250 --save  "random10000_step250_out_OCL_CPU.gol" |
| Benchmarking      | add parameter "--measure" |

Requirements
------------
What you need for compiling:
* [Visual C++ or rather Visual Studio 2010 or above](http://www.microsoft.com/visualstudio/eng#products/visual-studio-express-products)
* [NVIDIA CUDA OpenCL SDK](https://developer.nvidia.com/cuda-downloads)
* [Intel SDK for OpenCL Applications 2012](http://software.intel.com/en-us/vcsource/tools/opencl-sdk)

How to compile?
------------
1. Install the NVIDIA und Intel OpenCL SDKs
2. Get the source code and open the project in Visual Studio
3. In the menu bar, click on VIEW and open the PROPERTY MANAGER
4. In the Property Manager Window, expand the tree and double click "Microsoft.CPP.Win32.user" under "Debug | Win32"
5. You will see the Property Pages Window. Click VC++ Directories
6. Edit the Include Directories and add the include directory of your CUDA SDK, e.g. "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v5.0\include\CL".
7. Also add the include directory of your Intel OpenCL SDK, e.g. "C:\Program Files (x86)\Intel\OpenCL SDK\2.0\include". 
8. Click okay and edit the Library Directories. Add the Lib Directories of your CUDA SDK, e.g. "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v5.0\lib\Win32".
9. Add the include directory from step 7 here and also the lib directory of your Intel OpenCL SDK, e.g. "C:\Program Files (x86)\Intel\OpenCL SDK\2.0\lib\x86". 
10. Click okay. Switch from VC++ Directories to Linker and expand it. Select the Input node and add "OpenCL.lib" to the Additional Dependencies.  
11. Close the Property Pages Window and repeat steps 4-9 but with "Microsoft.CPP.Win32.user" under "Release | Win32"
12. Hit Build :)

Where to get help?
------------
* Check the docs directory for documentation
* Have a look at the [project's website](http://www.pertiller.net/projects/)
* Contact the author at david@pertiller.net

License
------------
(The MIT License)

Copyright (c) 2012 [David Pertiller](http://www.pertiller.net)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.