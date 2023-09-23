#include <vector>
#include "disjoint_set.h"
using namespace std;

DisjointSet::DisjointSet(int size) {
    parent.resize(size);
    rank.resize(size, 0);

    // Initialize each element as its own parent (representative)
    for (int i = 0; i < size; ++i) {
        parent[i] = i;
    }
}

    // Find the representative (root) of the set containing element x
int DisjointSet::find(int x) {
    if (parent[x] == x) {
        return x; // x is its own parent, so it's the representative
    } else {
        // Path compression: Make x's parent the representative
        parent[x] = find(parent[x]);
        return parent[x];
    }
}

    // Union two sets containing elements x and y
void DisjointSet::unionSets(int x, int y) {
    int rootX = find(x);
    int rootY = find(y);

    // Union by rank: Attach the smaller tree to the root of the larger tree
    if (rootX != rootY) {
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
    }
}
