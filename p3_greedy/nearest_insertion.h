#ifndef NEAREST_INSERTION_H
#define NEAREST_INSERTION_H

#include <utility>
#include <vector>

std::pair<std::vector<int>, int> heuristic_nearest_insertion(
    const std::vector<int>& initial_tour,
    const std::vector<std::vector<int>>& distance_matrix
);

#endif
