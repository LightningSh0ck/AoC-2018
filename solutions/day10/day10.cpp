#include <iostream>
#include <fstream>
#include <cstring>
#include <stdlib.h>

#define maxheight 20

using namespace std;

ifstream f("input.txt");

//converts a char array into an integer.
int convertNum(char *array) {
	int num = 0, sign = 1;
	if (*array == '-') {
		sign = -1; 
		array++;
	}
	while (*array != 0) {
		num = num * 10 + *array - '0';
		array++;
	}
	return num * sign;
}

struct point {
	int px, py, vx, vy;
}points[400], p;

//comparison function for the points
int comparPoint(const void *a, const void *b) {
	int comp = ((point*)a)->py - ((point*)b)->py;
	if (comp != 0) return comp;
	return ((point*)a)->px - ((point*)b)->px;
}

int length, timeframe;

int main() {
	char s[51];
	int step, leftedge, rightedge, bottomedge, topedge;
	//reading input
	while (!f.getline(s, 50).eof()) {
		p.px = convertNum(strtok(s + 10, " ,"));
		p.py = convertNum(strtok(NULL, ", >"));
		p.vx = convertNum(strtok(NULL, ",> velocity=<"));
		p.vy = convertNum(strtok(NULL, ", >"));
		points[length] = p;
		length++;
	}
	points[length].px = 10000000;
	points[length].py = 10000000;

	//Did you really expect me to code in an OCR? Hell no, you're on your own!
	cout << "You can also go backwards in time by typing a negative value, typing 0 will stop the program.\n";
	while (true) {
		cout << "You're at time frame " << timeframe << ". Insert step: ";
		cin >> step;
		timeframe += step;
		if (step == 0) break;

		//updating the position of the points
		for (int i = 0; i < length; i++) {
			points[i].px += points[i].vx * step;
			points[i].py += points[i].vy * step;
		}

		//since I don't know how to use fancy output libraries for now, I will sort and output the points in reading order
		qsort(points, length, sizeof(point), comparPoint);
		system("cls");//will differ from OS-es, supposed to clear the screen, but this is optional anyway.

		//recycling step variable to remember the position of the point we are currently looking to print
		step = 0;

		//calculating the boundaries of the area the points sit in
		leftedge = 10000000;
		rightedge = -10000000;
		bottomedge = 10000000;
		topedge = -10000000;
		for (int i = 0; i < length; i++) {
			if (leftedge > points[i].px) leftedge = points[i].px;
			if (rightedge < points[i].px) rightedge = points[i].px;
			if (bottomedge > points[i].py) bottomedge = points[i].py;
			if (topedge < points[i].py) topedge = points[i].py;
		}

		//If the area is too large we print its height as an orientation
		if (topedge - bottomedge > maxheight) {
			cout << "The height of the area is too large to print: " << topedge - bottomedge << '\n';
			continue;
		}

		//Printing the result for you to read.
		for (int i = points[step].py; step < length; i++) {
			//skipping empty spaces. This is overkill but I put this just in-case
			while (points[step].py > i) {
				cout << '\n';
				i++;
			}
			for (int j = leftedge; j <= rightedge; j++) {
				if (points[step].px == j) {
					step++;
					//skipping overlapping points. Nobody told me that points can overlap in the final answer, REEEEEEEEEEEEEEEEEEEEEEEEEEE
					//I had to commit Excel because of that, I'm so sorry C++ gods, please forgive my blasphemious acts.
					while (points[step].px == points[step - 1].px && points[step].py == points[step - 1].py) step++;
					cout << '#';
				}
				else cout << ' ';
			}
			cout << "\n";
		}
	}
	cout << "\n";
	system("pause");
	return 0;
}
