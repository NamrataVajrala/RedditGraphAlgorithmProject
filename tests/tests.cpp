#include <string>
#include <vector>

#include "../catch/catch.hpp"
#include "../extractData.h"
#include "../algorithms.h"


TEST_CASE("Verify number of vertices") {
	ExtractData ed;
  Graph g = ed.extract("test_extract.tsv");
	REQUIRE(g.getVertices().size() == 18);
}	

TEST_CASE("Verify number of edges") {
	ExtractData ed;
  Graph g = ed.extract("test_extract.tsv");
	REQUIRE(g.getEdges().size() == 9);
}	

TEST_CASE("Basic SCC") {
	ExtractData ed;
  Graph g = ed.extract("test_scc.tsv");
	Algorithms algo(g);
	REQUIRE(algo.GetEdgesToAdd().size() == 9);
}	

TEST_CASE("Three Vertices") {
	ExtractData ed;
  Graph g = ed.extract("test_scc_three.tsv");
	Algorithms algo(g);
	vector<string> ans = algo.GetEdgesToAdd();
	REQUIRE(ans.size() == 1);
	REQUIRE(ans.front() == "theredlion->leagueoflegends");
}	

TEST_CASE("Graph of Two SCC") {
	ExtractData ed;
  Graph g = ed.extract("test_scc_two_sccs.tsv");
	Algorithms algo(g);
	REQUIRE(algo.GetEdgesToAdd().size() == 2);
}	

TEST_CASE("SCC One Vertex") {
	ExtractData ed;
  Graph g = ed.extract("test_scc_one_vertex.tsv");
	Algorithms algo(g);
	vector<string> ans = algo.GetEdgesToAdd();
	REQUIRE(ans.size() == 1);
	REQUIRE(ans.front() == "bestof2013->posthardcore");
}	

TEST_CASE("BFS - small") {
	Graph g(false);
	int numVertices = 3;
	for (int i = 0; i < numVertices; i++) {
		g.insertVertex(std::to_string(i) + "");
	}
	g.insertEdge("0", "1");
	g.insertEdge("0", "2");
	g.insertEdge("1", "2");

	Algorithms alg(g);
	Graph bfs = alg.BFS();

	REQUIRE(bfs.getEdges().size() == numVertices - 1);
}


TEST_CASE("BFS - large") {
	Graph g(false, false);
	int numVertices = 15;
	for (int i = 0; i < numVertices; i++) {
		g.insertVertex(std::to_string(i) + "");
	}
	while (g.getEdges().size() < 45) {
		int start = rand() % numVertices;
		int end = rand() % numVertices;

		if (start != end && !g.edgeExists(std::to_string(start), std::to_string(end))) {
			g.insertEdge(std::to_string(start), std::to_string(end));
		}
	}

	Algorithms alg(g);
	Graph bfs = alg.BFS();
	
	REQUIRE(bfs.getEdges().size() == numVertices - 1);
}