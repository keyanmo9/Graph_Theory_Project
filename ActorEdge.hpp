/*
 * Filename: ActorEdge.hpp
 * Author: Keyan Mo, Luting Zhang
 * UserId: cs100sp19ed, cs100sp19ci
 * Date: 2019/05/27
 */

#ifndef ACTOREDGE_HPP
#define ACTOREDGE_HPP

#include <string>
#include <vector>
#include <utility>
//#include "ActorNode.hpp"

using namespace std;

class ActorNode;

class ActorEdge {
public:
  pair<string, ActorNode*> edge;
  
  ActorEdge(string movieName, ActorNode* nextActor): edge({movieName, nextActor}){}
  ~ActorEdge();
};

#endif
