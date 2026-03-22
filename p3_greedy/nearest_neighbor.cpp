#include "nearest_neighbor.h"
#include "data_loader.h"

#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

pair<vector<int>, int> heuristic_nearest_neighbor(const vector<vector<int>>& distance_matrix) {
    /*
     * TODO: Implement the Nearest Neighbor heuristic.
     * Return: A pair containing the tour (vector of node indices) and the total tour cost.
     */
    vector<int> tour;
    int total_cost = 0;

    return {tour, total_cost};
}

int main(int argc, char* argv[]) {
    // g++ nearest_neighbor.cpp data_loader.cpp -o nearest_neighbor.bin -O3 && ./nearest_neighbor.bin ./data/berlin52.tsp
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <path_to_tsp_file>" << endl;
        return 1;
    }

    string file_path = argv[1];
    vector<Node> nodes = parse_tsplib_file(file_path);

    if (nodes.empty()) {
        cout << "No nodes loaded. Please check the file path and format." << endl;
        return 1;
    }

    vector<vector<int>> dist_matrix = build_distance_matrix(nodes);
    cout << "Successfully loaded " << dist_matrix.size() << " nodes." << endl;

    pair<vector<int>, int> nn_result = heuristic_nearest_neighbor(dist_matrix);
    cout << "Nearest Neighbor Cost: " << nn_result.second << endl;

    return 0;
}
