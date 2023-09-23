#include<bits/stdc++.h>
#include "disjoint_set.h"
using namespace std;
using namespace std::chrono;

class Graph
{
    private:
        int numOfVertices;
        vector<vector<double>> graph;
    public:
        Graph(int N) {
            this->numOfVertices = N;
            graph.resize(N, vector<double>(N, numeric_limits<double>::max())); // Initialize all values to 0
        }
        

        void addEdge(int origin, int destination, double weight)
        {
            if( origin < numOfVertices && origin >= 0 && destination < numOfVertices && destination >= 0)
            {
                graph[origin][destination] = weight;
                if(origin != destination){
                    graph[destination][origin] = weight;
                }
            }
        }
        

        void printGraph()
        {
            int i,j;
            for(i = 0;i < numOfVertices;i++)
            {
                cout << "[";
                for(j = 0; j < numOfVertices-1; j++)
                    cout<< min(graph[i][j], 2.001) << ",  ";
                cout<< min(graph[i][j], 2.001) << "]" << endl;
            }
        }

        Graph Kruskal() {
            // initialize the MST
            Graph MST(numOfVertices);

            // struct edge
            struct Edge {
                int source;
                int destination;
                double weight;
            };

            // get edges
            vector<Edge> edges;
            for (int i = 0; i < numOfVertices; ++i) {
                for (int j = i + 1; j < numOfVertices; ++j) {
                    if (graph[i][j] != numeric_limits<double>::max()) {
                        edges.push_back({i, j, graph[i][j]});
                    }
                }
            }

            // sort the edges in increasing order of weight
            sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
                return a.weight < b.weight;
            });

            // initialize a disjoint set data structure
            DisjointSet ds(numOfVertices);

            // add edges to the MST one by one
            for (const Edge& edge : edges) {
                if (ds.find(edge.source) != ds.find(edge.destination)) {
                    // add the edge to the MST and merge the sets
                    MST.addEdge(edge.source, edge.destination, edge.weight);
                    ds.unionSets(edge.source, edge.destination);
                }
            }

            return MST;
        }

        Graph PrimsLazy() {
            // initialize the MST
            Graph MST(numOfVertices);
            vector<bool> inMST(numOfVertices, false);
            vector<double> minWeight(numOfVertices, numeric_limits<double>::max());
            // vector<int> parent(numOfVertices, -1);

            // start with a random vertex
            srand(time(0));
            int initialVertex = rand() % numOfVertices;
            minWeight[initialVertex] = 0;

            // create a min-heap to store edges as (weight, source, destination) tuples
            priority_queue<tuple<double, int, int>, vector<tuple<double, int, int>>, greater<tuple<double, int, int>>> minHeap;

            // add the edges from the initial vertex to the min-heap
            for (int i = 0; i < numOfVertices; ++i) {
                if (i != initialVertex && graph[initialVertex][i] != numeric_limits<double>::max()) {
                    minHeap.push(make_tuple(graph[initialVertex][i], initialVertex, i));
                }
            }

            while (!minHeap.empty()) {
                double weight;
                int source, destination;
                tie(weight, source, destination) = minHeap.top();
                minHeap.pop();

                if (inMST[destination]) {
                    continue;
                }

                inMST[destination] = true;
                MST.addEdge(source, destination, weight);

                // update minWeight and parent for adjacent vertices
                for (int i = 0; i < numOfVertices; ++i) {
                    if (!inMST[i] && graph[destination][i] < minWeight[i]) {
                        minWeight[i] = graph[destination][i];
                        // parent[i] = destination;
                        minHeap.push(make_tuple(minWeight[i], destination, i));
                    }
                }
            }

            return MST;
        }

        Graph PrimsEager() {
            // initialize the MST
            Graph MST(numOfVertices);
            std::vector<bool> inMST(numOfVertices, false);
            std::vector<double> key(numOfVertices, std::numeric_limits<double>::max());
            std::vector<int> parent(numOfVertices, -1);

            // use a set as an indexed priority queue
            std::set<std::pair<double, int>> priorityQueue;

            // randomly select the initial vertex
            srand(time(0));
            int initialVertex = rand() % numOfVertices;
            key[initialVertex] = 0;
            priorityQueue.insert({0, initialVertex});

            while (!priorityQueue.empty()) {
                // extract the vertex with the smallest key (minimum-weight edge)
                int currentVertex = priorityQueue.begin()->second;
                priorityQueue.erase(priorityQueue.begin());

                // add the edge to the MST
                int p = parent[currentVertex];
                if (p != -1) {
                    double weight = key[currentVertex];
                    MST.addEdge(currentVertex, p, weight);
                }

                inMST[currentVertex] = true;

                // update key values and insert/update vertices in the indexed priority queue
                for (int i = 0; i < numOfVertices; ++i) {
                    if (!inMST[i] && graph[currentVertex][i] < key[i]) {
                        priorityQueue.erase({key[i], i});
                        key[i] = graph[currentVertex][i];
                        parent[i] = currentVertex;
                        priorityQueue.insert({key[i], i});
                    }
                }
            }

            return MST;
        }

};


int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <graph_type> <algorithm>" << endl;
        cerr << "\tEg: ./adj_list sparse kruskals" << endl;
        cerr << "\nGraph Types: sparse, dense" << endl;
        cerr << "Algorithms: kruskals, lazyprims, eagerprims\n\n" << endl;
        return 1;
    }

    const string graphType = argv[1];
    string algorithm = argv[2];
    transform(algorithm.begin(), algorithm.end(), algorithm.begin(), ::tolower);
    ifstream graphFile(graphType+".txt");
    if (!graphFile) {
        cerr << "Error: Could not open the file." << endl;
        return 1;
    }

    int numVertices,numEdges, source, destination;
    double weight;
    graphFile >> numVertices >> numEdges;

    Graph G(numVertices);
    for (int i=0; i < numEdges; ++i) {
        graphFile >> source >> destination >> weight;
        G.addEdge(source, destination, weight);
    }
    graphFile.close();

    Graph Result(numVertices);
    high_resolution_clock::time_point start = high_resolution_clock::now(); // start clock

    if(algorithm == "kruskals"){
        Result = G.Kruskal();
    }else if(algorithm == "lazyprims"){
        Result = G.PrimsLazy();
    }else{
        Result = G.PrimsEager();
    }

    high_resolution_clock::time_point end = high_resolution_clock::now(); // stop clock
    auto duration = duration_cast<microseconds>(end - start);
    cout << numEdges << " " << duration.count() << endl;

    // freopen("output.txt","w",stdout);
    // Result.printGraph();
    
    return 0;
}