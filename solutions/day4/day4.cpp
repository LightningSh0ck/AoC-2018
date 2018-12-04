#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstring>
#include <map>

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

struct record {
	int month, day, hour, minute, ID;
	//ID is -1 if the record is a sleep state
} records[1100];

//comparison function for records based on date
int comparRecord(const void *a, const void *b) {
	int comp = ((record*)a)->month - ((record*)b)->month;
	if (comp != 0) return comp;

	comp = (((record*)a)->day - ((record*)b)->day);
	if (comp != 0) return comp;

	comp = (((record*)a)->hour - ((record*)b)->hour);
	if (comp != 0) return comp;

	return ((record*)a)->minute - ((record*)b)->minute;
}

//I could have excluded this but initially I didn't have maps in mind
struct guard {
	//how many days they slept at that minute
	int sleepingDays[60];
};

map<int,guard*> guards;

void part2() {
	guard* GUARD;
	int maxSleeperID, mostSleptMinuteDays = 0, mostSleptMinute;
	for (auto G : guards) {
		GUARD = G.second;
		for (int i = 0; i < 60; i++) {
			if (GUARD->sleepingDays[i] > mostSleptMinuteDays) {
				mostSleptMinuteDays = GUARD->sleepingDays[i];
				maxSleeperID = G.first;
				mostSleptMinute = i;
			}
		}
	}
	cout << maxSleeperID * mostSleptMinute;
}

void part1() {
	guard* GUARD;
	int sleepTime, maxSleepTime = 0, maxSleeperID, mostSleptMinute, mostSleptMinuteDays;
	for (auto G : guards) {
		GUARD = G.second;
		sleepTime = 0;
		for (int i = 0; i < 60; i++) {
			sleepTime += GUARD->sleepingDays[i];
		}
		if (sleepTime > maxSleepTime) {
			maxSleepTime = sleepTime;
			maxSleeperID = G.first;
			mostSleptMinuteDays = 0;
			for (int i = 0; i < 60; i++) {
				if (GUARD->sleepingDays[i] > mostSleptMinuteDays) {
					mostSleptMinuteDays = GUARD->sleepingDays[i];
					mostSleptMinute = i;
				}
			}
		}
	}
	cout << maxSleeperID * mostSleptMinute;
}

int main() {
	//This is all dedicated in reading the records and storing guards by ID and activity
	int length = 0;
	char s[101], *p;
	while (!f.getline(s, 100).eof()) {
		//discarding year, it's always 1518
		strtok(s, "[- :Guard]");
		records[length].month = convertNum(strtok(NULL, "[- :Guard]"));
		records[length].day = convertNum(strtok(NULL, "[- :Guard]"));
		records[length].hour = convertNum(strtok(NULL, "[- :Guard]"));
		records[length].minute = convertNum(strtok(NULL, "[- :Guard]"));
		p = strtok(NULL, "[- :Guard]");
		if (*p == '#')
			records[length].ID = convertNum(p + 1);
		else
			records[length].ID = -1;
		length++;
	}
	qsort(records, length, sizeof(record), comparRecord);

	//making the gathered data centered on the guards and their activity
	map<int, guard*>::iterator IT;
	guard* GUARD;
	for (int i = 0; i < length; i++)
	{
		IT = guards.find(records[i].ID);

		//if there is no guard we make one
		if (!guards.count(records[i].ID)) {
			IT = guards.insert(pair<int, guard*>(records[i].ID, new guard)).first;
			for (int j = 0; j < 60; j++) {
				//initialization of the new guard record
				IT->second->sleepingDays[j] = 0;
			}
		}
		GUARD = IT->second;
		int j;

		//iterating one guard's shift
		for (j = i + 1; j < length && records[j].ID == -1; j += 2)
		{
			for (int k = records[j].minute; k < records[j + 1].minute; k++)
			{
				GUARD->sleepingDays[k]++;
			}
		}

		//jumps directly to the next shift
		i = j - 1;
	}

	//edit the code here to execute the demanded part
	part1();
	cout << "\n";
	system("pause");
	return 0;
}
