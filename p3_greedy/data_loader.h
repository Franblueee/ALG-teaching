#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include <string>
#include <vector>

struct Node {
    int id;
    double x;
    double y;
};

std::vector<Node> parse_tsplib_file(const std::string& filepath);
std::vector<std::vector<int>> build_distance_matrix(const std::vector<Node>& nodes);

#endif
