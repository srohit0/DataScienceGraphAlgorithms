///////////////////////////////////////////////////////////////////
//    Copyright (c) 2018 Rohit Sharma. All rights reserved.
//    This program is free software; you can redistribute it and/or
//    modify it under the terms as GNU General Public License.
///////////////////////////////////////////////////////////////////
//

#include <fstream>
#include "graph.h"

using namespace std;

const long basicGraph::bEdge::INVALID_WEIGHT = LONG_MIN;

bool basicGraph::nodeCompare::operator()(const bNode * n1, const bNode * n2) const
{
	return n1->name() < n2->name();
}

bool basicGraph::edgeCompare::operator()(const bEdge * e1, const bEdge * e2) const
{
	size_t w1 = 0, w2 = 0;
	if (e1->hasWeight() && e2->hasWeight())
	{
		w1 = dynamic_cast<const bWeightedEdge*>(e1)->weight();
		w2 = dynamic_cast<const bWeightedEdge*>(e2)->weight();
	}
	
	return w1 == w2 ? e1->name() < e2->name() : w1 < w2;

}

void basicGraph::tokenizeLine(char* line, std::vector<std::string> &tokens)
{
	stringstream stokens(line);
	string token;

	// Tokenizing w.r.t. space ' ' 
	while (getline(stokens, token, ' '))
	{
		tokens.push_back(token);
	}
}

void
basicGraph::bGraph::print() const
{
	cout << "graph " << (isDirected_ ? "directed" : "undirected") << endl;
	set<const bEdge*, edgeCompare>::iterator eiter = edgeset_.begin();
	for (; eiter != edgeset_.end(); eiter++)
		(*eiter)->print();
}

basicGraph::bGraph *basicGraph::bGraph::readBasicGraph(string filename)
{
	const unsigned int BUFFER_SIZE = 1024;
	char line[BUFFER_SIZE];

	filebuf buf;
	buf.open(filename.c_str(), ios::in);
	if (!buf.is_open()) {
		cerr << "could not open file" << filename << endl;
		return nullptr;
	}
	bGraph* new_graph = new bGraph();

	istream graph_stream(&buf);
	while (!graph_stream.eof() ) { 
		graph_stream.getline(line, BUFFER_SIZE);

		vector<string> tokens;
		tokenizeLine(line, tokens);

		if (tokens.size() == 0 || tokens[0] == "#") {
			continue;
		}
		if (tokens.size() < 2)
		{
			cerr << "invlid input line in graph " << line << endl;
			continue;
		}


		if (tokens[0] == "graph") {
			if (tokens[1] == "undirected")
				new_graph->setDirected(false);
			else if (tokens[1] == "directed")
				new_graph->setDirected(true);
			else
				cerr << "invalid keyword after \'graph\' : " << tokens[1];
		}
		else {
			basicGraph::bNode* node1 = new_graph->addNode(tokens[0]);
			basicGraph::bNode* node2 = new_graph->addNode(tokens[1]);
			size_t weight = tokens.size() > 2 ? 
				stol(tokens[2]) : basicGraph::bEdge::INVALID_WEIGHT;

			const basicGraph::bEdge* edge = new_graph->addEdge(node1, node2, weight);
			node1->addEdge(edge);
			if (!new_graph->directed())
				node2->addEdge(edge);
		}
	}
	buf.close();

	return new_graph;
}

basicGraph::bGraph::~bGraph() {
	set<const bNode*, nodeCompare>::iterator niter = nodeset_.begin();
	for (; niter != nodeset_.end(); niter++)
		delete (*niter);

	set<const bEdge*, edgeCompare>::iterator eiter = edgeset_.begin();
	for (; eiter != edgeset_.end(); eiter++)
		delete(*eiter);
}