/*
 * Filename: ActorEdge.hpp
 * Author: Keyan Mo, Luting Zhang
 * UserId: cs100sp19ed, cs100sp19ci
 * DEscription: This file creats the edge that connects actor nodes. 
 * Date: 2019/05/27
 */

#ifndef ACTOREDGE_HPP
#define ACTOREDGE_HPP

#include <string>
#include <vector>
#include <utility>

using namespace std;

class ActorNode;

/*
 * This class creats the edges between actor nodes.
 * In each eage, it contains the information of movieName, movie's year, and
 * a list of actors that have been connected.
 */
class ActorEdge {
public:
  string movieName;
  int year;
  vector<ActorNode*> actors;

  ActorEdge(string movieName, int year): movieName(movieName), year(year){}
};

#endif
