#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <list>
#include <queue>
#include "GraphNode.h"
#include "GraphArc.h"


class Graph {
private:
	
public:           
    // Constructor and destructor functions
						Graph() ;
						~Graph();

    // Accessors
    vector<GraphNode*> nodeArray() const {
       return _nodes;
    }
	


    void				removeNode( int index );
    bool				addArc( int from, int to, float weight, bool directed = true );
    void				removeArc( int from, int to );
	GraphArc*			getArc(int from, int to);
	void				reset();
	int					geNodesSize();
	
	//Pathfinding Assignment
	void				aStar(GraphNode* pStart, GraphNode* pDest, std::vector<GraphNode *>& path);

private:
	vector<GraphNode*>		_nodes;

	class NodeSearchCostComparer {
	public:
		bool operator()(GraphNode * n1, GraphNode * n2) {
			int f1 = n1->hCost() + n1->gCost();
			int f2 = n2->hCost() + n2->gCost();
			return f1 > f2;
		}
	};

};


Graph::Graph()
{
	_nodes = vector<GraphNode*>();
}


Graph::~Graph() {
   for(int index = 0; index < _nodes.size(); index++ ) 
   {
        if(_nodes[index] != nullptr ) 
		{
            delete _nodes[index];
			_nodes[index] = nullptr;
        }
   }
   _nodes.clear();
}



void Graph::removeNode(int index) {
	
	if (_nodes[index] != nullptr) {

         GraphArc* arc = nullptr;
         for(int node = 0; node < _nodes.size(); node++ ) {
              if(_nodes[node] != nullptr) {

                  arc = _nodes[node]->getArc(_nodes[index] );
				  
              }
			 
              if( arc != nullptr) {
				  removeArc(node, index);
              }
         }
       // delete _nodes[index];
		//_nodes[index] = nullptr;
    }
}

bool Graph::addArc(int from, int to, float weight, bool directed) {
     bool proceed = true; 
     // make sure both nodes exist.
     if(_nodes[from] == 0 || _nodes[to] == 0 ) {
         proceed = false;
     }
        
     // if an arc already exists we should not proceed
     if(_nodes[from]->getArc(_nodes[to] ) != 0 ) {
         proceed = false;
     }

     if (proceed == true) {
        // add the arc to the "from" node.
		 _nodes[from]->addArc(_nodes[to], weight);
		 if (directed == false) //add node back the other way if undirected
			 _nodes[to]->addArc(_nodes[from], weight);
     }
        
     return proceed;
}

void Graph::removeArc(int from, int to) {
     // Make sure that the node exists before trying to remove
     // an arc from it.
     bool nodeExists = true;
     if(_nodes[from] == NULL || _nodes[to] == NULL ) {
         nodeExists = false;
     }

     if (nodeExists == true) {
        // remove the arc.
		 
		 _nodes[from]->removeArc(_nodes[to] );
     }
}


GraphArc* Graph::getArc(int from, int to) {
     GraphArc* pArc = 0;
     // make sure the to and from nodes exist
     if(_nodes[from] != NULL && _nodes[to] != NULL ) {
         pArc = _nodes[from]->getArc(_nodes[to] );
     }
                
     return pArc;
}

int Graph::geNodesSize() {

	return _nodes.size();
}

void Graph::reset() {
	int index;
	for (index = 0; index < _nodes.size(); index++) {
		if (_nodes[index] != NULL) {
			_nodes[index]->reset();
		}
	}
}

void Graph::aStar(GraphNode* pStart, GraphNode* pDest, std::vector<GraphNode *>& path) {

	if (pStart != 0 && pDest != 0) {
		priority_queue<GraphNode*, vector<GraphNode*>, NodeSearchCostComparer> pq;
		pq.push(pStart);
		pStart->setMarked(true);
		pStart->setHCost(0);
		pStart->setGCost(0);

		while (pq.size() != 0 && pq.top() != pDest) {
			list<GraphArc>::const_iterator iter = pq.top()->arcList().begin();
			list<GraphArc>::const_iterator endIter = pq.top()->arcList().end();

			for (; iter != endIter; iter++) {
				GraphNode * child = (*iter).node();
				if (child != pq.top()->getPrevious()) {
					GraphArc arc = (*iter);
					//Sleep(1000);
					int Hc = child->hCost();
					int Gc = pq.top()->gCost() + arc.weight();
					int Fc = Hc + Gc;
					if (Fc < child->fCost() || child->gCost() == -1) {  //is G(n) not set, H(n) should be always set with setHeuristics()
						//child->setHCost(Hc);
						child->setGCost(Gc);
						child->setPrevious(pq.top());
					}

					if (child->marked() == false) {
						pq.push(child);
						child->setMarked(true);
						//child->setColour(sf::Color(0, 128, 128, 255));
					}
				}
			}
			pq.pop();
		}

		if (pq.size() != 0 && pq.top() == pDest) {
			for (GraphNode* previous = pDest; previous->getPrevious() != 0; previous = previous->getPrevious()) {
				path.push_back(previous);

			}
			path.push_back(pStart);
			std::reverse(path.begin(), path.end());
		}
		else if (pq.size() == 0)
			cout << "Couldn't find path." << endl;
	}
}




#endif
