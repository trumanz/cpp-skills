#include "ClangJitCompiler.h"


int error_handler(int level, const char* filename, int line, int column, const char *message)
{
    static int count = 0;
    static const char *levelString[] = {"Ignore", "Note", "Remark", "Warning", "Error", "Fatal"};
    printf("%s(%d): %s\n", levelString[level], ++count, message);
    printf("    File: %s\n", filename);
    printf("    Line: %d\n\n", line);
    return 1;
}

int main(int argc, char** argv) {
    ClangJitCompiler compiler;
    
        printf(" %s: %d\n", __FILE__, __LINE__);


    compiler.setOptimizeLevel(3);
            printf(" %s: %d\n", __FILE__, __LINE__);

    compiler.compile(argv[1],  error_handler);
            printf(" %s: %d\n", __FILE__, __LINE__);

    compiler.clang_JitFinalizeCode();
        printf(" %s: %d\n", __FILE__, __LINE__);

    typedef int (*mainf_t)();

    mainf_t ff = compiler.getFunctionAddress<mainf_t>("main");
        printf(" %s: %d\n", __FILE__, __LINE__);

    if(ff) {
        return ff();
    }

    return 1;
}