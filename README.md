# 🧮 TI301I – Study of Markov Graphs (Parts 1–3)

## 📌 1. Project overview

This repository contains the full implementation of the TI301I project  
**“Study of Markov Graphs”** (Parts 1, 2 and 3).

A **Markov graph** is a directed graph where:
- Each vertex represents a state.
- Each edge carries a transition probability in `(0, 1]`.
- For every vertex, the **sum of outgoing probabilities is 1** (Markov constraint). :contentReference[oaicite:1]{index=1}

The project is split into three main parts:

- **Part 1 – Adjacency list & Markov validation**
    - Read a graph from a text file.
    - Store it as an *adjacency list*.
    - Check whether it is a **Markov graph**.
    - Export the graph to **Mermaid** format for visualisation.

- **Part 2 – Strongly Connected Components & Hasse diagram**
    - Compute **strongly connected components (SCCs)** using **Tarjan’s algorithm**. :contentReference[oaicite:2]{index=2}
    - Build the **Hasse diagram** of classes.
    - Classify classes and states as *transient*, *persistent*, *absorbing*.
    - Check whether the graph is **irreducible**.

- **Part 3 – Matrix representation & Markov chain behaviour**
    - Convert the adjacency list to a **transition matrix**.
    - Compute matrix powers \(M^k\) to observe multi-step transitions.
    - Study **convergence** and **stationary distributions**.
    - For each class, analyse convergence and compute the **period** (when relevant).

---

## 📂 2. Repository structure

```text
    2526_TI301I6_PRJ_GRP8/
    ├── CMakeLists.txt           # Build configuration (C99, multiple targets)
    ├── README.md                # This document
    │
    ├── data/                    # our first input graphs
    │   ├── example1.txt
    │   └── example2.txt
    │
    ├── doc/                     # Official statements and evaluation grid
    │   ├── Auto-evaluation grid.xlsx
    │   ├── sujet_partie1_EN.pdf
    │   ├── sujet_partie2_EN.pdf
    │   ├── Plan-PART1-PRJ.pdf       # Coding plan – Part 1
    │   ├── Plan-PART2-PRJ.pdf       # Coding plan – Part 2
    │   └── sujet_partie_3_EN.pdf
    │
    ├── include/                 # Public headers (project API)
    │   ├── adj_list.h
    │   ├── export_mermaid.h
    │   ├── hasse.h
    │   ├── markov_check.h
    │   ├── matrix.h
    │   ├── partition.h
    │   └── tarjan.h
    │
    ├── src/                     # Core implementation
    │   ├── adj_list.c           # Adjacency list creation / display / free / file read
    │   ├── export_mermaid.c     # Export Markov graph (Part 1) to Mermaid
    │   ├── hasse.c              # Hasse diagram construction & export (Part 2)
    │   ├── main_part1.c         # Executable: graph_part1
    │   ├── main_part2.c         # Executable: graph_part2 (Tarjan + Hasse)
    │   ├── main_part3.c         # Executable: part3 (matrix & convergence)
    │   ├── markov_check.c       # Markov validation (sum of outgoing probabilities)
    │   ├── matrix.c             # Matrix operations & Part 3 logic
    │   ├── partition.c          # Classes & partition management
    │   └── tarjan.c             # Tarjan strongly connected components
    │
    ├── interface/               # Small SDL2 experiments (not required for grading)
    │   ├── sdl_test.c
    │   └── sdl_weather.c
    │
    └── test_bench/              # Bench data and generated Mermaid examples
        ├── exemple1.txt
        ├── etc...
```
---
## ⚙️ 3. Build instructions

### 3.1 Requirements

- C compiler with C99 support (GCC/Clang).
- CMake ≥ 3.30.
- (Optional) SDL2 development files for `/interface` demos.

If SDL2 is missing, you can still build the core executables.

### 3.2 Build (command line)
```
mkdir -p cmake-build
cd cmake-build
cmake ..
cmake --build . --target graph_part1 graph_part2 part3
```

---
## 📄 4. Input File Format

Graph input files in ```test_bench/``` follow the specification:

```
N
i j p
```

Example:
```
4
1 1 0.95
1 2 0.04
1 3 0.01
2 2 0.90
2 3 0.05
2 4 0.05
3 3 0.80
3 4 0.20
4 1 1.00
```

---
## 🧩 5. Part 1 – Markov Graph & Mermaid Export

### **Executable**
`graph_part1`

### **Main Features**
- Construction of adjacency lists
- Verification of Markov constraints
- Human-readable graph display
- Mermaid diagram export (`../output_files/<basename>_graph.mmd`)

### **Example Execution**
```
./graph_part1
Enter graph file path: ../data/example1.txt
```

---
## 🔗 6. Part 2 – SCC, Partition & Hasse Diagram

### **Executable**
`graph_part2`

### **Academic Contributions**
- Complete SCC decomposition via Tarjan
- Construction of the class graph
- Hasse diagram reduction (partial order)
- Identification of:
    - Transient classes
    - Persistent classes
    - Absorbing states
- Irreducibility assessment

### **Example Execution**
```
./graph_part2
Enter graph file path: ../data/example1.txt
```

---

## 🔢 7. Part 3 – Matrix Analysis, Convergence & Periods

### **Executable**
`part3`

### **Capabilities**
- Matrix representation of a Markov system
- Powers of M: M, M³, M⁷, …
- Convergence evaluation
- Numerical approximation of stationary behaviour
- Per-class period computation

### **Example Execution**
```
./part3
Enter graph file path: ../data/example1.txt
```

---

## ⚠️ 8. Error Handling

The system performs robust validation for:
- Missing/unreadable files
- Incorrect formatting
- Invalid probabilities
- Non-Markov vertices
- Allocation failures
- Export errors

All errors are reported with explicit academic-style diagnostics in the shell.

---
## 🚀 9. Quick Testing Suite

```
./graph_part1    # Part 1: Markov + Mermaid
./graph_part2    # Part 2: SCC + Hasse
./part3          # Part 3: Matrices + Convergence
```

Test datasets are available in `data/` and `test_bench/` with outputs file in `output_files/` .

---

## 🎓 10. Summary

This repository provides:
- A robust C implementation aligned with TI301I academic standards
- Three fully independent executables
- Complete analysis pipeline (graph → SCC → Hasse → matrix → chain behaviour)
- Mermaid exports for visual interpretation
- Clean and modular code architecture

This project satisfies the complete specification for the TI301I Markov Graph study.

---
## 🧠 11. Work Distribution

Developed by **Group 8**,

### Team Members

- **Daccache Hadi**
- **Gastaldo Raphael**
- **Johannel Alexandre**
- **Rubem Adrian**

- **Raphael Gastaldo**  
  Lead on **Part 1** (file loading, adjacency list, Markov validation, Mermaid export).  
  Co-responsible for **Part 2 design and implementation** (Tarjan partition, Hasse diagram, Mermaid export).

- **Rubem Adrian**  
  Co-responsible for **Part 2**: design documents, Tarjan-based partitioning into classes, links between classes, Hasse diagram and Mermaid export.  
  Contributed to the interpretation and validation of the class structure.

- **Daccache Hadi**  
  Co-responsible for **Part 3**: transformation of the graph into a matrix, creation of class sub-matrices, computation of stationary distributions.  
  Contributed to the analysis of convergence behaviour.

- **Johannel Alexandre**  
  Co-responsible for **Part 3**: matrix construction pipeline, management of sub-matrices by class, stationary distribution calculations.  
  Participated in testing and verification of numerical results.

This work distribution section reflects the information reported in the official auto-evaluation grid.

---

## 🧩 12. Bonus Features (Optional Development)

Although not required in the TI301I specification, the group implemented an additional **SDL2-based graphical extension** to explore how Markov graph processing could be connected to a minimal interactive interface.

### 12.1 Objective of the Bonus
This optional module demonstrates:

- Basic visual display through **SDL2**
- Rendering of simple shapes, text, or graph-related data
- Exploration of UI loops and event handling
- A potential direction for extending Parts 1–3 into a fully interactive viewer

While not evaluated, this bonus illustrates initiative beyond the core academic expectations.

### 12.2 Accessing the Bonus Code
The bonus code is available in the `interface/` directory of the project:

👉 **[Click here to download the bonus folder](https://mega.nz/file/HdkGhSjT#ZszxWI3Xtjz4TFY8R1Tvnc4G0ZcZS0iJXPZeudwP25g)**

This folder contains:

- `sdl_test.c` — SDL initialisation and rendering sandbox
- `sdl_weather.c` — extended text and UI experimentation

### 12.3 Build Notes
The bonus requires **SDL2** and **SDL_ttf**.  
If these libraries are not installed, the bonus can be ignored safely without affecting Parts 1–3.

The entire bonus is fully isolated and does not modify any mandatory component of the project.