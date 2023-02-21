#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <deque>

// #define DEBUG

using namespace std;

typedef struct {
    deque<int> requests;
    int loc_in_cache;
} page_entry_t;

typedef unordered_map<int, page_entry_t> page_table_t;

/// @brief read standard input and create a page table
/// @param num_requests number of lines to read 
/// @return the "page table", ready to be consumed by the paging simulation
page_table_t processInput(int num_requests, vector<int> &requests) {
    int node_counter = 0;
    page_table_t page_table;

    string curr_line; 
    getline(cin, curr_line);
    stringstream curr_line_stream(curr_line);
    for (int t = 0; t < num_requests; t++) {
        string page_str;
        curr_line_stream >> page_str;
        int page = std::stoi(page_str);
        auto page_search = page_table.find(page);
        if (page_search == page_table.end()) {
            #ifdef DEBUG
            cout << "did not find page " << page << endl;
            #endif // DEBUG
            deque<int> d{t};
            page_table[page] = (page_entry_t) {
                requests: d,
                loc_in_cache: -1
            };
        } else {
            #ifdef DEBUG
            cout << "found page " << page << endl;
            #endif // DEBUG
            page_search->second.requests.push_back(t);
        }
        requests.push_back(page);
    }

    return page_table;
}

/// @brief paging simulation based on furthest-in-future heuristic
/// @param page_table the prepared page table ready to be simulated
int furthestInFuture(page_table_t page_table, vector<int> requests, int cache_size) {
    int ctr = 0; // number of misses
    auto it = requests.begin();

    int cache[cache_size];
    #ifdef DEBUG
    for (int i = 0; i < cache_size; i++) {
        cache[i] = -1;
    }
    #endif // DEBUG
    int filled_yet = 0;

    // TODO: refactor this so that you always just fill the cache first instead
    // of forcing every request to do a branch if it's not full
    while (it != requests.end()) {
        #ifdef DEBUG
        cout << "Request: " << *it;
        cout << " Cache Start: ";
        for (int i = 0; i < cache_size; i++) {
            cout << cache[i] << " ";
        }
        #endif // DEBUG

        auto page_search = page_table.find(*it);
        if (page_search == page_table.end()) {
            cout << "something went wrong" << endl;
        } else {
            if (page_search->second.loc_in_cache == -1) {
                // Do we have to evict a page?
                if (filled_yet >= cache_size) {
                    // Process the eviction
                    page_entry_t *fif_page; // furthest in the future
                    int fif_page_req = 0;
                    int fif_page_i = 0;
                    for (int i = 0; i < cache_size; i++) {
                        int c_i = cache[i];
                        auto page_search_ev = page_table.find(c_i);
                        if (page_search_ev != page_table.end()) {
                            if (page_search_ev->second.requests.empty()) {
                                fif_page = &page_search_ev->second;
                                fif_page_i = i;
                                #ifdef DEBUG
                                cout << " Empty Req ";
                                #endif // DEBUG
                                break; // don't care about the rest of the elements. we found an empty one
                            }
                            auto page_search_ev_req = *(page_search_ev->second.requests.begin());
                            if (page_search_ev_req > fif_page_req) {
                                fif_page_req = page_search_ev_req;
                                fif_page = &page_search_ev->second;
                                fif_page_i = i;
                            }
                        }
                    }
                    #ifdef DEBUG
                    cout << " Miss, evict " << cache[fif_page_i] << endl;
                    #endif // DEBUG
                    // TODO: pop requests as we make them?
                    fif_page->loc_in_cache = -1;
                    page_search->second.loc_in_cache = fif_page_i;
                    cache[fif_page_i] = *it;
                    
                    //fif_page_req.requests.pop();
                } else {
                    cache[filled_yet] = *it;
                    page_search->second.loc_in_cache = filled_yet;
                    filled_yet++;
                    #ifdef DEBUG
                    cout << " Fill cache" << endl;
                    #endif // DEBUG
                }
                ctr++; // a miss
            } else {
                #ifdef DEBUG
                cout << " Hit!" << endl;
                #endif // DEBUG
            } // else it's a hit in the cache so there's not anything extra to do
            // we always need to pop the request from the corresponding element
            page_search->second.requests.pop_front();
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
        int cache_size = std::stoi(curr_line);
        getline(cin, curr_line);
        int num_requests = std::stoi(curr_line);
        vector<int> requests;
        auto page_table = processInput(num_requests, requests);

        cout << furthestInFuture(page_table, requests, cache_size) << endl;
    }

    return 0;
}