#include "stdio.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef unsigned int        UInt;
typedef unsigned long int   UInt64;
typedef int                 Int;

/* Take a graph in edgelist format
 * count how many vertices are not there
 * vertices which are there but not connected */

void findNumberOfEdgesAndNodesInFile(const char *filename, UInt64 *pNumNodes,
                                     UInt64 *pNumEdges, FILE *fp_outputFile)
{
    FILE *fp;
    int fromnode = 0, tonode = 0;

    int number_edges = 0;
    int number_nodes = 0;

    int num_edges = 0;
    int num_nodes = 0;

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
        fgets_return = fgets(str, 500-1, fp);
        //Debug: print title of the data set
        //printf("%s",str);
        if (NULL != fgets_return)
        {
            sscanf (str,"%*s %d %*s %d", &num_nodes, &num_edges); //number of nodes
        }
        ch = getc(fp);
    }

    ungetc(ch, fp);

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
        //printf("Edge:%d %d->%d\n", number_edges, fromnode, tonode);


        if (fromnode >= tonode)
        {
            number_nodes = (fromnode > number_nodes) ? fromnode : number_nodes;
        }
        else
        {
            number_nodes = (tonode > number_nodes) ? tonode : number_nodes;
        }
    }

    /* As nodeID is 0-based, number of nodes needs adjustment */
    //TODO This need to be verified as all the graphs are not zero based
    number_nodes++;

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


int main(void)
{
  char   *pGraphFile = "test_graph_edgeList.txt";
  UInt64 numNodes = 0, numEdges = 0;
  FILE   *fp_outputFile = NULL;
  char   *pstrResultFile = "./resultsFile.txt";

  if ((fp_outputFile = fopen(pstrResultFile, "w")) == NULL)
  {
      printf("[Error] Cannot open the file:%s", pstrResultFile);
      exit(1);
  }

  findNumberOfEdgesAndNodesInFile(pGraphFile, &numNodes, &numEdges, fp_outputFile);
}
