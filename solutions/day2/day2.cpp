#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

ifstream f("input.txt");

struct box {
	char label[30];
	int sum = 0;
} boxes[260];

int comparBox(const void *a, const void *b) {
	return ((box*)a)->sum - ((box*)b)->sum;
}

//returns the position of the different letter or -1 if they differ by more letters
int checkDif(char a[], char b[]) {
	int i = 0, pos = -1;
	do {
		if (a[i] != b[i]) {
			if (pos != -1) return -1;
			else pos = i;
		}
		i++;
	} while (a[i] != 0);
	return pos;
}

char result[29];

void part2() {
	int total = 0, pos, i;
	//reads and makes a sum of the letters
	while (!f.getline(boxes[total].label, 30).eof()) {
		for (int i = 0; boxes[total].label[i] != 0; i++) {
			boxes[total].sum += boxes[total].label[i];
		}
		total++;
	}

	//sorts the array of labels based on their sums
	qsort(boxes, total, sizeof(box), comparBox);

	/*the trick is that if the labels differ by one letter the difference in sums is at most 25 but the reciprocal is not true so we still have to check.
	As the array is sorted, it will move on once the difference in sums passes 25.*/
	for (i = 0; i < total-1; i++) {
		for (int j = i+1; j < total && (boxes[j].sum - boxes[i].sum < 26); j++) {
			pos = checkDif(boxes[j].label, boxes[i].label);
			if (pos != -1) break;
		}
		if (pos != -1) break;
	}
	//worst case scenario should still be n^2 but at least it has some optimisation to it
	boxes[i].label[pos] = 0;
	cout << boxes[i].label << (char*)&boxes[i].label[pos + 1];
}

int occurence[260][26];

void part1() {
	int total = 0, nr2 = 0, nr3 = 0;
	char s[30];
	bool found2, found3;
	//reads and makes an array of occurences for each letter in each string
	while (!f.getline(s, 30).eof()) {
		for (int i = 0; s[i] != 0; i++) {
			occurence[total][s[i] - 'a']++;
		}
		total++;
	}
	//iterates and finds using the occurence array
	for (int i = 0; i < total; i++) {
		found2 = found3 = 0;
		for (int j = 0; j < 26; j++) {
			if (occurence[i][j] == 2) found2 = 1;
			else if (occurence[i][j] == 3) found3 = 1;
		}
		nr2 += found2;
		nr3 += found3;
	}
	cout << nr2 * nr3;
}

int main() {
	//edit the code here to execute the demanded part
	part2();
	cout << "\n";
	system("pause");
	return 0;
}