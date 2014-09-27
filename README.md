Source-Analyzer
===============

Source Analyzer is a simple [Clang](http://clang.llvm.org) compiler  plugin for source characterizing  applications. It walks the AST of an application, using Clang's RecursiveASTVisitor, and classifies nodes  into categories such as  **for** statement, **if** statement, etc. It was used in this work ([CHO: A Benchmark Suite for OpenCL-based FPGA Accelerators](https://www.escholar.manchester.ac.uk/uk-ac-man-scw:228247)).

Building and Running
====================
+ If you don't have Clang/LLVM already set them up as recommend here http://clang.llvm.org/get_started.html
+ Copy this folder to tools/clang/examples
+ Add plugin to makefile in tools/clang/Makefile by appending  the name of this directory  to the line that starts with PARALLEL_DIRS. If you are using cmake update CMakeLists.txt.
+ build/rebuild LLVM or Clang. 
+ Run the plugin  as follows
```shell
$ export BD=/path/to/build/directory
$ clang -cc1 -load $BD/lib/libSourceAnalyzer.so   some-input-file.c
```
SourceAnalyzer was tested with LLVM3.4 and Clang 3.4. May not compile with other versions of LLVM or Clang as APIs may differ.  The latest information about  Clang plugins can be found [here](http://clang.llvm.org/docs/ClangPlugins.html)
