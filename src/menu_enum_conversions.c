#include <string.h>

#include"../ananas.h"


matrix_op_lookup_table allowed_matrix_operations_strenum_lookup_table [] = {
	{NONE, "NONE"},
	{ADD, "ADD"},
	{SUBTRACT, "SUBTRACT"},
	{MULTIPLY, "MULTIPLY"},
	{TRANSPOSE, "TRANSPOSE"}
};
data_type_lookup_table data_type_strenum_lookup_table [] = {
	{VOID, "void"},
	{PTR_VOID, "void*"},
	{INT, "int"},
	{PTR_INT, "int*"},
	{MATRIX, "allowed_matrix_operations"},
	{SIZE_T, "size_t"},
	{INVALID, "invalid"}
};


allowed_matrix_operations matrix_toolbox_str_to_enum(const char *str) {
	// converts matching string to value of allowed_matrix_operations,
	// returns NONE if none matched str
	for (int i = 0; i < sizeof(allowed_matrix_operations_strenum_lookup_table) / sizeof(allowed_matrix_operations_strenum_lookup_table[0]); ++i) {
		if (!strcmp(str, allowed_matrix_operations_strenum_lookup_table[i].str)) {
			return allowed_matrix_operations_strenum_lookup_table[i].value;
		}
	}
	return allowed_matrix_operations_strenum_lookup_table[0].value;
}

const char* function_type_enum_to_str(c_data_type type) {
	for (int i = 0; i < sizeof(data_type_strenum_lookup_table) / sizeof(data_type_strenum_lookup_table[0]); ++i) {
		if (type == data_type_strenum_lookup_table[i].value) {
			return data_type_strenum_lookup_table[i].str;
		}
	}
	return "";
}