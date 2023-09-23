#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#define NAME "sample.txt" // define the outputfile name
#define NODES 5001        // define the required number of vertices
#define EDGES 100         // EDGES < (NODES/2)
                          // EDGES = number of outgoing edges per vertex
                          // change above definitions accordingly and run the program

using namespace std;

int main() {
    // Seed the random number generator
    srand(time(nullptr));

    // Open the output file
    ofstream outputFile(NAME);
    outputFile << NODES << endl;
    outputFile << NODES*EDGES << endl;

    if (!outputFile) {
        cerr << "Error: Could not open output file." << endl;
        return 1;
    }

    // Generate a fully connected graph and write to the file
    for (int source = 0; source < NODES; ++source) {
        for (int iteration = 0; iteration < EDGES; ++iteration) {
            double weight = static_cast<double>(rand()) / RAND_MAX;
            outputFile << source << " " << (source+iteration+2)%NODES << " " << weight << endl;
        }
    }

    // Close the output file
    outputFile.close();

    cout << "Graph data written to '" << NAME << "'" << endl;

    return 0;
}
