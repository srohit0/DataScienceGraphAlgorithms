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


