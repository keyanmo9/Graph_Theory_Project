/* 
 * Filename: readme.md
 * Authors: Luting Zhang, Keyan Mo
 * Userids: cs100sp19ci, cs100sp19ed
 * Date: 06/02/2019
 */

1. Given an undirected graph, I can verify it is connected by making a DFS/BFS
   traversal on the graph. The basic idea is if we have visited every vertex in
   the graph, then the graph is connected. Otherwise, it is unconnected.

   Below are the steps if we use DFS traversal:
   DFS(S,G):
     Initialize stack
     Initialize all vertices as not visited
     Add S to stack
     while stack is not empty:
	pop node curr from the top of stack
  	set curr as visited
 	for each of curr's neighbors, m
	   if m is not visted
 		add m into the stack

   The runtime for BFS/DFS is O(|V| + |E|), V is the number of vertices and E
   is the number of edges. 

2. I can find the number of connected components in an undirected graph by 
   using a counter to count how many times I have called DFS

   Below are the steps:
   numberOfConnectedGraph():
     Store all vertices into a vector
     Initialize a counter count
     while the vector is not empty:
       DFS the first element in the vector
       delete the vertices that we have visited in the vector
       count++
     return count
  
   The runtime of doing so is still O(|V| + |E|).

3. 
   
     
