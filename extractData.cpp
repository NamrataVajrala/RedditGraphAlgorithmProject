#include <fstream>
#include <sstream>
#include <vector>

#include "extractData.h"

using std::ifstream;
using std::string;
using std::getline;
using std::vector;
using std::istringstream;
using std::cout;
using std::endl;

// https://techoverflow.net/2020/01/30/how-to-read-tsv-tab-separated-values-in-c/
// http://snap.stanford.edu/data/soc-RedditHyperlinks.html
Graph ExtractData::extract(string filename) {
  Graph g(true, true);
  ifstream fin(filename);
  string line;
  getline(fin, line);
  while (getline(fin, line)) {
      // Split line into tab-separated parts
      vector<string> parts;
      istringstream iss(line);
      string token;
      while (getline(iss, token, '\t')) {
        parts.push_back(token);
      }
      istringstream iss_prp(parts[5]);
      string properties;
      int i = 0;
      while (getline(iss_prp, properties, ',')) {
        if (i == 1) {
          break;
        }
        ++i;
      }
      parts[5] = properties;
      if (!g.vertexExists(parts[0])) {
        g.insertVertex(parts[0]);
      }
      if (!g.vertexExists(parts[1])) {
        g.insertVertex(parts[1]);
      }
      if (!g.edgeExists(parts[0], parts[1])) {
        g.insertEdge(parts[0], parts[1]);
        g.setEdgeWeight(parts[0], parts[1], 0);
      }
      g.setEdgeWeight(parts[0], parts[1], g.getEdgeWeight(parts[0], parts[1]) + 1);
  }
  fin.close();
  return g;
}
