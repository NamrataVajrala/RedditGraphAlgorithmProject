#pragma once

#include <string>

#include "graph.h"
#include "edge.h"

class ExtractData {
  public:
    Graph extract(std::string filename);
};