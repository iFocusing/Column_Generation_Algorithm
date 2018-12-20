/*	***********************************************************************************************************************************
Computational Optimization in Logistics: Project 2
*********************************************************************************************************************************** */

	#pragma warning(disable : 4996)

/*	**********************************************************************************************************************************
	Include the file mcf_data.h which provides the class for reading and storing the instance data. It also includes other files we need
*********************************************************************************************************************************** */

	#include "mcf_data.h"
	#include "pathflow_model.h"
	#include "spp.h"
    #include<iostream>
/*	**********************************************************************************************************************************
	Include CPLEX header files 
*********************************************************************************************************************************** */

	#include "ilcplex/cplex.h"
	#include "ilcplex/ilocplex.h"

/*	***********************************************************************************************************************************
	CG algorithm: solves the path-flow model stored inside the object pathflow. Here I assume that: Dijkstra algorithm is available as  
	a member function of class dijkstra, the pool of paths is stored by the object pathpool, the problem data is stored by the object mcf, 
	the forward star is stored by an object g.  Class binheap provides heap functions
*********************************************************************************************************************************** */

bool cgsolve(mcfdata &mcf, IloEnv &env, IloCplex &solver, pathflowdata &pathflow, mcfgraph &g, mcfpaths &pathpool, binheap &H, dijkstra &djk) {
	
	bool err = false;



































	return err;
}

/*	***********************************************************************************************************************************
	The main function is the entry point of our program
*********************************************************************************************************************************** */
int main() {
    std::cout <<"begin"<<std::endl;































	return 0;

}
