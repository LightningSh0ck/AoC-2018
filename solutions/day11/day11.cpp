#include <iostream>
#include <stdlib.h>
//insert input here
#define input 5093

using namespace std;

int cellpower[301][301], maxx, maxy, sum, max = 0xFFFFFFFF/*(minus infinity)*/, maxsize;

void part2(){
	//turning cellpower into an array of partial sums(dynamic programming)
	//cout << cellpower[4][4] + cellpower[4][5] + cellpower[5][4] + cellpower[5][5] << ' ';
	for (int y = 1; y < 300; y++)
		for (int x = 1; x <= 300; x++) {
			cellpower[x][y] += cellpower[x - 1][y];
		}

	//now turning the transpose of cellpower into an array of partial sums of the partial sums. in this case, the code explains better
	for (int y = 1; y < 300; y++)
		for (int x = 1; x <= 300; x++) {
			cellpower[x][y] += cellpower[x][y - 1];
		}

	/*here's the trick, in order to find the sums we calculate using the corners like so: (bottomright - bottomleft - topright + topleft) with the left and top edge actually one position out of the square.
	It's hard to demonstrate why just with text, take a pen and paper, do a 4x4 square with the top and left edges all 0(representing position 0 in our array, remember that first cell is at 1,1) and the rest of the cells random, apply my algorithm and see for yourself.
	For the sake of easying some calculation, the size of 1 will be represented by 0 and so on.*/
	for (int size = 0; size < 300; size++){
		for (int y = 1; y < 300 - size; y++)
			for (int x = 1; x <= 300 - size; x++) {
				//doing this is not worth for up to 3x3 squares but from 4x4+ it is
				sum = cellpower[x + size][y + size] - cellpower[x - 1][y + size] - cellpower[x + size][y - 1] + cellpower[x - 1][y - 1];
				if (sum > max) {
					max = sum;
					maxx = x;
					maxy = y;
					maxsize = size + 1;
				}
			}
	}
	//cout << cellpower[5][5]- cellpower[3][5]- cellpower [5][3] + cellpower[3][3];
	cout << maxx << ',' << maxy << ',' << maxsize;
}

short offsetx[9] = { -1,-1,-1,0,0,0,1,1,1 }, offsety[9] = { -1,0,1,-1,0,1,-1,0,1 };

void part1() {
	for (int y = 2; y < 300; y++)
		for (int x = 2; x < 300; x++) {
			sum = 0;
			for (short i = 0; i < 9; i++)
			{
				sum += cellpower[x + offsetx[i]][y + offsety[i]];
				if (sum > max) {
					max = sum;
					//I did't realize it was asking for the top-left corner until too late
					maxx = x-1;
					maxy = y-1;
				}
			}
		}
	cout << maxx << ',' << maxy;
}

int main() {
	//calculating the power of all cells
	for (int y = 1; y <= 300; y++)
		for (int x = 1; x <= 300; x++) {
			//my bad, the axis should have went the other way: y,x
			//((rackID * y) + input)*rackID / 100 %10 - 5
			cellpower[x][y] = ((x + 10) * y + input)*(x + 10) / 100 % 10 - 5;
		}
	//edit the code here to execute the demanded part
	part2();
	cout << "\n";
	system("pause");
	return 0;
}