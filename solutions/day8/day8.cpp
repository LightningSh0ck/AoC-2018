#include <iostream>
#include <fstream>

using namespace std;

ifstream f("input.txt");

//a struct to store a node that also stores its child nodes;
struct Node {
	int children, metadatas, *metadata, value = 0;
	Node *child;
	Node(){}
	Node(int children, int metadatas) {
		this->children = children;
		this->metadatas = metadatas;
		child = new Node[children];
		metadata = new int[metadatas];
	}
};

int metadatasum;

Node constructNode() {
	int children, metadatas;
	f >> children >> metadatas;
	Node node = Node(children, metadatas);

	for (int i = 0; i < children; i++) {
		//using recursion *taps head*
		node.child[i] = constructNode();
	}

	//now that all children have been iterated, we are left with the metadata
	for (int i = 0; i < metadatas; i++) {
		f >> node.metadata[i];
		node.value += node.metadata[i];
	}
	//output for part 1
	metadatasum += node.value;

	//if this node does have children we calculate this node's value accordingly
	if (children) {
		node.value = 0;
		//yes I could have compressed this into the for loop above but at this point it was microoptimizing
		for (int i = 0; i < metadatas; i++){
			if (node.metadata[i] > 0 && node.metadata[i] <= children) {
				node.value += node.child[node.metadata[i] - 1].value;
			}
		}
	}
	return node;
}

void part2() {
	//we simply output the value of the first node
	cout << constructNode().value;
}

void part1() {
	constructNode();
	//our function will calculate the sum because it's declared globally
	cout << metadatasum;
}

int main() {
	//edit the code here to execute the demanded part
	part2();
	cout << "\n";
	system("pause");
	return 0;
}