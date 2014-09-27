set(MODULE TRUE)

set( LLVM_LINK_COMPONENTS support mc)

add_clang_library(SourceAnalyzer ourceAnalyzer.cpp)

add_dependencies(ourceAnalyzer
  ClangAttrClasses
  ClangAttrList
  ClangCommentNodes
  ClangDeclNodes
  ClangDiagnosticCommon
  ClangStmtNodes
  )

target_link_libraries(SourceAnalyzer
  clangFrontend
  clangAST
  )

set_target_properties(ourceAnalyzer
  PROPERTIES
  LINKER_LANGUAGE CXX
  PREFIX "")
