#include "algtest.hpp"
#include <MyQueue.hpp>
#include <math.h>
#include <vector>

struct Tempcache
{
    double z;
    double r;
    double s_max;
    double hat_z;
    double del,del_prev;
    double m;
    bool mask;
};

Opinion HB_vol(const CSRGraph& graph,Opinion& start, double epsilon){
    const size_t n = graph.num_vertices();
    const size_t max_degree = graph.max_degree();
    std::vector<size_t> Vol;
    Tempcache* cache = new Tempcache[n];
    std::vector<size_t> Q,Q_prev;
    std::vector<size_t> S;
    Q.reserve(n);
    S.reserve(n);

    for(size_t i=0; i < n; ++i) {
        auto degree = graph.out_degree(i);
        cache[i].m = sqrt(degree+2.0)/(degree+1.0);
        cache[i].z = 0;
        cache[i].r = start[i]/sqrt(degree+2.0)/(degree+1.0);
        cache[i].s_max = epsilon*degree;
        cache[i].mask = false;
        cache[i].hat_z = 0;
        cache[i].del_prev = 0;
        if (degree > 0) {
            Q.push_back(i);
            cache[i].mask = true;
        }
    }

    double omega = sqrt(2*(max_degree+1.0)*(max_degree+1.0)-1);
    omega = (omega-1)/(omega+1);
    omega = omega*omega*0.9;

    size_t iter = 0;
    while( !Q.empty()){
        Vol.push_back(Q.size());
        ++iter;
        for(auto v: Q) {
            S.emplace_back(v);
            cache[v].mask = false;
            cache[v].del = (1+omega)*cache[v].r + omega*cache[v].hat_z;
            cache[v].z += cache[v].del;
            cache[v].r -= cache[v].del;
            cache[v].hat_z += cache[v].del;
        }
        for(auto v: Q_prev) {
            cache[v].hat_z -= cache[v].del_prev;
        }
        std::swap(Q, Q_prev);
        Q.clear();

        for(const auto& v: S) {
            auto neighbours = graph.neighbors(v);
            double tmp = cache[v].del*cache[v].m;
            for(auto it = neighbours.first; it != neighbours.second; ++it) {
                auto u = *it;
                cache[u].r += cache[u].m*tmp;
                if (!cache[u].mask && fabs(cache[u].r) > cache[u].s_max) {
                    Q.emplace_back(u);
                    cache[u].mask = true;
                }
            }
            cache[v].del_prev = cache[v].del;
        }
        S.clear();
    }
    Opinion z(n,0);
    for(size_t i=0; i<n; ++i){
        z[i] = cache[i].m*cache[i].z;
    }
    delete[] cache;
    for(size_t i=0; i<Vol.size(); ++i){
        printf("%zu ", Vol[i]);
    }
    printf("0\n");
    return z;
}

Opinion CH_vol(const CSRGraph& graph,Opinion& start, double epsilon){
    //printf("Start MyIter\n");
    const size_t n = graph.num_vertices();
    const size_t max_degree = graph.max_degree();
    Tempcache* cache = new Tempcache[n];
    std::vector<size_t> Q,Q_prev;
    std::vector<size_t> S;
    Q.reserve(n);
    S.reserve(n);

    std::vector<size_t> Vol;

    for(size_t i=0; i < n; ++i) {
        auto degree = graph.out_degree(i);
        cache[i].m = sqrt(degree+2.0)/(degree+1.0);
        cache[i].z = 0;
        cache[i].r = start[i]/sqrt(degree+2.0)/(degree+1.0);
        cache[i].s_max = epsilon*degree;
        cache[i].mask = false;
        cache[i].hat_z = 0;
        cache[i].del_prev = 0;
        if (degree > 0) {
            Q.push_back(i);
            cache[i].mask = true;
        }
    }

    double alpha = (1.0-1.0/(max_degree+1.0)/(max_degree+1.0))*0.9;
    double delta = alpha,delta_prev = 0;

    size_t iter = 0;
    while( !Q.empty()){
        Vol.push_back(Q.size());
        ++iter;
        double para = delta*delta_prev;
        for(auto v: Q) {
            S.emplace_back(v);
            cache[v].mask = false;
            cache[v].del = (1+para)*cache[v].r + para*cache[v].hat_z;
            cache[v].z += cache[v].del;
            cache[v].r -= cache[v].del;
            cache[v].hat_z += cache[v].del;
        }
        for(auto v: Q_prev) {
            cache[v].hat_z -= cache[v].del_prev;
        }
        std::swap(Q, Q_prev);
        Q.clear();

        for(const auto& v: S) {
            auto neighbours = graph.neighbors(v);
            double tmp = cache[v].del*cache[v].m;
            for(auto it = neighbours.first; it != neighbours.second; ++it) {
                auto u = *it;
                cache[u].r += cache[u].m*tmp;
                if (!cache[u].mask && fabs(cache[u].r) > cache[u].s_max) {
                    Q.emplace_back(u);
                    cache[u].mask = true;
                }
            }
            cache[v].del_prev = cache[v].del;
        }
        S.clear();
        delta_prev = delta;
        delta = 1.0/(2.0/alpha-delta_prev);
    }
    Opinion z(n,0);
    for(size_t i=0; i<n; ++i){
        z[i] = cache[i].m*cache[i].z;
    }
    delete[] cache;
    for(size_t i=0; i<Vol.size(); ++i){
        printf("%zu ", Vol[i]);
    }
    printf("0\n");
    return z;
}



Opinion HB_CVG(const CSRGraph& graph,Opinion& start, double epsilon){
    const size_t n = graph.num_vertices();
    const size_t max_degree = graph.max_degree();
    std::vector<double> CVG;
    Tempcache* cache = new Tempcache[n];
    std::vector<size_t> Q,Q_prev;
    std::vector<size_t> S;
    Q.reserve(n);
    S.reserve(n);

    for(size_t i=0; i < n; ++i) {
        auto degree = graph.out_degree(i);
        cache[i].m = sqrt(degree+2.0)/(degree+1.0);
        cache[i].z = 0;
        cache[i].r = start[i]/sqrt(degree+2.0)/(degree+1.0);
        cache[i].s_max = epsilon*degree;
        cache[i].mask = false;
        cache[i].hat_z = 0;
        cache[i].del_prev = 0;
        if (degree > 0) {
            Q.push_back(i);
            cache[i].mask = true;
        }
    }

    double omega = sqrt(2*(max_degree+1.0)*(max_degree+1.0)-1);
    omega = (omega-1)/(omega+1);
    omega = omega*omega*0.9;

    size_t iter = 0;
    while( !Q.empty()){
        for(auto v: Q) {
            S.emplace_back(v);
            cache[v].mask = false;
            cache[v].del = (1+omega)*cache[v].r + omega*cache[v].hat_z;
            cache[v].z += cache[v].del;
            cache[v].r -= cache[v].del;
            cache[v].hat_z += cache[v].del;
        }
        for(auto v: Q_prev) {
            cache[v].hat_z -= cache[v].del_prev;
        }
        std::swap(Q, Q_prev);
        Q.clear();

        double max_error = 0;
        for(const auto& v: S) {
            auto neighbours = graph.neighbors(v);
            double tmp = cache[v].del*cache[v].m;
            for(auto it = neighbours.first; it != neighbours.second; ++it) {
                auto u = *it;
                cache[u].r += cache[u].m*tmp;
                if (!cache[u].mask && fabs(cache[u].r) > cache[u].s_max) {
                    Q.emplace_back(u);
                    max_error = std::max(max_error,fabs(cache[u].r) / cache[u].s_max*epsilon);
                    cache[u].mask = true;
                }
            }
            cache[v].del_prev = cache[v].del;
        }
        S.clear();
        for(auto u: Q_prev) {
           if (!cache[u].mask && fabs(cache[u].r) > cache[u].s_max) {
                Q.emplace_back(u);
                max_error = std::max(max_error,fabs(cache[u].r) / cache[u].s_max*epsilon);
                cache[u].mask = true;
            }
        }
        CVG.push_back(max_error);
        printf("%lf ", max_error);
        ++iter;
    }
    Opinion z(n,0);
    for(size_t i=0; i<n; ++i){
        z[i] = cache[i].m*cache[i].z;
    }
    delete[] cache;
    //for(size_t i=0; i<CVG.size(); ++i){
      //  printf("%ld ", CVG[i]);
    //}
    printf("0\n");
    return z;
}

Opinion CH_CVG(const CSRGraph& graph,Opinion& start, double epsilon){
    //printf("Start MyIter\n");
    const size_t n = graph.num_vertices();
    const size_t max_degree = graph.max_degree();
    Tempcache* cache = new Tempcache[n];
    std::vector<size_t> Q,Q_prev;
    std::vector<size_t> S;
    Q.reserve(n);
    S.reserve(n);

    std::vector<double> CVG;

    for(size_t i=0; i < n; ++i) {
        auto degree = graph.out_degree(i);
        cache[i].m = sqrt(degree+2.0)/(degree+1.0);
        cache[i].z = 0;
        cache[i].r = start[i]/sqrt(degree+2.0)/(degree+1.0);
        cache[i].s_max = epsilon*degree;
        cache[i].mask = false;
        cache[i].hat_z = 0;
        cache[i].del_prev = 0;
        if (degree > 0) {
            Q.push_back(i);
            cache[i].mask = true;
        }
    }

    double alpha = (1.0-1.0/(max_degree+1.0)/(max_degree+1.0))*0.99;
    double delta = alpha,delta_prev = 0;

    size_t iter = 0;
    while( !Q.empty()){
        double para = delta*delta_prev;
        for(auto v: Q) {
            S.emplace_back(v);
            cache[v].mask = false;
            cache[v].del = (1+para)*cache[v].r + para*cache[v].hat_z;
            cache[v].z += cache[v].del;
            cache[v].r -= cache[v].del;
            cache[v].hat_z += cache[v].del;
        }
        for(auto v: Q_prev) {
            cache[v].hat_z -= cache[v].del_prev;
        }
        std::swap(Q, Q_prev);
        Q.clear();

        double max_error =0;
        for(const auto& v: S) {
            auto neighbours = graph.neighbors(v);
            double tmp = cache[v].del*cache[v].m;
            for(auto it = neighbours.first; it != neighbours.second; ++it) {
                auto u = *it;
                cache[u].r += cache[u].m*tmp;
                if (!cache[u].mask && fabs(cache[u].r) > cache[u].s_max) {
                    Q.emplace_back(u);
                    max_error = std::max(max_error,fabs(cache[u].r));
                    cache[u].mask = true;
                }
            }
            cache[v].del_prev = cache[v].del;
        }
        S.clear();
        for(auto u: Q_prev) {
           if (!cache[u].mask && fabs(cache[u].r) > cache[u].s_max) {
                Q.emplace_back(u);
                max_error = std::max(max_error,fabs(cache[u].r));
                cache[u].mask = true;
            }
        }
        CVG.push_back(max_error);
        delta_prev = delta;
        delta = 1.0/(2.0/alpha-delta_prev);
        ++iter;
    }
    Opinion z(n,0);
    for(size_t i=0; i<n; ++i){
        z[i] = cache[i].m*cache[i].z;
    }
    delete[] cache;
    for(size_t i=0; i<CVG.size(); ++i){
        printf("%lf ", CVG[i]);
    }
    printf("0\n");
    return z;
}

Opinion GD_CVG(const CSRGraph& graph,Opinion& start, double epsilon){
    //printf("Start MyIter\n");
    const size_t n = graph.num_vertices();
    //const size_t max_degree = graph.max_degree();
    Tempcache* cache = new Tempcache[n];
    std::vector<size_t> Q;
    std::vector<std::pair<size_t,double>> S;
    std::vector<double> CVG;
    Q.reserve(n);
    S.reserve(n);

    for(size_t i=0; i < n; ++i) {
        auto degree = graph.out_degree(i);
        cache[i].m = sqrt(degree+2.0)/(degree+1.0);
        cache[i].z = 0;
        cache[i].r = start[i]/sqrt(degree+2.0)/(degree+1.0);
        cache[i].s_max = epsilon*cache[i].r;
        cache[i].mask = false;

        if (degree > 0) {
            Q.push_back(i);
            cache[i].mask = true;
        }
    }

    size_t iter = 0;
    while( !Q.empty()){
        for(auto v: Q) {
            S.emplace_back(v,cache[v].r);
            cache[v].mask = false;
            cache[v].z += cache[v].r;
            cache[v].r = 0;
        }
        Q.clear();
        double max = 0;
        for(const auto& p: S) {
            auto v = p.first;
            auto r = p.second;
            auto neighbours = graph.neighbors(v);
            for(auto it = neighbours.first; it != neighbours.second; ++it) {
                auto u = *it;
                cache[u].r += cache[u].m*cache[v].m * r;
                if (cache[u].mask == false && fabs(cache[u].r) > cache[u].s_max) {
                    max = std::max(max, fabs(cache[u].r));
                    Q.push_back(u);
                    cache[u].mask = true;
                }
            }
        }
        S.clear();
        CVG.push_back(max);
        ++iter;
    }
    for(size_t i=0; i<CVG.size(); ++i){
        printf("%lf ", CVG[i]);
    }
    Opinion z(n,0);
    for(size_t i=0; i<n; ++i){
        z[i] = cache[i].m*cache[i].z;
    }
    printf("0\n");
    delete[] cache;
    return z;
}