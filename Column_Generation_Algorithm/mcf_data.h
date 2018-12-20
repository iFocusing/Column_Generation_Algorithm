
/*	***********************************************************************************************************************************
	Include files: Provide access to functions classes constants and macros defined in include files filename.h :
	use <filename> to include a file located in one of the locations specified by the compiler 
	use "filename.h" to include a file located in the same directory of this file 
	*********************************************************************************************************************************** */

	#include <fstream>			// for file manipulation
	#include <iostream>			// for console input output
	#include <string>			// for strings manipulation
	#include <vector>			// for using vectors (arrays with several built in functionalities)

	#pragma once

	/*	***********************************************************************************************************************************
	Define a class to read and store the instance data. We assume that the input file is formatted
	as follows:
	-------------------------------------------------
	name_of_instance  #_of_nodes  #_of_arcs
	tail_node_of_arc_1  head_node_of_arc_1  unit_flow_cost_of_arc_1  capacity_of_arc_1
	tail_node_of_arc_2  head_node_of_arc_2  unit_flow_cost_of_arc_2  capacity_of_arc_2
	...					...					...				   ...
	origin_node_of_commodity_1  destination_node_of_commodity_1  demand_of_commodity_1
	origin_node_of_commodity_2  destination_node_of_commodity_2  demand_of_commodity_2
	...					...					...				   ...
	-------------------------------------------------

	*********************************************************************************************************************************** */

class mcfdata {
public:
	int  nNode;						// the # of nodes
	int  nArcs;						// the # of arcs
	int  nComm;						// the # of commodities
	double bigM;					// a very big number, will be used as cost of the artificial 
									// variables (virtual paths) in the CG algorithm

	std::vector<int> ArcTail;		// the tail vertex of each arc	
	std::vector<int> ArcHead;		// the head vertex of each arc
	std::vector<int> ArcCost;		// the cost of each arc
	std::vector<int> ArcCapa;		// the capacity of each arc
	std::vector<int> CommOri;		// the starting vertex of each commodity
	std::vector<int> CommEnd;		// the terminal vertex of each commodity
	std::vector<int> CommDem;		// the demand of each commodity
	std::vector<int> OutDegr;		// the number of arcs outgoing from each vertex
	std::vector<int> InDegre;		// the number of arcs ingoing into each vertex

	std::string ProbName;			// the name of the instance

									// Constructor function: must have same name as the class
									// Is called automatically to initialize a mcfdata object upon creation
	mcfdata() {
		nNode = 0;
		nArcs = 0;
		nComm = 0;
		bigM = 1000000;
		ProbName = "";

		for (int i = 0; i<nNode; i++) {
			OutDegr[i] = 0;
			InDegre[i] = 0;
		}
	}

	// read: Reads a MMCF instance from file. 
	// In input a string with the location (path) of the instance file
	// NOTE: In the input file nodes are numbered starting from 1
	// Since C / C++ numbers from 0 all node indexes are automatically 
	// decreased by 1 by the read function (node 1 becomes 0, node 2  becomes 1, ...)
	bool read(std::string path) {

		// Create an input stream attached to the input file
		std::ifstream input(path);
		if (input.is_open()) {

			// Read instance name, # of nodes, # of arcs
			input >> ProbName;
			input >> nNode >> nArcs >> nComm;

			// Dimension the vectors 
			ArcTail.resize(nArcs);
			ArcHead.resize(nArcs);
			ArcCost.resize(nArcs);
			ArcCapa.resize(nArcs);

			CommOri.resize(nComm);
			CommEnd.resize(nComm);
			CommDem.resize(nComm);

			OutDegr.resize(nNode);
			InDegre.resize(nNode);

			// Read arc data
			for (int a = 0; a < nArcs; a++) {
				input >> ArcTail[a] >> ArcHead[a] >> ArcCost[a] >> ArcCapa[a];
				ArcTail[a] --;
				ArcHead[a] --;
				OutDegr[ArcTail[a]] ++;
				InDegre[ArcHead[a]] ++;
			}

			// Read commodity data
			for (int k = 0; k < nComm; k++) {
				input >> CommOri[k] >> CommEnd[k] >> CommDem[k];
				CommOri[k] --;
				CommEnd[k] --;
			}

			//  Close the input file
			input.close();
		}
		else { return false; }

		return true;
	}
};


// *****************************************************************************
// CLASS FOR GRAPH REPRESENTATION OF THE MCF NETWORK
// *****************************************************************************

class mcfgraph {
public:
	int n; // number of nodes in the graph 
	int m; // number of arcs in the graph

	// see lectures slides 06_netflows.pdf for a description of these vectors
	std::vector<int> point;
	std::vector<int> head;
	std::vector<double> cost;
	std::vector<int> capa;
	// the next vector will be useful for storing the arc modified costs to be used when solving 
	// the pricing problem within the column generation algorithm
	std::vector<double> modifiedcost;

	// the constructor just sizes the data structures
	// input to the constructor is the # of nodes n and # of arcs m
	mcfgraph(int n, int m) {
		cost.resize(m);
		head.resize(m);
		point.resize(n + 1);
		modifiedcost.resize(m);
		capa.resize(m);
	}

	// function that constructs the forward star representation of the input network 
	// see lectures slides 06_netflows.pdf for a description of the procedure
	void fstar(mcfdata &mcf) {
		std::vector<int> numf(mcf.nArcs);

		n = mcf.nNode;
		m = 0;

		// Initialize the vector point and numf


		// Construct the forward star (fill vectors head, cost, and capa)

	}

	// function that given in input two nodes returns the corresponding arc #  
	// with respect to the forward order. Returns - 1 if no such arc exists
	int get_arc_id(int i, int j) {
        return 0;
	}
};




