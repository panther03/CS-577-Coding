#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <assert.h>

//#define DEBUG

typedef struct item_t {
    int weight;
    int value;
} item_t;

using namespace std;

/// @brief read standard input and create array
/// @param num_items number of knapsack items to process
void processInput(int num_intervals, item_t arr[]) {
    string temp;
    for (int i = 0; i < num_intervals; i++) {
        getline(cin, temp, ' ');
        int weight = std::stoi(temp);
        getline(cin, temp);
        int value = std::stoi(temp);

        arr[i] = item_t {weight, value};
    }
}

/// @brief determine maximum value subset/knapsack items
/// @param num_intervals number of intervals to schedule
/// @param arr array of sorted intervals
int knapsack(int num_items, int capacity, item_t arr[]) {
    // subset of first i items of maximum sum <= w
    int v[num_items+1][capacity+1];
    for (int i = 0; i < num_items+1; i++)
        v[i][0] = 0; 
    for (int w = 0; w < capacity+1; w++)
        v[0][w] = 0;

    #ifdef DEBUG
    cout << num_items+1 << endl;
    cout << capacity+1 << endl;
    #endif // DEBUG

    for (int i = 1; i < num_items+1; i++) {
        int wi = arr[i-1].weight;
        int vi = arr[i-1].value;
        #ifdef DEBUG
        cout << wi << ' ' << vi << endl;
        #endif // DEBUG
        for (int w = 1; w < capacity+1; w++) {
            v[i][w] = max(v[i-1][w],\
                    (wi > w) ? 0 : (v[i-1][w-wi]+vi));
        }
    }
    return v[num_items][capacity];
}

int main() {
    string curr_line;
    string temp;

    getline(cin, curr_line);
    int num_instances = std::stoi(curr_line);

    for (int i = 0; i < num_instances; i++) {
        getline(cin, curr_line, ' ');
        int num_items = std::stoi(curr_line);
        getline(cin, curr_line);
        int capacity = std::stoi(curr_line);

        item_t arr[num_items];
        processInput(num_items, arr);

        cout << knapsack(num_items, capacity, arr) << endl;
    }

    return 0;
}