#include "DijkstraVertex.h"

DijkstraVertex::DijkstraVertex(): label(""), dist(INT32_MAX), prev("") {}

DijkstraVertex::DijkstraVertex(string vertexLabel, int distance, Vertex previous): label(vertexLabel), dist(distance), prev(previous) {}