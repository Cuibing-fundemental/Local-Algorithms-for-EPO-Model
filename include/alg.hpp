#pragma once
#include "graph.hpp"

using Opinion = std::vector<double>;

Opinion MyIterator(const CSRGraph& graph, Opinion &start, double epsilon);
Opinion MyIterator_NEW(const CSRGraph& graph, Opinion &start, double epsilon);
Opinion ExactSol(const CSRGraph& graph, Opinion &start,double epsilon, bool option = false);
Opinion MyIterator_NEW_SOR(const CSRGraph& graph, Opinion &start, double epsilon, double omega);
Opinion RichardsonIterator(const CSRGraph& graph, Opinion &start, double epsilon);
Opinion ChebyshevIterator(const CSRGraph& graph, Opinion &start, double epsilon);
Opinion HeavyBallIterator(const CSRGraph& graph, Opinion &start, double epsilon);

std::pair<Opinion,Opinion> NaiveIterator(const CSRGraph& graph, Opinion &s, double epsilon);
std::pair<Opinion,Opinion> NaiveIterator_new(const CSRGraph& graph, Opinion &s, double epsilon);