#include <iostream>
#include <string>
#include <sstream>
#include <assert.h>
#include <algorithm>

// #define DEBUG

typedef struct point_t {
    int p;
    int q;
} point_t;

using namespace std;

/// @brief read standard input and create array of points
/// @param num_points number of points to read
void processInput(int num_points, point_t arr[]) {
    string point_line; 
    for (int i = 0; i < num_points; i++) {
        getline(cin, point_line);
        arr[i].q = std::stoi(point_line);
    }
    for (int i = 0; i < num_points; i++) {
        getline(cin, point_line);
        arr[i].p = std::stoi(point_line);
    }
}

/// @brief merge two subarrays in-place, while also counting inversions
/// @param as the start of the 1st subarray
/// @param c the index of the last element in the 1st subarray + 1, and the index of the 1st element in the 2nd subarray
/// @param be the index of the last element in the 2nd subarray + 1
/// @return the number of new inversions after merging the subarrays
int merge(point_t arr[], int as, int c, int be) {
    #ifdef DEBUG
    cout << "as,c,be:" << as << "," << c << "," << be << endl;
    // sanity checks to make sure countInversions hasn't given us anything weird
    assert(as < c);
    assert(be >= c);
    #endif // DEBUG
    // this represents the combination of the two subarrays which we we will copy the merged result to
    int arr_s_len = be - as;
    point_t arr_s[arr_s_len];

    uint64_t inv_cnt = 0;

    int i = as;
    int j = c;
    int k = 0;
    while (i < c && j < be) {
        if (arr[j].p < arr[i].p) {
            inv_cnt += c - i; // remaining length of A
            arr_s[k] = arr[j];
            j++;
        } else {
            arr_s[k] = arr[i];
            i++;
        }
        k++;
    }

    // only one of these will trigger
    // these are to finish copying the other subarray based on which finished first
    while (i < c) {
        arr_s[k] = arr[i];
        i++;
        k++;
    }

    while (j < be) {
        arr_s[k] = arr[j];
        j++;
        k++;
    }

    // now copy it back to our original array
    for (k = as; k < be; k++) {
        point_t temp = arr_s[k - as];
        #ifdef DEBUG
        cout << temp << " ";
        #endif // DEBUG
        arr[k] = temp;
    }
    #ifdef DEBUG
    cout << endl;
    #endif // DEBUG

    return inv_cnt;
}

/// @brief count intersections in an array of points, while also sorting it by p-value
/// @param num_points number of points to count
uint64_t countIntersections(int num_points, point_t arr[]) {
    uint64_t inv_cnt = 0;
    for (int slice_size = 1; slice_size < num_points; slice_size *= 2) {
        for (int slice_ind = 0; slice_ind + slice_size <= num_points; slice_ind += slice_size * 2) {
            inv_cnt += merge(arr, slice_ind, slice_ind + slice_size, min(slice_ind + slice_size*2, num_points));
        }
    }
    return inv_cnt;
}

int main() {
    string curr_line;

    getline(cin, curr_line);
    int num_instances = std::stoi(curr_line);

    for (int i = 0; i < num_instances; i++) {
        getline(cin, curr_line);
        int num_points = std::stoi(curr_line);
        point_t arr[num_points];
        processInput(num_points, arr);

        // Want to sort by q first. This makes it so that when we go to count inversions of p,
        // the "indices" we're referencing against are in fact q
        // Basically: an inversion is defined as i > j and p_i < p_j
        // In a list sorted by q, i > j iff q_i > q_j
        // Thus, we have q_i > q_j for every inversion found in the sorted by q list
        // A pair of points where q_i > q_j and p_i < p_j is an intersection
        // O(nlogn) worst case
        sort(arr,arr+num_points, 
            [](point_t pt1, point_t pt2) {
                return pt1.q < pt2.q;
            }
        );

        cout << countIntersections(num_points, arr) << endl;
    }

    return 0;
}