#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cmath>
#include <cstring>
//32 is the distance between letter cases
#define canReact(a, b) (abs((a)-(b))==32)

using namespace std;

ifstream f("input.txt");
char s[1000000];

int collapse(char* s) {
	int length = strlen(s);
	//TODO: optimise!
	for (int i = 0; i < length-1; i++) {
		if (canReact(s[i], s[i + 1])) {
			strcpy(&s[i], &s[i + 2]);
			length-=2;

			//UNSAFE TO DO SO!
			i-=2;
		}
	}
	return length;
}
//stack overflow if I don't put those here
char a[1000000], cpy[1000000], *p;
void part2() {
	char c[4];
	int minLength = 1000000, length;
	for (char i = 'a'; i <= 'z'; i++) {
		c[0] = i;
		c[1] = (i - 32);
		c[2] = 0;
		
		strcpy(cpy, s);
		strcpy(a, strtok(cpy, c));
		while ((p=strtok(NULL, c)) != NULL) {
			strcat(a, p);
		}
		length = collapse(a);
		if (length < minLength)
			minLength = length;
	}
	cout << minLength;
}

void part1() {
	cout << collapse(s);
}

int main() {
	f >> s;
	//edit the code here to execute the demanded part
	part2();
	cout << "\n";
	system("pause");
	return 0;
}
