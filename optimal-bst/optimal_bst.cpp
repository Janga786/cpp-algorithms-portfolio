#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Item {
    string key;
    double prob;
};

bool compareItems(const Item &a, const Item &b) {
    return a.key < b.key;
}

int main() {
    vector<Item> items = {
            {"cat", 0.15},
            {"dog", 0.1},
            {"fish", 0.05},
            {"lion", 0.1},
            {"mouse", 0.2},
            {"zebra", 0.05},
            {"bird", 0.1},
            {"frog", 0.05},
            {"ant", 0.1},
            {"cow", 0.1}
    };

    sort(items.begin(), items.end(), compareItems);

    int N = items.size();

    vector<string> keys(N);
    vector<double> p(N);

    for (int i = 0; i < N; i++) {
        keys[i] = items[i].key;
        p[i] = items[i].prob;
    }

    vector<vector<double>> cost(N, vector<double>(N, 0.0));
    vector<vector<double>> weight(N, vector<double>(N, 0.0));
    vector<vector<int>> root(N, vector<int>(N, 0));

    for (int i = 0; i < N; i++) {
        cost[i][i] = p[i];
        weight[i][i] = p[i];
        root[i][i] = i;
    }

    for (int k = 2; k <= N; k++) {
        for (int i = 0; i <= N - k; i++) {

            int j = i + k - 1;

            weight[i][j] = weight[i][j - 1] + p[j];
            cost[i][j] = 1000000;

            for (int r = i; r <= j; r++) {
                double leftCost = 0;

                if (r > i) {
                    leftCost = cost[i][r - 1];
                }

                double rightCost = 0;

                if (r < j) {
                    rightCost = cost[r + 1][j];
                }


                double totalCost = leftCost + rightCost + weight[i][j];

                if (totalCost < cost[i][j]) {
                    cost[i][j] = totalCost;
                    root[i][j] = r;
                }
            }
        }
    }

    cout << "Optimal BST cost: " << cost[0][N - 1] << endl;

    return 0;
}
