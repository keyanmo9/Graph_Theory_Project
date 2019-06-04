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

/*
 * This class is a compare function that used 
 */
class cmpFunc {
public:
  bool operator()(pair<int, ActorNode*> node1, pair<int, ActorNode*> node2) {
     return node1.first > node2.first;
  }
};

/*
 * This class is a compare function that is used to initialize a priotity queue
 */
class cmp {
public:
  bool operator()(ActorEdge* first, ActorEdge* second) {
    return (1+(2019-first->year)) > (1+(2019-second->year));
  }
};
 
/**
 * This class is a main driver to help find paths between actors using
 * different methods, such as bfs, dijkstra and minimum spinning tree
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

    // Destructor
    ~ActorGraph(void) {
      deleteAll();
    }

    /*
     * Name: deleteAll
     * Description: this method is used to delete all nodes and edges
     * Parameter: none
     * Return: void
     */   
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

    /*
     * Name: loadFromFileTwo
     * Description: load actors, movies and years information from file and
     *              also record the edges information into queue
     * Parameters: in_filename - name of the input file
     * Return: return true if loaded successfully, otherwise false
     */  
    bool loadFromFileTwo(const char* in_filename);
    
    /*
     * Name: buildSpinning
     * Description: help build a minimum spinning tree by linking all the
     *              edges poped from our priority queue that do not create
     *              a cycle
     * Parameters: none
     * return: string - string that has actors' name and their edges
     */    
    string buildSpinning();
    
    /*
     * Name: find
     * Description: find the root of the set a node is belonged to
     * Parameters: node - the node we want to find the root of its set
     * Return: ActorNode* - the root of the set the specific node belongs to
     */    
    ActorNode* find(ActorNode* node);
 
    /*
     * Name: setUnion
     * Description: reconstruct the whole tree by linking smaller subtree
     *              to bigger subtree
     * Parameters: node1 - first node
     *             node2 = second node             
     * return: void
     */   
    void setUnion(ActorNode* node1, ActorNode* node2);

   /*
    * Name: bfs
    * Description: find the shortest path using bfs method between 2 actors
    * Parameters: one - first actor
    * 	          two - second actor
    * Return: string - the string that contains all the connecting info
    */    
    string bfs(ActorNode* one, ActorNode* two);
    

   /*
    * Name: dijkstra
    * Description: find the shortest path using dijkstra between 2 actors
    *              with weighted edge
    * Parameters: one - first actor
    *             two - second actor
    * return: string - the stirng that contains all the connecting info
    */  
    string dijkstra(ActorNode* one, ActorNode* two);   
};


#endif // ACTORGRAPH_HPP
