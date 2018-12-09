#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>
//32 is the distance between letter cases
#define canReact(a, b) (abs((a)-(b))==32)

using namespace std;

ifstream f("input.txt");
char s[1000000];

//does all reactions within the received string and returns its length
int collapse(char* s) {
	int length = strlen(s);
	//this could have been optimized by making a linked list, but sorry, not today ;_;
	for (int i = 0; i < length-1; i++) {
		if (canReact(s[i], s[i + 1])) {
			strcpy(&s[i], &s[i + 2]);
			length-=2;
			i-=2;
			if (i < -1) i = -1;
		}
	}
	return length;
}

//stack overflow if I don't put those here
char a[1000000], cpy[1000000], *p;
void part2() {
	char c[4];
	c[2] = 0;
	int minLength = 1000000, length;
	for (char i = 'a'; i <= 'z'; i++) {
		//constructing string for strtok. looks like "iI".
		c[0] = i;
		c[1] = (i - 32);
		
		//removing one type
		strcpy(cpy, s);
		strcpy(a, strtok(cpy, c));
		while ((p=strtok(NULL, c)) != NULL) {
			strcat(a, p);
		}

		//calculating the length and recording the minimum
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
