#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstring>
#include <unordered_set>

using namespace std;

ifstream f("input.txt");

int convertNum(char *array) {
	int num = 0;
	while (*array != 0) {
		num = num * 10 + *array - '0';
		array++;
	}
	return num;
}

unordered_set<int> candidates;
short M[1000][1000];

void part2() {
	int id, x, y, width, height;
	bool noOverlap;
	char s[51];
	while (!f.getline(s, 50).eof()) {
		id = convertNum(strtok(s, "# @,:x"));
		x = convertNum(strtok(NULL, "# @,:x"));
		y = convertNum(strtok(NULL, "# @,:x"));
		width = convertNum(strtok(NULL, "# @,:x"));
		height = convertNum(strtok(NULL, "# @,:x"));
		noOverlap = 1;
		for (int i = y; i < y + height; i++) {
			for (int j = x; j < x + width; j++)
			{
				//if we find that a claim overlaps we mark that and we also remove the overlapped claim from the candidates if it is one
				if (M[i][j]) {
					noOverlap = 0;
					candidates.erase(M[i][j]);
				}
				else {
					M[i][j] = id;
				}
			}
		}
		//if we didn't overlap so far we put this as a candidate
		if (noOverlap)
			candidates.insert(id);
	}
	//the only candidate standing is the solution
	cout << *candidates.begin();
}

void part1() {
	int id, x, y, width, height, nr = 0;
	char s[51];
	while (!f.getline(s, 50).eof()) {
		id = convertNum(strtok(s, "# @,:x"));
		x = convertNum(strtok(NULL, "# @,:x"));
		y = convertNum(strtok(NULL, "# @,:x"));
		width = convertNum(strtok(NULL, "# @,:x"));
		height = convertNum(strtok(NULL, "# @,:x"));
		//simply using a table to actually draw the spots.
		for (int i = y; i < y + height; i++) {
			for (int j = x; j < x + width; j++) {	
				//any spot that wasn't touched before is marked by 0 and we mark it by
				if(!M[i][j])M[i][j] = 1;
				//any spot that has been touched only once is marked by 1 and we mark it by 2. This is so we don't count overlaps multiple times
				else if (M[i][j] == 1) {
					M[i][j] = 2;
					nr++;
				}
				
			}
		}
	}
	cout << nr;
}

int main() {
	//edit the code here to execute the demanded part
	part2();
	cout << "\n";
	system("pause");
	return 0;
}