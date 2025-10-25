#include "graph.hpp"
#include <iostream>

// 获取顶点的邻居范围
std::pair<const size_t*, const size_t*> CSRGraph::neighbors(size_t v) const {
    if (v >= num_vertices_) {
        throw std::out_of_range("Vertex index out of range");
    }
    return {&col_idx_[row_ptr_[v]], &col_idx_[row_ptr_[v+1]]};
}

// 检查边是否存在
bool CSRGraph::has_edge(size_t u, size_t v) const {
    if (u >= num_vertices_) return false;
    auto [begin, end] = neighbors(u);
    return std::binary_search(begin, end, v);
}

// 获取顶点出度
size_t CSRGraph::out_degree(size_t v) const {
    if (v >= num_vertices_) {
        throw std::out_of_range("Vertex index out of range");
    }
    return degree_[v];
}
size_t CSRGraph::get_index(size_t v) const {
    if (v >= num_edges_) {
        throw std::out_of_range("Out of range");
    }
    return col_idx_[v];
}

std::vector<size_t> CSRGraph::get_neighbors(size_t v) const {
    if (v >= num_vertices_) {
        throw std::out_of_range("Vertex index out of range");
    }
    auto [begin, end] = neighbors(v);
    std::vector<size_t> result;
    for (auto it = begin; it != end; ++it) {
        result.push_back(*it);
    }
    return result;
}
// 打印图结构
void CSRGraph::print() const {
    std::cout << "CSR Graph (" << num_vertices_ << " vertices, " 
              << num_edges_ << " edges)\n";
    for (size_t i = 0; i < num_vertices_; ++i) {
        std::cout << "Vertex " << i << ": ";
        auto [begin, end] = neighbors(i);
        for (auto it = begin; it != end; ++it) {
            std::cout << *it << " ";
        }
        std::cout << "\n";
    }
}

std::vector<bool> CSRGraph::get_adjacency_matrix() const {
    std::vector<bool> matrix(num_vertices_ * num_vertices_, false);
    
    for (size_t u = 0; u < num_vertices_; ++u) {
        size_t start = row_ptr_[u];
        size_t end = row_ptr_[u + 1];
        
        for (size_t i = start; i < end; ++i) {
            size_t v = col_idx_[i];
            matrix[u * num_vertices_ + v] = true;
        }
    }
    
    return matrix;
}

std::vector<int> CSRGraph::get_degree_vector() const {
    std::vector<int> vec(num_vertices_ , 0);
    
    std::transform(row_ptr_.begin() + 1, row_ptr_.end(), row_ptr_.begin(),
                   vec.begin(), [](size_t end, size_t start) { return end - start; });
    
    return vec;
}

std::vector<int> CSRGraph::get_laplacian_matrix() const {
    std::vector<int> matrix(num_vertices_ * num_vertices_, 0);
    
    for (size_t u = 0; u < num_vertices_; ++u) {
        size_t degree = out_degree(u);
        matrix[u * num_vertices_ + u] = degree; // 对角线元素为度数
        
        size_t start = row_ptr_[u];
        size_t end = row_ptr_[u + 1];
        
        for (size_t i = start; i < end; ++i) {
            size_t v = col_idx_[i];
            matrix[u * num_vertices_ + v] = -1; // 非对角线元素为-1
        }
    }
    
    return matrix;
}
