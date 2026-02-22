// g++ benchmark_bubblesort.cpp -o benchmark_bubblesort.bin -O3 && ./benchmark_bubblesort.bin

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <fstream>
#include <iomanip>

void bubbleSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                // std::swap(arr[j], arr[j + 1]);
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 10000);

    std::string filename = "benchmark_bubblesort.dat";
    std::ofstream outfile(filename);

    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
        return 1;
    }

    outfile << "# Input_Size(N) Time(seconds)\n";

    std::cout << "Running Benchmark and saving to " << filename << "..." << std::endl;

    for (int n = 500; n <= 20000; n += 500) {
        std::vector<int> data(n);
        for(int& x : data) x = distrib(gen);

        auto start = std::chrono::high_resolution_clock::now();
        
        bubbleSort(data);
        
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        outfile << n << " " << std::fixed << std::setprecision(6) << elapsed.count() << "\n";
        
        std::cout << "N=" << n << " took " << elapsed.count() << "s" << std::endl;
    }

    outfile.close();
    std::cout << "Benchmark complete. Results successfully saved." << std::endl;

    return 0;
}