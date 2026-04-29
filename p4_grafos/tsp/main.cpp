#include "data_loader.h"
#include "branch_and_bound_mst.h"
#include "branch_and_bound_simple.h"
#include "nearest_neighbor.h"
#include "plot_tour.h"

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <filesystem>

using namespace std;

namespace {
void print_tour(const string& label, const vector<int>& tour) {
    cout << label << " Tour: ";
    if (tour.empty()) {
        cout << "(empty)" << endl;
        return;
    }

    for (size_t i = 0; i < tour.size(); ++i) {
        cout << tour[i];
        if (i + 1 < tour.size()) {
            cout << " -> ";
        }
    }
    cout << " -> " << tour.front() << endl;
}
} // namespace

int main(int argc, char* argv[]) {
    // g++ main.cpp nearest_neighbor.cpp branch_and_bound_simple.cpp branch_and_bound_mst.cpp data_loader.cpp plot_tour.cpp -o main.bin
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <path_to_tsp_file> [--nn] [--bnb-simple] [--bnb-mst] [--all]" << endl;
        cout << "Example: " << argv[0] << " data/example16.tsp --nn --bnb-mst" << endl;
        return 1;
    }

    string file_path = argv[1];
    bool run_nn = false;
    bool run_bnb_simple = false;
    bool run_bnb_mst = false;
    bool any_flag = false;

    for (int i = 2; i < argc; ++i) {
        string flag = argv[i];
        if (flag == "--nn") {
            run_nn = true;
            any_flag = true;
        } else if (flag == "--bnb-simple") {
            run_bnb_simple = true;
            any_flag = true;
        } else if (flag == "--bnb-mst") {
            run_bnb_mst = true;
            any_flag = true;
        } else if (flag == "--all") {
            run_nn = true;
            run_bnb_simple = true;
            run_bnb_mst = true;
            any_flag = true;
        } else {
            cout << "Unknown flag: " << flag << endl;
            cout << "Usage: " << argv[0] << " <path_to_tsp_file> [--nn] [--bnb-simple] [--bnb-mst] [--all]" << endl;
            return 1;
        }
    }

    if (!any_flag) {
        run_nn = true;
        run_bnb_simple = true;
        run_bnb_mst = true;
    }
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
        
        std::filesystem::create_directories("plots");
        string optimal_png_path = "plots/" + instance_name + "_optimal_tour.png";
        if (export_tour_to_png(nodes, optimal_tour, optimal_png_path, "Optimal Tour", "#1f77b4")) {
            cout << "Saved optimal tour visualization to " << optimal_png_path << endl;
        } else {
            cout << "Could not write optimal tour PNG (requires gnuplot)." << endl;
        }
    } else {
        cout << "No optimal tour found in input file." << endl;
    }

    if (run_nn) {
        pair<vector<int>, int> nn_result = heuristic_nearest_neighbor(dist_matrix);
        cout << "Nearest Neighbor Cost: " << nn_result.second << endl;
        print_tour("Nearest Neighbor", nn_result.first);

        if (!nn_result.first.empty()) {
            std::filesystem::create_directories("plots");
            string computed_png_path = "plots/" + instance_name + "_nearest_neighbor_tour.png";
            if (export_tour_to_png(nodes, nn_result.first, computed_png_path, "Nearest Neighbor Tour", "#d62728")) {
                cout << "Saved nearest-neighbor tour visualization to " << computed_png_path << endl;
            } else {
                cout << "Could not write nearest-neighbor tour PNG (requires gnuplot)." << endl;
            }
        } else {
            cout << "No computed nearest-neighbor tour to visualize." << endl;
        }
    }

    if (run_bnb_simple) {
        pair<vector<int>, int> bnb_simple_result = branch_and_bound_simple(dist_matrix);
        cout << "Branch and Bound (simple bound) Cost: " << bnb_simple_result.second << endl;
        print_tour("BnB Simple", bnb_simple_result.first);

        if (!bnb_simple_result.first.empty()) {
            std::filesystem::create_directories("plots");
            string computed_png_path = "plots/" + instance_name + "_bnb_simple_tour.png";
            if (export_tour_to_png(nodes, bnb_simple_result.first, computed_png_path, "BnB Simple Tour", "#d62728")) {
                cout << "Saved BnB simple tour visualization to " << computed_png_path << endl;
            } else {
                cout << "Could not write BnB simple tour PNG (requires gnuplot)." << endl;
            }
        } else {
            cout << "No computed BnB simple tour to visualize." << endl;
        }
    }

    if (run_bnb_mst) {
        pair<vector<int>, int> bnb_mst_result = branch_and_bound_mst(dist_matrix);
        cout << "Branch and Bound (MST bound) Cost: " << bnb_mst_result.second << endl;
        print_tour("BnB MST", bnb_mst_result.first);

        if (!bnb_mst_result.first.empty()) {
            std::filesystem::create_directories("plots");
            string computed_png_path = "plots/" + instance_name + "_bnb_mst_tour.png";
            if (export_tour_to_png(nodes, bnb_mst_result.first, computed_png_path, "BnB MST Tour", "#d62728")) {
                cout << "Saved BnB MST tour visualization to " << computed_png_path << endl;
            } else {
                cout << "Could not write BnB MST tour PNG (requires gnuplot)." << endl;
            }
        } else {
            cout << "No computed BnB MST tour to visualize." << endl;
        }
    }

    return 0;
}
