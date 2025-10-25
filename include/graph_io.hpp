#pragma once
#ifndef GRAPH_IO_HPP
#define GRAPH_IO_HPP

#include <string>
#include <vector>
#include <utility>
#include "graph.hpp"

std::vector<std::pair<size_t, size_t>> read_edge_list(const std::string& filename, bool undirected);

CSRGraph build_csr_from_file(const std::string& filename, bool undirected);

#endif // GRAPH_IO_HPP