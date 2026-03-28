// Includes
#include <stdio.h>

// Compilers
#include "./Compilers/ChaosLangCompiler.h" // ChaosLang
#include "./Compilers/CCompiler.h" // C

// C specific includes

// Assembly specific includes
#include "./Translator/x86_64/translator.h"
#include "./Assembler/x86_64/assembler.h"

int main(int argc, char* argv[]) {
	bool bflags[2] = {
		true, // Translate
		true  // Assemble
	};
	int flags[1] = {
		0 // x86_64 Assembly
	};
	int language = 0;
	char* fileName;
	FILE* fptr;
	File file = { fptr, fileName, NULL };
	switch(language) {
		case 0: {
			printf("Language could not be detected.");
		} break;
		case 1: {
			// ChaosLang compile route
			file = ChaosLangCompiler(file, argv[0]);
			if(bflags[0]) {
				if(flags[0] == 0) {
					Translator__x86_64();
					if(bflags[1]) {
						Assembler__x86_64();
					}
				}
				else {
					printf("[ERROR] Invalid assembler specified.\n");
				}
			}
		} break;
		case 2: {
			// C compile route
			file = CCompiler(file, argv[0]);
			if(bflags[0]) {
				if(flags[0] == 0) {
					Translator__x86_64();
					if(bflags[1]) {
						Assembler__x86_64();
					}
				}
				else {
					printf("[ERROR] Invalid assembler specified.\n");
				}
			}
		} break;
	}
	return 0;
}