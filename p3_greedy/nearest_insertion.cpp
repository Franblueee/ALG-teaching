#include "nearest_insertion.h"
#include "data_loader.h"

#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

pair<vector<int>, int> heuristic_nearest_insertion(
    const vector<int>& initial_tour,
    const vector<vector<int>>& distance_matrix
) {
    /*
     * TODO: Implement the Nearest Insertion heuristic.
     * Return: A pair containing the optimized tour and the total cost.
     */
    vector<int> optimized_tour = initial_tour;
    int total_cost = -1;

    return {optimized_tour, total_cost};
}

int main(int argc, char* argv[]) {
    // g++ nearest_insertion.cpp data_loader.cpp -o nearest_insertion.bin -O3 && ./nearest_insertion.bin ./data/berlin52.tsp
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <path_to_tsp_file>" << endl;
        return 1;
    }

    string file_path = argv[1];
    pair<vector<Node>, vector<int>> parsed_data = parse_tsplib_file(file_path);
    vector<Node> nodes = parsed_data.first;
    vector<int> initial_tour = parsed_data.second;

    if (nodes.empty()) {
        cout << "No nodes loaded. Please check the file path and format." << endl;
        return 1;
    }

    vector<vector<int>> dist_matrix = build_distance_matrix(nodes);
    cout << "Successfully loaded " << dist_matrix.size() << " nodes." << endl;

    if (initial_tour.empty()) {
        // Default initial tour: visit nodes in index order.
        initial_tour.reserve(dist_matrix.size());
        for (int i = 0; i < static_cast<int>(dist_matrix.size()); ++i) {
            initial_tour.push_back(i);
        }
    }

    pair<vector<int>, int> insertion_result = heuristic_nearest_insertion(initial_tour, dist_matrix);
    cout << "Nearest Insertion Cost: " << insertion_result.second << endl;

    return 0;
}
