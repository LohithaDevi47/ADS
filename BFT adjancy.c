#include <stdio.h> 
#include <stdlib.h> 
#include <stdbool.h> 
#define MAX_NODES 100 
// Function to perform Breadth-First Traversal (BFT) 
void bft(int graph[MAX_NODES][MAX_NODES], int start, int num_nodes)  
{ 
    bool visited[MAX_NODES] = { false }; // Array to keep track of visited nodes 
    int queue[MAX_NODES], front = 0, rear = 0; 
    // Start by visiting the start node 
    visited[start] = true; 
    queue[rear++] = start; 
    while (front < rear)  
   { 
        int node = queue[front++];
		int i; 
        printf("Visited %d\n", node); 
        // Visit all adjacent nodes 
        for ( i = 0; i < num_nodes; i++)  
       { 
            if (graph[node][i] == 1 && !visited[i]) { // Check if there's an edge and the node isn't  
                visited[i] = true; 
                queue[rear++] = i; 
            } 
        } 
    } 
} 
int main() 
     { 
    int graph[MAX_NODES][MAX_NODES]; 
    int num_nodes, num_edges;
	int i,j; 
    // Initialize the graph with zeros 
    for (i = 0; i < MAX_NODES; i++)  
   { 
        for ( j = 0; j < MAX_NODES; j++) 
       { 
            graph[i][j] = 0; 
        }
          } 
    printf("Enter the number of nodes: "); 
    scanf("%d", &num_nodes); 
    printf("Enter the number of edges: "); 
    scanf("%d", &num_edges); 
    printf("Enter the edges (format: start end):\n"); 
    for (i = 0; i < num_edges; i++) 
   { 
        int start, end; 
        scanf("%d %d", &start, &end); 
        graph[start][end] = 1; // Add an edge from start to end 
        graph[end][start] = 1; // For undirected graph: add edge in both directions 
    } 
    int start_node; 
    printf("Enter the start node for BFT: "); 
    scanf("%d", &start_node); 
    printf("BFT traversal starting from node %d:\n", start_node); 
    bft(graph, start_node, num_nodes); 
    return 0; 
} 
 
