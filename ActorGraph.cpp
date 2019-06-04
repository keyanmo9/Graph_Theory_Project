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
const int TWO = 2;
const int THREE = 3;
const int YEAR = 2019;

using namespace std;


void ActorGraph::deleteAll() {
    for(auto i : seen) {           
      delete i.second;                                                            
    }                                                                             
    for(auto i : seenMovies) {         
      delete i.second;                                                            
    }   

}

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
      
        // get the next string before hitting a tab character
            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }
    
        if (record.size() != THREE) {
            // we should have exactly 3 columns
            continue;
        }

        string actor_name(record[0]);
        string movie_title(record[1]);
        int movie_year = stoi(record[TWO]);
        
        // TODO: we have an actor/movie relationship, now what?
        
        // check if actors already exist 
        auto get = seen.find(actor_name);

        ActorNode* curr;

        // if not exist, create a node for it
        if (get == seen.end()) {
            curr = new ActorNode(actor_name);
            seen.insert({actor_name, curr});
        }
        else {
           curr = seen.find(actor_name)->second;
        }

       // establish a new edge
       ActorEdge* newLink;
       // check if a movie already be seen before
       auto getTwo = seenMovies.find(movie_title + "#@" + record[TWO]);
       if(getTwo == seenMovies.end()){
         newLink = new ActorEdge(movie_title+"#@"+record[TWO], movie_year);
         seenMovies.insert({movie_title + "#@" + record[TWO], newLink});
       }
       else {
         newLink = seenMovies[movie_title + "#@" + record[TWO]];
       }
       
       // make the connection
       curr->connect.push_back(newLink);
       newLink->actors.push_back(curr);
       
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

  // to find the actual nodes we are operating  
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
  // if one of them is null, return
  if(left == nullptr || right == nullptr) {
    return "";
  }

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
    // if connections is empty, return empty
    if(collect.size()==0) {
      string empty = "";
      return empty;
    }    
    // iterate through all the edges
    vector<ActorEdge*>::iterator allEdge;
    for(unsigned int j = 0; j < collect.size(); j++) {
        ActorEdge* each = collect[j];
      // if not visited
      for(unsigned int m = 0; m < each->actors.size(); m++) {
        if( each->actors[m]->visited == false) {
          // deal with the relationship
          each->actors[m]->visited = true;
          each->actors[m]->prev = now;
          each->actors[m]->movie = each->movieName;
          recover.push_back(each->actors[m]);
          myQueue.push(each->actors[m]);
        }
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
  
  //reset visited to false and prev to nullptr;  

  return output;
}

string ActorGraph::dijkstra(ActorNode* one, ActorNode* two) {
  ActorNode* left = nullptr;                                                    
  ActorNode* right = nullptr;
  
  // finding the actual nodes
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
        int dist = curr.second->distance + (1 + (YEAR - each->year) );
        // if c is less than w.dist, set w.prev = v and w.dist = c
        for(unsigned int m = 0; m < each->actors.size(); m++) {
        if(dist < each->actors[m]->distance) {
          each->actors[m]->distance = dist;
          each->actors[m]->prev = curr.second;
          each->actors[m]->movie = each->movieName;
          // enqueue into the queue
          myQueue.push({dist, each->actors[m]});
        }
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

bool ActorGraph::loadFromFileTwo(const char* in_filename) {
  // Initialize the file stream 
  ifstream infile(in_filename);                                                
  bool have_header = false;

  // keep reading lines until the end of file is reached
  while(infile) {
    string s;
    // get the next line
    if(!getline(infile, s)) break;
      if(!have_header) {
        //skip the header
        have_header = true;
        continue;
      }
      istringstream ss(s);
        vector<string> record;
        while(ss) {
          string next;
          // get the next string before hitting a tab character and put it in 'next'
          if(!getline(ss, next, '\t')) break;
            record.push_back(next);
        }
        if( record.size() != THREE ) {
          // we should have exactly 3 columns
          continue;
        }

        string actor_name(record[0]);                                           
        string movie_title(record[1]);                                          
        int movie_year = stoi(record[TWO]);

        // check if actors already exist
        auto get = seen.find(actor_name);                                       
        ActorNode* curr;
        // if not exist, create a node for it
        if (get == seen.end()) {
            curr = new ActorNode(actor_name); 
            //curr->prev = curr;                    
            seen.insert({actor_name, curr});                                   
            vactors.push_back(curr);
        }                                                                       
        else {                                                                  
           curr = seen.find(actor_name)->second;                                
        }

        // establish a new edge
        ActorEdge* newLink;
       // check if a movie already be seen before
        auto getTwo = seenMovies.find(movie_title + "#@" + record[TWO]);          
       if(getTwo == seenMovies.end()){                                          
         newLink = new ActorEdge(movie_title+"#@"+record[TWO], movie_year);
         seenMovies.insert({movie_title + "#@" + record[TWO], newLink});
         pq.push(newLink);          
       }                                                                        
       else {                                                                   
         newLink = seenMovies[movie_title + "#@" + record[TWO]];                  
       }                                                                        
       // make the connection
       curr->connect.push_back(newLink);                                        
       newLink->actors.push_back(curr); 
                                            
  }
                                                                         
      if (!infile.eof()) {                                                      
        cerr << "Failed to read " << in_filename << "!\n";                      
        return false;                                                           
    }                                                                           
    infile.close();                                                             
                                                                                
    return true;                                                                
}
                                   
string ActorGraph::buildSpinning() {
  unsigned int numEdges = 0;
  unsigned int weight = 0;
  string result;
  while(numEdges < seen.size() - 1){
    int size = 1;
    // get the edge that has smallest weight
    ActorEdge* least = pq.top();
    pq.pop();

    // randomly choose one actorNode, and make edge with all other actorNodes
    ActorNode* single = least->actors[0];
    single->isRoot = true;
    // case that just has one actor in this movie, go to next edge
    if(least->actors.size()<=1) continue;
    // continue making edges
    for(unsigned int i = 1; i < least->actors.size(); i++) {
      ActorNode* root1 = find(single);
      ActorNode* root2 = find(least->actors[i]);
      if(root1 != root2 && numEdges < seen.size() - 1) {
        size++;
        // printing current edge
        result += '\n';
        result += "(" + single->actorName + ")<--[" + least->movieName + 
                  "]-->(" + least->actors[i]->actorName + ")";
        setUnion(root1, root2);
        numEdges++;
        weight += 1+(YEAR-least->year);
      }
    }
    // update size of current subtree
    single->size = size;
    single->isRoot = false;
  }
  // printing information
  string numNode = to_string(seen.size());
  string numEdge = to_string(numEdges);
  string numWeight = to_string(weight);
  result += '\n';
  result += "#NODE CONNECTED: " + numNode;
  result += '\n';
  result += "#EDGE CHOSEN: " + numEdge;
  result += '\n';
  result += "TOTAL EDGE WEIGHTS: " +  numWeight;
  return result;
}

ActorNode* ActorGraph::find(ActorNode* node) {
  // if prev is null, means it does not belong to any set
  if(node->prev == nullptr) {
    return node;
  }
  // recursive call while prev is not null, path compression and find the root
  if(node->prev) {
    node->prev = find(node->prev);
  }
  return node->prev;
}

void ActorGraph::setUnion(ActorNode* node1, ActorNode* node2) {
  // find the root of the set node1 is in
  while(node1->prev != nullptr) {
    node1 = node1->prev;
  } 
  // find the root of the set node2 is in
  while(node2->prev != nullptr) {
    node2 = node2->prev;
  }
  // union by size, decide which root to be the new root of the whole tree
  if(node1->size < node2->size) {
    node1->prev = node2;
    node2->size = node2->size + node1->size;
  }else{
    node2->prev = node1;
    node1->size = node1->size + node2->size;
 }

}
