#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>

std::mutex mtx; // Mutex for thread synchronization

// Function to process a chunk of data
void processChunk(std::vector<int>& data, int start, int end, int& result) {
    int sum = 0;
    for (int i = start; i < end; ++i) {
        sum += data[i]; // Simulating data processing
    }
    
    std::lock_guard<std::mutex> lock(mtx); // Prevent race conditions
    result += sum;
}

int main() {
    std::vector<int> dataset;
    int value;

    // Read dataset from a file
    std::ifstream file("dataset.txt");
    while (file >> value) {
        dataset.push_back(value);
    }
    file.close();

    int numThreads = std::thread::hardware_concurrency(); // Get available threads
    std::vector<std::thread> threads;
    int result = 0;
    int chunkSize = dataset.size() / numThreads;

    // Launch threads
    for (int i = 0; i < numThreads; ++i) {
        int start = i * chunkSize;
        int end = (i == numThreads - 1) ? dataset.size() : start + chunkSize;
        threads.emplace_back(processChunk, std::ref(dataset), start, end, std::ref(result));
    }

    // Join threads
    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Processed Result: " << result << std::endl;
    return 0;
}
