#include "graph_io.hpp"
#include "alg.hpp"
#include <chrono>
#include <thread>
#include <iostream>
#include <random>

#define PRINTLIMIT 3

void test_HeavyBallIterator(CSRGraph& graph, Opinion &s, double epsilon)
{
    std::cout << "HeavyBallIterator:\n";
    auto start = std::chrono::high_resolution_clock::now();
    auto z = HeavyBallIterator(graph,s,epsilon);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "耗时: " << duration.count() << " 毫秒" << std::endl;
    std::cout << "First " << PRINTLIMIT << " vertices and their output:\n";
    
    for (size_t v = 0; v < PRINTLIMIT; ++v) {
        std::cout << "Vertex " << v << " : ";
        std::cout << z[v];
        std::cout << "\n";
    }
    std::cout << "\n";
}

void test_ChebyshevIterator(CSRGraph& graph, Opinion &s, double epsilon)
{
    std::cout << "ChebyshevIterator:\n";
    auto start = std::chrono::high_resolution_clock::now();
    auto z = ChebyshevIterator(graph,s,epsilon);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "耗时: " << duration.count() << " 毫秒" << std::endl;
    std::cout << "First " << PRINTLIMIT << " vertices and their output:\n";
    
    for (size_t v = 0; v < PRINTLIMIT; ++v) {
        std::cout << "Vertex " << v << " : ";
        std::cout << z[v];
        std::cout << "\n";
    }
    std::cout << "\n";
}

void test_RichardsonIterator(CSRGraph& graph, Opinion &s, double epsilon)
{
    std::cout << "RichardsonIterator:\n";
    auto start = std::chrono::high_resolution_clock::now();
    auto z = RichardsonIterator(graph,s,epsilon);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "耗时: " << duration.count() << " 毫秒" << std::endl;
    std::cout << "First " << PRINTLIMIT << " vertices and their output:\n";
    
    for (size_t v = 0; v < PRINTLIMIT; ++v) {
        std::cout << "Vertex " << v << " : ";
        std::cout << z[v];
        std::cout << "\n";
    }
    std::cout << "\n";
}

void test_MyIterator(CSRGraph& graph, Opinion &s, double epsilon)
{
    std::cout << "MyIterator:\n";
    auto start = std::chrono::high_resolution_clock::now();
    auto z = MyIterator(graph,s,epsilon);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "耗时: " << duration.count() << " 毫秒" << std::endl;
    std::cout << "First " << PRINTLIMIT << " vertices and their output:\n";
    
    for (size_t v = 0; v < PRINTLIMIT; ++v) {
        std::cout << "Vertex " << v << " : ";
        std::cout << z[v];
        std::cout << "\n";
    }
    std::cout << "\n";
}

void test_MyIterator_NEW(CSRGraph& graph, Opinion &s, double epsilon)
{
    std::cout << "MyIterator_NEW:\n";
    auto start = std::chrono::high_resolution_clock::now();
    auto z = MyIterator_NEW(graph,s,epsilon);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "耗时: " << duration.count() << " 毫秒" << std::endl;
    std::cout << "First " << PRINTLIMIT << " vertices and their output:\n";
    
    for (size_t v = 0; v < PRINTLIMIT; ++v) {
        std::cout << "Vertex " << v << " : ";
        std::cout << z[v];
        std::cout << "\n";
    }
    std::cout << "\n";
}

void test_MyIterator_NEW_SOR(CSRGraph& graph, Opinion &s, double epsilon, double omega)
{
    std::cout << "MyIterator_NEW_SOR:\n";
    std::cout << "omega = "<< omega << "\n";
    auto start = std::chrono::high_resolution_clock::now();
    auto z = MyIterator_NEW_SOR(graph,s,epsilon,omega);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "耗时: " << duration.count() << " 毫秒" << std::endl;
    /*
    std::cout << "First " << PRINTLIMIT << " vertices and their output:\n";
    for (size_t v = 0; v < PRINTLIMIT; ++v) {
        std::cout << "Vertex " << v << " : ";
        std::cout << z[v];
        std::cout << "\n";
    }
    */
    std::cout << "\n";
}

void test_NaiveIterator(CSRGraph& graph, Opinion &s, double epsilon)
{
    std::cout << "NaiveIterator:\n";
    auto start_naive = std::chrono::high_resolution_clock::now();
    auto result = NaiveIterator(graph,s,epsilon);
    auto end_naive = std::chrono::high_resolution_clock::now();
    auto duration_naive = std::chrono::duration_cast<std::chrono::milliseconds>(end_naive - start_naive);
    std::cout << "耗时: " << duration_naive.count() << " 毫秒" << std::endl;
    
    std::cout << "First " << PRINTLIMIT << " vertices and their output:\n";
    
    auto outer = result.second;
    for (size_t v = 0; v < PRINTLIMIT; ++v) {
        std::cout << "Vertex " << v << " : ";
        std::cout << outer[v];
        std::cout << "\n";
    }
    std::cout << "\n";
}

void test_NaiveIterator_new(CSRGraph& graph, Opinion &s, double epsilon)
{
    std::cout << "NaiveIterator_new:\n";
    auto start_naive = std::chrono::high_resolution_clock::now();
    auto result = NaiveIterator_new(graph,s,epsilon);
    auto end_naive = std::chrono::high_resolution_clock::now();
    auto duration_naive = std::chrono::duration_cast<std::chrono::milliseconds>(end_naive - start_naive);
    std::cout << "耗时: " << duration_naive.count() << " 毫秒" << std::endl;
    
    std::cout << "First " << PRINTLIMIT << " vertices and their output:\n";
    
    auto outer = result.second;
    for (size_t v = 0; v < PRINTLIMIT; ++v) {
        std::cout << "Vertex " << v << " : ";
        std::cout << outer[v];
        std::cout << "\n";
    }
    std::cout << "\n";
}

void test_ExcatSol_BICGSTAB(CSRGraph& graph, Opinion &s,double epsilon)
{
    std::cout << "ExactSol:\n";
    auto start_naive = std::chrono::high_resolution_clock::now();
    auto outer = ExactSol(graph,s,epsilon,true);
    auto end_naive = std::chrono::high_resolution_clock::now();
    auto duration_naive = std::chrono::duration_cast<std::chrono::milliseconds>(end_naive - start_naive);
    std::cout << "耗时: " << duration_naive.count() << " 毫秒" << std::endl;
    
    std::cout << "First " << PRINTLIMIT << " vertices and their output:\n";
    
    for (size_t v = 0; v < PRINTLIMIT; ++v) {
        std::cout << "Vertex " << v << " : ";
        std::cout << outer[v];
        std::cout << "\n";
    }
    std::cout << "\n";
}

void test_ExcatSol_LU(CSRGraph& graph, Opinion &s, double epsilon)
{
    std::cout << "ExactSol:\n";
    auto start_naive = std::chrono::high_resolution_clock::now();
    auto outer = ExactSol(graph,s,epsilon,false);
    auto end_naive = std::chrono::high_resolution_clock::now();
    auto duration_naive = std::chrono::duration_cast<std::chrono::milliseconds>(end_naive - start_naive);
    std::cout << "耗时: " << duration_naive.count() << " 毫秒" << std::endl;
    
    std::cout << "First " << PRINTLIMIT << " vertices and their output:\n";
    
    for (size_t v = 0; v < PRINTLIMIT; ++v) {
        std::cout << "Vertex " << v << " : ";
        std::cout << outer[v];
        std::cout << "\n";
    }
    std::cout << "\n";
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <edge_list_file>\n";
        return 1;
    }

    try {
        // 从文件构建CSR图
        CSRGraph graph = build_csr_from_file(argv[1],true);
        
        // 输出图的基本信息
        std::cout << "Graph loaded successfully:\n";
        std::cout << "Number of vertices: " << graph.num_vertices() << "\n";
        std::cout << "Number of true vertices: " << graph.true_num_vertices() << "\n";
        std::cout << "Number of edges: " << graph.num_edges() << "\n\n";
        std::cout << "Max degree: " << graph.max_degree() << "\n\n";
        
        Opinion s(graph.num_vertices(),1);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.1, 1.0);
        
        for(size_t i=0; i<graph.num_vertices(); ++i){
            s[i] = dis(gen);
        }
        double epsilon = 1e-6;

        //test_RichardsonIterator(graph, s, epsilon);

        test_HeavyBallIterator(graph, s, epsilon);
        test_ChebyshevIterator(graph, s, epsilon);
        test_RichardsonIterator(graph, s, epsilon);
        test_MyIterator_NEW_SOR(graph, s, epsilon, 1.7);
        test_MyIterator_NEW(graph, s, epsilon);
        test_NaiveIterator(graph, s, epsilon);
        test_ExcatSol_BICGSTAB(graph, s, epsilon);
        //test_ExcatSol_LU(graph, s);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}