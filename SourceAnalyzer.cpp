//===- SourceAnalyzer.cpp ---------------------------------------------===//
//
//                     Source Analyzer
//
// This file is distributed under the MIT  License. See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// clang plugin that classifies nodes in the input file.
//
//===----------------------------------------------------------------------===//

#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/CompilerInstance.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/YAMLTraits.h"
using namespace clang;
using llvm::yaml::ScalarEnumerationTraits;
using llvm::yaml::MappingTraits;
using llvm::yaml::IO;
using llvm::yaml::Output;

typedef struct {
    int numFunctions;
    int numForStmts;
    int numDoStmts;
    int numGotoStmts;
    int numSwitchStmts;
    int numWhileStmts;
    int numBreakStmts;
    int numContinueStmts;
    int numIndirectGotoStmts;
    int numIfStmts;
    int numDivOps;
    int numMultOps;
    int numAddSubOps;
    int numCompOps;
    int numShiftOps;
    int numAssgnOps;
    int numPtrOps;
    int numLogOps;
    int numArray;
    int numStruct;
    int numScalar;
    int numAggregate;
    int numUnion;
} Result;

namespace llvm {
namespace yaml {
template <>
struct MappingTraits<Result> {
    static void mapping(IO &io, Result &R) {
        io.mapRequired("Functions",       R.numFunctions);
        io.mapRequired("ForStmts",       R.numForStmts);
        io.mapRequired("IfStmts",       R.numIfStmts);
        io.mapRequired("DoStmts", R.numDoStmts);
        io.mapRequired("GotoStmts", R.numGotoStmts);
        io.mapRequired("SwitchStmts", R.numSwitchStmts);
        io.mapRequired("WhileStmts",R.numWhileStmts);
        io.mapRequired("BreakStmts",R.numBreakStmts);
        io.mapRequired("ContinueStmts", R.numContinueStmts);
        io.mapRequired("IndirectGotoStmts",R.numIndirectGotoStmts);
        io.mapRequired("DivOps", R.numDivOps);
        io.mapRequired("MultOps", R.numMultOps);
        io.mapRequired("AddSubOps",R.numAddSubOps);
        io.mapRequired("CompOps",R.numCompOps);
        io.mapRequired("ShiftOps",R.numShiftOps );
        io.mapRequired("AssgnOps", R.numAssgnOps);
        io.mapRequired("PtrOps", R.numPtrOps);
        io.mapRequired("LogOps",R.numLogOps);
        io.mapRequired("ScalarVars",R.numScalar);
        //io.mapRequired("ArrayVars", R.numArray);
        //io.mapRequired("StructVars",R.numStruct);
        //io.mapRequired("UnionVars", R.numUnion);
        io.mapRequired("AggreVars", R.numAggregate);

    }
};
}
}
//void dumpResult(const Result &R) {
//  Output yout(llvm::outs());
//  yout << R;
//}

namespace {



Result R = {0};




class MyASTConsumer : public ASTConsumer,
                      public RecursiveASTVisitor<MyASTConsumer>
{
    public:
        void HandleTranslationUnit (clang::ASTContext &ctx);
        bool VisitFunctionDecl (clang::FunctionDecl* decl);
        //bool VisitParmVarDecl (clang::ParmVarDecl* decl);
        bool VisitVarDecl (clang::VarDecl* decl);
        //bool VisitNamedDecl (clang::NamedDecl* decl);
        bool Visit (clang::TypeLoc TL);
        bool VisitStmt (clang::Stmt* stmt);
};



bool MyASTConsumer::VisitVarDecl (clang::VarDecl* decl)
{
    TypeLoc TL = decl->getTypeSourceInfo()->getTypeLoc();
    const Type *T = TL.getTypePtr();
    if (T->isArrayType())
        ++R.numArray;
    if(T->isStructureType())
    {
        ++R.numStruct;
        //T->dump();
    }
    if(T->isUnionType())
        ++R.numUnion;
    if(T->isScalarType())
        ++R.numScalar;
    if (T->isAggregateType())
    {
        //T->dump();
        ++R.numAggregate;
    }

    return true;

}

void MyASTConsumer::HandleTranslationUnit (clang::ASTContext &ctx)
{
    //fprintf(stderr, "Welcome to MyASTConsumer!\n");
    llvm::errs() << "Welcome to MyASTConsumer!\n";
    TraverseDecl(ctx.getTranslationUnitDecl());
    Output yout(llvm::errs());
    yout << R;
    //ctx.PrintStats();
    llvm::errs() << "Goodbye!\n";
}

bool MyASTConsumer::VisitStmt (clang::Stmt* stmt)
{
    if(ForStmt * s = dyn_cast<ForStmt>(stmt) )
    {
        ++R.numForStmts;
        //s->dumpColor();
        s = NULL;
    }

    if(DoStmt *s = dyn_cast<DoStmt>(stmt))
    {
        ++R.numDoStmts;
        s = NULL;
    }
    if (GotoStmt *s = dyn_cast<GotoStmt>(stmt))
    {
        ++R.numGotoStmts;
        s = NULL;
    }
    if (SwitchStmt *s = dyn_cast<SwitchStmt>(stmt))
    {
        ++R.numSwitchStmts;
        s = NULL;
    }
    if(WhileStmt *s = dyn_cast<WhileStmt>(stmt))
    {
        ++R.numWhileStmts;
        s = NULL;
    }
    if(IfStmt *s = dyn_cast<IfStmt>(stmt))
    {
        ++R.numIfStmts;
        s = NULL;
    }

    if(BreakStmt *s = dyn_cast<BreakStmt>(stmt))
    {
        ++R.numBreakStmts;
        s = NULL;
    }
    if(ContinueStmt *s = dyn_cast<ContinueStmt>(stmt))
    {
        ++R.numContinueStmts;
        s = NULL;
    }
    if(IndirectGotoStmt *s = dyn_cast<IndirectGotoStmt>(stmt))
    {
        ++R.numIndirectGotoStmts;
        //s->dumpColor();
        s = NULL;
    }
    if (BinaryOperator * binop = dyn_cast<BinaryOperator>(stmt))
    {
        if(binop->isAdditiveOp())
            ++R.numAddSubOps;
        else if(binop->isAssignmentOp() )
        {
            ++R.numAssgnOps;
            //binop->dumpColor();

        }
        else if(binop->isLogicalOp() || binop->isBitwiseOp())
            ++R.numLogOps;
        else if(binop->isComparisonOp() || binop->isRelationalOp() || binop->isEqualityOp())
            ++R.numCompOps;
        else if(binop->isMultiplicativeOp())
        {
            if(binop->getOpcode() != BO_Mul)
            {
                ++R.numDivOps;
                //binop->dumpColor();
            }
            else
                ++R.numMultOps;

        }
        else if(binop->isPtrMemOp())
            ++R.numPtrOps;
        else if (binop->isShiftOp())
            ++R.numShiftOps;

        if(binop->isCompoundAssignmentOp())
        {
            ++R.numAssgnOps;
            if(binop->getOpcode() == BO_MulAssign)
                ++R.numMultOps;
            else if (binop->getOpcode() == BO_DivAssign || binop->getOpcode() == BO_RemAssign)
                ++R.numDivOps;
            else if(binop->getOpcode() == BO_OrAssign || binop->getOpcode() == BO_AndAssign
                    || binop->getOpcode() == BO_OrAssign || binop->getOpcode()  == BO_XorAssign )
                ++R.numLogOps;
            else if (binop->getOpcode() == BO_ShrAssign || binop->getOpcode() == BO_ShlAssign)
                ++R.numShiftOps;

             else
                ++R.numAddSubOps;

        }


    }
    if(UnaryOperator * uniOps = dyn_cast<UnaryOperator>(stmt))
    {
        if(uniOps->isArithmeticOp())
            ++R.numAddSubOps;
    }

    return true;
}



 bool MyASTConsumer::VisitFunctionDecl (clang::FunctionDecl* decl)
{
     ++R.numFunctions;

     return true;



}

class MyASTAction : public PluginASTAction {
protected:
  ASTConsumer *CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) {
    return ( new MyASTConsumer());
  }

  bool ParseArgs(const CompilerInstance &CI,
                 const std::vector<std::string>& args) {
    for (unsigned i = 0, e = args.size(); i != e; ++i) {
      llvm::errs() << "SourceAnalyzer arg = " << args[i] << "\n";

      // Example error handling.
      if (args[i] == "-an-error") {
        DiagnosticsEngine &D = CI.getDiagnostics();
        unsigned DiagID = D.getCustomDiagID(
          DiagnosticsEngine::Error, "invalid argument '" + args[i] + "'");
        D.Report(DiagID);
        return false;
      }
    }
    if (args.size() && args[0] == "help")
      PrintHelp(llvm::errs());

    return true;
  }
  void PrintHelp(llvm::raw_ostream& ros) {
    ros << "Help for SourceAnalyzer plugin goes here\n";
  }

};

}

static FrontendPluginRegistry::Add<MyASTAction>
X("src-anlz", "source code analyzer");
