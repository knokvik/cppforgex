#include <benchmark/benchmark.h>
#include <random>
#include <algorithm>
#include <vector>
#include <cstdlib>

// Vectorizable Loop VS  Non-Vectorizable Loop 

static void BM_Vectorizable(benchmark::State& state)
{
    std::vector<float> a(state.range(0), 1.0f);
    std::vector<float> b(state.range(0), 2.0f);
    std::vector<float> c(state.range(0));

    for (auto _ : state)
    {
        for (size_t i = 0; i < a.size(); ++i)
        {
            c[i] = a[i] + b[i];
        }

        benchmark::DoNotOptimize(c);
    }
}

static void BM_NonVectorizable(benchmark::State& state)
{
    std::vector<float> a(state.range(0), 1.0f);
    std::vector<float> b(state.range(0), 2.0f);
    std::vector<float> c(state.range(0));

    for (auto _ : state)
    {
        for (size_t i = 0; i < a.size(); ++i)
        {
            if (a[i] > 0)
            {
                c[i] = a[i] + b[i];
            }
        }

        benchmark::DoNotOptimize(c);
    }
}

BENCHMARK(BM_Vectorizable)
    ->Arg(1000000);

BENCHMARK(BM_NonVectorizable)
    ->Arg(1000000);



// // Branch Prediction VS MisPrediction

// static void BM_PredictableBranch(benchmark::State& state)
// {
//     std::vector<int> v(state.range(0), 1);

//     for (auto _ : state)
//     {
//         int sum = 0;

//         for (int x : v)
//         {
//             if (x > 0)
//             {
//                 sum += x;
//             }
//         }

//         benchmark::DoNotOptimize(sum);
//     }
// }

// static void BM_RandomBranch(benchmark::State& state)
// {
//     std::vector<int> v(state.range(0));

//     for (auto& x : v)
//     {
//         x = rand() % 2;
//     }

//     for (auto _ : state)
//     {
//         int sum = 0;

//         for (int x : v)
//         {
//             if (x > 0)
//             {
//                 sum += x;
//             }
//         }

//         benchmark::DoNotOptimize(sum);
//     }
// }

// BENCHMARK(BM_PredictableBranch)
//     ->Arg(1000000);

// BENCHMARK(BM_RandomBranch)
//     ->Arg(1000000);

// Contiguous Memory VS Random Exp
// static void BM_SequentialAccess(benchmark::State& state)
// {
//     std::vector<int> v(state.range(0), 1);

//     for (auto _ : state)
//     {
//         int sum = 0;

//         for (size_t i = 0; i < v.size(); ++i)
//         {
//             sum += v[i];
//         }

//         benchmark::DoNotOptimize(sum);
//     }
// }

// static void BM_RandomAccess(benchmark::State& state)
// {
//     std::vector<int> v(state.range(0), 1);

//     std::vector<size_t> indices(v.size());

//     for (size_t i = 0; i < indices.size(); ++i)
//     {
//         indices[i] = i;
//     }

//     std::shuffle(indices.begin(), indices.end(),
//                  std::mt19937{std::random_device{}()});

//     for (auto _ : state)
//     {
//         int sum = 0;

//         for (size_t i = 0; i < indices.size(); ++i)
//         {
//             sum += v[indices[i]];
//         }

//         benchmark::DoNotOptimize(sum);
//     }
// }

// BENCHMARK(BM_SequentialAccess)
//     ->Arg(100000)
//     ->Arg(1000000);

// BENCHMARK(BM_RandomAccess)
//     ->Arg(100000)
//     ->Arg(1000000);