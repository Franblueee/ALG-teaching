#ifndef PLOT_TOUR_H
#define PLOT_TOUR_H

#include "data_loader.h"

#include <string>
#include <vector>

std::string get_instance_name(const std::string& path);

bool export_tour_to_png(
    const std::vector<Node>& nodes,
    const std::vector<int>& tour,
    const std::string& output_path,
    const std::string& title,
    const std::string& stroke_color
);

#endif
