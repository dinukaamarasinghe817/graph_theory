# INSTRUCTIONS TO EXECUTE
        i.  g++ <representation_code> disjoint_set.cpp <objectfile_name>
        ii. ./<objectfile_name> <graph_mode> <algorithm>
        iii.This will output the execution time

# SAMPLE EXECUTION
## Eg: kruskals algorithm in adjacency list representation for sparse graphs 
        i.  g++ adj_list.cpp disjoint_set.cpp adj_list
        ii. ./adj_list sparse kruskals

# INSTRUCTIONS TO GET THE MST
        i.  Uncomment the below two lines in required representation (adj_list.cpp or adj_mat.cpp)
                // freopen("output.txt","w",stdout);
                // Result.printGraph();

        ii. Once you run the .cpp file, it will print the MST to a file named 'output.txt'
        in given format either adjacency list or adjacency matrix

# SPECIAL NOTE
        i.   All the codes tested against 10 different graphs with increasing number of edges
        ii.  Sparsest graph have 10,002 edges where the denses graph have 11,252,250 edges
        iii. The graph files are very huge. Nearly 1 GB
        iv.  You can generate graphs using 'generate.cpp' by making relevent changes in the code 
        or download the entire graph dataset from the link given in the report and place then in the current directory
        
        v.   You can specify the graph text file names in the 'script.sh' file
        vi.  Give execution permission by 'chmod +x script.sh'
        vii. Then you only required to type './script.sh'
        viii.This will output the execution time for each graph under each algorithm in a file 
        named 'output.csv' (may take several minutes)
