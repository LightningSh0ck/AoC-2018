#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstring>
#include <queue>
#include <cmath>

#define size 400
#define maxpoints 60
#define maxdist 10000

using namespace std;

ifstream f("input.txt");

//used to remember the origin of the wave in flood fill
struct point {
	//ID and distance to the location that claims this point, ID of 0 means it is not claimed yet and -1 means that multiple locations claim this
	short ID,dist;
	point(short ID, short dist) {
		this->ID = ID;
		this->dist = dist;
	}
	point() {};
}A[size][size];

//just in-time learned about typedef today at my college 
typedef pair<int, int> coord;
typedef struct point point;
typedef pair<coord, point> element;

short length;

//offsets for floodfills
short offx[4] = { -1, 0, 1, 0 }, offy[4] = { 0, -1, 0, 1 };

//since I am in a hurry I am using a boolean matrix to make sure I don't iterate over the same spot.
//however this comes with the limitation that if a region is not within bounds, the computer running this will burst into flames
bool B[size][size];

//converts a char array into an unsigned integer. No sign support!
int convertNum(char *array) {
	int num = 0;
	while (*array != 0) {
		num = num * 10 + *array - '0';
		array++;
	}
	return num;
}

//beyond self-explanatory
int compar(const void *a, const void *b) {
	return *(int*)a - *(int*)b;
}

//binary search that returns a value between how many elements are below the query and how many elements are below or equal(this is all we care about, see calcDist)
int search(int arr[], int l, int r, int query)
{
	if (r > l) {
		//simplified from l + (r - l + 1) / 2 ;that 1 is to make it round up
		int mid = (r + l + 1) / 2;
		if (arr[mid] == query)
			return mid;//since we found a value equal to the query, this satisfies our demand
		if (arr[mid] > query)
			return search(arr, l, mid - 1, query);
		return search(arr, mid + 1, r, query);
	}
	//at this point r and l are the same
	return l + (arr[l] < query);
}

int search(int arr[], int query) {
	return search(arr, 0, length - 1, query);
}

//calculates the sum of the distances between a location and all coords
int calcDist(int p[], int partialsum[], int query) {
	int pos = search(p, query);

	//using the search function we calculate the sum of all abs(location - coord) of every spot for one axis.
	//essentially flips the substraction for the coordinates that are greater than our query

	if (pos == 0) return partialsum[length-1] - length * query;
	//equivalent of (pos * query - partialsum[pos - 1]) + (partialsum[length - 1] - partialsum[pos - 1] - (length - pos)*query);
	return (2*pos-length) * query - 2*partialsum[pos - 1] + partialsum[length - 1];
}

queue<coord> fil2;
int px[maxpoints], py[maxpoints], partSumx[maxpoints], partSumy[maxpoints], avgx, avgy, regionSize = 1;
void part2() {
	char s[21];
	//since distances are taxicab, we can separate the calculation for each coordinate and simply forget about order
	while (!f.getline(s, 20).eof()) {
		px[length] = convertNum(strtok(s, ", "));
		py[length] = convertNum(strtok(NULL, ", "));
		length++;
	}
	
	//the position of the coordinates is not relevant to us, therefore sorting is an option;
	qsort(px, length, sizeof(int), compar);
	qsort(py, length, sizeof(int), compar);
	
	//partial sum arrays(dynamic programming)
	partSumx[0] = px[0];
	partSumy[0] = py[0];
	for (int i = 1; i < length; i++) {
		partSumx[i] = partSumx[i - 1] + px[i];
		partSumy[i] = partSumy[i - 1] + py[i];
	}
	
	//preparing for a flood fill that starts from the average coordinates as they are always within the area if it exists
	avgx = round((float)partSumx[length - 1] / (float)length);
	avgy = round((float)partSumy[length - 1] / (float)length);
	B[avgx][avgy] = 1;
	fil2.push(coord(avgx,avgy));
	int x, y;
	coord *p;
	//it is mathmatically obvious that the area we are looking for is not only unintrerrupted, but convex as well. flood-fill is an option
	while (!fil2.empty()) {
		p = &fil2.front();
		//iterating each cardinal direction
		for (short i = 0; i < 4; i++) {
			x = p->first + offx[i];
			y = p->second + offy[i];
			//simply bordering the fill by distance
			if (!B[x][y] && calcDist(px, partSumx, x) + calcDist(py, partSumy, y) < maxdist){
				B[x][y] = 1;
				fil2.push(coord(x, y));
				regionSize++;
			}
		}
		fil2.pop();
	}
	cout << regionSize;
	//while this implementation is quite clever, I feel like this can be done in a much better fashion, but this is all I can do, this took my whole free time for this day...
}


//to mark point IDs that have infinite area
bool infinite[maxpoints];
//to remember the area of each ID, the infinite ones will be ignored
short area[maxpoints];
queue<element> fil;
void part1() {
	char s[21], *a;
	coord coordinate;
	point aPoint;

	//we read the points and put them in a queue
	while (!f.getline(s, 20).eof()) {
		length++;
		//for some reason I can't call strtok twice in one line...
		a = strtok(s, ", ");
		coordinate = coord(convertNum(a), convertNum(strtok(NULL, ", ")));
		aPoint = point(length, 0);
		fil.push(element(coordinate,aPoint));
		A[coordinate.first][coordinate.second] = aPoint;
	}

	//flood fill
	element *e;
	coord *c;
	point *p;
	short x, y;
	while (!fil.empty()) {
		e = &fil.front();
		c = &e->first;
		p = &e->second;
		//iterating each cardinal direction
		for (short i = 0; i < 4; i++) {
			x = c->first + offx[i];
			y = c->second + offy[i];
			//going out of bounds => area is infinite
			if (x < 0 || y < 0 || x >= size || y >= size) {
				infinite[p->ID] = 1;
				continue; 
			}
			if (!A[x][y].ID) {//is this coord unclaimed?
				A[x][y].ID = p->ID;
				A[x][y].dist = p->dist + 1;
				area[p->ID]++;
				fil.push(element(coord(x, y), point(p->ID, p->dist + 1)));
			}
			/*NOTE: when floodfill encounters an equidistant point it will stop and it won't find more nearby equidistant points, however this will not influence the final result
			as we look for an enclosed and convex area which will never have such points, you can take a look at what I mean by uncommenting the debug and slapping the test, use <10 coords*/
			else if (A[x][y].ID != p->ID && A[x][y].dist == p->dist + 1) {
				area[A[x][y].ID]--;
				A[x][y].ID = -1;
			}
		}
		fil.pop();
	}
	short max = 0;
	//prints the matrix for debug
	/*for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (A[j][i].ID == -1) cout << '.';
			else cout << A[j][i].ID;
		}
		cout << '\n';
	}
	cout << '\n'*/

	//finding the max finite area
	for (short i = 1; i <= length; i++){
		if (!infinite[i] && max < area[i]) max = area[i];
	}
	//plus 1 because we did not count the origin before!
	cout << max + 1;
	//yes, this one is brute-force, problem?
}

int main() {
	//edit the code here to execute the demanded part
	part2();
	cout << "\n";
	system("pause");
	return 0;
}