#include "nearest_neighbor.h"
#include "data_loader.h"
#include "plot_tour.h"

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <climits>

using namespace std;

pair<vector<int>, int> heuristic_nearest_neighbor(const vector<vector<int>>& distance_matrix) {
    int n = static_cast<int>(distance_matrix.size());
    if (n == 0) {
        return {{}, -1};
    }

    vector<int> tour(n, -1);
    vector<bool> visited(n, false);

    int start = 0;
    int current = start;
    tour[0] = start;
    visited[start] = true;

    long long total_cost = 0;

    for (int step = 1; step < n; ++step) {
        int best_next = -1;
        int best_dist = INT_MAX;

        for (int next = 0; next < n; ++next) {
            if (!visited[next] && distance_matrix[current][next] < best_dist) {
                best_dist = distance_matrix[current][next];
                best_next = next;
            }
        }

        if (best_next == -1) {
            return {{}, -1};
        }

        visited[best_next] = true;
        tour[step] = best_next;
        total_cost += best_dist;
        current = best_next;
    }

    total_cost += distance_matrix[current][start];

    return {tour, static_cast<int>(total_cost)};
}
