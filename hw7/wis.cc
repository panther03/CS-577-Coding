#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <assert.h>

//#define DEBUG

typedef struct interval_t {
    int start;
    int end;
    int weight;
} interval_t;

using namespace std;

/// @brief read standard input and create array
/// @param num_intervals number of intervals to process
void processInput(int num_intervals, interval_t arr[]) {
    string interval_line; 
    string interval_temp;
    for (int i = 0; i < num_intervals; i++) {
        getline(cin, interval_line);
        stringstream interval_stream(interval_line);

        int start, end, weight;
        interval_stream >> interval_temp;
        start = std::stoi(interval_temp);
        interval_stream >> interval_temp;
        end = std::stoi(interval_temp);
        interval_stream >> interval_temp;
        weight = std::stoi(interval_temp);
        arr[i] = interval_t {start, end, weight};
    }
}

/// @brief schedule intervals to produce the maximum value schedule
/// @param num_intervals number of intervals to schedule
/// @param arr array of sorted intervals
uint64_t maximumSchedule(int num_intervals, interval_t arr[]) {
    uint64_t M[num_intervals];
    // M[n] = maximum value of a compatible schedule for the first n+1 items in arr
    M[0] = arr[0].weight;
    for (int j = 1; j < num_intervals; j++) {
        int i = 0;
        int ip = -1;
        while (i < j && (arr[i].end <= arr[j].start)) {
            ip = i;
            i++;
        }
        #ifdef DEBUG
        assert(ip < num_intervals);
        assert(j < num_intervals && j >= 0);
        printf("ip = %d\n", ip);
        #endif // DEBUG
        M[j] = max(M[j-1], ((ip == -1) ? 0 : M[ip]) + arr[j].weight);
        
        #ifdef DEBUG
        printf("M[%d] = maximum: %d\n", j, M[j]);
        #endif // DEBUG
    }
    return M[num_intervals - 1];
}

int main() {
    string curr_line;

    getline(cin, curr_line);
    int num_instances = std::stoi(curr_line);

    for (int i = 0; i < num_instances; i++) {
        getline(cin, curr_line);
        int num_intervals = std::stoi(curr_line);
        interval_t arr[num_intervals];
        processInput(num_intervals, arr);

        
        // Preprocess intervals by sorting by finish time
        sort(arr,arr+num_intervals, 
            [](interval_t t1, interval_t t2) {
                return t1.end < t2.end;
            }
        );

        #ifdef DEBUG
        for (int j = 0; j < num_intervals; j++) {
            printf("%d %d %d\n", arr[j].start, arr[j].end, arr[j].weight);
        }
        #endif // DEBUG

        cout << maximumSchedule(num_intervals, arr) << endl;
    }

    return 0;
}