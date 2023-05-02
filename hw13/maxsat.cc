#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>

using namespace std;

typedef struct {
    int v1;
    int v2;
    int v3;
} clause_t; 

//#define DEBUG

/// @brief read standard input and create array
/// @param num_nodes number of nodes in the graph
/// @param num_edges number of edges in the graph
/// @param graph actually a 2D array, adjacency matrix for graph
void processInput(int num_clauses, clause_t problem[]) {
    string temp;
    for (int i = 0; i < num_clauses; i++) {
        // Convert to 0 indexed
        getline(cin, temp, ' ');
        int v1 = std::stoi(temp);
        getline(cin, temp, ' ');
        int v2 = std::stoi(temp) - 1;
        getline(cin, temp);
        int v3 = std::stoi(temp);

        problem[i] = (clause_t) {
            v1: v1,
            v2: v2,
            v3: v3
        };
    }
}

inline int sign(int x) {
    return x/abs(x);
}

bool verifySolution(int num_clauses, clause_t problem[], int solution[]) {
    int satisfiedClauses = 0;
    for (int i = 0; i < num_clauses; i++) {
        clause_t clause = problem[i];
        if ((sign(clause.v1) == solution[abs(clause.v1)-1]) 
        ||  (sign(clause.v2) == solution[abs(clause.v2)-1])
        ||  (sign(clause.v3) == solution[abs(clause.v3)-1])) {
            satisfiedClauses++;
        }
    }
    return (satisfiedClauses * 8 >= num_clauses * 7);
}

int main() {
    string curr_line;
    
    getline(cin, curr_line);
    int num_variables = std::stoi(curr_line);
    getline(cin, curr_line);
    int num_clauses = std::stoi(curr_line);

    clause_t problem[num_clauses];
    processInput(num_clauses, problem);

    int solution[num_variables];

    srand((unsigned) time(NULL));

    do {
        // code to randomize the solution w/ 50/50 chance
        for (int i = 0; i < num_variables; i++) {
            solution[i] = (rand() % 2) * 2 - 1;
        }
        #ifdef DEBUG
        cout << "Trying solution..." << endl;
        cout << solution[0];
        for (int i = 1; i < num_variables; i++) {
            cout << " " << solution[i];
        }
        cout << endl;
        #endif
            
    } while (!verifySolution(num_clauses, problem, solution));

    cout << solution[0];
    for (int i = 1; i < num_variables; i++) {
        cout << " " << solution[i];
    }
    cout << endl;

    return 0;
}