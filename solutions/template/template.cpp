#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

ifstream f("input.txt");

void part1() {
	//just a template to start all my source files with.
	char s[21];
	while (!f.getline(s, 20).eof()) {
	
	}
	cout << "\n";
}

int main() {
	//edit the code here to execute the demanded part
	part1();
	cout << "\n";
	system("pause");
	return 0;
}

//binary search
bool binSearch(int *array, int length, int query) {
	if (length == 0) return false;
	length /= 2;
	if (*(array + length) < query) return binSearch(array + length + 1, length, query);
	if (*(array + length) > query) return binSearch(array, length, query);
	return true;
}

//converts a char array into an unsigned integer. No sign support!
int convertNum(char *array) {
	int num = 0;
	while (*array != 0) {
		num = num * 10 + *array - '0';
		array++;
	}
	return num;
}
