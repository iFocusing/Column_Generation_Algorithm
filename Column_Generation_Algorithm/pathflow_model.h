#include "ilcplex/cplex.h"
#include "ilcplex/ilocplex.h"
#include "mcf_data.h"
#include "pathpool.h"

#pragma once

/*	***********************************************************************************************************************************
Define a class to store the data of the path-flow model (see also the similar code in CPLEX Tutorial for the arc-flow model)
*********************************************************************************************************************************** */

class pathflowdata {
public:

	// CPLEX model object

	// IloRangeArray arrays to store the capacity and demand constraint objects of the model 

	// IloNumVarArray Arrays to store the variable objects of the model 

	// IloObjective object to model obj function of the model

	// variables to store objective value and solution time


	/*	***********************************************************************************************************************************
	Constructor of pathflowdata 
	*********************************************************************************************************************************** */
	pathflowdata(const IloEnv env) {

	}

	/*	***********************************************************************************************************************************
	A function that creates and adds to the model all the the (empty) constraints of the path-flow model
	*********************************************************************************************************************************** */
	void add_constraints(mcfdata &mcf, mcfgraph &g, IloEnv &env) {


	}

	/*	***********************************************************************************************************************************
	A function that Takes in input two integers first and last and adds to the model the variables corresponding to the paths 
	numbered first, first+1, ..., last that are memorized in the path pool
	*********************************************************************************************************************************** */
	void add_paths(mcfdata &mcf, IloEnv &env, mcfpaths &pathpool, int first, int last) {

	}
};
