#include "graph_coloring.h"

#include <cstring>
#include <filesystem>
#include <iostream>
#include <string>
using namespace std;

/*
Instrucciones de compilación: g++ -std=c++17 main.cpp graph_coloring.cpp -o graph_coloring.bin
*/


/**
 * @brief Función principal que carga un grafo, realiza coloreo voraz y ejecuta algoritmos de backtracking y branch-and-bound.
 * @param argc Número de argumentos de línea de comandos.
 * @param argv Array de argumentos de línea de comandos (espera ruta de archivo de grafo y opcionalmente colores máximos).
 * @return 0 en éxito, 1 en error.
 */
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <graph_file> <max_colors_for_backtracking>" << endl;
        cout << "Example: " << argv[0] << " data/graph1.txt 3" << endl;
        cout << "Optional: --plot [--plot-dir output_dir]" << endl;
        return 0;
    }

    string file_path = argv[1];
    int max_colors = -1;
    bool plot_enabled = false;
    string plot_dir;

    for (int i = 2; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--plot") {
            plot_enabled = true;
            continue;
        }
        if (arg.rfind("--plot-dir=", 0) == 0) {
            plot_enabled = true;
            plot_dir = arg.substr(strlen("--plot-dir="));
            continue;
        }
        if (arg == "--plot-dir" && i + 1 < argc) {
            plot_enabled = true;
            plot_dir = argv[++i];
            continue;
        }
        if (max_colors < 0) {
            max_colors = stoi(arg);
            continue;
        }
        cout << "Warning: ignoring extra argument: " << arg << endl;
    }
    Graph graph = parse_graph_file(file_path);

    if (graph.num_vertices <= 0) {
        cout << "Failed to load graph. Please check the file format." << endl;
        return 1;
    }

    int edge_count = 0;
    for (const auto& neighbors : graph.adj) {
        edge_count += static_cast<int>(neighbors.size());
    }
    edge_count /= 2;

    cout << "Loaded graph with " << graph.num_vertices << " vertices and " << edge_count << " edges." << endl;
    cout << "=== Optimal coloring ===" << endl;
    if (graph.optimal_coloring.empty()) {
        cout << "No optimal coloring provided in input file." << endl;
    } else {
        int optimal_used = count_used_colors(graph.optimal_coloring);
        cout << "Colors used: " << optimal_used << endl;
        cout << "Coloring assignment:" << endl;

        // Print the optimal coloring assignment
        for (int i = 0; i < static_cast<int>(graph.optimal_coloring.size()); ++i) {
            cout << "  vertex " << i << " -> color " << graph.optimal_coloring[i] << endl;
        }
    }

    vector<int> greedy_colors = greedy_coloring(graph);
    int greedy_used = count_used_colors(greedy_colors);
    cout << "=== Greedy coloring ===" << endl;
    cout << "Colors used: " << greedy_used << endl;
    cout << "Coloring assignment:" << endl;
    for (int i = 0; i < static_cast<int>(greedy_colors.size()); ++i) {
        cout << "  vertex " << i << " -> color " << greedy_colors[i] << endl;
    }

    ColoringResult backtracking_result;
    if (max_colors >= 0) {
        backtracking_result = backtracking_color_with_k(graph, max_colors);
        print_coloring_solution(backtracking_result, "Backtracking with max K=" + to_string(max_colors));
    }

    ColoringResult bnb_result = branch_and_bound_min_colors(graph);
    print_coloring_solution(bnb_result, "Branch and Bound Optimal Coloring");

    if (plot_enabled) {
        namespace fs = std::filesystem;
        if (plot_dir.empty()) {
            plot_dir = "plots";
        }
        fs::create_directories(plot_dir);
        string instance = get_instance_name(file_path);

        if (!graph.optimal_coloring.empty()) {
            string out = plot_dir + "/" + instance + "_optimal.dot";
            write_coloring_dot(graph, graph.optimal_coloring, out, "Optimal coloring");
            cout << "Wrote " << out << endl;
        }

        {
            string out = plot_dir + "/" + instance + "_greedy.dot";
            write_coloring_dot(graph, greedy_colors, out, "Greedy coloring");
            cout << "Wrote " << out << endl;
        }

        if (max_colors >= 0 && backtracking_result.success) {
            string out = plot_dir + "/" + instance + "_backtracking_k" + to_string(max_colors) + ".dot";
            write_coloring_dot(graph, backtracking_result.colors, out, "Backtracking coloring");
            cout << "Wrote " << out << endl;
        }

        if (bnb_result.success) {
            string out = plot_dir + "/" + instance + "_bnb.dot";
            write_coloring_dot(graph, bnb_result.colors, out, "Branch and Bound coloring");
            cout << "Wrote " << out << endl;
        }
    }

    return 0;
}

