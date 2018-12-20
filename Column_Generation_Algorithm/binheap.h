#include <vector>

#pragma once

// *****************************************************************************
// CLASS PROVIDING A 2-HEAP IMPLEMENTATION OF A PRIORITY QUEUE
// See lecture slides shortestpaths.pdf for a pseudocode description of the heap 
// operations implemented below 
// *****************************************************************************

class binheap {
public:
	int nheap;			  // Denoted |H| in the lecture slides: It is the current number of nodes in the heap 
	int maxsize;		  // the maximum # of nodes that the heap can have 
	std::vector<int> key; // Denoted H[] in the lecture slides: key[i] is the element stored in the Heap node i 
	std::vector<int> pos; // Denoted P[] in the lecture slides: pos[i] is Heap node number of element i, 
						  // if element k is inside the heap node i then pos(k) = i
		
	// The Constructor: Just initializes and sizes the local data structs
	binheap(int maxsize) : maxsize(maxsize) {
		nheap = 0;
		maxsize = maxsize;
		key.resize(maxsize);
		pos.resize(maxsize);
	}
	// The function MoveUp(H,P,d,i) described in the lecture slides
	// Here, d[] is called label[]
	int Move_Up(std::vector<double> &label, int i) {
		if (i < 1) return -1;
		int p = (int)((double)(i - 1) * 0.5);
		if (label[key[i]] < label[key[p]]) {
			int tmp = key[i];
			key[i] = key[p];
			key[p] = tmp;
			tmp = pos[key[i]];
			pos[key[i]] = pos[key[p]];
			pos[key[p]] = tmp;
			return p;
		}
		return -1;
	}
	// The function MoveDown(H,P,|H|,d,i) described in the lecture slides
	// Here, d[] is called label[]
	int Move_Down(std::vector<double> &label, int i) {
		if (2 * i + 1 >= nheap) return nheap;
		int inew = i;
		if (label[key[2 * i + 1]] < label[key[inew]]) {
			inew = 2 * i + 1;
		}
		if (2 * i + 2 < nheap) {
			if (label[key[2 * i + 2]] < label[key[inew]]) {
				inew = 2 * i + 2;
			}
		}
		if (inew != i) {
			int tmp = key[i];
			key[i] = key[inew];
			key[inew] = tmp;
			tmp = pos[key[i]];
			pos[key[i]] = pos[key[inew]];
			pos[key[inew]] = tmp;
			return inew;
		}
		return nheap;
	}
	// The function HeapAdd(H,P,|H|,d,k) described in the lecture slides
	// Here, d[] is called label[] 
	void Add(std::vector<double> &label, int k) {
		if (nheap > maxsize) {
			// TO DO: Handle Heap overflow
			// NOW IT JUST WAITS FOR THE USER TO ACKNOWLEDGE AND TERMINATES
			std::cout << " Heap overflow: terminating program \n Press Enter to quit ";
			std::cin.get();
			exit(0);
		}
		nheap = nheap + 1;
		key[nheap - 1] = k;
		pos[k] = nheap - 1;
		int i = nheap - 1;
		while (i > 0) {
			i = Move_Up(label, i);
		}
	}
	// The function ExtractMin(H,P,|H|,d) described in the lecture slides
	// Here, d[] is called label[] 
	int Choose(std::vector<double> &label) {
		if (nheap < 1) return -1;
		int j = key[0];
		pos[j] = -1;
		key[0] = key[nheap - 1];
		pos[key[nheap - 1]] = 0;
		nheap = nheap - 1;
		int i = 0;
		while (i < nheap - 1) {
			i = Move_Down(label, i);
		}
		return j;
	}

	// The function DecreasedLabel(H,P,|H|,d,k) described in the lecture slides
	// Here, d[] is called label[] 
	void Decreased_Label(std::vector<double> &label, int k) {
		int i = pos[k];
		while (i > 0) {
			i = Move_Up(label, i);
		}
	}
	// The function IncreasedLabel(H,P,|H|,d,k) described in the lecture slides
	// Here, d[] is called label[] 
	void Increased_Label(std::vector<double> &label, int k) {
		int i = pos[k];
		while (i < nheap - 1) {
			i = Move_Down(label, i);
		}
	}
};