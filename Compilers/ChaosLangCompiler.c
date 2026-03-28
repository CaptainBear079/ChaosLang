#include "ChaosLangCompiler.h"

File ChaosLangCompiler(File file, char* argv0) {
    File* t_file;
    t_file = ChaosLang__PreProcessor(file, resolvedFile);
    t_file = ChaosLang__Parser(file, intermediate);
    return *t_file;
}