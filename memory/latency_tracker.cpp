#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <thread>

class LatencyTracker {
private:
    std::vector<double> m_samples; // Bucket storage for raw millisecond timings

public:
    void logSample(double ms) {
        m_samples.push_back(ms);
    }

    void reportPercentiles() {
        if (m_samples.empty()) return;

        // 1. Sort the entire collection from fastest to slowest
        std::sort(m_samples.begin(), m_samples.end());

        size_t size = m_samples.size();
        
        // 2. Calculate the exact index positions for our target percentiles
        double p50 = m_samples[static_cast<size_t>(size * 0.50)];
        double p95 = m_samples[static_cast<size_t>(size * 0.95)];
        double p99 = m_samples[static_cast<size_t>(size * 0.99)];

        std::cout << "--- Latency Distribution Metrics ---\n";
        std::cout << "p50 (Median Performance) : " << p50 << " ms\n";
        std::cout << "p95 (Slowest 5% Barrier) : " << p95 << " ms\n";
        std::cout << "p99 (Worst Case Outliers): " << p99 << " ms\n";
    }
};

int main() {
    LatencyTracker tracker;

    // Simulate 100 fast system operations (1ms each)
    for (int i = 0; i < 100; ++i) {
        tracker.logSample(1.0);
    }

    // Simulate 5 slower operations due to occasional hardware delays (25ms each)
    for (int i = 0; i < 5; ++i) {
        tracker.logSample(25.0);
    }

    // Simulate 1 massive outlier freeze (500ms)
    tracker.logSample(500.0);

    // See the real story of our software execution footprint
    tracker.reportPercentiles();

    return 0;
}
