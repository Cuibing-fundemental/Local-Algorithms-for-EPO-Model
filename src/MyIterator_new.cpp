#include "alg.hpp"
#include <MyQueue.hpp>
#include <queue>
#include <cmath>

struct Tempcache
{
    double z;
    double z_old;
    double r;
    double s_max;
    double upper_degree;
    double lower_degree;
    bool mask;
};

Opinion MyIterator_NEW_SOR(const CSRGraph& graph,Opinion& start, double epsilon, double omega){
    //printf("Start MyIter\n");
    const size_t n = graph.num_vertices();
    Tempcache* cache = new Tempcache[n];
    std::queue<size_t> Q;
    for(size_t i=0; i < n; ++i) {
        cache[i].r = start[i];
        cache[i].z = 0;
        cache[i].z_old = 0;
        cache[i].mask = false;
        auto degree = graph.out_degree(i);
        cache[i].upper_degree = (degree+2.0);
        cache[i].lower_degree = (1.0/(degree+1.0)/(degree+1.0));
        cache[i].s_max = epsilon*cache[i].r;

        if (degree > 0) {
            Q.push(i);
            cache[i].mask = true;
        }
    }
    //printf("Q size: %zu\n", Q.size());

    size_t iter = 0;
    while( !Q.empty() ){
        ++iter;
        size_t v = Q.front();
        Q.pop();
        double con = omega * cache[v].r*cache[v].lower_degree;
        cache[v].z += con;

        auto neighbours = graph.neighbors(v);
        for(auto it = neighbours.first; it != neighbours.second; ++it) {
            auto u = *it;
            cache[u].r += con * cache[u].upper_degree;
            if (cache[u].mask == false && fabs(cache[u].r) > cache[u].s_max) {
                Q.push(u);
                cache[u].mask = true;
            }
        }

        cache[v].r *= (1-omega);
        if (fabs(cache[v].r) > cache[v].s_max) {
            Q.push(v);
        }
        else{
            cache[v].mask = false;
        }
    }
    printf("Iter: %lu\n",iter);
    Opinion z(n,0);
    for(size_t i=0; i<n; ++i){
        z[i] = cache[i].z;
    }
    delete[] cache;
    return z;
}

Opinion MyIterator_NEW(const CSRGraph& graph,Opinion& start, double epsilon){
    //printf("Start MyIter\n");
    const size_t n = graph.num_vertices();
    Tempcache* cache = new Tempcache[n];
    std::queue<size_t> Q;
    for(size_t i=0; i < n; ++i) {
        cache[i].s_max = (start[i]*epsilon);
        cache[i].r = start[i];
        cache[i].z = 0;
        cache[i].mask = false;
        auto degree = graph.out_degree(i);
        cache[i].upper_degree = (degree+2.0);
        cache[i].lower_degree = (1.0/(degree+1.0)/(degree+1.0));
        if (degree > 0) {
            Q.push(i);
            cache[i].mask = true;
        }
    }
    //printf("Q size: %zu\n", Q.size());
    
    size_t iter = 0;
    while( !Q.empty() ){
        ++iter;
        size_t v = Q.front();
        Q.pop();
        cache[v].mask = false;
        double con = cache[v].r*cache[v].lower_degree;
        cache[v].z += con;

        auto neighbours = graph.neighbors(v);
        for(auto it = neighbours.first; it != neighbours.second; ++it) {
            auto u = *it;
            cache[u].r += con * cache[u].upper_degree;
            if (cache[u].mask == false && cache[u].r > cache[u].s_max) {
                Q.push(u);
                cache[u].mask = true;
            }
        }

        cache[v].r = 0;
    }
    printf("Iter: %lu\n",iter);
    Opinion z(n,0);
    for(size_t i=0; i<n; ++i){
        z[i] = cache[i].z;
    }
    delete[] cache;
    return z;
}