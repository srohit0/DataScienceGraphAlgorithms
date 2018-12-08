///////////////////////////////////////////////////////////////////
//    Copyright (c) 2018 Rohit Sharma. All rights reserved.
//    This program is free software; you can redistribute it and/or
//    modify it under the terms as GNU General Public License.
///////////////////////////////////////////////////////////////////

// Strongly Connected Components:
//      A graph is said to be strongly connected or diconnected if every vertex
// is reachable from every other vertex. 
//
// Reference: https://en.wikipedia.org/wiki/Strongly_connected_component
//            https://en.wikipedia.org/wiki/Kosaraju%27s_algorithm

#ifndef GRAPH_SCC_H
#define GRAPH_SCC_H

#pragma once
#include "transpose.h"
#include <map>
#include <stack>

using namespace std;

namespace SCC {
	map<const basicGraph::bNode*, basicGraph::NODE_MARKS> nodeMarks_;
	void resetMarks(const basicGraph::bGraph* graph) {
		set<const basicGraph::bNode*, basicGraph::nodeCompare>::iterator niter = graph->nodeBegin();
		for (; niter != graph->nodeEnd(); niter++) {
			const basicGraph::bNode* node = *niter;
			SCC::nodeMarks_[node] = basicGraph::NOT_VISITED;
		}
		return;
	}

	// Algorithm:
	//   1. For each vertex u of the graph, mark u as unvisited.Let stack_ be empty.
	//	 2. For each vertex u of the graph do Visit(u), where Visit(u) is the recursive subroutine :
	//      If u is unvisited then :
	//         1. Mark u as visited.
	//	       2. For each out - neighbour v of u, do Visit(v).
	//	       3. Prepend u to stack_.
	//	 3. Transpose graph.
	//   4. For each element u of stack_ in order,
	//         If u has not been assigned to a component then :
	//            1. explore all vertices connected to u with DFS and assign them to SCC group.
	//

	class kosaraju {
	private:
		const basicGraph::bGraph*                   graph_;
		vector < vector<const basicGraph::bNode*> > listSCC_;
		stack<const basicGraph::bNode*>             stack_;

		// fill up the stack in the reverse order or finishing time.
		void first_dfs_pass(const basicGraph::bNode* src)
		{
			if (SCC::nodeMarks_[src] == basicGraph::VISITED)
				return;
			if (SCC::nodeMarks_[src] == basicGraph::VISITING) // detected cycle.
				return;
			SCC::nodeMarks_[src] = basicGraph::VISITING;

			set<const basicGraph::bEdge*, basicGraph::edgeCompare>::iterator niter = src->edgeBegin();
			for (; niter != src->edgeEnd(); niter++) {
				const basicGraph::bNode* nextNode = (*niter)->otherNode(src);
				if (SCC::nodeMarks_[nextNode] == basicGraph::NOT_VISITED)
					first_dfs_pass(nextNode);
			}

			SCC::nodeMarks_[src] = basicGraph::VISITED;
			stack_.push(src);
		}


		bool second_dfs_pass(const basicGraph::bNode* src, vector<const basicGraph::bNode*>& sccGroup)
		{
			if (SCC::nodeMarks_[src] == basicGraph::VISITED)
				return false;
			if (SCC::nodeMarks_[src] == basicGraph::VISITING) // detected cycle.
				return false;
			SCC::nodeMarks_[src] = basicGraph::VISITING;

			set<const basicGraph::bEdge*, basicGraph::edgeCompare>::iterator niter = src->edgeBegin();
			for (; niter != src->edgeEnd(); niter++) {
				const basicGraph::bNode* nextNode = (*niter)->otherNode(src);
				if (SCC::nodeMarks_[nextNode] == basicGraph::NOT_VISITED)
					second_dfs_pass(nextNode, sccGroup);
			}

			SCC::nodeMarks_[src] = basicGraph::VISITED;
			sccGroup.push_back(src);

			return true; // found SCC
		}

		void build_dfs_stack()
		{
			set<const basicGraph::bNode*, basicGraph::nodeCompare>::iterator niter = graph_->nodeBegin();
			for (; niter != graph_->nodeEnd(); niter++)
			{
				first_dfs_pass(*niter);
			}
			return;
		}

		void build_scc_list()
		{
			size_t scc_index = 0;
			while (!stack_.empty())
			{
				if ( second_dfs_pass(stack_.top(), listSCC_[scc_index]) )
					scc_index++;
				stack_.pop();
			}
			return;
		}
	public:
		kosaraju(const basicGraph::bGraph* graph) : graph_(graph), listSCC_(graph->nNodes())
		{}
		void build()
		{
			if (!graph_->directed())
			{
				cerr << "Error: Strongly connected can not be determined for undirected graph.\n";
				return;
			}

			resetMarks(graph_);
			build_dfs_stack();

			transpose reverse(true);
			reverse.build(graph_);

			resetMarks(graph_);
			build_scc_list();

			// fix directed graph before leaving.
			reverse.build(graph_);
			return;
		}
		void print()
		{
			for (size_t i = 0; i < listSCC_.size(); i++)
			{
				if (listSCC_[i].size() == 0)
					continue;

				cout << "SCC Group " << i << " : ";
				for (size_t j = 0; j < listSCC_[i].size(); j++)
				{
					cout << listSCC_[i][j]->name() << " ";
				}
				cout << "\n";
			}
			return;
		}
	};
};

#endif
