#include "ilcplex/cplex.h"
#include "ilcplex/ilocplex.h"
#include "mcf_data.h"
#include "pathpool.h"
#include <vector>

#pragma once

/*	***********************************************************************************************************************************
Define a class to store the data of the path-flow model (see also the similar code in CPLEX Tutorial for the arc-flow model)
*********************************************************************************************************************************** */

class pathflowdata {
public:

	// CPLEX model object
    IloModel model;
	// IloRangeArray arrays to store the capacity and demand constraint objects of the model
    IloRangeArray CapacityConstrains;
    IloRangeArray DemandConstrains;
	// IloNumVarArray Arrays to store the variable objects of the model 
    IloNumVarArray PFlowVariables;
	// IloObjective object to model obj function of the model
    IloObjective obj;
	// variables to store objective value and solution time
    double bestobj;
    double soltime;
    
    // CPLEX array to store the dual vectors associated with demand constraints
    IloNumArray DemandDuals;
    // CPLEX array to store the dual vectors associated with capacity constraints
    IloNumArray CapacityDuals;
    // CPLEX array to store the flow variable values
    IloNumArray PFlowVariableValues;
    std::vector<int> DemandConstrainsIndex;
    std::vector<int> PFlowVariableIndex;
	/*	***********************************************************************************************************************************
	Constructor of pathflowdata 
	*********************************************************************************************************************************** */
	pathflowdata(const IloEnv env) {
        bestobj = 0;
        soltime = 0;
        // links the CPLEX model to the environment passed to the constructor
        model = (env);
        // links all CPLEX arrays to the environment passed to the constructor
        PFlowVariables = (env);
        CapacityConstrains = (env);
        DemandConstrains = (env);
        PFlowVariableValues = (env);
        DemandDuals = (env);
        CapacityDuals = (env);
        // defines the CPLEX objective function as a minimization objective for the CPLEX model
        obj = (IloMinimize(env));
        // adds the objective to the model
        model.add(obj);
	}

	/*	***********************************************************************************************************************************
	A function that creates and adds to the model all the the (empty) constraints of the path-flow model
	*********************************************************************************************************************************** */
	void add_constraints(mcfdata &mcf, mcfgraph &g, IloEnv &env) {
        char varname[100];
        for(int k = 0; k < mcf.nComm; k++){
            IloRange constraint(env, mcf.CommDem[k], mcf.CommDem[k], NULL);
            // add the constraint to the corresponding constraints array
            DemandConstrains.add(constraint);
        }
        // add the demand constraints to the model
        model.add(DemandConstrains);
        
        
        for (int a = 0; a < g.head.size(); a++) {
            IloRange constraint(env, g.capa[a], g.capa[a], NULL);
            // add the constraint to the corresponding constraints array
            CapacityConstrains.add(constraint);
            IloNumColumn col(env);
            col = CapacityConstrains[a](+1);
            
            IloNumVar var(col, 0, IloInfinity, ILOFLOAT);
            std::string strname = "s_" + std::to_string(a + 1);
            if (strname.size() < 100) {
                std::copy(strname.begin(), strname.end(), varname);
                varname[strname.size()] = '\0';
                var.setName(varname);
            }else{
                strname.resize(100);
                std::copy(strname.begin(), strname.end(), varname);
                varname[strname.size()] = '\0';
                var.setName(varname);
            }
            
            // add the variable to the CPLEX variables array and to the model
            PFlowVariables.add(var);
            model.add(PFlowVariables[PFlowVariables.getSize() - 1]);
        }
        // add the capacity constraints to the model
        model.add(CapacityConstrains);
        
	}

	/*	***********************************************************************************************************************************
	A function that Takes in input two integers first and last and adds to the model the variables corresponding to the paths 
	numbered first, first+1, ..., last that are memorized in the path pool
	*********************************************************************************************************************************** */
	void add_paths(mcfdata &mcf, IloEnv &env, mcfpaths &pathpool, int first, int last) {
        char varname[100];
        for(int i = first; i < last; i++){
            // create a column modeling the path
            IloNumColumn col(model.getEnv());
            col += obj(pathpool.co_path[i]);
            int start_arc = pathpool.po_path[i];
            int end_arc = pathpool.po_path[i+1];
//            for(int i =0; i< pathpool.pa_arcs.size(); i++){
//                std::cout << pathpool.pa_arcs[i] << " ";
//            }
            
            for (int j = start_arc; j < end_arc; j++){
//                std::cout << pathpool.pa_arcs[j] << " ";
                col += CapacityConstrains[pathpool.pa_arcs[j]](+1);
            }
            col += DemandConstrains[pathpool.nu_comm[i]](+1);
            std::string strname = "p_" + std::to_string(i+1);
            IloNumVar var(col, 0, IloInfinity, ILOFLOAT);
            
            if (strname.size() < 100) {
                std::copy(strname.begin(), strname.end(), varname);
                varname[strname.size()] = '\0';
                var.setName(varname);
            }else{
                strname.resize(100);
                std::copy(strname.begin(), strname.end(), varname);
                varname[strname.size()] = '\0';
                var.setName(varname);
            }
            
            // add the variable to the CPLEX variables array and to the model
            PFlowVariables.add(var);
            model.add(PFlowVariables[PFlowVariables.getSize() - 1]);
        }
	}
    
    /*    ***********************************************************************************************************************************
     Prints to screen and to file the last primal solution (flows) stored
     *********************************************************************************************************************************** */
    void print_solution(const mcfdata mcf, int status) {
         //        std::ofstream sol(mcf.ProbName + ".sol");
         //        if (sol.is_open()) sol.close();
        std::cout << "pathflow.bestobj: " << std::setprecision(10) << bestobj << std::endl;
//        for (int i = 0; i < PFlowVariables.getSize(); i++) {
//            std::cout << PFlowVariables[i] << PFlowVariableValues[i] << " ";
//        }
//        std::cout << "" << std::endl;
    }

    /*    ***********************************************************************************************************************************
     Prints to screen and to file the last dual solution (flows) stored
     *********************************************************************************************************************************** */
    void print_duals(const mcfdata mcf, int status) {
        //        std::ofstream sol(mcf.ProbName + ".dua");
        //        if (sol.is_open()) sol.close();
//        std::cout << "CapacityDuals: " << CapacityDuals << std::endl;
//        std::cout << "DemandDuals: " << DemandDuals << std::endl;
    }

};
