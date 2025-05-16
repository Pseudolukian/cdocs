#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <vector>
#include <thread>
#include <functional>
#include <algorithm>

class ThreadPool {
private:
    std::vector<std::thread> workers;

public:
    ThreadPool() = default;

    // Execute tasks across multiple threads
    template<typename Func>
    void execute(const std::vector<std::string>& files, Func task) {
        // Get number of available CPU cores
        unsigned int num_cores = std::max(1u, std::thread::hardware_concurrency());
        
        // Calculate base number of files per thread and remainder
        size_t files_count = files.size();
        size_t files_per_thread = files_count / num_cores;
        size_t remainder = files_count % num_cores;

        // Clear any existing workers
        workers.clear();

        size_t start_idx = 0;
        for (unsigned int i = 0; i < num_cores && start_idx < files_count; ++i) {
            // Calculate number of files for this thread
            size_t current_files = files_per_thread;
            if (i == 0) {
                current_files += remainder; // Add remainder to first thread
            }

            // Create sub-range for this thread
            size_t end_idx = std::min(start_idx + current_files, files_count);
            
            // Create thread for this batch of files
            workers.emplace_back([start_idx, end_idx, &files, &task]() {
                for (size_t j = start_idx; j < end_idx; ++j) {
                    task(files[j]);
                }
            });

            start_idx = end_idx;
        }
    }

    // Wait for all threads to complete
    void wait() {
        for (auto& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
        workers.clear();
    }

    ~ThreadPool() {
        wait();
    }
};

#endif // THREADPOOL_HPP