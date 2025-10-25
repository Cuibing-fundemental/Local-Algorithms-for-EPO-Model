#include "alg.hpp"
#define EIGEN_USE_OPENMP
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <iostream>
using Matrix = Eigen::MatrixXd;

Eigen::SparseMatrix<double> buildSparseFinalMatrix(const CSRGraph& graph) {
    
    const size_t n = graph.num_vertices();
    Eigen::SparseMatrix<double> sparse_mat(n, n);
    
    std::vector<Eigen::Triplet<double>> triplets;
    
    // 预估非零元素数量
    size_t estimated_nnz = n + 2 * graph.num_edges();
    triplets.reserve(estimated_nnz);
    
    // 构建对角线元素
    for (size_t i = 0; i < n; ++i) {
        double deg = static_cast<double>(graph.out_degree(i));
        triplets.emplace_back(i, i, 1.0 + 2.0 * deg + deg * deg);
    }
    
    // 构建非对角线元素（直接遍历图的边）
    for (size_t i = 0; i < n; ++i) {
        double deg_i = static_cast<double>(graph.out_degree(i));
        auto neighbors = graph.get_neighbors(i);
        
        for (auto j : neighbors) {
            if (i != j) {
                triplets.emplace_back(i, j, -(2.0 + deg_i));
            }
        }
    }
    
    sparse_mat.setFromTriplets(triplets.begin(), triplets.end());
    sparse_mat.makeCompressed();
    return sparse_mat;
}

Opinion ExactSol(const CSRGraph& graph, Opinion &start,double epsilon, bool option) {
    //option = true for BICGSTAB; = false for LU
    size_t n = graph.num_vertices();
    Opinion result(n, 0.0);
    
    auto final_matrix = buildSparseFinalMatrix(graph);
    Eigen::VectorXd b = Eigen::VectorXd::Zero(n);
    for (size_t i = 0; i < n; ++i) {
        b(i) = start[i];
    }
    
    Eigen::VectorXd solution;
    std::cout << "Begin solving the linear system...\n";
    if(option){
        Eigen::BiCGSTAB<Eigen::SparseMatrix<double>> solver;
        solver.setMaxIterations(4*n);
        solver.compute(final_matrix);
        solution = solver.solve(b);
    }   else{
        Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;
        solver.compute(final_matrix);
        solution = solver.solve(b);
    }
    
    // 将结果转换为 Opinion 类型
    for (size_t i = 0; i < n; ++i) {
        result[i] = solution(i);
    }
    return result;
}