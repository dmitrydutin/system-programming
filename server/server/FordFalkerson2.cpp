#include <iostream>
#include <memory.h>
#include <stdio.h>
#include<iostream>
#include<queue>
#include "FordFalkerson2.h"

using namespace std;

FordFalkerson2::FordFalkerson2() {
}

int minimum(int a, int b) {
    return (a < b) ? a : b;
}

/*
int resGraph[NODE][NODE];

int graph[NODE][NODE] = {
   {0, 16, 0, 0, 1, 0},
   {0, 0, 12, 0, 6, 0},
   {0, 0, 0, 0, 9, 20},
   {0, 0, 7, 0, 0, 4},
   {0, 0, 0, 14, 0, 0},
   {0, 0, 0, 0, 0, 0}
};
*/
/*
int graph[NODE][NODE] = {
   {0,7,4,0,0,0},
   {0,0,4,2,0,0},
   {0,0,0,8,4,0},
   {0,0,0,0,4,5},
   {0,0,0,0,0,12},
   {0,0,0,0,0,0}
};*/

int FordFalkerson2::bfs(node* vert, node start, node sink) {
    node u;
    int i, j;
    queue<node> que;

    for (i = 0; i < NODE; i++) {
        vert[i].state = 0;    //not visited
    }

    vert[start.val].state = 1;   //visited
    vert[start.val].pred = -1;   //no parent node
    que.push(start);   //insert starting node

    while (!que.empty()) {
        //delete from queue and print
        u = que.front();
        que.pop();

        for (i = 0; i < NODE; i++) {
            if (resGraph[u.val][i] > 0 && vert[i].state == 0) {
                que.push(vert[i]);
                vert[i].pred = u.val;
                vert[i].state = 1;
            }
        }
    }
    return (vert[sink.val].state == 1);
}

int FordFalkerson2::fordFulkerson(node* vert, node source, node sink) {
    int maxFlow = 0;
    int u, v;

    for (int i = 0; i < NODE; i++) {
        for (int j = 0; j < NODE; j++) {
            resGraph[i][j] = graph[i][j];    //initially residual graph is main graph
        }
    }

    while (bfs(vert, source, sink)) {    //find augmented path using bfs algorithm
        int pathFlow = 999;//as infinity
        for (v = sink.val; v != source.val; v = vert[v].pred) {
            u = vert[v].pred;
            pathFlow = minimum(pathFlow, resGraph[u][v]);
        }

        for (v = sink.val; v != source.val; v = vert[v].pred) {
            u = vert[v].pred;
            resGraph[u][v] -= pathFlow;   //update residual capacity of edges
            resGraph[v][u] += pathFlow;   //update residual capacity of reverse edges
        }

        maxFlow += pathFlow;
    }
    return maxFlow;    //the overall max flow
}

int FordFalkerson2::getMaxFlow(int verticesAmount, int start, int end, int** matrix){
    node* vertices = new node[MAX_VERTICES];
    node source, sink;

    NODE = verticesAmount;

    resGraph = new int* [MAX_VERTICES];
    for (int i = 0; i < MAX_VERTICES; i++) {
        resGraph[i] = new int[MAX_VERTICES];
    }

    graph = new int* [MAX_VERTICES];
    for (int i = 0; i < MAX_VERTICES; i++) {
        graph[i] = new int[MAX_VERTICES];
    }

    int i, j;
    for (i = 0; i < MAX_VERTICES; i++) {
        for (j = 0; j < MAX_VERTICES; j++) {
            graph[i][j] = 0;
            resGraph[i][j] = 0;
        }
    }

    for (i = 0; i < NODE; i++) {
        for (j = 0; j < NODE; j++) {
            graph[i][j] = matrix[i][j];
        }
    }

    for (int i = 0; i < NODE; i++) {
        vertices[i].val = i;
    }

    source.val = start;
    sink.val = end;
    int maxFlow = fordFulkerson(vertices, source, sink);
    cout << "Maximum flow is: " << maxFlow << endl;
    return maxFlow;
}
