///////////////////////////////////////////////////////////////////
//    Copyright (c) 2018 Rohit Sharma. All rights reserved.
//    This program is free software; you can redistribute it and/or
//    modify it under the terms as GNU General Public License.
///////////////////////////////////////////////////////////////////
//
// basic Graph Algorithms.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "graph.h"
#include "dfs.h"
#include "mst.h"
#include "shortestPath.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>

using namespace std;
int main(int argc, char** argv)
{
	if (argc < 1) {
		cerr << "Error: no input graph given.\n";
		return 1;
	}
	basicGraph::bGraph* graph = basicGraph::bGraph::readBasicGraph(argv[1]);
	if (!graph) {
		cerr << "Error: graph could not be built.\n";
		return 1;
	}
	cout << "created graph with " << graph->nNodes() << " nodes and " << graph->nEdges() << " edges.\n";
	cout << "type \'help' for more options\n";

	while (true) {
		string line, token;
		cout << ">> ";
		getline(cin, line);

		vector<string> tokens;
		stringstream stokens(line);

		if (line.empty()) continue;

		// Tokenizing w.r.t. space ' ' 
		while (getline(stokens, token, ' ')) tokens.push_back(token);

		string choice = tokens[0];

		transform(choice.begin(), choice.end(), choice.begin(), ::tolower);
		if (choice == "help" ) 
		{
			cout << " help\n";
			cout << " print\n";
			cout << " search  <root_node>\n";    // dfs search tree
			cout << " sort \n";                  // levelize or topological sort
			cout << " mst     [prim|kruskal]\n"; // minimal spanning tree
			cout << " path    <root_node>\n";    // Shortest Path from single source to all vertices
			cout << " quit\n";
		} 
		else if (choice == "print") 
		{
			graph->print();
			cout << "\n";
		}
		else if (choice == "search") 
		{
			if (tokens.size() < 2) {
				cerr << "Error: supply search node and try again.\n";
				continue;
			}
			string node = tokens[1];
			const basicGraph::bNode* src = graph->findNode(node);
			if (!src) {
				cerr << "Error: node " << node << "not found in the graph.\n";
			}
			else {
				DFS traverse(graph);
				traverse.search(src);
		    }
		}
		else if (choice == "sort") 
		{
			DFS traverse(graph);
			traverse.topological_sort();
		}
		else if (choice == "mst")
		{
			string algo = tokens.size() < 2 ? "kruskal" : tokens[1];
			if (algo == "kruskal")
			{
				MST::kruskal mst(graph);
				mst.build();
				mst.print();
			}
			else if (algo == "prim")
			{
				MST::prim mst(graph);
				mst.build();
				mst.print();
			}
			else {
				cerr << "Error: invalid option " << algo << ". Options are\n";
				cerr << "        prim.\n";
				cerr << "        kruskal.\n";
			}
		}
		else if (choice == "path")
		{
			if (tokens.size() < 2) {
				cerr << "Error: supply search node and try again.\n";
				continue;
			}
			string node = tokens[1];
			const basicGraph::bNode* src = graph->findNode(node);
			if (!src) {
				cerr << "Error: node " << node << "not found in the graph.\n";
			}
			else {
				short_paths::dijkstra single_source_path(src, graph);
				single_source_path.build();
				single_source_path.print();
			}
		}
		else if (choice == "quit" || choice == "exit")
		{
			cout << ".. good bye.\n";
			break;
		}
		else
		{
			cerr << "Error: option " << choice << " not recognized.\n";
		}
	}

	return 0;
}
