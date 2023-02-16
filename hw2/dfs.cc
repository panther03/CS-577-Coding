#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std;

typedef struct {
    vector<int> adj;
    string start_node;
} adj_list_t;

void processInput(int num_nodes, unordered_map<string, int> &node_names_to_ind, adj_list_t adj_list[]) {
    int node_counter = 0;
    vector<string> adj_node_names[num_nodes];

    // ingest data first. record strings for each adjacent node
    // i could not think of a way to build the data structure with integers live,
    // because we need to store the order that these nodes are coming in
    for (int curr_node_ind = 0; curr_node_ind < num_nodes; curr_node_ind++) {
        string curr_node_line; 
        getline(cin, curr_node_line);
        stringstream curr_node_stream(curr_node_line);

        // Grab the first string. This is the node which connects to the remaining nodes
        string curr_node_name;
        curr_node_stream >> curr_node_name;
        // Set the name in the adj_list so DFS knows what to print out later
        adj_list[curr_node_ind].start_node = curr_node_name;
        // Record the index in the hashmap
        node_names_to_ind[curr_node_name] = curr_node_ind;

        // The rest to follow will be connected so we save their names too
        string curr_node_adj;
        while (curr_node_stream >> curr_node_adj) {
            adj_node_names[curr_node_ind].push_back(curr_node_adj);
        }
    }

    // now we will construct lists with integer values for DFS
    for (int curr_node_ind = 0; curr_node_ind < num_nodes; curr_node_ind++) {
        for (auto adj_node: adj_node_names[curr_node_ind]) {
            auto node_search = node_names_to_ind.find(adj_node);
            if (node_search == node_names_to_ind.end()) {
                cout << "Adjacent node not found in full node list.. input malformed?" << endl;
                exit(1);
            } else {
                adj_list[curr_node_ind].adj.push_back(node_search->second);
            }
        }
    }

}

void dfs(int num_nodes, adj_list_t adj_list[]) {
    if (num_nodes == 0) return;

    // processInput says that 0 should always be first node in input order
    int s = 0;

    bool visited[num_nodes];
    // i believe this is the upper limit for stack size. you should hit this for a complete graph
    int dfs_stack[(num_nodes-1) * (num_nodes-1)];
    int dfs_stack_ind = -1;
    int last_unvisited = 0;
    
    // initialize visited array
    for (int i = 0; i < num_nodes; i++) {
        visited[i] = false;
    }

    bool havePrintedFirstNode = false;

    while (last_unvisited < num_nodes) {
        // we don't know for a fact this is not visited
        // we are only able to check for one value being visited at a time
        // if we jump around in the sequence this variable won't record those others
        // but we can just skip over it if we see it marked as visited
        if (!visited[last_unvisited]) {
            // Start off the stack by pushing the last unvisited node
            dfs_stack_ind++;
            dfs_stack[dfs_stack_ind] = last_unvisited;
            while (dfs_stack_ind >= 0) {
                int u = dfs_stack[dfs_stack_ind];
                dfs_stack_ind--;
                if (!visited[u]) {
                    // don't want to have whitespace at the start
                    if (!havePrintedFirstNode) {
                        cout << adj_list[u].start_node;  
                        havePrintedFirstNode = true;
                    } else {
                        cout << " " << adj_list[u].start_node;    
                    }
                    
                    visited[u] = true;
                    // for each neighbor of u..
                    auto adj_node_iter = adj_list[u].adj.rbegin();
                    while (adj_node_iter != adj_list[u].adj.rend()) {
                        dfs_stack_ind++;
                        dfs_stack[dfs_stack_ind] = *adj_node_iter;
                        ++adj_node_iter;
                    }
                }
                if (last_unvisited == u) {
                    last_unvisited++;
                }
            }
        } else {
            last_unvisited++;
        }
    }

    cout << endl;
}

int main() {
    string curr_line;

    getline(cin, curr_line);
    int num_instances = std::stoi(curr_line);

    for (int i = 0; i < num_instances; i++) {
        getline(cin, curr_line);
        int num_nodes = std::stoi(curr_line);

        unordered_map<string, int> node_names_to_ind;
        adj_list_t adj_list[num_nodes];
        processInput(num_nodes, node_names_to_ind, adj_list);

        dfs(num_nodes, adj_list);
    }

    return 0;
}