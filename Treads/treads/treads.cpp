#include <iostream>
#include <vector>
#include <thread>
#include <numeric> // ��� accumulate
#include <mutex>   // ��� mutex
#include <chrono>  // ��� ��������� �������

using namespace std;
using namespace std::chrono; // ��� ��������

// ���������� ���������� ��� ������������� ������
mutex output_mutex;

// ������� ��� ���������� ����� ����� �������
double partial_sum(const vector<double>& arr, size_t start, size_t end) {
    return accumulate(arr.begin() + start, arr.begin() + end, 0.0);
}

void thread_function(const vector<double>& arr, size_t start, size_t end, double& result) {
    result = partial_sum(arr, start, end);

    // ������������� ������
    {
        lock_guard<mutex> lock(output_mutex);
        cout << "Thread ID: " << this_thread::get_id() << ", Partial sum from " << start << " to " << end << ": " << result << endl;
    }
}

int main() {
    const size_t SIZE = 10'000'000; // ������ �������
    vector<double> arr(SIZE, 1.0); // ������������� ������� ���������� 1.0

    int mode;
    cout << "Choose mode (1 - single thread, 2 - multi-threaded): ";
    cin >> mode;

    if (mode == 1) {
        // ������������ ������
        auto start_time = high_resolution_clock::now(); // ������ ��������� �������
        double total_sum = partial_sum(arr, 0, SIZE);
        auto end_time = high_resolution_clock::now(); // ����� ��������� �������

        auto duration = duration_cast<microseconds>(end_time - start_time); // ���������� �����������������
        cout << "Total sum: " << total_sum << endl;
        cout << "Execution time (single thread): " << duration.count() << " microseconds" << endl;
    }
    else if (mode == 2) {
        // ������������� ������
        size_t num_threads;
        cout << "Enter the number of threads: ";
        cin >> num_threads;

        // �������� �� ������������ �����
        if (num_threads <= 0) {
            cerr << "Number of threads must be positive." << endl;
            return 1;
        }

        vector<thread> threads;
        vector<double> results(num_threads);
        size_t chunk_size = SIZE / num_threads;

        auto start_time = high_resolution_clock::now(); // ������ ��������� �������

        // ������ �������
        for (size_t i = 0; i < num_threads; ++i) {
            size_t start = i * chunk_size;
            size_t end = (i == num_threads - 1) ? SIZE : start + chunk_size; // ��������� ����� ����� �������
            threads.emplace_back(thread_function, ref(arr), start, end, ref(results[i]));
        }

        // �������� ���������� ���� �������
        for (auto& thread : threads) {
            thread.join();
        }

        // ��������� ����������
        double total_sum = accumulate(results.begin(), results.end(), 0.0);
        auto end_time = high_resolution_clock::now(); // ����� ��������� �������

        auto duration = duration_cast<microseconds>(end_time - start_time); // ���������� �����������������
        cout << "Total sum: " << total_sum << endl;
        cout << "Execution time (multi-threaded): " << duration.count() << " microseconds" << endl;
    }
    else {
        cerr << "Invalid mode selected." << endl;
        return 1;
    }

    return 0;
}