/*
 * Filename: pathfinder.cpp
 * Author: Keyan Mo, Luting Zhang
 * UserId: cs100sp19ed, cs100sp19ci
 * Date: 2019/05/28
 */
#include "ActorGraph.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char ** argv) {
  // check if 4 arguments entered
  if( argc != 5 ) {
    return -1;
  }

  // open output file to write the output  
  ofstream out;
  out.open(argv[4]);
  out<<"(actor)--[movie#@year]-->(actor)--..."<<endl;

  // make the graph
  ActorGraph graph;
  graph.loadFromFile(argv[1], false);

  // load pairs from the pair file
  ifstream in;
  in.open(argv[3]);
  string get;  

  // continue reading pairs
  getline(in, get);
  while(getline(in, get)) {
    string firstHalf = get.substr(0, get.find('\t'));
    string secondHalf = get.substr(get.find('\t')+1, get.length());
    
    ActorNode* one = new ActorNode(firstHalf);
    ActorNode* two = new ActorNode(secondHalf);
cout<<"Computing path for ("+firstHalf+") -> ("+secondHalf+")"<<endl; 
    string result = graph.bfs(one, two);
  
    delete(one);
    delete(two);
  
    out << result <<endl;
  }

  out.close();
  in.close();

  return 0;
 }
