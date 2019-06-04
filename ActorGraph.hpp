/*
 * ActorGraph.hpp
 * Authors: Keyan Mo, Luting Zhang
 * Userids: cs100sp19ed, cs100sp19ci
 * Date: 05.27.2019
 *
 * This file is meant to exist as a container for starter code that you can use
 * to read the input file format defined imdb_2019.tsv. Feel free to modify 
 * any/all aspects as you wish.
 */

#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include <iostream>
#include "ActorNode.hpp"
#include <unordered_map>
#include <queue>

// Maybe include some data structures here

using namespace std;

class cmpFunc {
public:
  bool operator()(pair<int, ActorNode*> node1, pair<int, ActorNode*> node2) {
     return node1.first > node2.first;
  }
};

class cmp {
public:
  bool operator()(ActorEdge* first, ActorEdge* second) {
    return (1+(2019-first->year)) > (1+(2019-second->year));
  }
};
 
/**
 * TODO: add class header
 */
class ActorGraph {
protected:
    
    // Maybe add class data structure(s) here

public:
    
    unordered_map<string, ActorNode*> seen;
    unordered_map<string, ActorEdge*> seenMovies;
    vector<ActorNode*> recover;
    priority_queue<ActorEdge*, vector<ActorEdge*>, cmp> pq;
    vector<ActorNode*> vactors;
    /**
     * Constuctor of the Actor graph
     */
    ActorGraph(void){};

    // Maybe add some more methods here
    ~ActorGraph(void) {
      deleteAll();
    }

    void deleteAll();
    /** You can modify this method definition as you wish
     *
     * Load the graph from a tab-delimited file of actor->movie relationships.
     *
     * in_filename - input filename
     * use_weighted_edges - if true, compute edge weights as 1 + (2019 - movie_year), 
     *                      otherwise all edge weights will be 1
     *
     * return true if file was loaded sucessfully, false otherwise
     */
    bool loadFromFile(const char* in_filename, bool use_weighted_edges);

    bool loadFromFileTwo(const char* in_filename);
    
    string buildSpinning();
    
    ActorNode* find(ActorNode* node);
 
    void setUnion(ActorNode* node1, ActorNode* node2);

    string bfs(ActorNode* one, ActorNode* two);
    
    string dijkstra(ActorNode* one, ActorNode* two);   
};


#endif // ACTORGRAPH_HPP
