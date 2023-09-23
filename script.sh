#!/bin/bash
# compile the code
g++ adj_list.cpp disjoint_set.cpp -o adj_list
g++ adj_mat.cpp disjoint_set.cpp -o adj_mat

# define the C++ object file and input graph files
objectfiles=("./adj_list" "./adj_mat")
# specify your graph files here
graph_files=("graph1" "graph2" "graph3" "graph4" "graph5" "graph6" "graph7" "graph8" "graph9" "graph10")
algorithms=("kruskals" "lazyprims" "eagerprims")

# create the CSV file and write headers
csv_file="output.csv"
echo "edges,kruskals_adj_list,kruskals_adj_mat,lazyprims_adj_list,lazyprims_adj_mat,eagerprims_adj_list,eagerprims_adj_mat" > "$csv_file"

declare -A data

# loop through graph files and algorithms
for graph_file in "${graph_files[@]}"; do
  num_of_edges=0
  index=0
  # declare a varible to store the num_of_edges
  for algorithm in "${algorithms[@]}"; do
    for objectfile in "${objectfiles[@]}"; do
      # run the C++ program and capture its output
      output=$("$objectfile" "$graph_file" "$algorithm")

      # parse the output to extract num_of_edges and execution_time
      num_of_edges=$(echo "$output" | awk '{print $1}')
      execution_time=$(echo "$output" | awk '{print $2}')

      # append the data to an array
      data["$index"]="$execution_time"
      index=$((index + 1))
    done
  done

  # echo the array here starting from first number
  echo -n "$num_of_edges" >> "$csv_file"
  for key in {0..5}; do
    echo -n ",${data[$key]}" >> "$csv_file"
  done
  echo >> "$csv_file"
  echo "$graph_file successful"
done

echo "CSV file '$csv_file' created successfully."
