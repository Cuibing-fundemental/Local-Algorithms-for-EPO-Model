#include <alg.hpp>
#include <MyQueue.hpp>
#include <queue>

Opinion MyIterator(const CSRGraph& graph,Opinion& start, double epsilon){
    //printf("Start MyIter\n");
    const size_t n = graph.num_vertices();
    Opinion z(n, 0);
    Opinion r = start;
    Opinion s_max(n);
    std::vector<double> upper_degree(n,0);
    std::vector<double> lower_degree(n,0);
    std::vector<bool> mask(n, false);
    std::queue<size_t> Q;
    for(size_t i=0; i < n; ++i) {
        s_max[i] = (start[i]*epsilon);
        auto degree = graph.out_degree(i);
        upper_degree[i] = (degree+2.0);
        lower_degree[i] = (1.0/(degree+1.0)/(degree+1.0));
        if (degree > 0) {
            Q.push(i);
            mask[i] = true;
        }
    }
    //printf("Q size: %zu\n", Q.size());

    size_t iter = 0;
    while( !Q.empty() ){
        ++iter;
        size_t v = Q.front();
        Q.pop();
        mask[v] = false;
        double con = r[v]*lower_degree[v];
        z[v] += con;

        auto neighbours = graph.neighbors(v);
        for(auto it = neighbours.first; it != neighbours.second; ++it) {
            r[*it] += con * upper_degree[*it];
            if (mask[*it] == false && r[*it] > s_max[*it]) {
                Q.push(*it);
                mask[*it] = true;
            }
        }

        r[v] = 0;
    }
    printf("Iter: %lu\n",iter);
    return z;
}