#include "mcf_data.h"
#include "binheap.h"
#include "spp.h"

#pragma once

// **********************************************************************************************************************************************************
// A class that implements the path pool. Paths are represented as a sequence of ARCS and each arc is identified using its position in the forward ordering 
// that is, the arc id is the arc # in the forward star representation. All paths are stored contiguously in a vector pathseq[]. A vector pathini[p]  
// indicates the position in the vector pathseq[] of the first arc of path p (see lecture slides 09_ColGen_II.pdf)
// **********************************************************************************************************************************************************


class mcfpaths {					
public:
	// A variable to store the number of paths in the pool 

	// A variable to store the number of arcs in all paths stored in the pool 

	// vectors storing the paths arcs, the position of each path's first arc, the paths costs, the paths commodities 

	// A variable to store the max number of paths that the pool can have

	// A variable to store the max number of arcs that the pool can have


	// Constructor: 
	// takes in input the max number of paths and max number of arcs that the pool 
	// will store to size the data structures
	mcfpaths(int maxpaths, int maxarcs) {

	}

	// A function to Initialize the path pool. After calling reset there should be no paths in the pool
	void reset(void) {

	}

	// A function to add to the pool a shortest path (computed with respect to the true arc costs) for each commodity. 
	// needs in input an object providing heap functions, and a dijkstra object providing dijkstra's algorithm 
	void init(mcfdata &mcf, mcfgraph &g, binheap &heap, dijkstra &djk) {

	}

	// A function to add the 'virtual paths' to the path pool (see lecture slides 09_ColGen_II.pdf)
	void add_artificial_paths(mcfdata &mcf) {

	}

	//A function that takes in input the forward star g, a path (node sequence including both start & end node)
	// and the commodity for which the path has to be added. Adds the path to the path pool and
	// returns false is no errors occur. Returns true if the path cannot be stored.
	bool add_nodepath(mcfgraph &g, std::vector<int> &nodepath, int pathsize, int comm) {
        return false;
	}
};
