#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <deque>
#include <cassert>
#include <cstring>

using namespace std;

#define graph_access(u,v) *(graph + u * num_nodes + v)

//#define DEBUG

/// @brief read standard input and create array
/// @param num_nodes number of nodes in the graph
/// @param num_edges number of edges in the graph
/// @param graph actually a 2D array, adjacency matrix for graph
void processInput(int num_nodes, int num_edges, int *graph) {
    string temp;
    for (int i = 0; i < num_edges; i++) {
        // Convert to 0 indexed
        getline(cin, temp, ' ');
        int src = std::stoi(temp) - 1;
        getline(cin, temp, ' ');
        int dest = std::stoi(temp) - 1;
        getline(cin, temp);
        int cap= std::stoi(temp);

        graph_access(src, dest) += cap;
    }
}

bool bfs_path(int num_nodes, int *graph, int path[]) {
    if (num_nodes == 0) return false;

    deque<int> q;
    int s = 0;

    bool visited[num_nodes];

    // initialize visited array, and initialize path
    for (int i = 0; i < num_nodes; i++) {
        visited[i] = false;
        path[i] = -1;
    }

    visited[0] = true;
    q.push_back(0);

    while (!q.empty()) {
        int u = q.front();
        q.pop_front();

        // Still O(n) even though we don't encode the neighbor list
        // directly - that neighbor list could be size n anyway
        for (int r = 0; r < num_nodes; r++) {
            if (graph_access(u, r) && !visited[r]) {
                path[r] = u;
                q.push_back(r);
                visited[r] = true;                
            }
        }
    }

    // we went through everything in the graph and we could not get to s from t
    return visited[num_nodes-1];
}

int residual(int num_nodes, int *graph, int path[]) {
    #ifdef DEBUG
    assert(num_nodes > 1);
    #endif
    int bottleneck = INT32_MAX;
    int node = num_nodes - 1;
    int prev_node = path[node];
    #ifdef DEBUG
    cout << "BFS SAYS PATH IS: ";
    #endif
    while (prev_node != -1) {
        int capacity = graph_access(prev_node, node);
        #ifdef DEBUG
        cout << "P:" << prev_node << ",N:" << node << ",C:" << capacity << "  ";
        // capacity should not be 0, otherwise we wouldn't have a path
        assert(capacity > 0);
        #endif
        if (capacity < bottleneck) {
            bottleneck = capacity;
        }
        node = prev_node;
        prev_node = path[node];
    }
    #ifdef DEBUG
    cout << node << " with bottleneck " << bottleneck << endl;
    #endif
    // have to traverse again because we didn't know what the bottleneck was before
    // this doesn't affect asymptotic runtime
    node = num_nodes - 1;
    prev_node = path[node];
    while (prev_node != -1) {
        int capacity = graph_access(prev_node, node);
        graph_access(prev_node, node) -= bottleneck;
        graph_access(node, prev_node) += bottleneck;
        node = prev_node;
        prev_node = path[node];
    }

    return bottleneck;
}

int maxflow(int num_nodes, int *graph) {
    // This array records the index to the parent element
    int path[num_nodes];
    int path_len = 0;

    int bottleneck_sum = 0;
    while (bfs_path(num_nodes, graph, path)) {
        bottleneck_sum += residual(num_nodes, graph, path);
    }
    
    return bottleneck_sum;
}

int main() {
    string curr_line;

    getline(cin, curr_line);
    int num_instances = std::stoi(curr_line);
    
    for (int i = 0; i < num_instances; i++) {
        getline(cin, curr_line, ' ');
        int num_nodes = std::stoi(curr_line);
        getline(cin, curr_line);
        int num_edges = std::stoi(curr_line);

        int *graph = (int*) calloc(num_nodes * num_nodes, sizeof(int));

        processInput(num_nodes, num_edges, graph);

        #ifdef DEBUG
        for (int u = 0; u < num_nodes; u++) {
            for (int v = 0; v < num_nodes; v++) {
                cout << graph_access(u,v) << " ";
            }
            cout << endl;
        }
        #endif //DEBUG

        cout << maxflow(num_nodes, graph) << endl;

        free(graph);
    }

    return 0;
}