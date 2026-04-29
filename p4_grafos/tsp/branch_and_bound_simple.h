#ifndef BRANCH_AND_BOUND_SIMPLE_H
#define BRANCH_AND_BOUND_SIMPLE_H

#include <utility>
#include <vector>

std::pair<std::vector<int>, int> branch_and_bound_simple(
    const std::vector<std::vector<int>>& distance_matrix
);

#endif
