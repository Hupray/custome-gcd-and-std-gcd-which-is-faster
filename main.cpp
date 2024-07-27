#include <iostream>
#include <algorithm>
#include <chrono>
#include <random>

///*
#pragma GCC optimize("O3")
#pragma GCC optimize("O1")
#pragma GCC optimize("O1")
#pragma GCC target("avx2")
#pragma GCC optimize("unroll-loops")
#pragma GCC target("avx")
#pragma GCC optimize(3)
#pragma GCC optimize("Ofast")
#pragma GCC optimize("inline")
#pragma GCC optimize("-fgcse")
#pragma GCC optimize("-fgcse-lm")
#pragma GCC optimize("-fipa-sra")
#pragma GCC optimize("-ftree-pre")
#pragma GCC optimize("-ftree-vrp")
#pragma GCC optimize("-fpeephole2")
#pragma GCC optimize("-ffast-math")
#pragma GCC optimize("-fsched-spec")
#pragma GCC optimize("-falign-jumps")
#pragma GCC optimize("-falign-loops")
#pragma GCC optimize("-falign-labels")
#pragma GCC optimize("-fdevirtualize")
#pragma GCC optimize("-fcaller-saves")
#pragma GCC optimize("-fcrossjumping")
#pragma GCC optimize("-fthread-jumps")
#pragma GCC optimize("-freorder-blocks")
#pragma GCC optimize("-fschedule-insns")
#pragma GCC optimize("inline-functions")
#pragma GCC optimize("-ftree-tail-merge")
#pragma GCC optimize("-fschedule-insns2")
#pragma GCC optimize("-fstrict-aliasing")
#pragma GCC optimize("-falign-functions")
#pragma GCC optimize("-fcse-follow-jumps")
#pragma GCC optimize("-fsched-interblock")
#pragma GCC optimize("-fpartial-inlining")
#pragma GCC optimize("no-stack-protector")
#pragma GCC optimize("-freorder-functions")
#pragma GCC optimize("-findirect-inlining")
#pragma GCC optimize("-fhoist-adjacent-loads")
#pragma GCC optimize("-frerun-cse-after-loop")
#pragma GCC optimize("inline-small-functions")
#pragma GCC optimize("-finline-small-functions")
#pragma GCC optimize("-ftree-switch-conversion")
#pragma GCC optimize("-foptimize-sibling-calls")
#pragma GCC optimize("-fexpensive-optimizations")
#pragma GCC optimize("inline-functions-called-once")
#pragma GCC optimize("-fdelete-null-pointer-checks")
#pragma GCC target("avx,avx2,sse,sse2,sse3,ssse3,sse4,sse4.1,sse4.2,mmx,abm")
//*/

using namespace std;
using namespace std::chrono;

// Custom GCD function
long long custom_gcd(long long a, long long b) {
    long long tmp;
    while (b > 0) {
        if (b > a) swap(a, b);
        tmp = a % b;
        a = b;
        b = tmp;
    }
    return a;
}

// Function to generate random numbers in the range from 1 to max_value
long long random_number(long long max_value) {
    static random_device rd;
    static mt19937_64 gen(rd());
    uniform_int_distribution<long long> dist(1, max_value);
    return dist(gen);
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    const int num_tests = 1e3; // Number of queries in each trial
    const long long max_value = 1e18; // Maximum value for a and b
    const int num_trials = 1e3; // Number of trials

    int custom_faster_count = 0; // Count the number of times custom_gcd is faster
    double total_custom_percentage = 0.0; // Percentage of time custom_gcd is faster

    long long total_duration_std_gcd = 0; // Total time for all __gcd executions
    long long total_duration_custom_gcd = 0; // Total time for all custom_gcd executions

    for (int trial = 0; trial < num_trials; ++trial) {
        // Variables to store total time for each trial
        long long total_time_std_gcd = 0;
        long long total_time_custom_gcd = 0;

        for (int i = 0; i < num_tests; ++i) {
            long long a = random_number(max_value);
            long long b = random_number(max_value);

            // Measure time for __gcd
            auto start1 = high_resolution_clock::now();
            long long result1 = __gcd(a, b);
            auto stop1 = high_resolution_clock::now();
            auto duration1 = duration_cast<nanoseconds>(stop1 - start1);
            total_time_std_gcd += duration1.count();

            // Measure time for custom_gcd
            auto start2 = high_resolution_clock::now();
            long long result2 = custom_gcd(a, b);
            auto stop2 = high_resolution_clock::now();
            auto duration2 = duration_cast<nanoseconds>(stop2 - start2);
            total_time_custom_gcd += duration2.count();

            // Check correctness
            if (result1 != result2) {
                cout << "Error: Results differ for " << a << " and " << b << endl;
                cout << "Result __gcd: " << result1 << ", Result custom_gcd: " << result2 << endl;
                return -1; // End program if there is an error
            }
        }

        total_duration_std_gcd += total_time_std_gcd;
        total_duration_custom_gcd += total_time_custom_gcd;

        if (total_time_custom_gcd < total_time_std_gcd) {
            custom_faster_count++;
            double percentage_faster = 100.0 * (total_time_std_gcd - total_time_custom_gcd) / total_time_std_gcd;
            total_custom_percentage += percentage_faster;
        }
    }

    // Calculate the overall percentage and number of times custom_gcd is faster
    double average_custom_percentage = custom_faster_count > 0 ? total_custom_percentage / custom_faster_count : 0.0;

    cout << "Number of times custom_gcd is faster: " << custom_faster_count << " out of " << num_trials << " trials" << endl;
    cout << "Percentage of times custom_gcd is faster: " << (100.0 * custom_faster_count / num_trials) << "%" << endl;
    cout << "On average, custom_gcd is faster by: " << average_custom_percentage << "%" << endl;
    cout << "Total time taken by __gcd: " << total_duration_std_gcd << " nanoseconds" << endl;
    cout << "Total time taken by custom_gcd: " << total_duration_custom_gcd << " nanoseconds" << endl;

    return 0;
}
