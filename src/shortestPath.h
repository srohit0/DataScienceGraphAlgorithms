///////////////////////////////////////////////////////////////////
//    Copyright (c) 2018 Rohit Sharma. All rights reserved.
//    This program is free software; you can redistribute it and/or
//    modify it under the terms as GNU General Public License.
///////////////////////////////////////////////////////////////////

// Dijkstra's algorithm 
//     given source node in the graph, the algorithm finds the shortest path between that
//     node and every other node in the graph.
//     It is a single node shortest path and works on both directional and bi-directional path.
//     Reference: https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
//
// A-star algorithm
//     given source and destination node in the graph, the algorithm finds the shortest path
//     between them with exploring as fewer nodes as possible.
//     It relied on a heuristic to provide estimation of distance from every node to destination.
//     This estimate is used to guide the search (using priority Q) and terminate search.
//     It is a single node shortest path and works on both directional and bi-directional path.
//     Reference: https://en.wikipedia.org/wiki/A*_search_algorithm


#ifndef GRAPH_PATH_H
#define GRAPH_PATH_H

#pragma once
#include <algorithm>
#include <queue>
#include "graph.h"
#include "heap.h"

using namespace std;

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
		void print_all_paths()
		{

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

		void print_path(const basicGraph::bNode* dst)
		{
			pathNode* tnode = retrievePathNode(dst);

			vector<pathNode*> src_2_dst;
			for (; tnode != src_ ; tnode = tnode->prev_node_)
			{
				if (tnode == nullptr)
				{
					cout << "no path found.\n";
					return;
				}
				src_2_dst.push_back(tnode);
			} 
			src_2_dst.push_back(src_);

			cout << "start";
			for (int i=src_2_dst.size()-1; i>=0; i--)
				cout << " --(" << src_2_dst[i]->dist_ << ")--> " << src_2_dst[i]->node_->name() ;
			cout << " ...done.\n";
			cout << "distance covered is " << src_2_dst[0]->root_dist_ << ".\n";
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
		pqNode(const basicGraph::bNode* node, size_t dist=LONG_MAX) : node_(node), min_dist_(dist)
		{
			assert(node);
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
 //    add source to PQ                         
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

		size_t node_min_edge(const basicGraph::bNode* node)
		{
			set<const basicGraph::bEdge*, basicGraph::edgeCompare>::iterator eiter = node->edgeBegin();
			return eiter != node->edgeEnd() ? 
				dynamic_cast<const basicGraph::bWeightedEdge*>(*eiter)->weight() :
				0;
		}
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


			pqNode qnode(shortest_path_.src()->node_, 0); // node in priority queue
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
						pqNode qnode_next(other_gnode, node_min_edge(other_gnode));
						if ( PQ.exists(qnode_next) == false )
							PQ.push(qnode_next); 
					}
				}
			}
			return;
		}
		void print()
		{
			shortest_path_.print_all_paths();
		}
	};


//function aStar (Graph, source, destination) :
//    call heuristic h_x to compute estimation of node-destination distance.
//
//    create vertex set in Path
//    for each vertex v in Graph :           // Initialization
//       dist[v] ← INFINITY                  // Unknown distance from source to v
//       prev[v] ← UNDEFINED                 // Previous node in optimal path from source
//       compute h_x[v, destimation]         // distance estimation from v to destination
//
//    dst_dist[source, destimation] ← h_x    // Distance from source to dst
//    src_dist[source] ← 0                   // Distance from source to source
//    priority[source] ← src_dist[source] + dst_dist[source, destination] 
//    add source to PQ 
//    while PQ is not empty:
//        u ← vertex in PQ with min dist[u]   // Node with the least distance
//                                            // will be selected first
//        remove u from PQ
//
//        for each neighbor v of u :
//            alt ← src_dist[u] + length(u, v)
//            if alt < src_dist[v] :          // A shorter path to v has been found
//                src_dist[v] ← alt
//                prev[v] ← u
//
//            alt_priority[v] ← src_dist[v] + dst_dist[v, destination] 
//            if v in PQ
//                if alt_priority[v] < priority[v] 
//                    priority[v] ← = alt_priority[v]
//            else
//                add v to PQ
//
//		terminate if priority(PQ[0]) >= src_dist[destination]
//
//    return dist[], prev[]

	class aStar {
	private:
		const basicGraph::bGraph*  graph_;
		path                       shortest_path_;
		map<const basicGraph::bNode*, size_t, basicGraph::nodeCompare> 
			                       h_x;

		// this returns estimated distance of node to destination node.
		size_t estimate_distance(const basicGraph::bNode* node, const basicGraph::bNode* dst)
		{
			size_t estimate = 
				(h_x.find(node) != h_x.end() && h_x.find(dst) != h_x.end()) ?
				h_x[dst] - h_x[node] : 0;
			return estimate > 0 ? estimate : 0;
		}
		// heuristic is aadmissible if it never overestimates the cost of reaching the goal.
		// Algorithm:
		//       Compute the minimum weight of all graph nodes starting from src.
		// Complexity:
		//       O(V) + O(E)
		void heuristic(const basicGraph::bNode* src)
		{
			const basicGraph::bNode* node;
			queue<const basicGraph::bNode*> nodeq;
			set<const basicGraph::bEdge*, basicGraph::edgeCompare> visited;

			nodeq.push(src);
			h_x[src] = 0;

			while (!nodeq.empty())
			{
				node = nodeq.front();
				nodeq.pop();

				// calculate min arrival wt of every node starting from src.
				set<const basicGraph::bEdge*, basicGraph::edgeCompare>::iterator eiter;
				for (eiter = node->edgeBegin(); eiter != node->edgeEnd(); eiter++)
				{
					// avoid cycles
					if (visited.find(*eiter) != visited.end())
						continue;
					visited.insert(*eiter);

					const basicGraph::bNode* next_node = (*eiter)->otherNode(node);
					size_t wt = dynamic_cast<const basicGraph::bWeightedEdge*>(*eiter)->weight();
					size_t next_node_wt = h_x[node] + wt;
					if (h_x.find(next_node) != h_x.end())
					{
						h_x[next_node] = next_node_wt < h_x[next_node] ? next_node_wt : h_x[next_node];
					}
					else
					{
						h_x[next_node] = next_node_wt;
					}
					nodeq.push(next_node);
				}
			}

			return;
		}
	public:
		aStar(const basicGraph::bNode* src, const basicGraph::bGraph* graph) :
			graph_(graph), shortest_path_(new pathNode(src))
		{
			heuristic(src);
		}
		void build(const basicGraph::bNode* dst)
		{
			Heap<pqNode, pqNodeCompare> PQ; // priority queue, i.e. open set
			pathNode* dstPNode = nullptr;

			set<const basicGraph::bNode*, basicGraph::nodeCompare>::iterator niter;
			for (niter = graph_->nodeBegin(); niter != graph_->nodeEnd(); niter++)
			{
				pathNode* pNode = shortest_path_.retrieveOrAddPathNode(*niter);
				if (*niter == dst)
					dstPNode = pNode;
			}

			pqNode qnode(shortest_path_.src()->node_, 0); 
			PQ.push(qnode); // first node in priority queue

			while (PQ.pop(qnode))
			{
				//cout << qnode.node_->name() << "(" << qnode.min_dist_ << ") -> ";

				pathNode* pnode = shortest_path_.retrievePathNode(qnode.node_);
				assert(pnode);

				set<const basicGraph::bEdge*, basicGraph::edgeCompare>::iterator eiter;
				for (eiter = pnode->node_->edgeBegin(); eiter != pnode->node_->edgeEnd(); eiter++)
				{
					const basicGraph::bNode* other_gnode = (*eiter)->otherNode(pnode->node_);
					size_t dist = dynamic_cast<const basicGraph::bWeightedEdge*>(*eiter)->weight();
					pathNode* other_pnode = shortest_path_.retrievePathNode(other_gnode);

					size_t estimate = estimate_distance(other_gnode, dst);
					size_t alt_root_dist = pnode->root_dist_ + dist;
					size_t src2dst_estimate = alt_root_dist + estimate;

					if ( alt_root_dist < other_pnode->root_dist_ )
						pnode->setParent(other_pnode, dist, alt_root_dist);

					pqNode qnode_next(other_gnode, src2dst_estimate);

					size_t idx;
					if (PQ.index(qnode_next, idx) )
					{
						if (PQ[idx].min_dist_ > src2dst_estimate)
						{
							PQ.erase(qnode_next); // erase old element with wrong priority
							PQ.push(qnode_next);  // add new element
						}
					}
					else
					{
						PQ.push(qnode_next);
					}
				}

				// terminate early, if all nodes in q have distance estimate greater than
				// the destination found distance.
				if (PQ.size() && PQ[0].min_dist_ >= dstPNode->root_dist_)
					break;
			}
			return;
		}
		void print(const basicGraph::bNode* dst)
		{
			shortest_path_.print_path(dst);
		}

	};
}

#endif