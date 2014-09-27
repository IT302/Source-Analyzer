###SourceAnalyzer

SourceAnalyzer is a simple [clang](http://clang.llvm.org) compiler  plugin for source characterizing  applications. It walks the AST of an application and classifies nodes  into categories such as  **for** statement, **if** statement, etc. It was used in this work ([CHO: A Benchmark Suite for OpenCL-based FPGA Accelerators](https://www.escholar.manchester.ac.uk/uk-ac-man-scw:228247)).

###Building and Running

+ If you don't have clang/LLVM already set them up as recommend here http://clang.llvm.org/get_started.html
+ Copy this folder to tools/clang/examples
+ Add plugin to makefile in tools/clang/Makefile by appending  the name of this directory  to the line that starts with PARALLEL_DIRS. If you are using cmake update CMakeLists.txt.
+ build/rebuild LLVM or clang. 
+ Run the plugin  as follows
```shell
$ export BD=/path/to/build/directory
$ clang -cc1 -load $BD/lib/libSourceAnalyzer.so   some-input-file.c
```
SourceAnalyzer was tested with LLVM 3.4 and clang 3.4  may not compile with other versions of LLVM or clang as APIs may differ.  The latest information about  clang plugins can be found [here](http://clang.llvm.org/docs/ClangPlugins.html)
