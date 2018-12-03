# Basic Graph Algorithms

## Introduction

This repo covers basic graph algorithms for directed and undirected graphs with/without weights on edges.

Base graph data structure is targeted for **sparse graphs**. For example, it maintains adjancy list and pointrs.

This is a relatively poor quality code produced over a weekend. Main objective behind this repository is purely educational.

## Algorithms Covered

* DFS
* Topological Sort
* Prim's Mimimal Spanning Tree
* Kruskal's Mimimal Spanning Tree
* Dijkstra's Single Source Shortest Path

## How to Compile
```
cd src
<c++-compiler> bgaMain.cpp -o bgaMain.exe
```

## How to Run
``` bgaMain.exe <graph-file>```

## Example run


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


## Graph Format

```
graph (un)directed`
<node-x> <node-y> [edge-weight]
```

***Example Graph File*** 

![](https://github.com/srohit0/BasicGraphAlgorithmsCpp/blob/master/docs/ExampleGraph.JPG)


      
```
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


## Summary

It has quite a bit of room for improvement. Feel free to reach out if you spot weakness, bug, enancement or simply a suggestion.
