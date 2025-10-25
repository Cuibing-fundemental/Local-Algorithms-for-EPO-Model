#pragma once
#include "graph.hpp"

using Opinion = std::vector<double>;

Opinion CH_vol(const CSRGraph& graph, Opinion &start, double epsilon);
Opinion HB_vol(const CSRGraph& graph, Opinion &start, double epsilon);
Opinion GSSOR_vol(const CSRGraph& graph, Opinion &start, double epsilon, double omega);

Opinion CH_CVG(const CSRGraph& graph, Opinion &start, double epsilon);
Opinion HB_CVG(const CSRGraph& graph, Opinion &start, double epsilon);
Opinion GD_CVG(const CSRGraph& graph, Opinion &start, double epsilon);
Opinion GSSOR_CVG(const CSRGraph& graph, Opinion &start, double epsilon, double omega);