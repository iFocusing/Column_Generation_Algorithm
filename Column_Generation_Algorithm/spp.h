#include "binheap.h"
#include "mcf_data.h"
#include <vector>
#include "time.h"
#include <iomanip>

#pragma once

// *****************************************************************************
// CLASS PROVIDING A 2-HEAP IMPLEMENTATION OF DIJKSTRA ALGORITHMS 
// See lecture slides 07_shrtpaths.pdf for a description of the algorithm
// *****************************************************************************

	class dijkstra {
	public:
		// a vector to store the node distance labels (denoted by d[] in the lecture slides)
        std::vector<double> d;
		// a vector to store the predecessor labels (denoted by π[] in the lecture slides)
        std::vector<int> pi;
		// Constructor just resizes the local vectors
		dijkstra(int nNodes) {
            d.resize(nNodes, INT_MAX);
            pi.resize(nNodes, 0);
            
		}

        void reset() {
            for(int i = 0; i < d.size(); i++){
                d[i] = INT_MAX;
                pi[i] = 0;
            }
        }
        
		// finds a shortest path from root to sink node of a forward strar representation of a graph
		// At termination, the shortest path should be reconstructed by following backward the predecessor
		// labels starting from the desitination node. The predecessor label of the root node is always -1
        std::vector<int> Dijkheap(int s, int dest, mcfdata mcf, mcfgraph g, binheap h, double &PP_total_time) {
            // s is start node
            // Node expansion: Use a forward star representation of G. This brings down the operation count for node expansion to O(m)
            // Node selection: Avoid scanning the entire T at each iteration by keeping it partially sorted using a heap
            // a vector to store the node that has permanent lable and add the start node into it;
            double time = 0;
            clock_t start, end;
            start = clock();
            
            reset();
            d[s] = 0;
            pi[s] = -1;
            h.Add(d, s);
            while(h.nheap != 0){
                int v = h.Choose(d);
                for (int i = g.point[v]; i < g.point[v+1]; i++){
                    int dnew = d[v] + g.modifiedcost[i];
                    int j = g.head[i];
                    if(d[j] > dnew){
                        pi[j] = v;
                        if(d[j] == INT_MAX){
                            d[j] = dnew;
                            h.Add(d, j);
                        }else{
                            d[j] = dnew;
                            h.Decreased_Label(d, j);
                        }
                    }
                }
            }
            int pre = dest;
            std::vector<int> path;
            path.push_back(dest);
            while(pi[pre]!=-1){
                pre = pi[pre];
                path.insert(path.begin(), pre);
            }
//            std::cout << "Print from spp.h" << std::endl;
//            std::cout << "start: " << s << " dest: " << dest << std::endl;
            
//            int i;
//            for (i = 0; i < path.size() - 1; i++){
//                std::cout << path[i] << " -> ";
//            }
//            std::cout << path[i] << std::endl;
            end = clock();
            time = double(end - start) / CLOCKS_PER_SEC;
            PP_total_time += time;
            return path;
		}
        
        double path_cost(mcfgraph g, std::vector<int> nodepath){
            double p_cost = 0;
            for(int i = 0; i < nodepath.size()-1; i++){
                int arc = g.get_arc_id(nodepath[i], nodepath[i+1]);
                p_cost += g.modifiedcost[arc];
            }
            return p_cost;
        }
    };
