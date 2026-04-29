#include "branch_and_bound_simple.h"

#include <utility>
#include <vector>

using namespace std;

pair<vector<int>, int> branch_and_bound_simple(const vector<vector<int>>& distance_matrix) {
    /*
     * TODO: Implement Branch and Bound (simple bound).
     * Return: A pair containing the tour (vector of node indices) and the total cost.
     */
    (void)distance_matrix;
    vector<int> tour;
    int total_cost = -1;

    return {tour, total_cost};
}
