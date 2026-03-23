#include "nearest_neighbor.h"
#include "data_loader.h"
#include "plot_tour.h"

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
    int total_cost = -1;

    return {tour, total_cost};
}

int main(int argc, char* argv[]) {
    // g++ nearest_neighbor.cpp data_loader.cpp plot_tour.cpp -o nearest_neighbor.bin -O3 && ./nearest_neighbor.bin ./data/berlin52.tsp
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <path_to_tsp_file>" << endl;
        return 1;
    }

    string file_path = argv[1];
    pair<vector<Node>, vector<int>> parsed_data = parse_tsplib_file(file_path);
    vector<Node> nodes = parsed_data.first;
    vector<int> optimal_tour = parsed_data.second;

    if (nodes.empty()) {
        cout << "No nodes loaded. Please check the file path and format." << endl;
        return 1;
    }

    vector<vector<int>> dist_matrix = build_distance_matrix(nodes);
    cout << "Successfully loaded " << dist_matrix.size() << " nodes." << endl;
    string instance_name = get_instance_name(file_path);

    if (!optimal_tour.empty()) {
        int optimal_cost = compute_tour_cost(optimal_tour, dist_matrix);
        cout << "Optimal Tour Cost from file: " << optimal_cost << endl;

        string optimal_png_path = instance_name + "_optimal_tour.png";
        if (export_tour_to_png(nodes, optimal_tour, optimal_png_path, "Optimal Tour", "#1f77b4")) {
            cout << "Saved optimal tour visualization to " << optimal_png_path << endl;
        } else {
            cout << "Could not write optimal tour PNG (requires gnuplot)." << endl;
        }
    } else {
        cout << "No optimal tour found in input file." << endl;
    }

    pair<vector<int>, int> nn_result = heuristic_nearest_neighbor(dist_matrix);
    cout << "Nearest Neighbor Cost: " << nn_result.second << endl;

    if (!nn_result.first.empty()) {
        string computed_png_path = instance_name + "_nearest_neighbor_tour.png";
        if (export_tour_to_png(nodes, nn_result.first, computed_png_path, "Nearest Neighbor Tour", "#d62728")) {
            cout << "Saved nearest-neighbor tour visualization to " << computed_png_path << endl;
        } else {
            cout << "Could not write nearest-neighbor tour PNG (requires gnuplot)." << endl;
        }
    } else {
        cout << "No computed nearest-neighbor tour to visualize." << endl;
    }

    return 0;
}
