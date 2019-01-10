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
    int nPaths;
	// A variable to store the number of arcs in all paths stored in the pool 
    int nArcs;
    std::vector<int> pa_arcs; // the paths arcs
    std::vector<int> po_path; // the position of each path's first arc
    std::vector<int> co_path; // the paths costs
    std::vector<int> nu_comm; // the number of the paths commodities
	// A variable to store the max number of paths that the pool can have
    int max_nPaths;
	// A variable to store the max number of arcs that the pool can have
    int max_nArcs;

	// Constructor: 
	// takes in input the max number of paths and max number of arcs that the pool 
	// will store to size the data structures
	mcfpaths(int maxpaths, int maxarcs) {
        nPaths = 0;
        nArcs = 0;
        max_nArcs = maxarcs;
        max_nPaths = maxpaths;
        pa_arcs.resize(max_nArcs);
        po_path.resize(max_nPaths);
        co_path.resize(max_nPaths);
        nu_comm.resize(max_nPaths); // this is strange?
	}

	// A function to Initialize the path pool. After calling reset there should be no paths in the pool
	void reset(void) {
        nPaths = 0;
        nArcs = 0;
        pa_arcs.clear();
        po_path.clear();
        co_path.clear();
        nu_comm.clear();
	}

	// A function to add to the pool a shortest path (computed with respect to the true arc costs) for each commodity. 
	// needs in input an object providing heap functions, and a dijkstra object providing dijkstra's algorithm
	void init(mcfdata &mcf, mcfgraph &g, binheap &heap, dijkstra &djk, double &PP_total_time) {
        std::vector<int> path;
        for (int i = 0; i < mcf.nComm; i++){
            add_artificial_paths(mcf, g, i);
        }
        for (int i = 0; i < mcf.nComm; i++){
//            std::cout << "For comm " << i << " finding the shortest path: (" << mcf.CommOri[i] << ", " << mcf.CommEnd[i] << "):" << std::endl;
            path = djk.Dijkheap(mcf.CommOri[i], mcf.CommEnd[i], mcf, g, heap, PP_total_time);
            add_nodepath(g, path, int(path.size()-1), i);
        }
	}

	// A function to add the 'virtual paths' to the path pool (see lecture slides 09_ColGen_II.pdf)
	void add_artificial_paths(mcfdata &mcf, mcfgraph &g, int comm) {
        std::vector<int> path;
        add_nodepath(g, path, 0, comm);
        co_path[nPaths-1] = mcf.bigM;
	}

	//A function that takes in input the forward star g, a path (node sequence including both start & end node)
	// and the commodity for which the path has to be added. Adds the path to the path pool and
	// returns true is no errors occur. Returns false if the path cannot be stored.
	bool add_nodepath(mcfgraph &g, std::vector<int> &nodepath, int pathsize, int comm) {
        if (nPaths > max_nPaths || nArcs + pathsize > max_nArcs){
            return false;
        }
        po_path[nPaths] = nArcs;
        nu_comm[nPaths] = comm;
        co_path[nPaths] = 0;
        for(int i = 0; i < pathsize; i++){
            int arc = g.get_arc_id(nodepath[i], nodepath[i+1]);
            pa_arcs[nArcs] = arc;
            nArcs++;
            co_path[nPaths] += g.cost[arc];
        }
        nPaths++;
        po_path[nPaths] = nArcs;
        return true;
	}
};
