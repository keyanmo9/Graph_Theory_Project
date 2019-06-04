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
//ActorGraph::ActorGraph(void) {}
/*ActorGraph::~ActorGraph() {
  deleteAll();
}*/

void ActorGraph::deleteAll() {
    for(auto i : seen) {           
      delete i.second;                                                            
    }                                                                             
    for(auto i : seenMovies) {         
      delete i.second;                                                            
    }   

}

/*  
unordered_map<string, ActorNode*> seen; 
unordered_map<string, ActorEdge*> seenMovies;
*/
int hold = -1;
//vector<ActorNode*> recover;

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
       auto getTwo = seenMovies.find(movie_title + "#@" + record[2]);
       if(getTwo == seenMovies.end()){
         newLink = new ActorEdge(movie_title+"#@"+record[2], movie_year);
         seenMovies.insert({movie_title + "#@" + record[2], newLink});
       }
       else {
         newLink = seenMovies[movie_title + "#@" + record[2]];
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
    for(unsigned int j = 0; j < collect.size(); j++) {
        ActorEdge* each = collect[j];
      // if not visited
      for(unsigned int m = 0; m < each->actors.size(); m++) {
        if( each->actors[m]->visited == false) {
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
        int dist = curr.second->distance + (1 + (2019 - each->year) );
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
        if( record.size() != 3 ) {
          // we should have exactly 3 columns
          continue;
        }

        string actor_name(record[0]);                                           
        string movie_title(record[1]);                                          
        int movie_year = stoi(record[2]);

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
        auto getTwo = seenMovies.find(movie_title + "#@" + record[2]);          
       if(getTwo == seenMovies.end()){                                          
         newLink = new ActorEdge(movie_title+"#@"+record[2], movie_year);
         seenMovies.insert({movie_title + "#@" + record[2], newLink});
         pq.push(newLink);          
       }                                                                        
       else {                                                                   
         newLink = seenMovies[movie_title + "#@" + record[2]];                  
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
cout<<numEdges<<endl;
cout<<seen.size()<<endl;
    int size = 1;
    // get the edge that has smallest weight
    ActorEdge* least = pq.top();
cout<<"movie is now"+least->movieName<<endl;
    pq.pop();

    // randomly choose one actorNode, and make edge with all other actorNodes
    ActorNode* single = least->actors[0];
    //single->prev = single;
    single->isRoot = true;
    // case that just has one actor in this movie, go to next edge
    if(least->actors.size()<=1) continue;
    // continue making edges
    for(unsigned int i = 1; i < least->actors.size(); i++) {
      ActorNode* root1 = find(single);
      ActorNode* root2 = find(least->actors[i]);
      if(root1 != root2 && numEdges < seen.size() - 1) {
        size++;
        //least->actors[i]->prev = single;

        result += '\n';
        result += "(" + single->actorName + ")<--[" + least->movieName + 
                  "]-->(" + least->actors[i]->actorName + ")";
        setUnion(root1, root2);
        numEdges++;
        weight += 1+(2019-least->year);
      }
    }
    single->size = size;
    single->isRoot = false;
  }
 for (ActorNode* act: vactors) {
    cout<<act->actorName<<endl;
    if (act->prev)
    cout<<act->prev->actorName<<endl<<endl;
 }
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
//cout<<"inside is: "+node->prev->actorName<<endl;
//cout<<"inside node is: "+node->actorName<<endl;
  //ActorNode* real = seen.find(node->actorName)->second;
  if(node->prev == nullptr) {
    return node;
  }
  //if(real->isRoot) {
  //  return real;
  //}
  if(node->prev) {
    node->prev = find(node->prev);
  }
  return node->prev;
}

void ActorGraph::setUnion(ActorNode* node1, ActorNode* node2) {

  while(node1->prev != nullptr) {
    node1 = node1->prev;
  } 
  while(node2->prev != nullptr) {
    node2 = node2->prev;
  }
  if(node1->size < node2->size) {
    node1->prev = node2;
    node2->size = node2->size + node1->size;
  }else{
    node2->prev = node1;
    node1->size = node1->size + node2->size;
 }

/*
 if(find(node1) == find(node2)) {
  return;
 } 
 if(find(node1) == nullptr || find(node2) == nullptr) {
  return;
 } 
 
 ActorNode* tmp1 = find(node1);
 ActorNode* tmp2 = find(node2);
 
 if(tmp1->chil 
*/ 
}
