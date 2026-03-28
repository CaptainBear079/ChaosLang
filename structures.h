#pragma once

#include <stdio.h>
#include <stdbool.h>

// Code file stages
typedef struct _File_ {
	FILE* fptr;
	char* fileName;
	char* extension;
} File;

typedef struct _Token_ {
	char* str;
	int length;
	int line;
	int column;
} Token;

typedef struct _ARG_LIST_ {
	int type;
	char* name;
} ARG_LIST;

typedef struct _IDENTIFIER_ {
	char* name;
	unsigned long long id;
} IDENTIFIER;

typedef struct _FUNCTION_ {
	char* name;
	unsigned long long id;
	ARG_LIST* args;
	IDENTIFIER* local_identifiers;
} FUNCTION;

typedef struct _DEFINE_ {
	char* name;
	char* value;
} DEFINE;

typedef struct _PCC__BOOL_ {
	bool global;                       // Indicates if the boolean is global or local (0 = local, 1 = global)
	char* identifier;                  // Name of the boolean
	char* asm_identifier;              // Name that will be used in assembly
	bool value;                        // Standard value of the boolean
	int group[2];                      // First index (0) saves in which byte group (every eight bool becomes a byte group) the bool is saved, second (1) saves which bit
	unsigned long long target_address; // Target address offset for runtime memory space, 0 = not set (gets generated before translation)
} PCC__BOOL;

typedef struct _PCC__INT_ {
	bool global;                       // Indicates if the integer is global or local (0 = local, 1 = global)
	int type;                          // Indicates the size of the integer (char, short, int, long int, long long int/long long)
	char* identifier;                  // Name of the integer
	char* asm_identifier;              // Name that will be used in assembly
	long long value;                   // Standard value of the integer
	unsigned long long target_address; // Target address offset for runtime memory space, 0 = not set (gets generated before translation)
} PCC__INT;

typedef struct _PCC__UINT_ {
	bool global;                       // Indicates if the integer is global or local (0 = local, 1 = global)
	int type;                          // Indicates the size of the integer (uchar, ushort, uint, ulong int, ulong long int/ulong long)
	char* identifier;                  // Name of the integer
	char* asm_identifier;              // Name that will be used in assembly
	unsigned long long value;          // Standard value of the integer
	unsigned long long target_address; // Target address offset for runtime memory space, 0 = not set (gets generated before translation)
} PCC__UINT;

typedef struct _PCC__FLOAT_ {
	bool global;                       // Indicates if the float is global or local (0 = local, 1 = global)
	int type;                          // Indicates the size of the float (float, double, long double)
	char* identifier;                  // Name of the float
	char* asm_identifier;              // Name that will be used in assembly
	long double value;                 // Standard value of the float
	unsigned long long target_address; // Target address offset for runtime memory space, 0 = not set (gets generated before translation)
} PCC__FLOAT;

typedef struct _PCC__UFLOAT_ {
	bool global;                       // Indicates if the float is global or local (0 = local, 1 = global)
	int type;                          // Indicates the size of the float (ufloat, udouble, ulong double)
	char* identifier;                  // Name of the float
	char* asm_identifier;              // Name that will be used in assembly
	long double value;                 // Standard value of the float
	unsigned long long target_address; // Target address offset for runtime memory space, 0 = not set (gets generated before translation)
} PCC__UFLOAT;

typedef struct _PCC__STRINGS_ {
	bool global;                       // Indicates if the char is global or local (0 = local, 1 = global)
	int type;                          // Indicates if string or single character
	char* identifier;                  // Name of the char
	char* asm_identifier;              // Name that will be used in assembly
	union _STRINGS_VALUE_ {                    // Standard value of the char
		char value;
		unsigned char uvalue;
		char* str_value;
	} STRINGS_VALUE;
	unsigned long long target_address; // Target address offset for runtime memory space, 0 = not set (gets generated before translation)
} PCC__STRINGS;

typedef struct _PCC__POINTER_ {
	bool global;                       // Indicates if the pointer is global or local (0 = local, 1 = global)
	bool array;                        // Indicates if the pointer is an array pointer.
	int type;                          // Indicates the type the pointer points to.
	char* identifier;                  // Name of the pointer
	char* asm_identifier;              // Name that will be used in assembly
	union _POINTER_VALUE_ { // Standard value of the pointer
		unsigned long long value;
		unsigned long long* array_values;
	} POINTER_VALUE;
	unsigned long long target_address; // Target address offset for runtime memory space, 0 = not set (gets generated before translation)
} PCC__POINTER;

typedef struct _PCC__CODE_BLOCK_ {
	int return_type;                   // Type of the return value
	int start_index;                   // Start index in pre-compiled code
	int end_index;                     // End index in pre-compiled code
	int function_index;                // Index of the function in the function table
	char* identifier;                  // Name of the function/code block
	char* asm_identifier;              // Name that will be used in assembly
	unsigned long long target_address; // Target address offset for runtime memory space, 0 = not set (gets generated before translation)
} PCC_CODE_BLOCK;

typedef struct _CODE_OBJECT_ {
	// Represents a line of code
	// Supported:
	//  - variable declaration
	//  - function declaration
	int type; // See CODE_OBJECT_TYPE enum
	union CODE_OBJECT_DATA_ {
		PCC__BOOL* _bool;
		PCC__INT* _int;
		PCC__UINT* _uint;
		PCC__FLOAT* _float;
		PCC__UFLOAT* _ufloat;
		PCC__STRINGS* _string;
		PCC__POINTER* _pointer;
		PCC_CODE_BLOCK* _code_block;
	} CODE_OBJECT_DATA;
} CODE_OBJECT;

typedef struct _COMPILER_ {
	// Flags
	int flags[3];                   // 0 = Interpretation path, 1 = Functions complexity level (0 = no functions, 1 = functions used), 2 = Current section (0 = source, 1 = script)
	bool bflags[4];                 // 0 = In-/Outside function (true = In-, false = Outside), 1 = Optimize and translate, 2 = First error flag, 3 = End not set
	bool bflagsArgs[3];             // 0 = Assemble Flag, 1 = List tokens (DEBBUG), 2 = Long return method (false = jump to end, true = delete stack frame and use 'ret')

	// Meta data
	int column, line;               // Position
	char* fName;                    // Name of compiled file
	FILE* fptr;                     // File
	int pcc_entries;

	// Changing data
	int token_start;                // Start column of the currently read token
	int current_function;           // The next free function entry
	int current_identifier;         // The next free identifier entry
	int current_define;             // The next free define entry
	int current_token_index;        // The next free token entry

	// Assembler meta data
	int assembler_length;           // Default: 4;
	char* ASSEMBLER;                // Default: "nasm"
	int current_assembler_flag;
	int* assembler_flags_length;
	char** assembler_flags;
	int temp_assembly_file_length;  // Default: 14
	char* temp_assembly_file;       // Default: "./build/ChaosLangCompiler/temp_asm.asm"

	// Limits
	int MAX_TOKENS;                 // Max tokens: default 1024
	int MAX_FUNCTIONS;              // Max functions: default 250
	int MAX_IDENTIFIERS;            // Max identifiers: default 450
	int MAX_ERRORS;                 // Max errors before terminating compiler: default 500
	int MAX_DEFINES;                // Max defines: default 250
	int MAX_ASM_ID;                 // Max assembly identifiers: default 700

	// Compilation data
	FILE* temp_script_file;         // Temporary script file for script section
	FILE* temp_assembly;            // Temporary assembly file for assembler output
	Token* tokens;
	FUNCTION* functions;
	IDENTIFIER* identifiers;
	DEFINE* defines;
	int* list_of_types;
	char* code_buffer;
	CODE_OBJECT* pre_compiled_code; // Pre-compiled code (Next translation and optimization)
	char** asm_identifier_list;     // All identifiers used in assembly
} COMPILER;