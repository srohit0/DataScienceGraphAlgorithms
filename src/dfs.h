///////////////////////////////////////////////////////////////////
//    Copyright (c) 2018 Rohit Sharma. All rights reserved.
//    This program is free software; you can redistribute it and/or
//    modify it under the terms as GNU General Public License.
///////////////////////////////////////////////////////////////////
//
// Depth First Search Algorithms
// 1. Build DFS Tree
// 2. Topologica Sorting

#ifndef GRAPH_DFS_H
#define GRAPH_DFS_H

#include "graph.h"
#include <map>

class DFS {
private:
	map<const basicGraph::bNode*, basicGraph::NODE_MARKS> nodeMarks_;
	basicGraph::bGraph*                       graph_;

	string tabs(unsigned int level) {
		string tab_str = "";
		for (unsigned int i = 0; i < level; i++) tab_str += " ";
		return tab_str;
	}

	void resetMarks() {
		set<const basicGraph::bNode*, basicGraph::nodeCompare>::iterator niter = graph_->nodeBegin();
		for (; niter != graph_->nodeEnd(); niter++) {
			const basicGraph::bNode* node = *niter;
			DFS::nodeMarks_[node] = basicGraph::NOT_VISITED;
		}
		return;
	}

	void search_int(const basicGraph::bNode* src, unsigned int level = 0)
	{
		cout << tabs(level++) << src->name() << " visiting" << endl;
		DFS::nodeMarks_[src] = basicGraph::VISITING;

		set<const basicGraph::bEdge*, basicGraph::edgeCompare>::iterator niter = src->edgeBegin();
		for (; niter != src->edgeEnd(); niter++) {
			const basicGraph::bNode* nextNode = (*niter)->otherNode(src);
			if (DFS::nodeMarks_[nextNode] == basicGraph::NOT_VISITED)
				search_int(nextNode);
		}

		DFS::nodeMarks_[src] = basicGraph::VISITED;
		cout << tabs(level--) << src->name() << " visited." << endl;
	}

	void ts_visit(const basicGraph::bNode* src, vector<vector<const basicGraph::bNode*>>& container, unsigned int level=0)
	{
		if (DFS::nodeMarks_[src] == basicGraph::VISITED)
			return;

		if (DFS::nodeMarks_[src] == basicGraph::VISITING)
		{
			cerr << "Error: cycle deteceted at node " << src->name() << ". Topological sorting abandoned.\n";
			return;
		}

		DFS::nodeMarks_[src] = basicGraph::VISITING;

		set<const basicGraph::bEdge*, basicGraph::edgeCompare>::iterator niter = src->edgeBegin();
		for (; niter != src->edgeEnd(); niter++) {
			const basicGraph::bNode* nextNode = (*niter)->otherNode(src);
			ts_visit(nextNode, container, level+1);
		}

		DFS::nodeMarks_[src] = basicGraph::VISITED;
		container[level].push_back(src);

		return;
	}
public:

	DFS(basicGraph::bGraph* graph) : graph_(graph) {}

	void search(const basicGraph::bNode* src)
	{
		if (!src || !graph_) {
			if (!src)    cerr << "Error: source node not in the graph.";
			if (!graph_) cerr << "Error: graph not found.";
			return;
		}

		resetMarks();

		search_int(src);

		return;
	}

	// input  : a directed acyclic graph with input node.
	// output : levelized queues sorted by dependency.
	// algorithm: reverse order of DFS's finishing times.
	void topological_sort()
	{
		if ( !graph_) {
			if (!graph_) cerr << "Error: graph not found.";
			return;
		}
		// works only on DAGs
		if (!graph_->directed()) {
			cerr << "Error: topological sort works only on directed graphs.\n";
			return;
		}

		const unsigned int MAX_LEVEL = graph_->nNodes();
		vector<vector<const basicGraph::bNode*>> levelized_container(MAX_LEVEL);
		set<const basicGraph::bNode*, basicGraph::nodeCompare>::iterator niter = graph_->nodeBegin();
		for (; niter != graph_->nodeEnd(); niter++) 
		{
			const basicGraph::bNode* src = *niter;
			if (DFS::nodeMarks_[src] != basicGraph::VISITED)
				ts_visit(src, levelized_container);
		}

		// print levelized queues.
		for (size_t level = 0; level < MAX_LEVEL; level++) {
			if (levelized_container[level].size() == 0)
				continue;
			
			cout << "level " << level + 1 << " : ";
			for (size_t i = 0; i < levelized_container[level].size(); i++)
			{
				cout << levelized_container[level][i]->name() << " ";
			}
			cout << "\n";
		}

		return;
	}
};

#endif 