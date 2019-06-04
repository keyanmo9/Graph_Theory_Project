/*
 * Filename: movietraveler.cpp
 * Author: Keyan Mo, Luting Zhang
 * Account: cs100sp19ed, cs100sp19ci
 * Description: This is the main driver of the program that finds optimal route
 * 		to connected all the actors and actresses.
 * Date: 6/2/2019
 */

#include "ActorGraph.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>

using namespace std;
const int ARGC_NUM = 3;
const int TWO = 2;

int main(int argc, char ** argv) {
  // Store command lines
  char * program_name = argv[0];
  char * input = argv[1];
  char * output = argv[TWO];

  // Check if 2 arguments entered
  if(argc != ARGC_NUM) {
    cerr << program_name << " called with incorrect arguments." << endl;
    return -1;
  }
  
  // Open output file for writing the output
  ofstream outFile;
  outFile.open(output);
  outFile << "(actor)<--[movie#@year]-->(actor)";

  // Build the graph
  ActorGraph graph;
  graph.loadFromFileTwo(input);
  string result = graph.buildSpinning();
  outFile << result << endl;
  
  outFile.close();
  
  return 0;
}
