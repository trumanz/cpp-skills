#include <sstream>


#include <clang/AST/ASTContext.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/Basic/DiagnosticOptions.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/FileManager.h>
#include <clang/Basic/FileSystemOptions.h>
#include <clang/Basic/LangOptions.h>
//#include <clang/Basic/MemoryBufferCache.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Basic/TargetInfo.h>
#include <clang/CodeGen/CodeGenAction.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/CompilerInvocation.h>
#include <clang/Lex/HeaderSearch.h>
#include <clang/Lex/HeaderSearchOptions.h>
#include <clang/Lex/Preprocessor.h>
#include <clang/Lex/PreprocessorOptions.h>
#include <clang/Parse/ParseAST.h>
#include <clang/Sema/Sema.h>

#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/Bitcode/BitcodeReader.h>
#include <llvm/InitializePasses.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include <llvm/IR/DataLayout.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Transforms/Utils/Cloning.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/Host.h>
//#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/DynamicLibrary.h>
#include <llvm/Object/ObjectFile.h>
#include <llvm/Linker/Linker.h>

#include "DiagnosticConsumerHandlerCall.h"
struct ClangJitContext {
    ClangJitContext() : llvm(0), engine(0) //, options()
    {
       // options[ClangJitOption_OptimizeLevel] = 2;
       // options[ClangJitOption_ErrorLimit] = 100;
       // options[ClangJitOption_DegugMode] = false;
        triple = llvm::sys::getDefaultTargetTriple();
    }

    ~ClangJitContext()
    {
        delete engine;
        module.reset();
        delete llvm;
    }

    llvm::LLVMContext *llvm;
    llvm::ExecutionEngine* engine;
    std::string triple;
    //int options[ClangJitOption_MaxOptionCount];
    std::unique_ptr<llvm::Module> module;
};

class ClangJitCompiler{

ClangJitContext* jitContext;

public:
   ClangJitCompiler() {
     InitializeLLVM();
     jitContext = new ClangJitContext();
   }

   void setOptimizeLevel(int level){

   }
   void compile(const char* source,  
           int (*error_handler)(int level, const char* filename, int line, int column, const char *message)) {

       if(!clang_JitIrCompile(source, error_handler)) {
         throw std::runtime_error("Compile error");
       }
            printf(" %s: %d\n", __FILE__, __LINE__);
       if(!clang_JitIrOptimize()) {
         throw std::runtime_error("Optimization error");
       }
            printf(" %s: %d\n", __FILE__, __LINE__);
                
      if(!clang_JitGenerateTargetCode()) {
          throw std::runtime_error("Failed to generate a target code.");
      }
            printf(" %s: %d\n", __FILE__, __LINE__);

   }

   bool clang_JitIrCompile(const char *source,
         int (*error_handler)(int level, const char* filename, int line, int column, const char *message)) {
      llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs> diagIDs(new clang::DiagnosticIDs());
      llvm::IntrusiveRefCntPtr<clang::DiagnosticOptions> diagnosticOptions(new clang::DiagnosticOptions());
      diagnosticOptions->ErrorLimit = 100;
      std::unique_ptr<DiagnosticConsumerHandlerCall> diagHandler = std::make_unique<DiagnosticConsumerHandlerCall>(error_handler);
      std::unique_ptr<clang::DiagnosticsEngine> diagnosticsEngine =
         std::make_unique<clang::DiagnosticsEngine>(diagIDs, diagnosticOptions, diagHandler.get(), false);


      clang::CompilerInstance compilerInstance;
      auto&  compilerInvocation = compilerInstance.getInvocation();

      std::stringstream ss;
      ss << "-triple=" << llvm::sys::getDefaultTargetTriple();
      ss << " -O3  -fcxx-exceptions  -fms-extensions";
      
     std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::istream_iterator<std::string> i = begin;
    std::vector<const char*> itemcstrs;
    std::vector<std::string> itemstrs;
    for ( ; i != end; ++i) {
        itemstrs.push_back(*i);
    }
    

    for (unsigned idx = 0; idx < itemstrs.size(); idx++) {
        itemcstrs.push_back(itemstrs[idx].c_str());
        printf("%s\n", itemcstrs[idx]);
    }

   llvm::ArrayRef< const char * >  cmd_line_args(itemcstrs);


    // compiler instance.
    clang::CompilerInvocation::CreateFromArgs(compilerInvocation, cmd_line_args, *diagnosticsEngine.get());


       // Options.
    // auto& preprocessorOptions = compilerInvocation.getPreprocessorOpts();
    // auto& codeGenOptions = compilerInvocation.getCodeGenOpts();
    auto* languageOptions = compilerInvocation.getLangOpts();
    auto& targetOptions = compilerInvocation.getTargetOpts();
    targetOptions.Triple = llvm::sys::getDefaultTargetTriple();
    auto& headerSearchOptions = compilerInvocation.getHeaderSearchOpts();
    auto& frontEndOptions = compilerInvocation.getFrontendOpts();
    frontEndOptions.Inputs.clear();
    bool debug_mode = true;
    if(debug_mode){
       frontEndOptions.ShowStats = true;
        headerSearchOptions.Verbose = true;
    }

    std::unique_ptr<llvm::MemoryBuffer> buffer;
    
    //languageOptions->Bool = 1;
    //languageOptions->CPlusPlus = 1;
    
    clang::InputKind input_kind(clang::Language::C);
    frontEndOptions.Inputs.push_back(clang::FrontendInputFile(source, input_kind));
    
    compilerInstance.createDiagnostics(diagHandler.get(), false);
     
    compilerInstance.createDiagnostics(diagHandler.get(), false);
    jitContext->llvm = new llvm::LLVMContext();
    std::unique_ptr<clang::CodeGenAction> action = std::make_unique<clang::EmitLLVMOnlyAction>(jitContext->llvm);
    if (!compilerInstance.ExecuteAction(*action)) {
        return false;
    }
    if (diagHandler->getTotalErrorCount() > 0) {
        if ((diagHandler->getErrorCount() > 0) || (diagHandler->getFatalCount() > 0) ) {
            return false;
        }
    }

    // get IR module.
    jitContext->module = action->takeModule();
    if (!jitContext->module) {
        return false;
    }


      return true;
   }

   bool clang_JitIrOptimize() {

       if (!jitContext || !jitContext->module) return false;

    clang::CompilerInstance compilerInstance;
    auto& compilerInvocation = compilerInstance.getInvocation();
    auto& codeGenOptions = compilerInvocation.getCodeGenOpts();

    // optimizations.
    llvm::PassBuilder passBuilder;
    llvm::LoopAnalysisManager loopAnalysisManager;
    llvm::FunctionAnalysisManager functionAnalysisManager;
    llvm::CGSCCAnalysisManager cGSCCAnalysisManager;
    llvm::ModuleAnalysisManager moduleAnalysisManager;
    passBuilder.registerModuleAnalyses(moduleAnalysisManager);
    passBuilder.registerCGSCCAnalyses(cGSCCAnalysisManager);
    passBuilder.registerFunctionAnalyses(functionAnalysisManager);
    passBuilder.registerLoopAnalyses(loopAnalysisManager);
    passBuilder.crossRegisterProxies(loopAnalysisManager, functionAnalysisManager, cGSCCAnalysisManager, moduleAnalysisManager);
    llvm::ModulePassManager modulePassManager = 
            passBuilder.buildPerModuleDefaultPipeline(llvm::OptimizationLevel::O3);
    modulePassManager.run(*jitContext->module, moduleAnalysisManager);

            return true;

   }
   bool clang_JitGenerateTargetCode() {
      
          if (!jitContext || !jitContext->module) return false;

    llvm::EngineBuilder builder(std::move(jitContext->module));
    builder.setMCJITMemoryManager(std::make_unique<llvm::SectionMemoryManager>());
    builder.setOptLevel(llvm::CodeGenOpt::Level::Aggressive);
    auto executionEngine = builder.create();
    if (!executionEngine) {
        return false;
    }

    jitContext->engine = executionEngine;
            return true;

   }

 void *clang_JitGetFunctionAddress(const char *name)
{
    if (!jitContext) return nullptr;
    if (!jitContext->engine) return nullptr;
    return (void *)(jitContext->engine->getFunctionAddress(name));
}

   void clang_JitFinalizeCode() {
            if (!jitContext || !jitContext->engine) return;
    jitContext->engine->finalizeObject();
   }
   template<typename T>
   T getFunctionAddress(const char* name) {
             printf(" %s: %d\n", __FILE__, __LINE__);

     void *addr = clang_JitGetFunctionAddress(name);
             printf(" %s: %d, %p \n", __FILE__, __LINE__, addr);

        if (!addr) {
            throw std::runtime_error("Function not found.");
        }
         T x = (T)addr;
         printf(" %s: %d, %p \n", __FILE__, __LINE__, addr);

        return x;
        //return reinterpret_cast<T>(addr);
   }



private:
   void InitializeLLVM() {
    static bool LLVMinit = false;
    if(LLVMinit) {
        return;
    }
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    auto& Registry = *llvm::PassRegistry::getPassRegistry();
    llvm::initializeCore(Registry);
    llvm::initializeScalarOpts(Registry);
    llvm::initializeVectorization(Registry);
    llvm::initializeIPO(Registry);
    llvm::initializeAnalysis(Registry);
    llvm::initializeTransformUtils(Registry);
    llvm::initializeInstCombine(Registry);
    llvm::initializeInstrumentation(Registry);
    llvm::initializeTarget(Registry);

    llvm::sys::DynamicLibrary::LoadLibraryPermanently(nullptr);

    LLVMinit = true;

   }




};