#include <benchmark/benchmark.h>
#include <vector>
#include <random>

struct Particle
{
    int x;
    int y;
    int vx;
    int vy;
    char padding[128];
};

static void BM_ParticleUpdate(
    benchmark::State& state)
{
    const int n = state.range(0);

    std::vector<Particle> particles(n);

    std::mt19937 gen(42);
    std::uniform_int_distribution<int>
        distrib(1,10000);

    for(auto& p : particles)
    {
        p.x = distrib(gen);
        p.y = distrib(gen);
        p.vx = distrib(gen);
        p.vy = distrib(gen);
    }

    for(auto _ : state)
    {
        for(auto& p : particles)
        {
            p.x += p.vx;
            p.y += p.vy;
        }

        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_ParticleUpdate)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000);

BENCHMARK_MAIN();// Cleaned up includes
