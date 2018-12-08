///////////////////////////////////////////////////////////////////
//    Copyright (c) 2018 Rohit Sharma. All rights reserved.
//    This program is free software; you can redistribute it and/or
//    modify it under the terms as GNU General Public License.
///////////////////////////////////////////////////////////////////
//
// Reference: Introduction to Algorithms
//            by Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest, Clifford Stein

#ifndef BASIC_GRAPH_H
#define BASIC_GRAPH_H

#include <iostream>
#include <sstream>
#include <set>
#include <vector>

using namespace std;

namespace basicGraph {
	typedef enum { NOT_VISITED = 1, VISITING = 2, VISITED = 3 } NODE_MARKS;
	class bNode;
	class bEdge;

	static struct nodeCompare
	{
		bool operator() (const bNode* n1, const bNode* n2) const;
	} nodeCmp;

	static struct edgeCompare
	{
		bool operator() (const bEdge* d1, const bEdge* d2) const;
	} edgeCmp;

	void tokenizeLine(char*  line, std::vector<std::string> &tokens);

	class bNode {
	private:
		string                         name_;
		set<const bEdge*, edgeCompare> edgelist_;
	public:
		bNode(string name) : name_(name) {}
		string name() const { return name_; }
		bool addEdge(const bEdge* e) {
			if (!e || edgelist_.find(e) != edgelist_.end())
				return false;
			edgelist_.insert(e);
			return true;
		}
		void clearEdgeList() { edgelist_.clear(); }
		set<const bEdge*, edgeCompare>::iterator edgeBegin() const { return edgelist_.begin(); }
		set<const bEdge*, edgeCompare>::iterator edgeEnd()   const { return edgelist_.end(); }
	};

	class bEdge {
	private:
		const bNode* n1_;    // from node for directd graphs
		const bNode* n2_;    // to   node for directed graphs
	public:
		static const long INVALID_WEIGHT;
		bEdge(const bNode* n1 = 0x0, const bNode* n2 = 0x0)
			: n1_(n1), n2_(n2)
		{}
		string name() const
		{
			if (n1_ && n2_)
				return n1_->name() + " " + n2_->name();
			else if (n1_)
				return n1_->name();
			else if (n2_)
				return n2_->name();
			else
				return " ";
		}
		const bNode* n1()             const { return n1_;  }
		const bNode* n2()             const { return n2_;  }
		virtual bool  hasWeight()     const {return false; }
		void swap_nodes() // used to compute transpose of graph
		{
			const bNode* tmp = n1_;
			n1_ = n2_;
			n2_ = tmp;
		}
		const bNode* otherNode(const bNode* n) const
		{
			if (n1_ == n)
				return n2_;
			else if (n2_ == n)
				return n1_;
			else
				return nullptr;
		}
		virtual void print()          const
		{
			cout << name() << "\n";
		}
		virtual ~bEdge() {}
	};
	class bWeightedEdge : public bEdge {
	private:
		const size_t weight_; // weight of the edge.
	public:
		bWeightedEdge(const bNode* n1=0x0, const bNode* n2=0x0, const size_t w=INVALID_WEIGHT)
			:  bEdge(n1, n2), weight_(w)
		{}
		virtual bool  hasWeight()     const { return true; }
		const size_t weight() const { return weight_; }
		virtual void print()          const 
		{ 
			stringstream wt;
			if (weight_ != INVALID_WEIGHT)
				wt << " " << weight_;

			cout << name() << wt.str() << "\n";
		}
		virtual ~bWeightedEdge() {}
	};

	class bGraph {
	private:
		bool                           isDirected_;
		set<const bNode*, nodeCompare> nodeset_;
		set<const bEdge*, edgeCompare> edgeset_;
	public:
		bGraph(bool directed=false) : isDirected_(directed) {}
		bGraph(const bGraph& other);
		void setDirected(bool directed) { isDirected_ = directed; }
		bool directed() const { return isDirected_;  }
		const bNode* findNode(string name) {
			if (name.empty())
				return nullptr;

			bNode nd(name);
			set<const bNode*, nodeCompare>::iterator iter = nodeset_.find(&nd);
			return (iter != nodeset_.end() ? *iter : nullptr);
		}
		bNode* addNode(string name) {
			const bNode* n = findNode(name);
			if (!n) {
				n = new bNode(name);
				nodeset_.insert(n);
			}
			return const_cast<bNode*>(n);
		}
		const bEdge* findEdge(const bNode* n1, const bNode* n2) {
			if (!n1 || !n2)
				return nullptr;

			bEdge edge12(n1, n2);
			set<const bEdge*, edgeCompare>::iterator iter = edgeset_.find(&edge12);
			if (!isDirected_ && iter == edgeset_.end()) {
				bEdge edge21(n2, n1);
				iter = edgeset_.find(&edge21);
			}
			return (iter != edgeset_.end() ? *iter : nullptr);
		}
		const bEdge* addEdge(const bNode* n1, const bNode* n2, size_t weight=bEdge::INVALID_WEIGHT) {
			if (!n1 || !n2)
				return nullptr;

			const bEdge* e = findEdge(n1, n2);
			if (!e) {
				e = weight == bEdge::INVALID_WEIGHT ? 
					new bEdge(n1, n2) : 
					new bWeightedEdge(n1, n2, weight);
				edgeset_.insert(e);
			}
			return e;
		}
		void addNodesAndEdge(string, string, size_t);
		size_t nNodes() const { return nodeset_.size(); }
		size_t nEdges() const { return edgeset_.size(); }

		// graph access methods
		set<const bNode*, nodeCompare>::iterator nodeBegin() const { return nodeset_.begin(); }
		set<const bNode*, nodeCompare>::iterator nodeEnd()   const { return nodeset_.end();   }
		set<const bEdge*, edgeCompare>::iterator edgeBegin() const { return edgeset_.begin(); }
		set<const bEdge*, edgeCompare>::iterator edgeEnd()   const { return edgeset_.end();   }

		void print() const;
		static bGraph* readBasicGraph(string filename);
		~bGraph();

	};
}

#endif //BASIC_GRAPH_H