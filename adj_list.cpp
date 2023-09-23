#include<bits/stdc++.h>
#include <cctype>
#include "disjoint_set.h"
using namespace std;
using namespace std::chrono;
 
// Adj list edge
struct Edge
{
    int dest;
    double weight;
    struct Edge* next;
};
 
// Adj list
struct AdjList
{
    struct Edge *head;
};
 

class Graph
{
    private:
        int numOfVertices;
        AdjList* nodes; 
    public:
        // initialize the graph
        Graph(int N)
        {
            this->numOfVertices = N;
            nodes = new AdjList [N];
            for (int i = 0; i < N; ++i)
                nodes[i].head = NULL;
        }

        // Adding an new edge
        void addEdge(int src, int dest, double weight)
        {	
            // 0-->(2, weight, NULL)
            // 1-->NULL
            // 2-->(0, weight, NULL)

            // adding an edge from source dest
        	Edge* newEdge = new Edge;
            newEdge->dest = dest;
            newEdge->next = nodes[src].head;
            newEdge->weight = weight;
            nodes[src].head = newEdge;
            
			// adding an edge from destination to source (since an undirected graph)
            if(src != dest){
                // if the edge is not a loop edge
                newEdge = new Edge;
                newEdge->dest = src;
                newEdge->next = nodes[dest].head;
                newEdge->weight = weight;
                nodes[dest].head = newEdge;
            }
        }
        
        // printing the graph
        void printGraph()
        {
            int v;
            for (v = 0; v < numOfVertices; ++v)
            {
                Edge* tmp = nodes[v].head;
                cout<<"["<< v <<"]";
                while (tmp)
                {
                    cout<<" -> ("<<tmp->dest<<", "<<tmp->weight<<")";
                    tmp = tmp->next;
                }
                cout<<endl;
            }
        }

        Graph Kruskal(){
            // initialize the MST
            Graph MST(numOfVertices);

            // sort the edges in increasing order
            vector<tuple<int,int,double>> edges;
            for(int i=0; i<numOfVertices; ++i){
                Edge* newEdge = nodes[i].head;
                while(newEdge!=NULL){
                    // add edge to edges to sort later
                    if(i <= newEdge->dest){
                        // avoid adding the same edge in reverse order (for undirected graphs)
                        edges.push_back(make_tuple(i,newEdge->dest, newEdge->weight));
                    }
                    newEdge = newEdge->next;
                }
            }
            sort(edges.begin(), edges.end(), [](const auto& a, const auto& b) {
                return get<2>(a) < get<2>(b);
            });
            
            // make sets for vertices
            DisjointSet ds(numOfVertices);

            // add it to sets and union them one by one
            for (const auto& edge : edges) {
                if(ds.find(get<0>(edge)) != ds.find(get<1>(edge))){
                    // add the edge and merge the sets
                    MST.addEdge(get<0>(edge), get<1>(edge), get<2>(edge));
                    ds.unionSets(get<0>(edge), get<1>(edge));
                }
            }

            return MST;
        }

        Graph PrimsLazy() {
            // initialize the MST
            Graph MST(numOfVertices);
            vector<bool> inMST(numOfVertices, false);
            priority_queue<tuple<double, int, int>, vector<tuple<double, int, int>>, greater<tuple<double, int, int>>> minHeap;

            // Start with a random vertex
            srand(time(0));
            int initialVertex = rand() % numOfVertices;
            inMST[initialVertex] = true;

            // Add all edges from the selected vertex to the min-heap
            Edge* currentEdge = nodes[initialVertex].head;
            while (currentEdge != NULL) {
                minHeap.push({currentEdge->weight, initialVertex, currentEdge->dest});
                currentEdge = currentEdge->next;
            }

            while (!minHeap.empty()) {
                // Get the edge with the smallest weight from the min-heap
                double weight;
                int src, dest;
                tie(weight, src, dest) = minHeap.top();
                minHeap.pop();

                // If the destination vertex is already in MST, skip this edge
                if (inMST[dest]) {
                    continue;
                }

                // Add the edge to the MST
                MST.addEdge(src, dest, weight);
                inMST[dest] = true;

                // Add all edges from the newly added vertex to the min-heap
                currentEdge = nodes[dest].head;
                while (currentEdge != nullptr) {
                    if (!inMST[currentEdge->dest]) {
                        minHeap.push({currentEdge->weight, dest, currentEdge->dest});
                    }
                    currentEdge = currentEdge->next;
                }
            }

            return MST;
        }

        Graph PrimsEager(){
            // Initialize the MST
            Graph MST(numOfVertices); 
            vector<bool> inMST(numOfVertices, false);
            vector<double> key(numOfVertices, numeric_limits<double>::max());
            vector<int> parent(numOfVertices, -1);

            // Use a set as an indexed priority queue
            set<pair<double, int>> priorityQueue;

            // Randomly select the initial vertex
            srand(time(0));
            int initialVertex = rand() % numOfVertices;
            key[initialVertex] = 0;
            priorityQueue.insert({0, initialVertex});

            while (!priorityQueue.empty()) {
                // Extract the vertex with the smallest key (minimum-weight edge)
                int currentVertex = priorityQueue.begin()->second;
                priorityQueue.erase(priorityQueue.begin());

                // Add the edge to the MST
                int p = parent[currentVertex];
                if (p != -1) {
                    double weight = key[currentVertex];
                    MST.addEdge(currentVertex, p, weight);
                }

                inMST[currentVertex] = true;

                // Update key values and insert/update vertices in the indexed priority queue
                Edge* currentEdge = nodes[currentVertex].head;
                while (currentEdge != NULL) {
                    int destination = currentEdge->dest;
                    double weight = currentEdge->weight;
                    if (!inMST[destination] && weight < key[destination]) {
                        priorityQueue.erase({key[destination], destination});
                        key[destination] = weight;
                        parent[destination] = currentVertex;
                        priorityQueue.insert({key[destination], destination});
                    }
                    currentEdge = currentEdge->next;
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