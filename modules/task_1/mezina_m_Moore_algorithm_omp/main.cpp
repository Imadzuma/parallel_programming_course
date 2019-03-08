#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>

using namespace std;

int main() {
	// Graph params
	unsigned int vertex_count;
	unsigned int edges_count;
	unsigned char** graph_matrix;
	bool writing_work = 0;
	unsigned int start_vertex;

	// Enter graph params
	/*cout << "Enter count of vertexes: ";
	cin >> vertex_count;
	cout << "Enter count of edges: ";
	cin >> edges_count;
	cout << "Enter start vertex: ";
	cin >> start_vertex;
	start_vertex--;
	cout << "Enter 1, if should print intermidiate information, else enter any other value: ";
	string s;
	cin >> s;
	if (s == "1") writing_work = 1;*/
	vertex_count = 10000;
	edges_count = 99990000;
	start_vertex = 0;
	writing_work = 0;

	// Memory allocation matrix
	srand(time(0));
	graph_matrix = new unsigned char*[vertex_count];
	for (int i = 0; i < vertex_count; ++i) {
		graph_matrix[i] = new unsigned char[vertex_count];
	}

	// Generate matrix
	if (edges_count <= vertex_count * (vertex_count - 1) / 2) {
		for (int i = 0; i < vertex_count; ++i)
			memset(graph_matrix[i], 0, vertex_count * sizeof(unsigned char));
		for (int i = 0; i < edges_count; ++i) {
			int x, y;
			do {
				x = rand() % vertex_count;
				y = rand() % vertex_count;
			} while (x == y || graph_matrix[x][y] != 0);
			unsigned char value = rand() % 100 + 1;
			graph_matrix[x][y] = value;
		}
	}
	else {
		for (int x = 0; x < vertex_count; ++x) {
			for (int y = 0; y < vertex_count; ++y)
				graph_matrix[x][y] = (x != y) ? rand() % 100 + 1 : 0;
		}
		for (int i = 0; i < vertex_count * (vertex_count - 1) - edges_count; ++i) {
			int x, y;
			do {
				x = rand() % vertex_count;
				y = rand() % vertex_count;
			} while (graph_matrix[x][y] == 0);
			graph_matrix[x][y] = 0;
		}
	}
	if (writing_work) {
		cout << endl << "GENERATE MATRIX:" << endl;
		for (int x = 0; x < vertex_count; ++x) {
			for (int y = 0; y < vertex_count; ++y)
				cout << "\t" << (int)graph_matrix[x][y];
			cout << endl;
		}
	}

	// Starting time
	cout << "START WORK!" << endl;
	unsigned int start_time, finish_time;
	start_time = clock();

	// Work params
	unsigned int** distance_matrix;
	unsigned int* prev_vertex;
	const unsigned int PATH_INFINITY = ~(unsigned int)0;
	bool check_compare;
	int iter;
	vector<int>* path;

	// Memory allocation params
	distance_matrix = new unsigned int*[vertex_count + 1];
	for (int i = 0; i <= vertex_count; ++i) {
		distance_matrix[i] = new unsigned int[vertex_count];
		memset(distance_matrix[i], 255, vertex_count * sizeof(unsigned int));
	}
	distance_matrix[0][start_vertex] = 0;
	prev_vertex = new unsigned int[vertex_count];
	memset(prev_vertex, 255, vertex_count * sizeof(unsigned int));
	prev_vertex[start_vertex] = start_vertex;
	path = new vector<int>[vertex_count];

	// Algorithm
	iter = 0;
	check_compare = 0;
	if (writing_work) {
		cout << "WORK ITERATION: "<<endl;
		cout << "\tIteration 0:\t";
		for (int i = 0; i < vertex_count; ++i) {
			if (distance_matrix[0][i] == PATH_INFINITY) cout << "inf\t";
			else cout << distance_matrix[0][i] << "\t";
		}
		cout << endl;
	}
	while(!check_compare && iter < vertex_count) {
		check_compare = 1;
		// Copy elements
		for (int vertex = 0; vertex < vertex_count; ++vertex)
			distance_matrix[iter + 1][vertex] = distance_matrix[iter][vertex];
		// Update distance for vertexes 
		for (int from = 0; from < vertex_count; ++from) {
			if (distance_matrix[iter][from] == PATH_INFINITY || iter != 0 && distance_matrix[iter][from] == distance_matrix[iter - 1][from])
				continue;
			for (int to = 0; to < vertex_count; ++to) {
				if (graph_matrix[from][to] == 0)
					continue;
				if (distance_matrix[iter + 1][to] > distance_matrix[iter][from] + graph_matrix[from][to]) {
					check_compare = 0;
					distance_matrix[iter + 1][to] = distance_matrix[iter][from] + graph_matrix[from][to];
					prev_vertex[to] = from;
				}
			}
		}
		iter++;
		if (writing_work) {
			cout << "\tIteration " << iter << ":\t";
			for (int i = 0; i < vertex_count; ++i) {
				if (distance_matrix[iter][i] == PATH_INFINITY) cout << "inf\t";
				else cout << distance_matrix[iter][i] << "\t";
			}
			cout << endl;
		}
	}
	
	// Find pathes
	for (int vertex = 0; vertex < vertex_count; ++vertex) {
		if (distance_matrix[iter][vertex] == PATH_INFINITY)
			continue;
		int cur_vertex = vertex;
		do {
			path[vertex].push_back(cur_vertex);
			cur_vertex = prev_vertex[cur_vertex];
		} while (cur_vertex != start_vertex);
		if (path[vertex].back() != start_vertex)
			path[vertex].push_back(start_vertex);
		reverse(path[vertex].begin(), path[vertex].end());
	}

	// Print results
	if (writing_work) {
		cout << "RESULTS: "<<endl;
		for (int vertex = 0; vertex < vertex_count; ++vertex) {
			cout << "\t" << vertex + 1 << ")\t";
			if (distance_matrix[iter][vertex] == PATH_INFINITY) {
				cout << "not path" << endl;
				continue;
			}
			cout << "Distance: " << distance_matrix[iter][vertex] << endl;
			cout << "\t\tPath: ";
			cout << path[vertex][0] + 1;
			for (int i = 1; i < path[vertex].size(); ++i)
				cout << "->" << path[vertex][i] + 1;
			cout << endl;
		}
	}

	// Finish time
	finish_time = clock();
	cout << "TIME: " << (double)(finish_time - start_time) / 1000 << endl;

	system("pause");
	return 0;
}