#include "algtest.hpp"
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

Opinion GSSOR_vol(const CSRGraph& graph,Opinion& start, double epsilon, double omega){
    //printf("Start MyIter\n");
    const size_t n = graph.num_vertices();
    std::vector<size_t> Vol;
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
        cache[i].s_max = epsilon*degree;

        if (degree > 0) {
            Q.push(i);
            cache[i].mask = true;
        }
    }
    //printf("Q size: %zu\n", Q.size());

    size_t iter = 0;
    while( !Q.empty() ){
        //if ( iter % n == 0)
            Vol.push_back(Q.size());
        ++iter;
        size_t v = Q.front();
        Q.pop();
        cache[v].mask = false;
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
        if (cache[v].mask == false && fabs(cache[v].r) > cache[v].s_max) {
                Q.push(v);
                cache[v].mask = true;
            }
    }
    Opinion z(n,0);
    for(size_t i=0; i<n; ++i){
        z[i] = cache[i].z;
    }
    delete[] cache;
    for(size_t i=0; i<Vol.size(); ++i){
        printf("%zu ", Vol[i]);
    }
    printf("0\n");
    return z;
}

Opinion GSSOR_CVG(const CSRGraph& graph,Opinion& start, double epsilon, double omega){
    //printf("Start MyIter\n");
    const size_t n = graph.num_vertices();
    std::vector<double> CVG;
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
        cache[i].s_max = epsilon*degree;

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
        double con = omega * cache[v].r*cache[v].lower_degree;
        cache[v].z += con;

        double max_error = 0;
        auto neighbours = graph.neighbors(v);
        for(auto it = neighbours.first; it != neighbours.second; ++it) {
            auto u = *it;
            cache[u].r += con * cache[u].upper_degree;
            if (cache[u].mask == false && fabs(cache[u].r) > cache[u].s_max) {
                Q.push(u);
                max_error = std::max(max_error,fabs(cache[u].r)/cache[u].s_max*epsilon);
                cache[u].mask = true;
            }
        }
        cache[v].r *= (1-omega);
        if (cache[v].mask == false && fabs(cache[v].r) > cache[v].s_max) {
                Q.push(v);
                max_error = std::max(max_error,fabs(cache[v].r)/cache[v].s_max*epsilon);
                cache[v].mask = true;
            }
        
        if(iter % (n/10) ==0)
            CVG.push_back(max_error);
    }
    Opinion z(n,0);
    for(size_t i=0; i<n; ++i){
        z[i] = cache[i].z;
    }
    delete[] cache;
    for(size_t i=0; i<CVG.size(); ++i){
        printf("%lf ", CVG[i]);
    }
    printf("0\n");
    return z;
}