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
#include <climits>
#include "ActorGraph.hpp"
#include "ActorNode.hpp"
#include "ActorEdge.hpp"
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
unordered_map<string, vector<ActorNode*>> seenMovies;
int hold = -1;
vector<ActorNode*> recover;

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

        // if not exist, create a node for it
        if (get == seenActors.end()) {
            seenActors.insert({actor_name, 1});
            seen.insert({actor_name, curr});
        }
        else {
           curr = (seen.find(actor_name))->second;
        }

        //seenMovies.insert({movie_title + record[2], curr});

       // establish a new edge
       ActorEdge* newLink = new ActorEdge({movie_title+"#@"+record[2], nullptr});
       // check if a movie already be seen before
       // if no one is in the same movie
       auto getTwo = seenMovies.find(movie_title+record[2]);
       if(getTwo == seenMovies.end()){
         // establish a relationship and point to nullptr
	 curr->connect.push_back(newLink);
         vector<ActorNode*> added;
         added.push_back(curr);
         seenMovies.insert({movie_title + record[2], added});
       }
       
       // if others in the same movie, add more edges between them
       else{
         (seenMovies.find(movie_title + record[2]))->second.push_back(curr);
         //unordered_map<string, ActorNode*>:: iterator it = seen.begin();
         //while( it != seen.end() ) {
           //int getSize = it->second->connect.size();
         vector<ActorNode*> list = (seenMovies.find(movie_title+record[2]))->second;
         for(unsigned int k = 0; k < list.size(); k++) {
           int getSize = list[k]->connect.size();
           ActorNode* found = list[k];
           for(unsigned int i = 0; i < getSize; i++) {
             hold++;
             ActorNode* real = seen.find(list[k]->actorName)->second;
             if(real->connect[i]->edge.first == movie_title+"#@"+record[2]
                && real->actorName != curr->actorName) {
               // if the edge currently points to nullptr, change it
               if(real->connect[hold]->edge.second == nullptr) {
                 real->connect[hold]->edge.second = curr;
                 hold = -1;
                 
                 ActorEdge* newLink = new ActorEdge({movie_title+"#@"+record[2], real});
                 curr->connect.push_back(newLink);
               }
               // if this is not the first collision, add more edges
               else{
                 ActorEdge* moreLink = new ActorEdge({movie_title+"#@"+record[2], curr});
                 real->connect.push_back(moreLink);
  		 ActorEdge* newLink = new ActorEdge({movie_title+"#@"+record[2], real});
                 curr->connect.push_back(newLink);
                 //delete(moreLink);
              }
            }
          }
        
          hold = -1;
         }
         // it++;
         // }
        }
      //delete (curr);
      //delete (newLink);
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
/*
  unordered_map<string, ActorNode*>::iterator all;
  for(all = seen.begin(); all != seen.end(); ++all) {
    all->second->visited = false;
    all->second->prev = nullptr;
  }
*/

  // update vector recover, set all nodes to be unvisited and prev to be nullptr
  for( unsigned int u = 0; u < recover.size(); u++) {
    recover[u]->visited = false;
    recover[u]->prev = nullptr;
  }
  recover.clear();


  // make a queue
  queue<ActorNode*> myQueue;
  myQueue.push(left);
  left->visited = true;
  recover.push_back(left);


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

    //collect = now->connect;
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
        recover.push_back(each->edge.second);
        myQueue.push(each->edge.second);
      }
    }
  }

  // recovering
//  for(unsigned int u = 0; u < recover.size(); u++) {
//    recover[u]->edge.second->visited = false;
//    recover[u]->edge.second->prev = nullptr;
//  }

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
  
  //reset visited to false and prev to nullptr;  

  return output;
}

string ActorGraph::dijkstra(ActorNode* one, ActorNode* two) {
  ActorNode* left = nullptr;                                                    
  ActorNode* right = nullptr;

  unordered_map<string, ActorNode*>::iterator find1 = seen.find(one->actorName);
  if(find1 != seen.end()) {
    left = find1->second;
  } else {
      return "";
    }
  unordered_map<string, ActorNode*>::iterator find2 = seen.find(two->actorName);
  if(find2 != seen.end()) {
    right = find2->second;
  } else {
      return "";
    }
 
  // Reset all the nodes 
  unordered_map<string, ActorNode*>::iterator all;
  for(all = seen.begin(); all != seen.end(); ++all) {
    all->second->visited = false;
    all->second->prev = nullptr;
    all->second->distance = INT_MAX;
  }

  // Initialize a priority queue
  priority_queue<pair<int,ActorNode*>, vector<pair<int,ActorNode*>>,cmpFunc>
		myQueue;

  myQueue.push({0,left});
  left->distance = 0;
  
  // While myQueue is not empty, continue pushing
  while(!myQueue.empty()){
    pair<int, ActorNode*> curr = myQueue.top();
    myQueue.pop();
 
    // If curr is not visited 
    if(curr.second->visited == false){
      curr.second->visited = true;
      vector<ActorEdge*> collect = curr.second->connect;
      // for each of curr's neighbors
      for(vector<ActorEdge*>::iterator all = collect.begin(); 
          all != collect.end(); ++all) {
        ActorEdge * each = *all;
        // distance to w through v, c = v.dist + edgeWeight(v, w)
        int dist = curr.second->distance + (1 + (2019) );
        // if c is less than w.dist, set w.prev = v and w.dist = c
        if(dist < each->edge.second->distance) {
          each->edge.second->distance = dist;
          each->edge.second->prev = curr.second;
          // enqueue into the queue
          myQueue.push({dist, each->edge.second});
        }
      }
    }
  }
  
  // make a string to output
  string output = "";
  string movieName = "";
  
  output = "(" + right->actorName + ")";
  ActorNode* trace = right;

  while( trace != left ) {
    output.insert(0, "--[" + trace->movie + "]-->");
    output.insert(0, "(" + trace->prev->actorName + ")");
    trace = trace->prev;
  }
  return output;
}
