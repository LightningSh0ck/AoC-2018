#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

ifstream f("input.txt");

//converts a char array into an unsigned integer. No sign support!
int convertNum(char *array) {
	int num = 0;
	while (*array != 0) {
		num = num * 10 + *array - '0';
		array++;
	}
	return num;
}

//storage struct for sorting
struct freq {
	//we don't actually care much about the changes themselves, only their actual frequency on the spot
	int remainder;
	int frequency;
	int pos;
}frequencies[2000];

//positive remainder operator
int mod(int a, int b) {
	return (a%b + b) % b;
}

//total is the sum of all frequency changes
int total = 0;

//comparison function for the frequencies
int comparFreq(const void *a, const void *b) {
	//the remainder is the 1st priority. in the case the total is 0 this will be skipped(comp will be 0)
	int comp = ((freq*)a)->remainder - ((freq*)b)->remainder;
	if(comp != 0) return comp;
	//the frequency is the 2nd priority, its order will reverse if the total is negative
	else comp = (((freq*)a)->frequency - ((freq*)b)->frequency) * (total < 0 ? -1 : 1);
	if (comp != 0) return comp;
	//the position is the 3rd priority
	return ((freq*)a)->pos - ((freq*)b)->pos;
}

void part2() {
	int length = 1, result, resultpos = 10000000, pos;
	char s[21];
	while (!f.getline(s, 20).eof()) {
		//makes the series of frequencies;
		if (s[0] == '+')
			total += convertNum(s + 1);
		else if (s[0] == '-')
			total -= convertNum(s + 1);
		frequencies[length].frequency = total;
		frequencies[length].pos = length;
		length++;
	}

	//particular case if the total is 0, in this case there is no looping so one value must repeat within 
	//note that for this case the last value which is the total(== 0) in itself must be kept because the first frequency is 0 so 0 can be a solution
	if (!total) {
		//see comparFreq comments. This will make the same values sit right next to each other and in order
		qsort(frequencies, length, sizeof(freq), comparFreq);
		for (int i = 0; i < length-1; i++)
		{
			if (frequencies[i].frequency == frequencies[i + 1].frequency) {
				if (frequencies[i + 1].pos < resultpos) {
					resultpos = frequencies[i + 1].pos;
					result = frequencies[i + 1].frequency;
				}
			}
		}
		cout << result;
		return;
	}

	/*At this point the array is going to be a recurring series with the recurrence relation A(n+length) = A(n) + total.

	The recurrence relation denotes 2 things:

	1.a reoccuring frequency must have the same positive remainder of the division by total
	as their initial frequency from which they "sprouted" from.

	2.to tell which frequency is the "sprout" and which one reoccurs without brute-force we compare them based on the sign of total*/
	
	//excluding the final value, which is the total, as it sits outside of one single loop of the series
	length--;

	//calculating remainder
	for (int i = 0; i < length; i++) {
		frequencies[i].remainder = mod(frequencies[i].frequency, total);
	}

	//see comparFreq comments
	qsort(frequencies, length, sizeof(freq), comparFreq);
	
	/*after sorting, the frequencies that reoccur will be to the right of their "sprout",
	now it's an easy matter of cheching the neighboring values and calculating their position in the recurring series*/
	for (int i = 0; i < length-1; i++)
	{
		if (frequencies[i].remainder == frequencies[i + 1].remainder) {
			pos = (frequencies[i+1].frequency - frequencies[i].frequency) / total * length + frequencies[i].pos;
			if (pos < resultpos) {
				resultpos = pos;
				result = frequencies[i+1].frequency;
			}
		}
	}
	cout << result;

	//excluding sorting, complexity is linear! Cool!
}

void part1() {
	int total = 0;
	char s[21];
	while (!f.getline(s, 20).eof()) {
		//checks the sign(first char) and calls the converter with the remaining array. 
		if (s[0] == '+')
			total += convertNum(s + 1);
		else if (s[0] == '-')
			total -= convertNum(s + 1);
	}
	cout << total;
}

int main() {
	//edit the code here to execute the demanded part
	part2();
	cout << "\n";
	system("pause");
	return 0;
}