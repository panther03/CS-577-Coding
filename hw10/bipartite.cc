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
/// @param num_nodes number of nodes in total
/// @param num_nodes_A number of nodes in the A set
/// @param num_edges number of edges (not including to and from s/t)
/// @param graph actually a 2D array, adjacency matrix for graph
void processInput(int num_nodes, int num_nodes_A, int num_edges, int *graph) {
    string temp;
    for (int i = 0; i < num_edges; i++) {
        getline(cin, temp, ' ');
        int src = std::stoi(temp);
        getline(cin, temp);
        int dest = std::stoi(temp) + num_nodes_A;

        graph_access(src, dest) = 1;
        // these will do the same thing multiple times but thats OK (asymptotically speaking)
    }

    for (int i = 1; i <= num_nodes_A; i++) {
        graph_access(0, i) = 1;
    }
    for (int i = num_nodes_A+1; i < num_nodes - 1; i++) {
        graph_access(i, num_nodes - 1) = 1;
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
        int num_nodes_A = std::stoi(curr_line);
        getline(cin, curr_line, ' ');
        int num_nodes_B = std::stoi(curr_line);
        getline(cin, curr_line);
        int num_edges = std::stoi(curr_line);

        // + 2 for s & t
        int num_nodes_total = num_nodes_A + num_nodes_B + 2;

        int *graph = (int*) calloc(num_nodes_total * num_nodes_total, sizeof(int));

        processInput(num_nodes_total, num_nodes_A, num_edges, graph);

        #ifdef DEBUG
        int num_nodes = num_nodes_total;
        for (int u = 0; u < num_nodes; u++) {
            for (int v = 0; v < num_nodes; v++) {
                cout << graph_access(u,v) << " ";
            }
            cout << endl;
        }
        #endif //DEBUG

        int f = maxflow(num_nodes_total, graph);

        cout << f << ' ' <<(((num_nodes_A == num_nodes_B) && (f == num_nodes_B)) ? 'Y' : 'N') << endl;

        free(graph);
    }

    return 0;
}