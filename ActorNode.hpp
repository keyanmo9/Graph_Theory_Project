/*
 * Filename: ActorNode.hpp
 * Author: Keyan Mo, Luting Zhang
 * Userid: cs100sp19ed, cs100sp19ci
 * Date: 2019/05/27
 */

#ifndef ACTORNODE_HPP
#define ACTORNODE_HPP
#include <string>
#include "ActorEdge.hpp"

using namespace std;

/**
 * This class help creates actor nodes
 */

class ActorNode {
public:
  string actorName;
  vector<ActorEdge*> connect;
  bool visited;
  ActorNode * prev;
  string movie;
  ActorNode(string actorName): actorName(actorName) {
    visited = false;
    prev = nullptr;
  }

  // helper to get the current node actor name
  string getName();

  // helper to add a connection
  void addConnect(ActorEdge* edge);

  // helper to get a connection
  vector<ActorEdge*> getConnect();
 
  // helper to compare actors' names
  bool compareNode(ActorNode* other);
 
  // Destructor
  ~ActorNode();
};

#endif
