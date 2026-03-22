#include "data_loader.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

vector<Node> parse_tsplib_file(const string& filepath) {
    /*
     * Reads a TSPLIB file and extracts the node coordinates.
     * Expects standard EUC_2D format.
     */
    vector<Node> nodes;
    ifstream file(filepath);

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filepath << endl;
        return nodes;
    }

    string line;
    bool reading_coords = false;

    while (getline(file, line)) {
        // Start reading after this header
        if (line.find("NODE_COORD_SECTION") != string::npos) {
            reading_coords = true;
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
    }

    file.close();
    return nodes;
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
