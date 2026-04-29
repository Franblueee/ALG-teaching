#ifndef NEAREST_NEIGHBOR_H
#define NEAREST_NEIGHBOR_H

#include <utility>
#include <vector>

std::pair<std::vector<int>, int> heuristic_nearest_neighbor(
    const std::vector<std::vector<int>>& distance_matrix
);

#endif
