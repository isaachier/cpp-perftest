#include <cstdlib>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include <benchmark/benchmark.h>

namespace {

using StrVec = std::vector<std::string>;

template <typename Distribution, typename RNG>
static std::string randomNumberString(Distribution& dist, RNG& rng)
{
    constexpr auto numDigits = 5;
    std::string result;
    const auto positive = dist(rng) < 5;
    if (!positive) {
        result += '-';
    }
    for (auto i = 0; i < numDigits; ++i) {
        auto digit = dist(rng);
        if (i == 0 && digit == 0) {
            digit = 1;
        }
        result += '0' + digit;
    }
    return result;
}

static StrVec makeRandomStrVec(int numStrings)
{
    std::random_device device;
    std::mt19937 rng(device());
    std::uniform_int_distribution<> dist(0, 9);
    StrVec result;
    for (auto i = 0; i < numStrings; ++i) {
        result.emplace_back(randomNumberString(dist, rng));
    }
    return result;
}

static const StrVec& data()
{
    constexpr auto kNumStrings = 1000;
    static StrVec dataSet(makeRandomStrVec(kNumStrings));
    return dataSet;
}

void readFromString(benchmark::State& state)
{
    for (auto _ : state) {
        for (auto&& str : data()) {
            benchmark::DoNotOptimize(std::atoi(str.c_str()));
        }
    }
}

void readFromStream(benchmark::State& state)
{
    for (auto _ : state) {
        for (auto&& str : data()) {
            std::istringstream iss(str);
            auto result = 0;
            benchmark::DoNotOptimize(iss >> result);
            (void)result;
        }
    }
}

}  // anonymous namespace

BENCHMARK(readFromString)->Arg(10);
BENCHMARK(readFromStream)->Arg(10);

BENCHMARK_MAIN()
