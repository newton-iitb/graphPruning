#include "stdio.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "test.h"
/* Taken from geeksforgeeks BEGIN
 * https://www.geeksforgeeks.org/count-number-non-reachable-nodes/ */
#include <iostream>
#include <list>
using namespace std;

// Graph class represents a directed graph
// using adjacency list representation
class Graph {
	int V; // No. of vertices

	// Pointer to an array containing
	// adjacency lists
	list<int>* adj;

	// A recursive function used by DFS
	void DFSUtil(int v, bool visited[]);

public:
	Graph(int V); // Constructor

	// function to add an edge to graph
	void addEdge(int v, int w);

	// DFS traversal of the vertices
	// reachable from v
	int countNotReach(int v);
};

Graph::Graph(int V)
{
	this->V = V;
	adj = new list<int>[V];
}

void Graph::addEdge(int v, int w)
{
	adj[v].push_back(w); // Add w to v’s list.
	adj[w].push_back(v); // Add v to w's list.
}

void Graph::DFSUtil(int v, bool visited[])
{
	// Mark the current node as visited and
	// print it
	visited[v] = true;

	// Recur for all the vertices adjacent
	// to this vertex
	list<int>::iterator i;
	for (i = adj[v].begin(); i != adj[v].end(); ++i)
		if (!visited[*i])
			DFSUtil(*i, visited);
}

// Returns count of not reachable nodes from
// vertex v.
// It uses recursive DFSUtil()
int Graph::countNotReach(int v)
{
	// Mark all the vertices as not visited
	bool* visited = new bool[V];
	for (int i = 0; i < V; i++)
		visited[i] = false;

	// Call the recursive helper function
	// to print DFS traversal
	DFSUtil(v, visited);

	// Return count of not visited nodes
	int count = 0;
	for (int i = 0; i < V; i++) {
		if (visited[i] == false)
		{
		  cout << "unconnected vertex: " << i << "\n";
		  count++;
		}
	}

	return count;
}
/* Taken from GeeksForGeeks END */

typedef unsigned int        UInt;
typedef unsigned long int   UInt64;
typedef int                 Int;
typedef unsigned char       UInt8;

/* Take a graph in edgelist format
 * count how many vertices are not there
 * vertices which are there but not connected */

void traverseGraph(const char *filename,
                   UInt64 *pNumNodes,
                   UInt64 *pNumEdges,
                   FILE *fp_outputFile,
                   UInt8 addEdge,
                   Graph *g)
{
    FILE *fp;
    int fromnode = 0, tonode = 0;
    int number_edges = 0;
    int number_nodes = 0;

    UInt8 zeroNodeExists = 0;

    if ((fp = fopen(filename,"r")) == NULL)
    {
        fprintf(stderr,"[Error] Cannot open the file:%s\n", filename);
        exit(1);
    }

    char str[500];
    while(NULL != fgets(str, 500-1, fp))
    {
        /* some graphs have their node information wrongly written e.g.
         * in Wiki-Vote.txt, the number of nodes mentioned is 7115, but there
         * is a node with entry 7478 and there are other nodes also, so its
         * better to find the number of nodes and edges in the file by parsing
         * its once.
         * The nodeID starts from zero, so number of nodes need adjustment.
         */
        sscanf(str,"%d%d", &fromnode, &tonode);
        number_edges++;

        if (addEdge && (g != NULL))
        {
          g->addEdge(fromnode, tonode);
        }

        if (fromnode >= tonode)
        {
            number_nodes = (fromnode > number_nodes) ? fromnode : number_nodes;
        }
        else
        {
            number_nodes = (tonode > number_nodes) ? tonode : number_nodes;
        }

        if ((0 == zeroNodeExists) && (0 == fromnode || 0 == tonode))
        {
          zeroNodeExists = 1;
        }
    }

    /* If nodeID is 0-based, number of nodes needs adjustment */
    if (zeroNodeExists)
    {
      if (0 == addEdge)
      fprintf(fp_outputFile, "Graph has vertexID 0!!!\n");

      number_nodes++;
    }

    *pNumNodes = number_nodes;
    *pNumEdges = number_edges;
    fclose(fp);
}

void findNumberOfEdgesAndNodesInFile(const char *filename, UInt64 *pNumNodes,
                                     UInt64 *pNumEdges, FILE *fp_outputFile)
{
    FILE *fp = NULL;
    int num_edges = 0;
    int num_nodes = 0;
    int number_edges = 0;
    int number_nodes = 0;

    if ((fp = fopen(filename,"r")) == NULL)
    {
        fprintf(stderr,"[Error] Cannot open the file:%s\n", filename);
        exit(1);
    }

    // Read the data set and get the number of nodes (n) and edges (e)
    char ch;
    char str[500];

    /* Reading the File */
    ch = getc(fp);
    char *fgets_return = NULL;

    /* if its a comment ignore it and move forward */
    while(ch == '#')
    {
        fgets_return = fgets(str, 500 - 1, fp);
        //Debug: print title of the data set
        //printf("%s",str);
        if (NULL != fgets_return)
        {
            sscanf (str,"%*s %d %*s %d", &num_nodes, &num_edges); //number of nodes
        }

        ch = getc(fp);
    }

    ungetc(ch, fp);

    traverseGraph(filename, pNumNodes, pNumEdges, fp_outputFile, 0, NULL);

    number_nodes = *pNumNodes;
    number_edges = *pNumEdges;

    if (number_edges != num_edges)
    {
        fprintf(fp_outputFile, "-------------------------------------\n");
        fprintf(fp_outputFile, "Graph file has num edges misreported in the header\n");
        fprintf(fp_outputFile, "Number of Edges=> Correct:%d Reported:%d. Corrected!\n",
            number_edges, num_edges);
        fprintf(fp_outputFile, "-------------------------------------\n");
        num_edges = number_edges;
    }

    if (number_nodes != num_nodes)
    {
        fprintf(fp_outputFile, "-------------------------------------\n");
        fprintf(fp_outputFile, "Graph file has num nodes misreported in the header\n");
        fprintf(fp_outputFile, "Number of Nodes=> Correct:%d Reported:%d. Corrected!\n",
            number_nodes, num_nodes);
        fprintf(fp_outputFile, "-------------------------------------\n");
        num_nodes = number_nodes;
    }

    *pNumNodes = num_nodes;
    *pNumEdges = num_edges;

    fclose(fp);
}

int addEdgesToGraph(Graph *g, const char *filename, UInt64 *pNumNodes,
                    UInt64 *pNumEdges, FILE *fp_outputFile)
{
  UInt64 numNodes = 0, numEdges = 0;
  traverseGraph(filename, &numNodes, &numEdges, fp_outputFile, 1, g);
}

int main(void)
{
  UInt64 numNodes = 0, numEdges = 0;
  FILE   *fp_outputFile = NULL;

  const char *pGraphFile = "test_graph_edgeList.txt";
  const char *pstrResultFile = "./resultsFile.txt";

  if ((fp_outputFile = fopen(pstrResultFile, "w")) == NULL)
  {
      printf("[Error] Cannot open the file:%s", pstrResultFile);
      exit(1);
  }

  findNumberOfEdgesAndNodesInFile(pGraphFile, &numNodes, &numEdges, fp_outputFile);
	Graph g(numNodes);

  addEdgesToGraph(&g, pGraphFile, &numNodes, &numEdges, fp_outputFile);
  cout << g.countNotReach(0) << "\n";
}
