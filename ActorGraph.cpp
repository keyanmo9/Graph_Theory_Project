/*
 * ActorGraph.cpp
 * Author: Keyan Mo
 * Date: 2019/05/27
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in imdb_2019.tsv. Feel free to modify any/all aspects as you wish.
 */
 
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "ActorGraph.hpp"
#include "ActorNode.hpp" 
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

/**
 * Constructor of the Actor graph
 */ 
ActorGraph::ActorGraph(void) {}
unordered_map<string, ActorNode*> seen; 
unordered_map<string,int> seenActors;                                   
unordered_map<string,int> seenMovies;
int hold = -1;

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
bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {

    // Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;
  
    // keep reading lines until the end of file is reached
    while (infile) {
        string s;
    
        // get the next line
        if (!getline( infile, s )) break;

        if (!have_header) {

            // skip the header
            have_header = true;
            continue;
        }

        istringstream ss( s );
        vector <string> record;

        while (ss) {
            string next;
      
            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }
    
        if (record.size() != 3) {
            // we should have exactly 3 columns
            continue;
        }

        string actor_name(record[0]);
        string movie_title(record[1]);
        int movie_year = stoi(record[2]);
        
        // TODO: we have an actor/movie relationship, now what?
        
        // check if actors already exist 
        auto get = seenActors.find(actor_name);

        ActorNode* curr = new ActorNode(actor_name);
        bool extraEdge = true;
        bool exist = false;
        // if not exist, create a node for it
        if (get == seenActors.end()) {
            seenActors.insert({actor_name, 1});
            seen.insert({actor_name, curr});
        }
        else {
           curr = seen.find(actor_name)->second;
        }
       // establish a new edge
       ActorEdge* newLink = new ActorEdge({movie_title+"#@"+record[2], nullptr});
       // check if a movie already be seen before
       // if no one is in the same movie
       auto getTwo = seenMovies.find(movie_title+record[2]);
       if(getTwo == seenMovies.end()){
         // establish a relationship and point to nullptr
	 curr->connect.push_back(newLink);
         seenMovies.insert({movie_title + record[2], 1});
       }
       // if others in the same movie, add more edges between them
       else{
         unordered_map<string, ActorNode*>:: iterator it = seen.begin();
         while( it != seen.end() ) {
           int getSize = it->second->connect.size();
           for(unsigned int i = 0; i < getSize; i++) {
             hold++;
             if(it->second->connect[i]->edge.first == movie_title+"#@"+record[2]
                && it->second->actorName != curr->actorName) {
           // if just has one person in the same film
               // if the edge currently points to nullptr, change it
               if(it->second->connect[hold]->edge.second == nullptr) {
                 it->second->connect[hold]->edge.second = curr;
                 hold = -1;
                 
                 ActorEdge* newLink = new ActorEdge({movie_title+"#@"+record[2], it->second});
                 //newLink->edge.second = it->first;
                 curr->connect.push_back(newLink);
               }
               // if this is not the first collision, add more edges
               else{
                 ActorEdge* moreLink = new ActorEdge({movie_title+"#@"+record[2], curr});
                 it->second->connect.push_back(moreLink);
  		 ActorEdge* newLink = new ActorEdge({movie_title+"#@"+record[2], it->second});
                 //newLink->edge.second = it->first;
                 curr->connect.push_back(newLink);
              }
            }
          }
          hold = -1;
          it++;
          }
        }
    }
    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();
    
    return true;
}

string ActorGraph::bfs(ActorNode* one, ActorNode* two) {
  ActorNode* left = nullptr;
  ActorNode* right = nullptr;
  
  unordered_map<string, ActorNode*>::iterator finding;
  if(seen.find(one->actorName)!=seen.end()) {
    finding = seen.find(one->actorName);
    left = finding->second;
  } else {
      return "";
    }
  if(seen.find(two->actorName)!=seen.end()) {
    finding = seen.find(two->actorName);
    right = finding->second;  
  } else {
      return "";
    }

  if(left == nullptr || right == nullptr) {
    return "";
  }

  unordered_map<string, ActorNode*>::iterator all;
  for(all = seen.begin(); all != seen.end(); ++all) {
    all->second->visited = false;
    all->second->prev = nullptr;
  }

  // make a queue
  queue<ActorNode*> myQueue;
  myQueue.push(left);
  left->visited = true;

  // while queue is not empty, keep pushing
  while( !myQueue.empty() ) {
    // get the first node in the queue and then pop out
    ActorNode* now = myQueue.front();
    myQueue.pop();
    
    // if already found
    if( now == right ) {
      break;
    }

    // otherwise, get the all connections in the current node
    vector<ActorEdge*> collect = now->connect;
    if(collect.size()==0) {
      string empty = "";
      return empty;
    }
    vector<ActorEdge*>::iterator allEdge;
    // for each of current node's neighbors
    //for( allEdge = collect.begin(); allEdge != collect.end(); ++allEdge ) {
      for(unsigned int j = 0; j < collect.size(); j++) {
      ActorEdge* each = collect[j];
      // if not visited
      if( each->edge.second != nullptr && each->edge.second->visited == false) {
        each->edge.second->visited = true;
        each->edge.second->prev = now;
        each->edge.second->movie = each->edge.first;
        myQueue.push(each->edge.second);
      }
    }
  }
  // make a string to output
  string output = "";
  string movieName = "";
  
  output = "(" + right->actorName + ")";
  ActorNode* trace = right;
  // keep printing the link
  while( trace != left ) {
    output.insert(0, "--[" + trace->movie + "]-->");
    output.insert(0, "(" + trace->prev->actorName + ")");
    trace = trace->prev;
  }
  return output;
}
