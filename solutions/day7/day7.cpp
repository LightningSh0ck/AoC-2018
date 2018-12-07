#include <iostream>
#include <fstream>
#include <cstring>
#include <queue>
#define numworkers 5
#define seconds 61
#define letters 26

using namespace std;

ifstream f("input.txt");

//adjacency matrix
bool G[letters][letters];

//A priority queue fits this scenario the best, as the earlier steps have priority
priority_queue<short, vector<short>, greater<short>> prqueue;

//indegree of nodes, which indicates how many steps have to be completed in order to unlock the coresponding step. We use this so we don't have to iterate the adjacency
short Dependencies[letters], length;
char dependency, dependent, out[27];

void part1() {
	char s[51];
	//treating the input as edges of a directed graph.
	while (!f.getline(s, 50).eof()) {
		strtok(s, " ");
		//substracting 'A' as we address by numbers, starting form 0=A ; 1=B and so on
		dependency = *strtok(NULL, " ");
		dependency -= 'A';

		strtok(NULL, "p");

		dependent = *strtok(NULL, " ");
		dependent -= 'A';

		//constructing graph
		G[dependency][dependent] = 1;
		Dependencies[dependent]++;
	}

	//Initializing the queue with all unlocked steps(that have no dependencies)
	for (short i = 0; i < letters; i++)
	{
		if (!Dependencies[i]) {
			prqueue.push(i);
		}
	}

	//when the queue is empty, it is done
	while (!prqueue.empty()) {
		dependency = prqueue.top();
		prqueue.pop();
		//appending the step
		out[length] = dependency + 'A';
		length++;
		//as the step is done we iterate over all dependents and decrementing their number of dependencies
		for (short i = 0; i < letters; i++)
		{
			if (G[dependency][i]) {
				Dependencies[i]--;
				//when dependencies reach 0, it means the step has been unlocked so we push it to the queue
				if (!Dependencies[i]) {
					prqueue.push(i);
				}
			}
		}
	}
	cout << out;
}

struct worker {
	short time;

	//-1 indicates that the worker is idling
	short step;
}workers[numworkers];

int time, mintime;
bool ok;

void part2() {
	//a lot of code here is borrowed and adapted from part1, make sure you read that first
	char s[51];
	while (!f.getline(s, 50).eof()) {
		strtok(s, " ");
		dependency = *strtok(NULL, " ");
		dependency -= 'A';
		strtok(NULL, "p");
		dependent = *strtok(NULL, " ");
		dependent -= 'A';
		G[dependency][dependent] = 1;
		Dependencies[dependent]++;
	}

	for (short i = 0; i < letters; i++)
	{
		if (!Dependencies[i]) {
			prqueue.push(i);
		}
	}

	//initialization for workers
	for (short i = 0; i < numworkers; i++) {
		workers[i].step = -1;
	}

	//uses a break statement
	while (true) {
		for (short i = 0; i < numworkers ; i++){
			if (!workers[i].time) {//worker is done or idling
				if (workers[i].step != -1) {//did the worker finish a step?
					for (short j = 0; j < letters; j++) {//if so, we mark the step done as we did in part 1
						if (G[workers[i].step][j]) {
							Dependencies[j]--;
							if (!Dependencies[j]) {
								prqueue.push(j);
							}
						}
					}
				}

				//assigning a new step if any is available
				if (!prqueue.empty()) {
					dependency = prqueue.top();
					prqueue.pop();
					workers[i].step = dependency;
					workers[i].time = seconds + dependency;
					//if the queue is still not empty we iterate the workers again so we make sure they all work NOTE: the order of assigning doesn't matter as long as we assign simultaneously(same time frame)
					if (!prqueue.empty()) {
						i = -1;
						continue;
					}
				} 
				else //no available step => idling worker
					workers[i].step = -1;
			}
		}
		ok = 1;
		//determining the moment a worker finishes a task(when they are going to change state)
		//also making sure at the same time that workers are still working
		mintime = seconds + letters;
		for (short i = 0; i < numworkers; i++) {
			if (workers[i].step != -1) {
				ok = 0;
				if (mintime > workers[i].time)
					mintime = workers[i].time;
			}
		}
		//if all workers are done it means it is over
		if (ok) break;
		//if not, we add this time interval to the total and substract it from the workers
		time += mintime;
		for (short i = 0; i < numworkers; i++) {
			if (workers[i].step != -1) {
				workers[i].time -= mintime;
			}
		}
	}
	cout << time;
}

int main() {
	//edit the code here to execute the demanded part
	part2();
	cout << "\n";
	system("pause");
	return 0;
}
