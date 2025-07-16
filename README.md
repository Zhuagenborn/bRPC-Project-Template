# *C++* *bRPC* Project Template

![C++](docs/badges/C++.svg)
[![CMake](docs/badges/Made-with-CMake.svg)](https://cmake.org)
[![GitHub Actions](docs/badges/Made-with-GitHub-Actions.svg)](https://github.com/features/actions)
![Linux](docs/badges/Linux.svg)

## Introduction

A *C++* *bRPC* project template on [*Ubuntu*](https://ubuntu.com) built with:

- [*bRPC*](https://brpc.apache.org)
- [*CMake*](https://cmake.org)
- [*GitHub Actions*](https://github.com/features/actions)

## Getting Started

### Prerequisites

- Install *CMake*.

### Building *bRPC*

Install dependencies.

```bash
sudo apt-get install -y libgflags-dev libprotobuf-dev libprotoc-dev protobuf-compiler libleveldb-dev
```

Clone and build *bRPC*.

```bash
git clone https://github.com/apache/brpc.git
cd brpc
sh config_brpc.sh --headers=/usr/include --libs=/usr/lib
make
```

### Building the Echo Service

Go to the project folder and run:

```bash
mkdir -p build
cd build
cmake -DBRPC_INSTALL_DIR=<brpc-path> ..
cmake --build .
```

`<brpc-path>` is the output directory of `brpc` (i.e., `brpc/output`), where `include` and `lib` folders are located.

## Structure

```console
.
├── .clang-format
├── .gitignore
├── CITATION.cff
├── client
│   ├── client.cpp
│   └── CMakeLists.txt
├── CMakeLists.txt
├── .github
│   └── workflows
│       └── cmake.yaml
├── docs
│   └── badges
│       ├── C++.svg
│       ├── Linux.svg
│       ├── Made-with-CMake.svg
│       └── Made-with-GitHub-Actions.svg
├── LICENSE
├── proto
│   ├── echo.proto
│   └── msg.proto
├── README.md
└── server
    ├── CMakeLists.txt
    └── server.cpp
```