MSTRINGIFY(
kernel void Gol_All(
			    global const int *data,
			    global int *swapData,
				int width,
				int height
			   )
{
	// get index into global data array
	const int x = get_global_id(0);

	int top = x-width;
	int bottom = x+width;
	int left = -1; //in the inner ring, we can be sure that the index isn't out of bounds without checking the borders
	int right = +1; //start position of the right neighbour

	if (x % width == 0)
		left += width;

	else if (x % width == (width - 1))
		right -= width;

	if (top < 0) //row 0
		top += width*height;

	else if (bottom >= (height * width))
		bottom -= width*height;

	int alive = data[x+left]
			  + data[x+right]
			  + data[top+left]
			  + data[top]
			  + data[top+right]
			  + data[bottom+left]
			  + data[bottom]
			  + data[bottom+right];
	
	//calculate next state according to the amount of alive cells in the neighbourhood
	if ((alive == 3) || //a cell with 3 neigbours becomes/stays alive
		(alive == 2 && data[x] == 1)) //a live cell with 2 (or 3) neighbours stays alive (with 3 neighbours it already got alive in the statement before)
			swapData[x] = 1;
	else								
		swapData[x] = 0;	//dies from overpopulation or isolation, or remains dead if it already was
};
);