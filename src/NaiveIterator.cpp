#include "alg.hpp"
#include <algorithm>
#include <iostream>

std::pair<Opinion,Opinion> NaiveIterator(const CSRGraph& graph, Opinion &s, double epsilon)
{
    const size_t n = graph.num_vertices();
    Opinion outer(n, 0);
    Opinion inner(n, 0);
    Opinion outer_new(n, 0);
    std::vector<bool> active(n, false);
    size_t iter = 0;
    size_t max_degree = 0;
    for(size_t v = 0; v<n; v++){
        active[v] = (graph.out_degree(v)!=0);
        max_degree = std::max(max_degree, graph.out_degree(v));
    }
    printf("Max degree: %zu\n", max_degree);

    while( true ){
        iter ++;
        for(size_t v = 0; v<n; v++){
            inner[v] = s[v];
            auto neighbours = graph.neighbors(v);
            for(auto it = neighbours.first; it != neighbours.second; ++it){
                auto u = *it;
                inner[v] += outer[u];
            }
            inner[v] /= (graph.out_degree(v) + 1.0);
        }

        for(size_t v = 0; v<n; v++){
            outer_new[v] = inner[v];
            auto neighbours = graph.neighbors(v);
            for(auto it = neighbours.first; it != neighbours.second; ++it){
                auto u = *it;
                outer_new[v] += outer[u];
            }
            outer_new[v] /= (graph.out_degree(v) + 1.0);
        }

        bool flag = true;
        double del;
        for(size_t v = 0; v<n; v++){
            del = (outer_new[v] - outer[v]) / s[v];
            del = del>0 ? del : -del;
            if(del > epsilon){
                flag = false;
            }
        }

        if(flag){
            std::cout << "Iter: " << iter << "\n";
            break;
        }
        
        outer = outer_new;
    }
    
    return {inner,outer_new};
}

std::pair<Opinion,Opinion> NaiveIterator_new(const CSRGraph& graph, Opinion &s, double epsilon)
{
    const size_t n = graph.num_vertices();
    Opinion outer(n, 0);
    Opinion inner(n, 0);
    Opinion outer_new(n, 0);
    std::vector<bool> active(n, false);
    size_t iter = 0;

    while( true ){
        iter ++;
        for(size_t v = 0; v<n; v++){
            inner[v] = s[v];
        }
        for(size_t v = 0; v<n; v++){
            auto neighbours = graph.neighbors(v);
            for(auto it = neighbours.first; it != neighbours.second; ++it){
                auto u = *it;
                inner[u] += outer[v];
            }
        }
        for(size_t v = 0; v<n; v++){
            inner[v] /= (graph.out_degree(v) + 1.0);
        }

        for(size_t v = 0; v<n; v++){
            outer_new[v] = inner[v];
        }
        for(size_t v = 0; v<n; v++){
            auto neighbours = graph.neighbors(v);
            for(auto it = neighbours.first; it != neighbours.second; ++it){
                auto u = *it;
                outer_new[u] += outer[v];
            }
        }

        for(size_t v = 0; v<n; v++){
            outer_new[v] /= (graph.out_degree(v) + 1.0);
        }

        bool flag = true;
        double del;
        for(size_t v = 0; v<n; v++){
            del = (outer_new[v] - outer[v]) / s[v];
            del = del>0 ? del : -del;
            if(del > epsilon*graph.out_degree(v)){
                flag = false;
            }
        }

        if(flag){
            std::cout << "Iter: " << iter << "\n";
            break;
        }
        
        outer = outer_new;
    }
    
    return {inner,outer_new};
}