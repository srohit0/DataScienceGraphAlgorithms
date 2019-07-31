///////////////////////////////////////////////////////////////////
//    Copyright (c) 2018 Rohit Sharma. All rights reserved.
//    This program is free software; you can redistribute it and/or
//    modify it under the terms as GNU General Public License.
///////////////////////////////////////////////////////////////////
//
//
// Minimal Spanning Trees argorithms.
//

#ifndef GRAPH_MST_H
#define GRAPH_MST_H

#include <algorithm>
#include "graph.h"
#include "heap.h"

using namespace std;

namespace MST {

	struct treeNode {
	public:
		const basicGraph::bNode* node_;   // to node in graph node
		treeNode*                parent_; // from node in graph node
		size_t                   iset_;   // index of set
		size_t                   weight_; // weight of the edge from parent to this node.
		treeNode(const basicGraph::bNode* node, size_t i = 0, size_t wt = 0) :
			node_(node), parent_(nullptr), iset_(i), weight_(wt)
		{}
		void setParent(treeNode* other, size_t wt) {
			other->parent_ = this;
			other->weight_ = wt;
		}
	};
	struct treeNodeCompare
	{
		bool operator() (const treeNode* tn1, const treeNode* tn2) const
		{
			return basicGraph::nodeCmp.operator()(tn1->node_, tn2->node_);
		}
	};

	class minTree {
	public:
		set<treeNode*, treeNodeCompare> tree_;
		virtual void build() = 0;
		bool nodePartOfMST(const basicGraph::bNode* gnode)
		{
            treeNode tmpTreeNode = treeNode(gnode);
			return tree_.find(&tmpTreeNode) != tree_.end();
		}
		treeNode* getTreeNode(const basicGraph::bNode* gnode)
		{
            treeNode tmpTreeNode = treeNode(gnode);
			set<treeNode*, treeNodeCompare>::iterator niter = tree_.find(&tmpTreeNode);
			return niter == tree_.end() ? nullptr : *niter;
		}
		void print(bool edges_only=false)
		{
			set<treeNode*, treeNodeCompare>::iterator niter = tree_.begin();
			for (; niter != tree_.end(); niter++)
			{
				treeNode* tnode = (*niter);
				if (!edges_only || tnode->parent_)
				{
					cout << (tnode->parent_ ? tnode->parent_->node_->name() : "root") << " "
						<< tnode->node_->name() << " "
						<< tnode->weight_;
					//				cout << " (set " << tnode->iset_ << ")";
					cout << "\n";
				}
			}
		}
		~minTree()
		{
			set<treeNode*, treeNodeCompare>::iterator niter = tree_.begin();
			for (; niter != tree_.end(); niter++)
				delete (*niter);
			tree_.clear();
		}
	};

	// ####################
	// Kruskal's algorithm
	// ####################
	// Kruskal's algorithm is a minimum-spanning-tree algorithm which finds an edge
	// of the least possible weight that connects any two trees in the forest. It 
	// is a greedy algorithm in graph theory as it finds a minimum spanning tree 
	// for a connected weighted graph adding increasing cost arcs at each step.
	// 
	// Note: This only works for undirected graphs.

	// Pseudo Code:
	//   KRUSKAL-MST(G, w) 
	//      T: = Ø
	//        for each vertex u in V
	//	      	MAKE - SET(tree_, u)
	//		  end for
	//	    for each edge(u, v) in E (sortedEdges) in order of nondecreasing weight
	//        if FIND - SET(tree_, u) != FIND - SET(tree_, v)
	//           UNION - SET(tree_, u, v)
	//           T : = T U{ (u,v) }
	//        end for
	//	return T

	class kruskal : public minTree {
	private:
		const basicGraph::bGraph*       graph_;

		// unionSet is used to combine two trees to one by adding an 
		// edge between two trees with nodes in two disjoint sets.
		// TODO: complexity is linear. Can be reduced to logarithmic
		void unionSet(treeNode* u, treeNode* v, size_t wt, bool directed)
		{
			size_t v_iset      = v->iset_;
			set<treeNode*, treeNodeCompare>::iterator niter = tree_.begin();
			for (; niter != tree_.end(); niter++)
			{
				treeNode* tnode = *niter;
				if (tnode->iset_ == v_iset)
				{
					tnode->iset_ = u->iset_;
				}
			}
			// change v's parent-child relationship for undirected graphs.
			if (!directed && v->parent_ != nullptr)
			{
				v->parent_->parent_ = v;
				v->parent_->weight_ = v->weight_;
			}

			// set v's parent to u.
			u->setParent(v, wt);

			return;
		}
	public:
		kruskal(const basicGraph::bGraph* graph) : minTree(), graph_(graph)
		{}
		~kruskal()
		{}

		// build MST.
		void build()
		{
			if (!graph_)
			{
				cerr << "Error: graph not found.";
				return;
			}			if (graph_->nNodes() == 0)
			{
				cerr << "Error: no nodes found in the graph.";
				return;
			}

			if (graph_->directed())
				cerr << "Warning: Kruskal's algorithm may produce invalid tree or \n" 
					 << "         more than one spanning multitree on directed graphs.\n";


			// create an empty tree node and add it to set.
			size_t set_index = 1;
			set<const basicGraph::bNode*, basicGraph::nodeCompare>::iterator niter;
			for (niter = graph_->nodeBegin(); niter != graph_->nodeEnd(); niter++)
			{
				tree_.insert(new treeNode(*niter, set_index++));
			}

			vector<const basicGraph::bEdge*> sortedEdges;
			set<const basicGraph::bEdge*, basicGraph::edgeCompare>::iterator eiter;
			for (eiter = graph_->edgeBegin(); eiter != graph_->edgeEnd(); eiter++)
			{
				sortedEdges.push_back(*eiter);
			}
			sort(sortedEdges.begin(), sortedEdges.end(), basicGraph::edgeCmp);

			for (size_t i = 0; i < sortedEdges.size(); i++)
			{
				const basicGraph::bNode* u = sortedEdges[i]->n1();
				const basicGraph::bNode* v = sortedEdges[i]->n2();
                treeNode tmpUNode = treeNode(u);
                treeNode tmpVNode = treeNode(v);
				auto uset = tree_.find(&tmpUNode);
				auto vset = tree_.find(&tmpVNode);
				if (uset != tree_.end() && vset != tree_.end() && 
					(*uset)->iset_ != (*vset)->iset_ )
				{
					treeNode* tuset = *uset;
					treeNode* tvset = *vset;
					unionSet(tuset, tvset, dynamic_cast<const basicGraph::bWeightedEdge*>(sortedEdges[i])->weight(), graph_->directed());
				}
			}
		}
	};


	// #################
	// Prim's Algorithm
	// #################
	// procedure prim(G,W)
	//    for i = 1 to n do
	//      MinHeap[i] ← i
	//      WhereInHeap[i] ← i
	//      d[i] ← ∞
	//      outside[i] ← true
	//      π[i] ← NIL
	//    end for
	//    d[1] ← 0
	//    for i = n downto 1 do
	//      u ← MinHeap[1]
	//      MinHeap[1] ← MinHeap(n)
	//      WhereInHeap[MinHeap[1]] ← SiftDown(MinHeap, 1, n - 1, d)
	//      for each v ∈ adj[u] do
	//        if v ∈ outside and W[u, v] < d[v] then
	//        	d[v] ← W[u, v]
	//        	π[v] ← u
	//        	WhereInHeap[v] ← SiftUp(MinHeap, WhereInHeap[v], d)
	//     	  end if
	//	    end for
	//	  end for
	//	end procedure

	class prim : public minTree {
	private:
		const basicGraph::bGraph*       graph_;

		bool nodeNotInMST(treeNode* tn)
		{
			return tree_.find(tn) == tree_.end();
		}
		void addEdgesToHeap(const basicGraph::bNode* node,
			Heap<const basicGraph::bWeightedEdge*, basicGraph::edgeCompare>& minEdgeHeap)
		{
			set<const basicGraph::bEdge*, basicGraph::edgeCompare>::iterator eiter = node->edgeBegin();
			for (; eiter != node->edgeEnd(); eiter++) {
				if (! nodePartOfMST((*eiter)->n1()) || !nodePartOfMST((*eiter)->n2()) )
					minEdgeHeap.push(dynamic_cast<const basicGraph::bWeightedEdge*>(*eiter));
			}
		}
	public:
		prim(const basicGraph::bGraph* graph) : minTree(), graph_(graph)
		{}
		// build MST.
		void build()
		{
			if (!graph_)
			{
				cerr << "Error: graph not found.";
				return;
			}
			if (graph_->nNodes() == 0)
			{
				cerr << "Error: no nodes found in the graph.";
				return;
			}
			if (graph_->directed())
			{
				cerr << "Warning: Prim's algorithm may produce invalid tree or \n"
					<< "         more than one spanning multitree on directed graphs.\n";
			}

			Heap<const basicGraph::bWeightedEdge*, basicGraph::edgeCompare> minEdgeHeap;

			// Initialize a tree with a single vertex, chosen arbitrrily from the graph.
			set<const basicGraph::bNode*, basicGraph::nodeCompare>::iterator niter = graph_->nodeBegin();
			const basicGraph::bNode* node = *niter;
			tree_.insert(new treeNode(node, 0));

			// Grow the tree by one edge : of the edges that connect the tree
			// to vertices not yet in the tree, find the minimum - weight edge, 
			// and transfer it to the tree.

			addEdgesToHeap(node, minEdgeHeap);
			const basicGraph::bWeightedEdge* minEdge = nullptr;
			while (minEdgeHeap.pop(minEdge))
			{
				// get the node, not in MST
				MST::treeNode* tn1 = getTreeNode(minEdge->n1());
				MST::treeNode* tn2 = getTreeNode(minEdge->n2());
				if (tn1 && tn2==nullptr)
				{
					tn2 = new treeNode(minEdge->n2());
					tree_.insert(tn2);
					tn1->setParent(tn2, minEdge->weight());
					addEdgesToHeap(tn2->node_, minEdgeHeap);
				}
				else if (tn1==nullptr && tn2)
				{
					tn1 = new treeNode(minEdge->n1());
					tree_.insert(tn1);
					tn2->setParent(tn1, minEdge->weight());
					addEdgesToHeap(tn1->node_, minEdgeHeap);
				}
				else
				{
					continue;
				}
			}
		}
	};
}

#endif 

