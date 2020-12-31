#include "algorithms.h"
#include <iostream>

#include <list>

using namespace std;

Algorithms::Algorithms(Graph graph) : g(graph) {}

Graph Algorithms::BFS() {

  // initialize a graph to return
  Graph ret(false, true);

  // set all vertices as unexplored
  for (Vertex v : g.getVertices()) {
    BFS_vertex_map[v] = UNEXPLORED;
    ret.insertVertex(v);
  }

  // set all edges as unexplored
  for (Edge e : g.getEdges()) {
    BFS_edge_map[edgeToString(e)] = UNEXPLORED;
  }

  // run BFS on each connected component of the graph
  for (Vertex v : g.getVertices()) {
    if (BFS_vertex_map[v] == UNEXPLORED) {
      BFS_helper(v);
    }
  }

  // add only the discovery edges to the return graph
  for (auto i : BFS_edge_map) {
    if (i.second == DISCOVERY) {
      Edge e = stringToEdge(i.first);
      ret.insertEdge(e.source, e.dest);
    }
  }

  return ret;
}

void Algorithms::BFS_helper(Vertex v) {
  queue<Vertex> q;
  BFS_vertex_map[v] = EXPLORED;
  q.push(v);

  while (!q.empty()) {
    Vertex front = q.front();
    q.pop();

    for (Vertex w : g.getAdjacent(front)) {
      // Check that the edge exists first
      if (g.edgeExists(front, w)) {
        if (BFS_vertex_map[w] == UNEXPLORED) {
          BFS_edge_map[edgeToString(g.getEdge(front, w))] = DISCOVERY;
          BFS_vertex_map[w] = EXPLORED;
          q.push(w);
        } else if (BFS_edge_map[edgeToString(g.getEdge(front, w))] == UNEXPLORED) {
          BFS_edge_map[edgeToString(g.getEdge(front, w))] = CROSS;
        }
      }
    }
  }
}

string Algorithms::edgeToString(Edge e) {
  return e.source + "$" + e.dest;
}

Edge Algorithms::stringToEdge(string s) {
  size_t split = s.find('$');
  Vertex source = s.substr(0, split);
  Vertex dest = s.substr(split+1);
  return Edge(source, dest);
}

void Algorithms::fillOrder(const Vertex& v, std::unordered_map<Vertex, bool>& visited, std::stack<Vertex>& st) {
  visited[v] = true;
  for (const Vertex& adj : g.getAdjacent(v)) {
    if (!visited[adj]) {
      fillOrder(adj, visited, st);
    }
  }
  st.push(v);
}

void Algorithms::SCC_DFS(Graph& transp, const Vertex& v, std::unordered_map<Vertex, bool>& visited, std::vector<Vertex>& scc, const std::string& i, std::unordered_map<std::string, std::string>& who) {
  visited[v] = true;
  scc.push_back(v);
  who[v] = i;
  for (const Vertex& adj : transp.getAdjacent(v)) {
    if (!visited[adj]) {
      SCC_DFS(transp, adj, visited, scc, i, who);
    }
  }
}

std::vector<std::string> Algorithms::GetEdgesToAdd() {
  std::stack<Vertex> st;
  std::unordered_map<Vertex, bool> visited;
  for (const Vertex& v : g.getVertices()) {
    visited[v] = false;
  }
  for (const Vertex& v : g.getVertices()) {
    if (!visited[v]) {
      fillOrder(v, visited, st);
    }
  }
  Graph transp = g.getTranspose();
  for (const Vertex& v : g.getVertices()) {
    visited[v] = false;
  }

  int i = 0;
  std::unordered_map<std::string, std::vector<Vertex>> scc;
  std::unordered_map<std::string, int> keymp;
  std::unordered_map<std::string, std::string> who;
  while (!st.empty()) {
    Vertex v = st.top();
    st.pop();
    if (!visited[v]) {
      scc[std::to_string(i)] = std::vector<Vertex>();
      keymp[std::to_string(i)] = i;
      SCC_DFS(transp, v, visited, scc[std::to_string(i)], to_string(i), who);
      ++i;
    }
  }

  DisjointSets disj;
  disj.addelements(scc.size());

  std::unordered_map<Vertex, int> indeg;
  std::unordered_map<Vertex, int> outdeg;

  for (auto it = scc.begin(); it != scc.end(); ++it) {
    indeg[it->first] = 0;
    outdeg[it->first] = 0;
  }

  for (const Vertex& v : g.getVertices()) {
    visited[v] = false;
  }

  for (const Edge& e : g.getEdges()) {
    if (who[e.source] != who[e.dest]) {
      disj.setunion(keymp[who[e.source]], keymp[who[e.dest]]);
      ++indeg[who[e.dest]];
      ++outdeg[who[e.source]];
    }
  }

  list<Vertex> indegZ, outdegZ;

  for (auto u = scc.begin(); u != scc.end(); ++u) {
    if (indeg[u->first] == 0) {
      indegZ.push_back(u->first);
    } 
    if (outdeg[u->first] == 0) {
      outdegZ.push_back(u->first);
    }
  }

  std::vector<std::string> ans;

  if (indegZ.size() > outdegZ.size()) {
    for (auto u = outdegZ.begin(); u != outdegZ.end(); ++u) {
      bool added_edge = false;
      for (auto v = indegZ.begin(); v != indegZ.end(); ++v) {
        if (disj.find(keymp[*u]) != disj.find(keymp[*v])) {
          ans.push_back(scc[*u].front() + "->" + scc[*v].front());
          disj.setunion(keymp[*u], keymp[*v]);
          indegZ.erase(v);
          added_edge = true;
          break;
        }
      }
      if (!added_edge) {
        for (auto v = indegZ.begin(); v != indegZ.end(); ++v) {
          if (*u != *v) {
            ans.push_back(scc[*u].front() + "->" + scc[*v].front());
            disj.setunion(keymp[*u], keymp[*v]);
            indegZ.erase(v);
            break;
          }
        }
      } 
    }
    for (auto v = indegZ.begin(); v != indegZ.end(); ++v) {
      for (auto u = scc.begin(); u != scc.end(); ++u) {
        if (u->first != *v) {
          ans.push_back(u->second.front() + "->" + scc[*v].front());
          disj.setunion(keymp[u->first], keymp[*v]);
          break;
        }
      }
    }
  } else {
    for (auto v = indegZ.begin(); v != indegZ.end(); ++v) {
      bool added_edge = false;
      for (auto u = outdegZ.begin(); u != outdegZ.end(); ++u) {
        if (disj.find(keymp[*u]) != disj.find(keymp[*v])) {
          ans.push_back(scc[*u].front() + "->" + scc[*v].front());
          disj.setunion(keymp[*u], keymp[*v]);
          outdegZ.erase(u);
          added_edge = true;
          break;
        }
      }
      if (!added_edge) {
        for (auto u = outdegZ.begin(); u != outdegZ.end(); ++u) {
          if (*u != *v) {
            ans.push_back(scc[*u].front() + "->" + scc[*v].front());
            disj.setunion(keymp[*u], keymp[*v]);
            outdegZ.erase(u);
            break;
          }
        }
      } 
    }
    for (auto u = outdegZ.begin(); u != outdegZ.end(); ++u) {
      for (auto v = scc.begin(); v != scc.end(); ++v) {
        if (*u != v->first) {
          ans.push_back(scc[*u].front() + "->" + v->second.front());
          disj.setunion(keymp[*u], keymp[v->first]);
          break;
        }
      }
    }
  }

  for (const std::string& e : ans) {
    size_t pos = e.find('-');
    g.insertEdge(e.substr(0, pos), e.substr(pos + 2));
  }

  std::stack<Vertex> st1;

  for (const Vertex& v : g.getVertices()) {
    visited[v] = false;
  }
  for (const Vertex& v : g.getVertices()) {
    if (!visited[v]) {
      fillOrder(v, visited, st1);
    }
  }
  transp = g.getTranspose();
  for (const Vertex& v : g.getVertices()) {
    visited[v] = false;
  }

  i = 0;
  while (!st1.empty()) {
    Vertex v = st1.top();
    st1.pop();
    if (!visited[v]) {
      SCC_DFS(transp, v, visited, scc[std::to_string(i)], to_string(i), who);
      ++i;
    }
  }
  std::cout << i << " SCCs in graph" << std::endl;

  return ans;
}

vector<Edge> Algorithms::getDijkstraPath(Vertex start, Vertex end) {
  DijkstraHeap dHeap;

  // Sets d[v] = +inf and p[v] = NULL for all vertices in graph
  for (Vertex v : g.getVertices()) {
    std::cout << v << "\n";
    DijkstraVertex dv(v, INT32_MAX, "");
    // Set d[v] = 0 for source
    if (v == start) {
      dv.dist = 0;
    }
    // Add all vertices to DijkstraHeap with lower values of d[v] having higher priority
    dHeap.push(dv);
  }

  Graph minWeightGraph(true, true);
  vector<Edge> dijkstraPath;

  for (unsigned long i = 0; i < g.getVertices().size(); i++) {
    DijkstraVertex du = dHeap.pop();
    Vertex u = du.label;
    std::cout << "POPPED: " << u << "\n";
    minWeightGraph.insertVertex(u);

    std::cout << u << " vs " << end << ": " << (u == end) << "\n";
    // Stop when ending vertex is reached
    if (u == end) {
      Vertex curr = end;
      Vertex prev = dHeap.dVertices[curr].prev;
      // Backtrace through vertices until starting vertex is reached
      while (prev != "") {
        std::cout << "CURRENT: " << curr << " PREV: " << prev << "\n";
        dijkstraPath.push_back(g.getEdge(prev, curr));
        curr = prev;
        prev = dHeap.dVertices[curr].prev;
      }
      // Reverse path vector to undo backtrace
      std::reverse(dijkstraPath.begin(), dijkstraPath.end());

      break;
    }

    for (Vertex v : g.getAdjacent(u)) {
      if (!minWeightGraph.vertexExists(v) && g.getEdgeWeight(u, v) + du.dist < dHeap.dVertices[v].dist) {
        // Update distances and previous vertices
        dHeap.updateDist(v, g.getEdgeWeight(u, v) + du.dist);
        dHeap.dVertices[v].prev = u;
      }
    }
  }

  return dijkstraPath;
}


vector<vector<Vertex>> Algorithms::graphColoring() {
  g2(false, false);

  vector<Vertex> vert = g.getVertices();
  for(size_t i =0; i< vert.size(); i++) {
    bool is_disconnected = true;
    for(size_t k =0; k< vert.size(); k++) {
      if (k != i && (g.edgeExists(vert[i], vert[k]) || g.edgeExists(vert[k], vert[i]))){
        g2.insertEdge(vert[i], vert[k]);
        is_disconnected = false;
      }
    }
    if (is_disconnected) {
      g2.insertVertex(vert[i]);
    }
  }

  vector<Vertex> vertices = g2.getVertices();
  int size = vertices.size();
  vector<int> vertex_colors(size, -1); 
  vertex_colors[0]  = 0; 

  std::vector<int> done;
  int num_of_colors = 0;

  for (int i = 1; i < size; i++) {
    vector<bool> isTaken(size, false); 
    vector<Vertex> adj = g2.getAdjacent(vertices[i]);
    if (adj.size() == 0) {
      vertex_colors[i]  = 0;
      continue;
    }

    for (size_t k = 0; k < adj.size(); ++k) {
      auto it = std::find(vertices.begin(), vertices.end(), adj[k]); 
            if (vertex_colors[it - vertices.begin()] != -1) {
                isTaken[vertex_colors[it - vertices.begin()]] = true;
            }
    }
    
    int color_num;
    for (color_num = 0; color_num < size; color_num++) {
      if (isTaken[color_num] == false) {
        if (std::find(done.begin(), done.end(), color_num) == done.end()) {
          num_of_colors++;
        }
        done.push_back(color_num);
        break;
      }
    } 
    vertex_colors[i] = color_num;

  } 


  vector<vector<Vertex>> ans(num_of_colors);
  for(int x = 0; x < size; x++) {
    ans[vertex_colors[x]].push_back(vertices[x]);
  }
  for (int u = 0; u < num_of_colors; u++) {
    cout << " color " << u;

    for(size_t x = 0; x < ans[u].size(); x++) {
    // vector<string> vert = result[u];
      cout << " Vertex: " << ans[u][x];
    }
    cout << endl;
  }


   for (int u = 0; u < num_of_colors; u++) {
    cout << " color " << u;

    for(size_t x = 0; x < ans[u].size(); x++) {
      vector<Vertex> adj = g2.getAdjacent(ans[u][x]);
      for(size_t x = 0; x < adj.size(); x++) {
        if(std::find(ans[u].begin(), ans[u].end(), adj[x]) != ans[u].end()) {
          cout << "false AHHHH";
        }
      }
    }
    cout << endl;
  }


  for(int h = 0; h < size; h++) {
    vector<Vertex> ad = g2.getAdjacent(vertices[h]);
    if (ad.size() == 0) {
      cout <<" HELLOBYEE: " << vertices[h] << ": " << endl;
    }
    for(size_t k = 0; k < ad.size(); k++) {
      cout << vertices[h] << ": " << ad[k] << endl;
    }
  }
  return ans;
} 

Graph Algorithms::getGraphDirected() {
  return g;
  }