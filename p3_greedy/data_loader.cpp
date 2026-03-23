#include "data_loader.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

std::pair<std::vector<Node>, std::vector<int>> parse_tsplib_file(const string& filepath) {
    /*
     * Reads a TSPLIB file and extracts the node coordinates and tour nodes.
     * Expects standard EUC_2D format.
     */
    vector<Node> nodes;
    vector<int> tour;
    ifstream file(filepath);

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filepath << endl;
        return {nodes, tour};
    }

    string line;
    bool reading_coords = false;
    bool reading_tour = false;

    while (getline(file, line)) {
        // Start reading after this header
        if (line.find("NODE_COORD_SECTION") != string::npos) {
            reading_coords = true;
            continue;
        }

        if (line.find("TOUR_SECTION") != string::npos) {
            reading_tour = true;
            continue;
        }

        // Stop reading at end of file marker
        if (line.find("EOF") != string::npos) {
            break;
        }

        if (reading_coords) {
            istringstream iss(line);
            Node n;
            // TSPLIB nodes are typically 1-indexed
            if (iss >> n.id >> n.x >> n.y) {
                nodes.push_back(n);
            }
        }

        if (reading_tour) {
            istringstream iss(line);
            int node_id;
            if (iss >> node_id) {
                tour.push_back(node_id - 1); // Convert to 0-indexed
            }
        }
    }

    file.close();
    return {nodes, tour};
}

vector<vector<int>> build_distance_matrix(const vector<Node>& nodes) {
    /*
     * Builds a 2D matrix representing the distance between all pairs of nodes.
     * Uses the TSPLIB standard EUC_2D distance calculation.
     */
    int num_nodes = nodes.size();
    // Initialize a 0-indexed distance matrix with zeros
    vector<vector<int>> matrix(num_nodes, vector<int>(num_nodes, 0));

    for (int i = 0; i < num_nodes; ++i) {
        for (int j = 0; j < num_nodes; ++j) {
            if (i != j) {
                double dx = nodes[i].x - nodes[j].x;
                double dy = nodes[i].y - nodes[j].y;

                // TSPLIB EUC_2D standard requires rounding to the nearest integer
                matrix[i][j] = round(sqrt(dx * dx + dy * dy));
            }
        }
    }
    return matrix;
}

int compute_tour_cost(const vector<int>& tour, const vector<vector<int>>& distance_matrix) {
    /*
     * Computes the total cost of a given tour based on the provided distance matrix.
     * Assumes the tour is a complete cycle (returns to the starting node).
     */
    int total_cost = 0;
    int num_nodes = tour.size();

    for (int i = 0; i < num_nodes; ++i) {
        int from_node = tour[i];
        int to_node = tour[(i + 1) % num_nodes]; // Wrap around to form a cycle
        total_cost += distance_matrix[from_node][to_node];
    }
    return total_cost;
}
