# Basic Graph Algorithms

## Introduction

This repo covers basic graph algorithms for directed and undirected graphs with/without weights on edges. Graph description is read from a file with ascii format.

Base graph data structure is targeted for **sparse graphs** efficiency. For example, it maintains adjancy list and pointrs. Here are big O time and space complexities.


|                 |    RAM    | node-add | edge-add | node-remove | edge-remove |   query   |
| --------------- |:---------:|:--------:|:--------:|:-----------:|:-----------:|:---------:|
| Adjacency List  |  [n]+[e]  |    1     |     1    |   [n]+[e]   |     [e]     |    [e]    |
| Incident List   |  [n]+[e]  |    1     |     1    |     [e]     |     [e]     |    [e]    |

Legend:
* Two vertices are called adjacent if they are connected by an edge.
* Two edges are called incident, if they share a vertex.

## Algorithms Covered

* [DFS](src/dfs.h)
* [Transpose](src/transpose.h)
* [Topological Sort](src/dfs.h)
* [Strongly Connected Components](src/scc.h)
* [Prim's Mimimal Spanning Tree](src/mst.h)
* [Kruskal's Mimimal Spanning Tree](src/mst.h)
* [Dijkstra's Single Source Shortest Path to All Nodes](src/shortestPath.h)
* [A* (aka Astar) Source-Destination Pair Shortest Path Finder Algorithm](src/shortestPath.h)

## Graph File Format

```
graph (un)directed`
<node-x> <node-y> [edge-weight]
```

***Example Graph File*** 

![](https://github.com/srohit0/BasicGraphAlgorithmsCpp/blob/master/docs/ExampleGraph.JPG)
      
```
# Comment, ignored by the graph reader
graph directed
n1 n2 2
n1 n3 4
n2 n3 1
n2 n4 4
n2 n5 2
n3 n5 3
n4 n6 2
n5 n4 3
n5 n6 2
```

## Application

### How to Compile
```
cd src
<c++-compiler> bgaMain.cpp -o bgaMain.exe
```

### How to Run
``` bgaMain.exe <graph-file>```

### Example run


$ src/bgaMain.exe test/dgraph5.txt
```
created graph with 6 nodes and 9 edges.
type 'help' for more options
```
>> help
```
 help
 print
 search  <root_node>
 sort
 mst     [prim|kruskal]
 path    <root_node>
 quit
 ```
 >> print
 ```
graph directed
n2 n3 1
n1 n2 2
...
```
>> path n1
```
nd dist_from_src edge
== ============= ====
n1       0       [none  n1 0]
n2       2       [n1  n2 2]
n3       3       [n2  n3 1]
```


## Disclaimer

This is a quick and dirty code produced over weekends. Main objective behind this repository is purely educational. It has quite a bit of room for improvement. Feel free to reach out if you spot weakness, bug, enancement or simply a suggestion. 

