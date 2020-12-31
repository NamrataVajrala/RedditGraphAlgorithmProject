#include <iostream>
#include <string>

#include "algorithms.h"
#include "extractData.h"
#include "graph.h"
#include "algorithms.h"

// https://techoverflow.net/2020/01/30/how-to-read-tsv-tab-separated-values-in-c/
// http://snap.stanford.edu/data/soc-RedditHyperlinks.html
int main(int argc, char** argv) {
  // ExtractData ed;
  // Graph g = ed.extract("test_scc.tsv");
  // std::cout << g.getEdges().size() << std::endl;
  // Algorithms algo(g);
  // std::vector<std::string> ans = algo.GetEdgesToAdd();
  // for (const std::string& e : ans) {
  //   std::cout << e << std::endl;
  // }

  // DijkstraHeap dh;
  // std::cout << dh.empty() << std::endl;

  // Algorithms a(g);
  // vector<Edge> path = a.getDijkstraPath("dankmemes", );
  // Graph g(false);
	// int numVertices = 3;
	// for (int i = 0; i < numVertices; i++) {
	// 	g.insertVertex(std::to_string(i) + "");
	// }
	// g.insertEdge("0", "1");
	// g.insertEdge("0", "2");
	// g.insertEdge("1", "2");

	// Algorithms alg(g);
	// Graph bfs = alg.BFS();

  // for (Edge e : bfs.getEdges()) {
	// 	std::cout << e.source << "->" << e.dest << std::endl;
	// }
  // std::cout << "Num V " << bfs.getVertices().size() << std::endl;

  // ExtractData ed;
  // Graph g = ed.extract("test_dijkstra.tsv");
  // // std::cout << g.getEdges().size() << std::endl;
  // Algorithms algo(g);

  // vector<Edge> path = algo.getDijkstraPath("a", "e");

  // std::cout << path.size() << endl;
  // for (Edge e : path) {
  //   std::cout << e.source << " -> " << e.dest << " (" << g.getEdgeWeight(e.source, e.dest) << ")\n";
  // }

  ExtractData ed;
    Graph g = ed.extract("test_extract.tsv");
 Algorithms x(g);
   x.graphColoring();
}
