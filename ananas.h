//
// Created by ana on 06/12/2025.
//
#ifndef EXTRA_ANANAS_MATRIX_H
#define EXTRA_ANANAS_MATRIX_H

#define CUR_FUNC_IDX function_array[i]
#define ARG_PARSER_BUF_SIZE 64 // function selector input buffer size
#define MTX_INPUT_BFR_SZ 80 // matrix input buffer size

typedef enum {
	NONE, ADD, SUBTRACT, MULTIPLY, TRANSPOSE
} allowed_matrix_operations;

typedef enum {
	INVALID, VOID, PTR_VOID, INT, PTR_INT, MATRIX, SIZE_T, FRACTAL, FUNCTION_DESC_STRC
} c_data_type;

typedef const struct {
	c_data_type value;
	const char *str;
} data_type_lookup_table;

typedef const struct {
	allowed_matrix_operations value;
	const char *str;
} matrix_op_lookup_table;

typedef struct {
	c_data_type type;
	char value[16];
} argument_definition;

typedef struct {
	argument_definition *arguments;
	int count;
} argument_package;

typedef struct {
	char function_name[20];
	void *function_ptr;
	int function_arg_count;
	c_data_type function_arg_list[3];
	char function_arg_description[3][20];
	c_data_type return_type;
} function_description_struct;

void matrix_toolbox(allowed_matrix_operations);
void life();
void hanoi();
char *get_arg(char*, int);
c_data_type determine_data_type(char*);
argument_package *function_selector_input_parser (char *);
allowed_matrix_operations matrix_toolbox_str_to_enum(const char*);
const char* function_type_enum_to_str(c_data_type);
void menu(function_description_struct*, int);
void clear(void);
void ana_exit(void);
int func_example(int);
int add(int, int);

#endif //EXTRA_ANANAS_MATRIX_H