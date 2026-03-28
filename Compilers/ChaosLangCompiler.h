// Includes
#include <stdio.h>

// Global structures
#include "./../structures.h"

// ChaosLang specific includes
#include "./../PreProcessors/ChaosLang/preprocessor.h"
#include "./../Parsers/ChaosLang/parser.h"

// Constants
const char* resolvedFile = ".r";
const char* intermediate = ".ir";

File ChaosLangCompiler(File file, char* argv0);