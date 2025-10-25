# Local-Algorithms-for-EPO-Model
Code for **Fast Local Algorithms for Expressed and Private Opinion Model** 

## Project Structure

```
├── data/          # datasets
│   ├── DBLP.txt
│   ├── Email-Enron.txt
│   ├── Hamsterster.txt
│   ├── LiveJournal.txt
│   ├── Youtube.txt
│   └── test.txt
├── include/       # header files
│   ├── alg.hpp
│   ├── algtest.hpp
│   ├── graph_io.hpp
│   ├── graph.hpp
│   └── MyQueue.hpp
├── src/          # source code
│   ├── ExactSol.cpp
│   ├── graph_io.cpp
│   ├── graph.cpp
│   ├── MyIterator.cpp
│   ├── MyIterator_new.cpp
│   ├── MyIterator_new_volume.cpp
│   ├── NaiveIterator.cpp
│   ├── RichardsonIterator.cpp
│   └── RichardsonIterator_volume.cpp
└── test/         # test files
    ├── CVG.cpp
    ├── test_direct.cpp
    ├── test_error.cpp
    ├── test_MyIterator.cpp
    └── vol.cpp
```
## Build and Run

The project uses a **Makefile** for compilation. Make sure a C++ compiler is installed.  

```bash
# Compile the project
make tests

# Run tests
python run_test.py
```