class FordFalkerson {
public:
	

	FordFalkerson();

	int getMaxFlow(int vertices, int start, int end, int** matrix);

private:
    const int MAX_VERTICES = 40;
	int NUM_VERTICES;
	const int INFINITY_ = 10000;

	int** f;
	int** c;

	int* Flow;
	int* Link;
	int* Queue;
	int QP, QC;


	int FindPath(int source, int target);
	int MaxFlow(int source, int target);
};