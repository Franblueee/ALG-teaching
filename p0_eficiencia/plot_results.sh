#!/bin/bash

# 1. Check if the data file exists before trying to plot
DATAFILE="benchmark_bubblesort.dat"

if [ ! -f "$DATAFILE" ]; then
    echo "Error: $DATAFILE not found! Please run the C++ benchmark first."
    exit 1
fi

echo "Fitting curve and generating plot..."

# 2. Feed commands to gnuplot using a Heredoc (EOF)
gnuplot <<-EOF
    # Set the output format to PNG
    set terminal pngcairo size 800,600 enhanced
    set output 'benchmark_plot.png'

    # Graph styling
    set title 'Bubble Sort Performance' font ',18'
    set xlabel 'Input Size (N)' font ',16'
    set ylabel 'Time (seconds)' font ',16'

    set xtics font ",14"
    set ytics font ",14"

    set grid
    set key left top font ",14"

    # Fit a quadratic function to the data
    f(x) = a * x**2 + b * x + c
    fit f(x) '$DATAFILE' using 1:2 via a, b, c

    # Plot the raw data points and the fitted curve
    plot \
        '$DATAFILE' using 1:2 with points pt 7 ps 1.2 lc rgb '#0066cc' title 'Measured Time', \
        f(x) with lines lw 2 lc rgb '#cc0000' title 'Quadratic Fit (O(N^2))'
EOF

echo "Success! The plot has been saved as 'benchmark_plot.png'."