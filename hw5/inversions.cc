#include <iostream>
#include <string>
#include <sstream>
#include <assert.h>

// #define DEBUG

using namespace std;

/// @brief read standard input and create array
/// @param num_elements number of elements in the array to sort
void processInput(int num_elements, int arr[]) {
    string elems_line; 
    getline(cin, elems_line);
    stringstream elem_stream(elems_line);
    for (int i = 0; i < num_elements; i++) {
        string elem_str;
        elem_stream >> elem_str;
        arr[i] = std::stoi(elem_str);
    }
}

/// @brief merge two subarrays in-place, while also counting inversions
/// @param as the start of the 1st subarray
/// @param c the index of the last element in the 1st subarray + 1, and the index of the 1st element in the 2nd subarray
/// @param be the index of the last element in the 2nd subarray + 1
/// @return the number of new inversions after merging the subarrays
int merge(int arr[], int as, int c, int be) {
    #ifdef DEBUG
    cout << "as,c,be:" << as << "," << c << "," << be << endl;
    // sanity checks to make sure countInversions hasn't given us anything weird
    assert(as < c);
    assert(be >= c);
    #endif // DEBUG
    // this represents the combination of the two subarrays which we we will copy the merged result to
    int arr_s_len = be - as;
    int arr_s[arr_s_len];

    int inv_cnt = 0;

    int i = as;
    int j = c;
    int k = 0;
    while (i < c && j < be) {
        if (arr[j] < arr[i]) {
            // significant inversion -> if (2 * arr[j] < arr[i])
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
        int temp = arr_s[k - as];
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

/// @brief count inversions in an array, while also sorting it
/// @param num_elements number of elements in the array to sort
int countInversions(int num_elements, int arr[]) {
    int inv_cnt = 0;
    for (int slice_size = 1; slice_size < num_elements; slice_size *= 2) {
        for (int slice_ind = 0; slice_ind + slice_size <= num_elements; slice_ind += slice_size * 2) {
            inv_cnt += merge(arr, slice_ind, slice_ind + slice_size, min(slice_ind + slice_size*2, num_elements));
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
        int num_elements = std::stoi(curr_line);
        int arr[num_elements];
        processInput(num_elements, arr);

        cout << countInversions(num_elements, arr) << endl;
    }

    return 0;
}