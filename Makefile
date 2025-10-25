# 编译器设置
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude -I/usr/include/eigen3
DEBUG_FLAGS := -g
RELEASE_FLAGS := -O3

CXXFLAGS += -fopenmp
LDLIBS += -fopenmp

# 目录结构
SRC_DIR := src
INCLUDE_DIR := include
TEST_DIR := test
BUILD_DIR := build
BIN_DIR := bin
TEST_BUILD_DIR := $(BUILD_DIR)/test
TEST_BIN_DIR := $(BIN_DIR)/test
DATA_DIR := data
ANS_DIR := ans

# 主项目源文件和目标文件
MAIN_SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
MAIN_OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(MAIN_SOURCES))
MAIN_TARGET := $(BIN_DIR)/main

# 测试源文件和目标文件
TEST_SOURCES := $(wildcard $(TEST_DIR)/*.cpp)
TEST_TARGETS := $(patsubst $(TEST_DIR)/%.cpp,$(TEST_BIN_DIR)/%,$(TEST_SOURCES))

# 头文件依赖
HEADERS := $(wildcard $(INCLUDE_DIR)/*.hpp) $(wildcard $(INCLUDE_DIR)/*.ipp)

# 默认构建release版本
all: release

# release构建
release: CXXFLAGS += $(RELEASE_FLAGS)
release: $(MAIN_TARGET) tests

# debug构建
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: $(MAIN_TARGET) tests

# 主目标链接
$(MAIN_TARGET): $(MAIN_OBJECTS) | $(BIN_DIR)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

# 编译主项目源文件
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 编译测试文件
$(TEST_BIN_DIR)/%: $(TEST_DIR)/%.cpp $(MAIN_OBJECTS) | $(TEST_BIN_DIR)
	$(CXX) $(CXXFLAGS) $< $(filter-out $(BUILD_DIR)/main.o, $(MAIN_OBJECTS)) $(LDLIBS) -o $@

# 创建目录
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(TEST_BIN_DIR):
	mkdir -p $(TEST_BIN_DIR)

# 构建所有测试
tests: $(TEST_TARGETS)

package:
	tar -czf FDUrop_Cuibing.tar.gz $(SRC_DIR) $(INCLUDE_DIR) $(TEST_DIR) Makefile run_test.py

package_data:
	tar -czf data.tar.gz $(DATA_DIR)

# 清理
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# 运行主程序
run: $(MAIN_TARGET)
	@$(MAIN_TARGET) data/graph.txt

# 显示帮助
help:
	@echo "可用命令:"
	@echo "  make           - 构建release版本"
	@echo "  make debug     - 构建debug版本"
	@echo "  make tests     - 构建所有测试"
	@echo "  make run       - 运行主程序"
	@echo "  make clean     - 清理编译结果"
	@echo "  make help      - 显示此帮助信息"

.PHONY: all debug release tests test run clean help