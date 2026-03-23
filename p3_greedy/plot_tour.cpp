#include "plot_tour.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

string get_instance_name(const string& path) {
    size_t last_slash = path.find_last_of("/\\");
    string filename = (last_slash == string::npos) ? path : path.substr(last_slash + 1);
    size_t last_dot = filename.find_last_of('.');
    return (last_dot == string::npos) ? filename : filename.substr(0, last_dot);
}

bool export_tour_to_png(
    const vector<Node>& nodes,
    const vector<int>& tour,
    const string& output_path,
    const string& title,
    const string& stroke_color
) {
    if (nodes.empty() || tour.empty()) {
        return false;
    }

    for (int node_idx : tour) {
        if (node_idx < 0 || node_idx >= static_cast<int>(nodes.size())) {
            return false;
        }
    }

    const string data_path = output_path + ".tmp.dat";
    const string script_path = output_path + ".tmp.plt";

    {
        ofstream data(data_path);
        if (!data.is_open()) {
            return false;
        }

        for (int node_idx : tour) {
            data << nodes[node_idx].x << " " << nodes[node_idx].y << "\n";
        }
        data << nodes[tour.front()].x << " " << nodes[tour.front()].y << "\n";
    }

    {
        ofstream script(script_path);
        if (!script.is_open()) {
            remove(data_path.c_str());
            return false;
        }

        script << "set terminal pngcairo size 1000,800 enhanced font 'Sans,12'\n";
        script << "set output \"" << output_path << "\"\n";
        script << "set title \"" << title << "\"\n";
        script << "unset key\n";
        script << "set size ratio -1\n";
        script << "set border linewidth 1\n";
        script << "plot \"" << data_path << "\" with lines lw 2 lc rgb \"" << stroke_color << "\", \\\n";
        script << "     \"" << data_path << "\" with points pt 7 ps 0.5 lc rgb \"#666666\"\n";
    }

    string cmd = "gnuplot \"" + script_path + "\" > /dev/null 2>&1";
    int rc = system(cmd.c_str());

    remove(data_path.c_str());
    remove(script_path.c_str());
    return rc == 0;
}
