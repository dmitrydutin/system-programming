typedef struct node {
	int val;
	int state; //status
	int pred; //predecessor
}node;

class FordFalkerson2 {
public:


	FordFalkerson2();

	int getMaxFlow(int vertices, int start, int end, int** matrix);

private:
	const int MAX_VERTICES = 40;
	int NODE;

	int** resGraph;
	int** graph;

	int fordFulkerson(node* vert, node source, node sink);
	int bfs(node* vert, node start, node sink);
};


