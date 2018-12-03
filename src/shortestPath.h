///////////////////////////////////////////////////////////////////
//    Copyright (c) 2018 Rohit Sharma. All rights reserved.
//    This program is free software; you can redistribute it and/or
//    modify it under the terms as GNU General Public License.
///////////////////////////////////////////////////////////////////

// Dijkstra's algorithm 
//     given source node in the graph, the algorithm finds the shortest path between that
//     node and every other node in the graph.
//     It is a single node shortest path and works on both directional and bi-directional path.
//
// Reference: https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm


#ifndef GRAPH_PATH_H
#define GRAPH_PATH_H

#pragma once
#include <algorithm>
#include "graph.h"
#include "heap.h"

namespace short_paths {
	struct pathNode {
	public:
		const basicGraph::bNode* node_;      // to node in graph node
		pathNode*                prev_node_; // from node in graph node
		size_t                   dist_;      // weight of the edge from parent to this node.
		size_t                   root_dist_; // sum of all dist_ from this node to root.
		pathNode(const basicGraph::bNode* node, size_t wt = 0) :
			node_(node), prev_node_(nullptr), dist_(wt), root_dist_(LONG_MAX)
		{}
		void setParent(pathNode* other, size_t dist, size_t root_dist) {
			other->prev_node_ = this;
			other->dist_      = dist;
			other->root_dist_ = root_dist;
		}
	};
	struct pathNodeCompare
	{
		bool operator() (const pathNode* tn1, const pathNode* tn2) const
		{
			return basicGraph::nodeCmp.operator()(tn1->node_, tn2->node_);
		}
	};

	class path {
	private:
		pathNode*                       src_;  // source for the path.
		set<pathNode*, pathNodeCompare> tree_; // path

	public:
		path(pathNode* src) : src_(src)
		{
			tree_.insert(src);
			src->root_dist_ = 0;
		}
		pathNode* src() { return src_; }
		pathNode* retrievePathNode(const basicGraph::bNode* gnode)
		{
			set<pathNode*, pathNodeCompare>::iterator niter = tree_.find(&pathNode(gnode));
			return niter == tree_.end() ? nullptr : *niter;
		}
		pathNode* retrieveOrAddPathNode(const basicGraph::bNode* gnode)
		{
			pathNode* pnode = retrievePathNode(gnode);
			if (pnode == nullptr)
			{
				pnode = new pathNode(gnode);
				tree_.insert(pnode);
			}
			return pnode;
		}
		void print()
		{
			set<pathNode*, pathNodeCompare>::iterator niter = tree_.begin();
			for (; niter != tree_.end(); niter++)
			{
				pathNode* tnode = (*niter);

				//cout << (tnode->prev_node_ ? tnode->prev_node_->node_->name() : "root") << " "
				//	<< tnode->node_->name() << " "
				//	<< tnode->dist_ << " "
				//	<< " (root dist " << tnode->root_dist_ << ")";
				//cout << "\n";



				cout << "nd dist_from_src edge\n";
				cout << "== ============= ====\n";
				set<pathNode*, pathNodeCompare>::iterator niter = tree_.begin();
				for (; niter != tree_.end(); niter++)
				{
					pathNode* tnode = (*niter);

					cout << tnode->node_->name() << " " << tnode->root_dist_ 
						<< " [" << (tnode->prev_node_ ? tnode->prev_node_->node_->name() : "none")
						<< "  " << tnode->node_->name() << " "	<< tnode->dist_ << "]\n";
				}
			}
		}
		~path()
		{
			set<pathNode*, pathNodeCompare>::iterator niter = tree_.begin();
			for (; niter != tree_.end(); niter++)
				delete (*niter);
			tree_.clear();
		}
	};

	struct pqNode {
	public:
		const basicGraph::bNode* node_;
		size_t                   min_dist_;

		pqNode() : node_(nullptr), min_dist_(LONG_MAX)
		{}
		pqNode(const basicGraph::bNode* node) : node_(node), min_dist_(LONG_MAX)
		{
			assert(node);
			set<const basicGraph::bEdge*, basicGraph::edgeCompare>::iterator eiter = node->edgeBegin();
			const basicGraph::bEdge* edge = ( eiter == node->edgeEnd() )? nullptr : *eiter;
			if ( edge )
				min_dist_ = dynamic_cast<const basicGraph::bWeightedEdge*>(edge)->weight();
		}
		bool operator==(const pqNode& other)
		{
			return other.node_ == node_;
		}
	};
	class pqNodeCompare {
	public:
		bool operator() (const pqNode& tn1, const pqNode& tn2) const
		{
			return tn1.min_dist_ < tn2.min_dist_;
		}
	};

  //function Dijkstra(Graph, source) :
  //    create vertex set PQ
  //    for each vertex v in Graph :             // Initialization
  //        dist[v] ← INFINITY                  // Unknown distance from source to v
  //        prev[v] ← UNDEFINED                 // Previous node in optimal path from source
  //
  //    dist[source] ← 0                        // Distance from source to source
 //     add source to PQ                         
  //    while PQ is not empty:
  //        u ← vertex in PQ with min dist[u]    // Node with the least distance
  //                                            // will be selected first
  //        remove u from PQ
  //
  //        for each neighbor v of u :
  //            alt ← dist[u] + length(u, v)
  //            if alt < dist[v] :               // A shorter path to v has been found
  //                dist[v] ← alt
  //                prev[v] ← u
  //                add v to PQ
  //
  //    return dist[], prev[]


	class dijkstra {
	private:
		const basicGraph::bGraph* graph_;
		path                      shortest_path_;
	public:
		dijkstra(const basicGraph::bNode* src, const basicGraph::bGraph* graph) :
			graph_(graph), shortest_path_(new pathNode(src))
		{}
		void build()
		{
			Heap<pqNode, pqNodeCompare> PQ; // priority queue

			set<const basicGraph::bNode*, basicGraph::nodeCompare>::iterator niter;
			for (niter = graph_->nodeBegin(); niter != graph_->nodeEnd(); niter++)
			{
				shortest_path_.retrieveOrAddPathNode(*niter);
			}


			pqNode qnode(shortest_path_.src()->node_); // node in priority queue
			qnode.min_dist_ = 0;
			PQ.push(qnode);

			while (PQ.pop(qnode))
			{
				pathNode* pnode = shortest_path_.retrievePathNode(qnode.node_);
				assert(pnode);

				set<const basicGraph::bEdge*, basicGraph::edgeCompare>::iterator eiter;
				for (eiter = pnode->node_->edgeBegin(); eiter != pnode->node_->edgeEnd(); eiter++)
				{
					const basicGraph::bNode* other_gnode = (*eiter)->otherNode(pnode->node_);
					size_t dist = dynamic_cast<const basicGraph::bWeightedEdge*>(*eiter)->weight();
					pathNode* other_pnode = shortest_path_.retrievePathNode(other_gnode);

					size_t alt_root_dist = pnode->root_dist_ + dist;
					if (alt_root_dist < other_pnode->root_dist_)
					{
						pnode->setParent(other_pnode, dist, alt_root_dist);
						pqNode qnode_next(other_gnode);
						if ( PQ.exists(qnode_next) == false )
							PQ.push(qnode_next);
					}
				}
			}
			return;
		}
		void print()
		{
			shortest_path_.print();
		}
	};
}

#endif