#pragma once
#ifndef CSR_GRAPH_HPP
#define CSR_GRAPH_HPP

#include <vector>
#include <algorithm>
#include <memory>
#include <utility>
#include <stdexcept>

class CSRGraph {
private:
    size_t num_vertices_;
    size_t true_num_vertices_;
    size_t num_edges_;
    std::vector<size_t> row_ptr_;
    std::vector<size_t> col_idx_;
    size_t max_degree_;
    std::vector<size_t> degree_;
    
public:
    CSRGraph() = delete;
    CSRGraph(const CSRGraph&) = delete;
    CSRGraph& operator=(const CSRGraph&) = delete;

    CSRGraph(CSRGraph&&) = default;
    CSRGraph& operator=(CSRGraph&&) = default;
    
    // 构造
    template <typename EdgeIter>
    CSRGraph(size_t num_vertices, EdgeIter edges_begin, EdgeIter edges_end);
    
    size_t num_vertices() const { return num_vertices_; };
    size_t num_edges() const { return num_edges_; };
    size_t true_num_vertices() const { return true_num_vertices_; };
    size_t max_degree() const {return max_degree_;};
    size_t get_index(size_t v)  const;
    
    // 获取顶点的邻居范围 [begin, end)s
    std::pair<const size_t*, const size_t*> neighbors(size_t v) const;
    std::vector<size_t> get_neighbors(size_t v) const;
    bool has_edge(size_t u, size_t v) const;
    size_t out_degree(size_t v) const;
    
    // 获取各种相关矩阵(使用稠密压缩格式)
    std::vector<bool> get_adjacency_matrix() const;
    std::vector<int> get_laplacian_matrix() const;
    std::vector<int> get_degree_vector() const;
    // 打印图结构 (调试用)
    void print() const;
};

#include "graph.ipp"
#endif // CSR_GRAPH_HPP