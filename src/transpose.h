///////////////////////////////////////////////////////////////////
//    Copyright (c) 2018 Rohit Sharma. All rights reserved.
//    This program is free software; you can redistribute it and/or
//    modify it under the terms as GNU General Public License.
///////////////////////////////////////////////////////////////////

// Transpose:
//      The transpose, converse or reverse of a directed graph G is another 
// directed graph on the same set of vertices with all of the edges reversed
// compared to the orientation of the corresponding edges in G. That is,
// if G contains an edge (u,v) then the converse/transpose/reverse of G 
// contains an edge (v,u) and vice versa.
//
// Transpose of undirected graph is same graph with no change.

// Reference: https://en.wikipedia.org/wiki/Transpose_graph

#pragma once
#ifndef GRAPH_TRANSPOSE_H
#define GRAPH_TRANSPOSE_H

#include "graph.h"

using namespace std;

class transpose {
private:
	bool inplace_; // return a new graph or compute transpose in the same one.
public:
	transpose(bool inplace) : inplace_(inplace)
	{}
	const basicGraph::bGraph* build(const basicGraph::bGraph* graph)
	{
		const basicGraph::bGraph* new_graph = graph;
		if (!inplace_)
			new_graph = new basicGraph::bGraph(*graph);

		if (!graph->directed())
		{
			cerr << "Warning: transpose of undirected graph is the same graph.\n";
			return new_graph;
		}

		// clear edge list of nodes.
		set<const basicGraph::bNode*, basicGraph::nodeCompare>::iterator niter;
		for (niter = new_graph->nodeBegin(); niter != new_graph->nodeEnd(); niter++)
		{
			const_cast<basicGraph::bNode*>(*niter)->clearEdgeList();
		}

		set<const basicGraph::bEdge*, basicGraph::edgeCompare>::iterator eiter;
		for (eiter = new_graph->edgeBegin(); eiter != new_graph->edgeEnd(); eiter++)
		{
			const_cast<basicGraph::bEdge*>(*eiter)->swap_nodes();
			const_cast<basicGraph::bNode*>((*eiter)->n1())->addEdge(*eiter);
		}
		return new_graph;
	}
};

#endif