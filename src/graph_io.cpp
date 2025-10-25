#include "graph_io.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <unordered_set>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <cstring>

class MMapFileReader {
private:
    int fd = -1;
    void* mapped_data = nullptr;
    size_t file_size = 0;
    size_t current_pos = 0;
    const char* data_ptr = nullptr;

public:
    explicit MMapFileReader(const std::string& filename) {
        // 打开文件
        fd = open(filename.c_str(), O_RDONLY);
        if (fd == -1) {
            throw std::runtime_error("Cannot open file: " + filename);
        }

        // 获取文件大小
        struct stat sb;
        if (fstat(fd, &sb) == -1) {
            close(fd);
            throw std::runtime_error("Cannot get file size: " + filename);
        }
        file_size = sb.st_size;

        if (file_size > 0) {
            // 映射文件到内存
            mapped_data = mmap(nullptr, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
            if (mapped_data == MAP_FAILED) {
                close(fd);
                throw std::runtime_error("Cannot mmap file: " + filename);
            }
            data_ptr = static_cast<const char*>(mapped_data);
        }
    }

    ~MMapFileReader() {
        if (mapped_data != nullptr && mapped_data != MAP_FAILED) {
            munmap(mapped_data, file_size);
        }
        if (fd != -1) {
            close(fd);
        }
    }

    // 禁止拷贝
    MMapFileReader(const MMapFileReader&) = delete;
    MMapFileReader& operator=(const MMapFileReader&) = delete;

    // 移动语义
    MMapFileReader(MMapFileReader&& other) noexcept 
        : fd(other.fd), mapped_data(other.mapped_data), 
          file_size(other.file_size), current_pos(other.current_pos),
          data_ptr(other.data_ptr) {
        other.fd = -1;
        other.mapped_data = nullptr;
        other.data_ptr = nullptr;
        other.file_size = 0;
        other.current_pos = 0;
    }

    MMapFileReader& operator=(MMapFileReader&& other) noexcept {
        if (this != &other) {
            this->~MMapFileReader();
            new (this) MMapFileReader(std::move(other));
        }
        return *this;
    }

    bool getline(std::string& line) {
        line.clear();
        
        if (current_pos >= file_size) {
            return false; // EOF
        }

        const char* start = data_ptr + current_pos;
        const char* end = data_ptr + file_size;
        
        // 查找行结束符
        const char* newline = static_cast<const char*>(
            memchr(start, '\n', end - start));
        
        if (newline != nullptr) {
            // 找到换行符
            size_t length = newline - start;
            line.assign(start, length);
            current_pos = newline - data_ptr + 1; // 跳过换行符
        } else {
            // 最后一行（没有换行符）
            line.assign(start, end - start);
            current_pos = file_size;
        }

        // 处理Windows风格的换行符(\r\n)
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        return true;
    }

    // 重置到文件开头
    void rewind() {
        current_pos = 0;
    }

    // 获取当前行号（从0开始）
    size_t getCurrentLineNumber() const {
        return std::count(data_ptr, data_ptr + current_pos, '\n');
    }
};

size_t edge_key(size_t a, size_t b) {
    return a < b ? (a << 32) | b : (b << 32) | a;
}

std::vector<std::pair<size_t, size_t>> read_edge_list(const std::string& filename, bool undirected) {
    MMapFileReader file(filename);

    std::vector<std::pair<size_t, size_t>> edges;
    std::string line;
    size_t line_num = 0;
    size_t max_vertex = 0;
    std::unordered_set<size_t> edge_keys; 

    while (file.getline(line)) {
        // 跳过空行和注释行
        if (line.empty() || line[0] == '#' || line[0] == '%') {
            continue;
        }
        line_num++;
        if(line_num % 10000000 == 0){
            std::cout << "Processed " << line_num << " lines." << std::endl;
        }

        std::istringstream iss(line);
        size_t src, dest;
        
        if (!(iss >> src >> dest)) {
            throw std::runtime_error("Invalid edge format at line " + std::to_string(line_num));
        }
        if( src == dest ) {
            continue; // 跳过自环
        }

        if(undirected == true){
            size_t key = edge_key(dest, src);
            if (edge_keys.find(key) == edge_keys.end()) {
                edges.emplace_back(src, dest);
                edge_keys.insert(edge_key(src, dest));
                edges.emplace_back(dest, src);
                edge_keys.insert(edge_key(dest, src));
            }
        }
        else{
            edges.emplace_back(src,dest);
        }
        max_vertex = std::max({max_vertex, src, dest});
    }


    return edges;
}

CSRGraph build_csr_from_file(const std::string& filename, bool undirected) {
    auto edges = read_edge_list(filename,undirected);
    
    if (edges.empty()) {
        throw std::runtime_error("No edges found in file");
    }

    // 计算顶点数量（最大顶点ID + 1）
    size_t num_vertices = 0;
    for (const auto& [src, dest] : edges) {
        num_vertices = std::max({num_vertices, src + 1, dest + 1});
    }
    
    return CSRGraph(num_vertices, edges.begin(), edges.end());
}