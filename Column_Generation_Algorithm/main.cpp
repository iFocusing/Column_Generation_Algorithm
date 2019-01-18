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
    #include "pathpool.h"
    #include <iostream>
    #include "time.h"
    #include <iomanip>
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

bool cgsolve(mcfdata &mcf, IloEnv &env, IloCplex &solver, pathflowdata &pathflow, mcfgraph &g, mcfpaths &p_pool, binheap &h, dijkstra &djk, int &iter, double &CG_total_time, double &PP_total_time) {
    double epsilon = 0.000001;
    bool err = false;
    bool optimal = false;
    while(!optimal){
        solver.exportModel("data/model.lp");
//        solver.exportModel("tiny-data/model.lp");
        iter++;
        IloNum tcpx1 = solver.getCplexTime();
        bool feasible = solver.solve();
        IloNum tcpx2 = solver.getCplexTime();
        int status = solver.getCplexStatus();
        pathflow.soltime = tcpx2 - tcpx1;
        std::cout << " CPLEX sol time" << pathflow.soltime << " sec." << std::endl;
        CG_total_time += pathflow.soltime;
        std::cout << " CPLEX sol status " << status << std::endl;
        // if a feasible solution is available
        if (feasible) {
            pathflow.bestobj = solver.getObjValue();
            solver.getValues(pathflow.PFlowVariables, pathflow.PFlowVariableValues);
            solver.getDuals(pathflow.DemandDuals, pathflow.DemandConstrains);
            solver.getDuals(pathflow.CapacityDuals, pathflow.CapacityConstrains);
            pathflow.print_solution(mcf, status);
            pathflow.print_duals(mcf, status);
        }else{
            std::cout << "dual not feasible!" << std::endl;
            err = false;
        }
        for (int i = 0; i < g.modifiedcost.size(); i++) {
            g.modifiedcost[i] = g.cost[i] - pathflow.CapacityDuals[i];
        }
        optimal = true;
        for(int c = 0; c < mcf.nComm; c++){
            std::vector<int> path;
            path = djk.Dijkheap(mcf.CommOri[c], mcf.CommEnd[c], mcf, g, h, PP_total_time);
            double p_cost = djk.path_cost(g, path);
            
            if(p_cost < pathflow.DemandDuals[c] - epsilon){
                optimal = false;
                int first = p_pool.nPaths;
                p_pool.add_nodepath(g, path, int(path.size()-1), c);
                int last = p_pool.nPaths;
                pathflow.add_paths(mcf, env, p_pool, first, last);
            }
        }
        IloCplex solver(pathflow.model);
        err = true;
    }
	return err;
}
/*	***********************************************************************************************************************************
	The main function is the entry point of our program
*********************************************************************************************************************************** */
int main() {
    std::cout <<"Begin:"<<std::endl;
    clock_t start, end;
    start = clock();
    int iter = 0;
    double CG_total_time = 0;
    double PP_total_time = 0;
    double total_time = 0;
    
    // create and intialize CPLEX environment
    IloEnv env;
    mcfdata mcf;
    // read data
    bool b = mcf.read("data/grid/grid12.dat");
//    bool b = mcf.read("tiny-data/tiny.dat");
    if(b){
        std::cout << "read succussful: "<< mcf.nNode << std::endl;
    }
    mcf.print();
    // create pathflow model;
    pathflowdata pathflow(env);
    dijkstra djk(mcf.nNode);
    binheap h(mcf.nNode);
    mcfgraph g(mcf.nNode, mcf.nArcs);
    g.fstar(mcf);
    int maxpaths = 1000000000;  // calculate the maxpaths from mcf.nNode.
    int maxarcs = 1000000000;
    mcfpaths p_pool(maxpaths, maxarcs);
    IloCplex solver(pathflow.model);
    int first = p_pool.nPaths;
    p_pool.init(mcf, g, h, djk, PP_total_time);
    int last = p_pool.nPaths;
    // add constraints and variables into the pathflow model;
    pathflow.add_constraints(mcf, g, env);
    pathflow.add_paths(mcf, env, p_pool, first, last);
    // creates a cplex object and links it to the model stored inside the arcflow object
    cgsolve(mcf, env, solver, pathflow, g, p_pool, h, djk, iter, CG_total_time, PP_total_time);
    bool error = false;
    pathflow.print_solution(mcf, true);
    pathflow.print_duals(mcf, true);
    int n = int(g.head.size());
    for (int i = n; i < n + mcf.nComm; i++){
        if (pathflow.PFlowVariableValues[i] > 0){
            error = true;
            mcf.bigM = mcf.bigM * 5;
            break;
        }
    }

    while (error){
        // Get the solution x using function solver.getValues() and check if the virtual path variables have positive value. (first ncomm paths in pool)
        IloEnv env1;
        pathflowdata pathflow1(env1);
        pathflow = pathflow1;
        p_pool.reset();
        int first = p_pool.nPaths;
        p_pool.init(mcf, g, h, djk, PP_total_time);
        int last = p_pool.nPaths;
        // add constraints and variables into the pathflow model;
        pathflow1.add_constraints(mcf, g, env1);
        pathflow1.add_paths(mcf, env1, p_pool, first, last);
        cgsolve(mcf, env1, solver, pathflow, g, p_pool, h, djk, iter, CG_total_time, PP_total_time);
        for (int i = n; i < n + mcf.nComm; i++){
            if (pathflow1.PFlowVariableValues[i] > 0){
                error = true;
                mcf.bigM = mcf.bigM * 5;
                break;
            }
            else{
                error = false;
            }
        }
    }
    
    
    end = clock();
    total_time = double(end - start) / CLOCKS_PER_SEC;
    std::cout << "1. Iteration number: "<< iter << "." << std::endl;
    std::cout << "2. The total number of columns: "<< pathflow.PFlowVariables.getSize() << "." << std::endl;
    std::cout << "3. The optimal cost obtained at the end(pathflow.bestobj): " << std::setprecision(10) << pathflow.bestobj << std::endl;
    std::cout << "4. The total time spent for solving the master problem over all iterations(CG_total_time): "<< CG_total_time << " sec." << std::endl;
    std::cout << "5. The total time spent for solving the pricing problem(PP_total_time): "<< std::setprecision(8) << PP_total_time << " sec." << std::endl;
    std::cout << "6. The total computing time: "<< total_time << " sec." << std::endl;
//    std::cin.get();
    env.end();
    return 0;
}
