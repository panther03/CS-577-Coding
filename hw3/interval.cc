#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

typedef struct {
    int start;
    int end;
} interval_t;

/// @brief parse an interval structure from a string
/// @param interval_line the input string
/// @return a new interval struct
interval_t new_interval(string interval_line) {
    string start_s;
    string end_s;
    // are we on the string representing the end time yet.
    bool end_yet = false;
    for (auto it = interval_line.begin(); it != interval_line.end(); it++) {
        if (end_yet) {
            end_s += *it;
        } else {
            if (*it == ' ') {
                end_yet = true;
            } else {
                start_s += *it;
            }
        }
    }
    interval_t i = {
        start: std::stoi(start_s),
        end: std::stoi(end_s),
    };
    return i;
}

/// @brief read standard input and create a sorted list of intervals
/// @param num_intervals number of lines to read
/// @return vector of intervals from stdin sorted by end time soonest to latest
vector<interval_t> processInput(int num_intervals) {
    int node_counter = 0;
    vector<interval_t> intervals;

    for (int i = 0; i < num_intervals; i++) {
        string interval_line;
        getline(cin, interval_line);
        intervals.push_back(new_interval(interval_line));
    }

    // O(nlogn)
    sort(intervals.begin(),intervals.end(), 
        [](interval_t i1, interval_t i2) {
            return i1.end < i2.end;
        }
    );

    return intervals;
}

/// @brief interval scheduling algorithm
/// @param intervals the list of intervals sorted by end time
int countIntervals(vector<interval_t> intervals) {
    int ctr = 0; // how many intervals we have scheduled
    int currtime = 0; // the current schedule time
    auto it = intervals.begin();

    while (it != intervals.end()) {
        // is the next interval compatible?
        // if yes, schedule it. advance the current time
        // if no, look at the next schedule in the list
        if ((*it).start >= currtime) {
            ctr ++;
            currtime = (*it).end;
        }
        it++;
    }
    return ctr;
}

int main() {
    string curr_line;

    getline(cin, curr_line);
    int num_instances = std::stoi(curr_line);

    for (int i = 0; i < num_instances; i++) {
        getline(cin, curr_line);
        int num_intervals = std::stoi(curr_line);
        auto intervals = processInput(num_intervals);
        cout << countIntervals(intervals) << endl;
    }

    return 0;
}