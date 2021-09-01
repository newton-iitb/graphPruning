# graphPruning
0. The main objective to take graph in the edge list format (sourceNode DestinationNode) and then make it fit so that it can be parsed by METIS program
1. Take a graph in edgelist format
2. Count how many vertices are there, how many are not there e.g. graph may have 1-50 vertices but vertex 4, 7, 15 may not be there. In such a case, the graph has only 47 vertices.
3. Vertices which are there but not connected to the other nodes in the graph. e.g. in the above example, node 11 may not be connected to the main graph, so then the graph has only 46 vertices.
