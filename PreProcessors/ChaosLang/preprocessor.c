#include "preprocessor.h"

// Pre-processor directives:
	// - include:
	//   Includes code from other files.
	//   Written: #include <file.h/file.soft/file.source/file.script>
	// - define:
	//   Defines the meaning for a token.
	//   Written: #define identifier value
	// - if:
	//   If but for preprocessor macros.
	//   Written: #if condition
	// - elif:
	//   Continuation of #if.
	//   Written: #elif condition
	// - else:
	//   Written: #else
	// - endif:
	//   Ends #if and #elif.
	//   Written: #endif
	// - INFO:
	//   Metadata for the executable.
	//   - TITLE:
	//     Sets the title of the window. (MOSTLY WINDOWS GUI)
	//   - USER:
	//     Sets which user/user type is needed to run the program.
	//     Options: "Root/Root" (SYS079 bootloader program only), "Root/Admin", "User/Any", "Background/ProgramUser"
	//   - MAIN:
	//     Sets the entry point.
	//   - RANDOM:
	//     Sets if extended randomization algorithms are used.
	//     Options: true, false
	//   - DEFINES_REQ:
	//     Sets which defines are needed to compile the program.
FILE* PreProcessor(FILE* fptr, COMPILER* compiler) {
    // Create/Reopen a temporary file for the pre-processed code
	FILE* preProcessedFile = fopen("./build/temp_preprocessed.pre", "w+");
	if(preProcessedFile == NULL) {
		printf("[ERROR] Could not create temporary pre-processed file.\n");
		return -1;
	}fptr;

    FILE* originalFile = compiler->fptr;

	// Find a pre-processor directive
	bool included_file = false;
	int c = fgetc(fptr);
	do {
		included_file = false;
		while(c != EOF) {
			if(c == (int)'#') {
				// Read directive
				char directive_buffer[33] = { 0 };
				directive_buffer[32] = '\0';
				c = fgetc(fptr);
				for(int i = 0; isalpha(c) && i < 32; i++) {
					if(c == EOF) {
						printf("[ERROR] Unexpected end of file in pre-processor directive.\n");
						return -1;
					}
					directive_buffer[i] = (char)c;
					c = fgetc(fptr);
				}

				if(strcmp(directive_buffer, "include") == 0) {
					// Include file
					included_file = true;
					// Skip whitespace
					while(c == (int)' ' || c == (int)'\t') {
						c = fgetc(fptr);
					}
				
					// Read file name
					char include_file_name[256] = { 0 };
					include_file_name[255] = '\0';
					int index = 0;
					if(c == (int)'"' || c == (int)'<') {
						int end_char = (c == (int)'"') ? (int)'"' : (int)'>';
						c = fgetc(fptr);
						while(c != end_char && c != EOF && index < 255) {
							include_file_name[index++] = (char)c;
							c = fgetc(fptr);
						}
						include_file_name[index] = '\0';
						if(c != end_char) {
							printf("[ERROR] Unterminated include file name.\n");
							return -1;
						}
					}
					else {
						printf("[ERROR] Invalid include directive format.\n");
						return -1;
					}

					// Open and read the include file
					FILE* include_file = fopen(include_file_name, "r");
					if(include_file == NULL) {
						printf("[ERROR] Could not open include file: %s\n", include_file_name);
						return -1;
					}
					int inc_c = fgetc(include_file);
					while(inc_c != EOF) {
						fputc(inc_c, preProcessedFile);
						inc_c = fgetc(include_file);
					}
					fclose(include_file);
				}
				else if(strcmp(directive_buffer, "define") == 0) {
					// Define macro
					// Skip whitespace
					while(c == (int)' ' || c == (int)'\t') {
						c = fgetc(fptr);
					}

					// Read identifier
					char define_identifier[64] = { 0 };
					define_identifier[63] = '\0';
					int index = 0;
					while((isalnum(c) || c == (int)'_') && c != EOF && index < 63) {
						define_identifier[index++] = (char)c;
						c = fgetc(fptr);
					}
					define_identifier[index] = '\0';
				
					// Skip whitespace
					while(c == (int)' ' || c == (int)'\t') {
						c = fgetc(fptr);
					}

					// Read value
					char define_value[256] = { 0 };
					define_value[255] = '\0';
					index = 0;
					while(c != EOF && index < 255) {
						define_value[index++] = (char)c;
						c = fgetc(fptr);
						if(c == (int)'\n') {
							if(define_value[index - 1] == '\\') {
								c = fgetc(fptr);
							}
							else {
								break;
							}
						}
					}
					define_value[index] = '\0';

					// Store the define
					if(compiler->current_define < compiler->MAX_DEFINES) {
						compiler->defines[compiler->current_define].name = strdup(define_identifier);
						compiler->defines[compiler->current_define].value = strdup(define_value);
						compiler->current_define++;
					}
					else {
						printf("[ERROR] Maximum number of defines reached.\n");
						return -1;
					}
				}
				else {
					printf("[ERROR] Unknown pre-processor directive: %s\n", directive_buffer);
					return -1;
				}
			}
			else {
				// Write character to pre-processed file
				fputc(c, preProcessedFile);
			}
			c = fgetc(fptr);
		}
		compiler->fptr = preProcessedFile;
	} while(included_file);
	fclose(originalFile);
	compiler->fptr = preProcessedFile;
	rewind(compiler->fptr);
	return 0;
}