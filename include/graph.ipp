#ifndef CSR_GRAPH_IPP
#define CSR_GRAPH_IPP

template <typename EdgeIter>
CSRGraph::CSRGraph(size_t num_vertices, EdgeIter edges_begin, EdgeIter edges_end)
    : num_vertices_(num_vertices),
      num_edges_(std::distance(edges_begin, edges_end)) {
    
    degree_.resize(num_vertices_, 0);
    for (auto it = edges_begin; it != edges_end; ++it) {
        degree_[it->first]++;
    }
    
    row_ptr_.resize(num_vertices_ + 1);
    row_ptr_[0] = 0;

    size_t true_size = 0;
    for (size_t i = 1; i <= num_vertices_; ++i) {
        row_ptr_[i] = row_ptr_[i-1] + degree_[i-1];
        if(degree_[i-1]){
            true_size++;
        }
    }
    
    col_idx_.resize(num_edges_);
    std::vector<size_t> temp_pos(row_ptr_.begin(), row_ptr_.end());
    
    for (auto it = edges_begin; it != edges_end; ++it) {
        size_t src = it->first;
        size_t dest = it->second;
        col_idx_[temp_pos[src]++] = dest;
    }
    
    
    auto max_it = max_element(degree_.begin(), degree_.end());
    max_degree_ = *max_it;

    // 4. 对每个顶点的邻居排序 (可选，便于二分查找)
    #pragma omp parallel for
    for (size_t i = 0; i < num_vertices_; ++i) {
        auto start = col_idx_.begin() + row_ptr_[i];
        auto end = col_idx_.begin() + row_ptr_[i+1];
        std::sort(start, end);
    }
    true_num_vertices_ = true_size;
}

#endif // CSR_GRAPH_IPP