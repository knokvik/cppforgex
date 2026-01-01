nirajrajendranaphade@Nirajs-MacBook-Air build % clear
nirajrajendranaphade@Nirajs-MacBook-Air build % cmake ..
-- Google Benchmark version: v1.9.5-59-g83c826c8, normalized to 1.9.5.59
-- Configuring done (0.1s)
-- Generating done (0.0s)
-- Build files have been written to: /Users/nirajrajendranaphade/Programming/cmake_tut/libs/build
nirajrajendranaphade@Nirajs-MacBook-Air build % make
[ 76%] Built target benchmark
[ 80%] Building CXX object CMakeFiles/EngineMasterclass.dir/main.cpp.o
.[ 84%] Linking CXX executable EngineMasterclass
[ 84%] Built target EngineMasterclass
/[ 92%] Built target benchmark_main
[100%] Built target vector_bench
nirajrajendranaphade@Nirajs-MacBook-Air build % ./EngineMasterclass 
Unable to determine clock rate from sysctl: hw.cpufrequency: No such file or directory
This does not affect benchmark measurements, only the metadata output.
***WARNING*** Failed to set thread affinity. Estimated CPU frequency may be incorrect.
2026-06-07T17:04:35+05:30
Running ./EngineMasterclass
Run on (10 X 24 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB
  L1 Instruction 128 KiB
  L2 Unified 4096 KiB (x10)
Load Average: 1.81, 2.03, 1.87
-------------------------------------------------------------------
Benchmark                         Time             CPU   Iterations
-------------------------------------------------------------------
BM_ParticleUpdate/1000          369 ns          369 ns      1838626
BM_ParticleUpdate/10000        8249 ns         8248 ns        84623
BM_ParticleUpdate/100000      85412 ns        83871 ns         8409
nirajrajendranaphade@Nirajs-MacBook-Air build % cmake ..
-- Google Benchmark version: v1.9.5-59-g83c826c8, normalized to 1.9.5.59
-- Configuring done (0.1s)
-- Generating done (0.0s)
-- Build files have been written to: /Users/nirajrajendranaphade/Programming/cmake_tut/libs/build
nirajrajendranaphade@Nirajs-MacBook-Air build % make 
[ 76%] Built target benchmark
[ 80%] Building CXX object CMakeFiles/EngineMasterclass.dir/main.cpp.o
[ 84%] Linking CXX executable EngineMasterclass
[ 84%] Built target EngineMasterclass
[ 92%] Built target benchmark_main
[100%] Built target vector_bench
nirajrajendranaphade@Nirajs-MacBook-Air build % ./EngineMasterclass 
Unable to determine clock rate from sysctl: hw.cpufrequency: No such file or directory
This does not affect benchmark measurements, only the metadata output.
***WARNING*** Failed to set thread affinity. Estimated CPU frequency may be incorrect.
2026-06-07T17:04:49+05:30
Running ./EngineMasterclass
Run on (10 X 24 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB
  L1 Instruction 128 KiB
  L2 Unified 4096 KiB (x10)
Load Average: 1.85, 2.03, 1.87
-------------------------------------------------------------------
Benchmark                         Time             CPU   Iterations
-------------------------------------------------------------------
BM_ParticleUpdate/1000          272 ns          272 ns      2576589
BM_ParticleUpdate/10000       12750 ns        12558 ns        56671
BM_ParticleUpdate/100000     139771 ns       138866 ns         4973
nirajrajendranaphade@Nirajs-MacBook-Air build % 