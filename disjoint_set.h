#include <vector>
using namespace std;

class DisjointSet {
public:
    DisjointSet(int size);

    // Find the representative (root) of the set containing element x
    int find(int x);

    // Union two sets containing elements x and y
    void unionSets(int x, int y);

private:
    vector<int> parent;
    vector<int> rank;
};