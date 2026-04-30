// =============================================================================
//  Radix sort for non-negative integers — least-significant-digit (LSD) variant.
// =============================================================================
//
//  Time:  O(d · (n + k))   where d = #digits in the largest element,
//                              k = base (10 here)
//  Space: O(n + k)         (counting array + temporary output buffer)
//
//  How it works
//  ------------
//  For each digit position from least- to most-significant, we run a stable
//  counting sort on that single digit.  Stability is essential because each
//  pass relies on the previous pass already having ordered the lower digits.
//
//  Key invariant after pass `place`: the array is sorted by the bottom
//  `log10(place * 10)` digits of every element.  After the final pass, the
//  whole array is sorted.
//
//  Build & run:
//      g++ -std=c++17 -O2 -Wall radix_sort.cpp -o radix_sort && ./radix_sort
// =============================================================================

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

// One stable counting-sort pass on a single base-10 digit.
static void counting_pass(vector<int>& arr, int place) {
    vector<int> output(arr.size());
    vector<int> count(10, 0);

    for (size_t i = 0; i < arr.size(); ++i)
        count[(arr[i] / place) % 10]++;

    for (int i = 1; i < 10; ++i)
        count[i] += count[i - 1];

    for (int i = static_cast<int>(arr.size()) - 1; i >= 0; --i) {
        int digit = (arr[i] / place) % 10;
        output[--count[digit]] = arr[i];
    }
    arr = std::move(output);
}

void radix_sort(vector<int>& arr) {
    if (arr.empty()) return;
    int maxv = *max_element(arr.begin(), arr.end());
    for (int place = 1; maxv / place > 0; place *= 10)
        counting_pass(arr, place);
}

static void print_vector(const vector<int>& v) {
    for (size_t i = 0; i < v.size(); ++i) {
        cout << v[i];
        if (i + 1 < v.size()) cout << ", ";
    }
    cout << "\n";
}

int main() {
    // --- deterministic small example ----------------------------------------
    vector<int> hand = { 170, 45, 75, 90, 802, 24, 2, 66 };
    cout << "input:  "; print_vector(hand);
    radix_sort(hand);
    cout << "sorted: "; print_vector(hand);
    cout << "\n";

    // --- 10 000-element random check ----------------------------------------
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(0, 1'000'000);
    vector<int> big(10'000);
    for (auto& x : big) x = dist(rng);
    radix_sort(big);
    bool ok = is_sorted(big.begin(), big.end());
    cout << "10 000-element random sort sanity check: " << (ok ? "OK" : "FAIL") << "\n";
    return ok ? 0 : 1;
}
