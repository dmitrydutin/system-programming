#include <iostream>
#include <memory.h>
#include <stdio.h>
#include "FordFalkerson.h"

FordFalkerson::FordFalkerson(){
}

int FordFalkerson::getMaxFlow(int vertices, int start, int end, int** matrix)
{
    NUM_VERTICES = vertices; 
    
    c = new int* [MAX_VERTICES];
    for (int i = 0; i < MAX_VERTICES; i++) {
        c[i] = new int[MAX_VERTICES];
    }

    f = new int* [MAX_VERTICES];
    for (int i = 0; i < MAX_VERTICES; i++) {
        f[i] = new int[MAX_VERTICES];
    }

    int i, j;
    for (i = 0; i < MAX_VERTICES; i++) {
        for (j = 0; j < MAX_VERTICES; j++) {
            c[i][j] = 0;
            f[i][j] = 0;
        }
    }

    for (i = 0; i < NUM_VERTICES; i++) {
        for (j = 0; j < NUM_VERTICES; j++) {
            c[i][j] = matrix[i][j];
        }
    }

    this->Flow = new int [MAX_VERTICES];
    this->Link = new int [MAX_VERTICES];
    this->Queue = new int [MAX_VERTICES];

    return MaxFlow(start, end);
};


int FordFalkerson::FindPath(int source, int target)
{
    QP = 0; QC = 1; Queue[0] = source;
    Link[target] = -1;
    int i = 0;
    int CurVertex = 0;
    memset(Flow, 0, sizeof(int) * NUM_VERTICES);
    Flow[source] = INFINITY_;
    while (Link[target] == -1 && QP < QC)
    {
        CurVertex = Queue[QP];
        for (i = 0; i < NUM_VERTICES; i++)
            if ((c[CurVertex][i] - f[CurVertex][i]) > 0 && Flow[i] == 0)
            {
                Queue[QC] = i; QC++;
                Link[i] = CurVertex;
                if (c[CurVertex][i] - f[CurVertex][i] < Flow[CurVertex])
                    Flow[i] = c[CurVertex][i];
                else
                    Flow[i] = Flow[CurVertex];
            }
        QP++;

    }

    if (Link[target] == -1) return 0;
    CurVertex = target;
    while (CurVertex != source)
    {
        f[Link[CurVertex]][CurVertex] += Flow[target];
        CurVertex = Link[CurVertex];
    }
    return Flow[target];
}

int FordFalkerson::MaxFlow(int source, int target)
{
    int MaxFlow = 0;
    int AddFlow;
    do
    {
        AddFlow = FindPath(source, target);
        MaxFlow += AddFlow;
    } while (AddFlow > 0);
    return MaxFlow;
}