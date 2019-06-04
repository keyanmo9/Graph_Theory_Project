/*
 * Filename: pathfinder.cpp
 * Author: Keyan Mo, Luting Zhang
 * UserId: cs100sp19ed, cs100sp19ci
 * Description: This is the main driver of the program that finds the path for
 *   	     	both unweighted and weighted graphs.
 * Date: 2019/05/28
 */
#include "ActorGraph.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
const int ARGC_NUM = 5;
const int FOUR = 4;
const int THREE = 3;
const int TWO = 2;

int main(int argc, char ** argv) {

  // check if 4 arguments entered
  if( argc != ARGC_NUM ) {
    cerr << argv[0] 
         << " movie_cast_tsv_file u/w pairs_tsv_file output_paths_tsv_file"
         << endl;
    return -1;
  }

  // open output file to write the output  
  ofstream out;
  out.open(argv[FOUR]);
  out << "(actor)--[movie#@year]-->(actor)--..." << endl;

  // make the graph
  ActorGraph graph;
  graph.loadFromFile(argv[1], false);

  // load pairs from the pair file
  ifstream in;
  in.open(argv[THREE]);
  string get;  

  // continue reading pairs
  getline(in, get);
  while(getline(in, get)) {
    string firstHalf = get.substr(0, get.find('\t'));
    string secondHalf = get.substr(get.find('\t')+1, get.length());
    
    ActorNode* one = new ActorNode(firstHalf);
    ActorNode* two = new ActorNode(secondHalf);
    cout << "Computing path for (" + firstHalf+ ") -> (" + secondHalf + ")" 
         <<endl; 
    vector<ActorNode*> recover;
    string result;
    string input = argv[TWO];
    if(input.compare("u")==0) {
      result = graph.bfs(one, two);
    }
    else if(input.compare("w")==0){
      result = graph.dijkstra(one, two);
    }
    
    delete one;
    delete two;
  
    out << result <<endl;
  }

  // avoid memory leak
  out.close();
  in.close();

  return 0;
 }
