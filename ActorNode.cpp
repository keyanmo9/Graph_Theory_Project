/*
 * Filename: ActorNode.cpp
 * Author: Keyan Mo, Luting Zhang
 * UserId: cs100sp19ed, cs100sp19ci
 * Date: 2019/05/27
 */

#include "ActorNode.hpp"
#include <string>
#include <vector>

string ActorNode::getName() {
  return actorName;
}

void ActorNode::addConnect(ActorEdge* edge) {
  connect.push_back(edge);
}

vector<ActorEdge*> ActorNode::getConnect() {
  return connect;
}

bool ActorNode::compareNode(ActorNode* other) {
  return this.getName() == other.getName();
}

ActorNode::~ActorNode() {};
