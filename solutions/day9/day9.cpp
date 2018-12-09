#include <iostream>

//no input file this time, it's not worth the hassle, use this:
#define players 17
//no separate parts either, to execute part 2 just append 2 zeros to last
#define last 1104

using namespace std;

//player 1's score will be at position 0!
unsigned int score[players], current;
//creating a double linked list that is looping, a position represents a marble in itself
int after[last+1], before[last+1], next1, next2;

//since these will be initialized with 0, the current marbe is 0, has its right neighbor 0 and its left neighbor 0, which is the initial state
int main() {
	//we iterate each marble
	for (int i = 1; i <= last; i++) {
		if (i % 23) {//the marble's worth is not a multiple of 23

			//after[current] is 1 marble after the current one
			next1 = after[current];
			//after[after[current]] is 2 marbles after the current one
			next2 = after[next1];

			//inserting the new marble in between next1 and next2
			after[next1] = i;
			before[next2] = i;

			//we register the new marble's neighbors as well 
			before[i] = next1;
			after[i] = next2;
			
			//updating the current marble
			current = i;
		} 
		else {//the marble's worth is a multiple of 23

			//we temporarily select the marble 7 marbles counter-clockwise from the current marble as the new current one
			current = before[before[before[before[before[before[before[current]]]]]]]; //there are 7 befores in this, take my word for it, stop counting!

			/*Adding the score.
			Since players go in order and back to 1 we can use the remainder of the step to keep track.
			Because we started numerotating from 1 we substract 1 beforehand. I will remind you that I count the players from 0.*/
			score[(i - 1) % players] += i + current;

			//we remove that marble
			after[before[current]] = after[current];
			before[after[current]] = before[current];

			//we select the next one
			current = after[current];
		}
		next1 = 0;
		do {
			cout << next1 << ' ';
			next1 = after[next1];
		} while (next1 != 0);
		cout << '\n';
	}

	//recycling the "current" variable to store the maximum score. Actually I could have recycled a couple of variables but since I've had complaints about my code readability I toned that down :(
	current = 0;
	for (int i = 0; i < players; i++)
	{
		if (current < score[i]) {
			current = score[i];
		}
	}
	cout << current << "\n";
	//yes, this one is bruteforce as well, but the complexity is linear.
	//I'm pretty sure this can be solved with a recurring series just like day1, but I'm not a pro mathmatician.
	system("pause");
	return 0;
}