#include "graph_io.hpp"
#include "algtest.hpp"
#include <chrono>
#include <thread>
#include <iostream>
#include <random>

#define PRINTLIMIT 3

void test_HeavyBallIterator(CSRGraph& graph, Opinion &s, double epsilon)
{
    std::cout << "HeavyBallIterator:\n";
    auto start = std::chrono::high_resolution_clock::now();
    auto z = HB_vol(graph,s,epsilon);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "耗时: " << duration.count() << " 毫秒" << std::endl;
    std::cout << "\n";
}

void test_ChebyshevIterator(CSRGraph& graph, Opinion &s, double epsilon)
{
    std::cout << "ChebyshevIterator:\n";
    auto start = std::chrono::high_resolution_clock::now();
    auto z = CH_vol(graph,s,epsilon);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "耗时: " << duration.count() << " 毫秒" << std::endl;
    std::cout << "\n";
}

void test_MyIterator_NEW_SOR(CSRGraph& graph, Opinion &s, double epsilon, double omega)
{
    auto start = std::chrono::high_resolution_clock::now();
    auto z = GSSOR_vol(graph,s,epsilon,omega);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <edge_list_file>\n";
        return 1;
    }

    try {
        // 从文件构建CSR图
        CSRGraph graph = build_csr_from_file(argv[1],true);
                
        Opinion s(graph.num_vertices(),1);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.5, 1.0);
        
        for(size_t i=0; i<graph.num_vertices(); ++i){
            s[i] = dis(gen);
        }
        double epsilon = 1e-6;

        GSSOR_vol(graph,s,epsilon,1.7);
        //HB_vol(graph,s,epsilon);
        //CH_vol(graph,s,epsilon);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}