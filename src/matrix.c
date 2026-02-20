#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "../ananas.h"

// begin utility
void display(int *a, int ln) {
	printf("{");
	for (int i = 0; i < ln; i++) {
		if (i != ln - 1) {
			printf("%d, ", *(a+i));
		}
		else {
			printf("%d", *(a+i));
		}
	}
	printf("}");
}

void matrix_display(int* input_matrix, char title) {
	int columns = *(input_matrix + 1);
	int rows = *(input_matrix + 2);
	int output_index = 4;
	printf("%c:\n", title);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			printf("%d\t", *(input_matrix + output_index++));
		}
		printf("\n\n");
	}
	printf("\n");

}

// end utility

int* matrix_parser(const char title) {
	// max. 7 digits per matrix element
	// no more than 15 columns and 30 elements
	int rows = 0;
	int inferred_cols;
	int output_cols = 0;
	char* user_input_buffer = malloc(MTX_INPUT_BFR_SZ);
	char* read_element = malloc(8);
	int read_element_digit_counter = 0;
	int is_reading_number_from_buffer = 0;
	int is_first_run = 1;
	int* row_output_array = malloc(15*sizeof(int));
	int row_output_array_position = 0;
	int function_output_array_size = 34;
	int* function_output_array = malloc((function_output_array_size) * sizeof(int)); // for now assume we are inputting less than 30 elements
	if (user_input_buffer == NULL || read_element == NULL || row_output_array == NULL || function_output_array == NULL) {
		exit(1);
	}
	int function_output_array_position = 0;
	*function_output_array = function_output_array_size+4;
	memset(user_input_buffer, 1, MTX_INPUT_BFR_SZ);
	memset(read_element, 0, 8);

	// prompt
	printf("%c:\t\t(int [tab] int [tab] ... ENTER)\n", title);
	printf("\"g\" after a new line to finish\n");
	//
	int i = 0;
	while (fgets(user_input_buffer, MTX_INPUT_BFR_SZ, stdin)) {
		if (user_input_buffer[0] == 'g') break;
		inferred_cols = 0;
		i = 0;
		row_output_array_position = 0;
		while (user_input_buffer[i] != '\0') {
			assert(user_input_buffer[0] != '\t' && user_input_buffer[0] != '\n');
			// if encountered digit start reading multiple char 'digits' of whole number
			if ((user_input_buffer[i] >= 0x30 && user_input_buffer[i] <= 0x39) || user_input_buffer[i] == '-') {
				if (is_reading_number_from_buffer == 0) {
					inferred_cols++;
					read_element_digit_counter = 0;
				}
				is_reading_number_from_buffer = 1;
				*(read_element + read_element_digit_counter) = *(user_input_buffer+i);
				read_element_digit_counter++;
			}
			// stop reading number after encountering a whitespace, \t or \n
			if (user_input_buffer[i] == 0x9 || user_input_buffer[i] == 0x20 || user_input_buffer[i] == '\n') {
				is_reading_number_from_buffer = 0;
				assert(inferred_cols != 0 && row_output_array_position < 15);
				row_output_array[row_output_array_position++] = (int)strtol(read_element, read_element+read_element_digit_counter, 10);
				// printf("%ld ", n);
				memset(read_element, 0, 8); // 8 * sizeof(char)
			}
			i++;
		}
		// finished reading row
		//
		if (is_first_run == 1) {
			is_first_run = 0;
			output_cols = inferred_cols;
		}
		// debug: printf("inf: %d\n out: %d\n", inferred_cols, output_cols);
		if (output_cols != inferred_cols){
			// discard current input
			memset(row_output_array, 0, row_output_array_position*sizeof(int));
			printf("mismatch with 1st row column count, pls go again\n");
			continue;
		}
		rows++;
		for (int j = 0; j < inferred_cols; j++) {
			*(function_output_array + function_output_array_position++ + 4) = row_output_array[j];
		}
		printf("\n");
	}
	// got 'g'
	*(function_output_array + 1) = output_cols;
	*(function_output_array + 2) = rows;
	free(user_input_buffer);
	free(read_element);
	free(row_output_array);
	return function_output_array;	// [0] = size
									// [1] = columns
									// [2] = rows
									// [3] = reserved
									// all appending operations need a +4
}

int matrix_is_matrix_pair_equal_dimensions(int* a, int* b) {
	for (int i = 0; i < 4; i++) {
		if (*(a + i) != *(b + i)) return 0;
	}
	return 1;
}

int matrix_is_matrix_pair_multipliable(int *a, int* b) {
	if (*(a+1) == *(b+2)) return 1;
	return 0;
}

int* matrix_multiply(int *a, int *b) {
	int a_columns = *(a + 1);
	int a_rows = *(a + 2);
	int b_columns = *(b + 1);
	int *a_intermediate = malloc(*a * sizeof(int));
	int *b_intermediate = malloc(*b * sizeof(int));
	if (a_intermediate == NULL || b_intermediate == NULL) {
		exit(1);
	}
	int *c_output = malloc(*a * sizeof(int));
	int a_intermediate_position = 0;
	int b_intermediate_position = 0;
	int c_output_position = 4;
	*c_output = *a;	// size
	*(c_output + 1) = *(b + 1); // columns
	*(c_output + 2) = *(a + 2); // rows
	// read row of A b_columns times, skip to the next
	for (int i = 0; i < a_rows; i++) {
		for (int j = 0; j < b_columns; j++) {
			// *(b_intermediate + b_intermediate_position++) = *(b + 4 + j);
			// *(b_intermediate + b_intermediate_position++) = *(b + 4 + j+b_columns);
			// this should work...
			for (int k = 0; k < a_columns; k++) {
				*(a_intermediate + a_intermediate_position++) = *(a + 4 + k + i*a_columns);
			}
		}
	}
	// read B by column top down, left to right a_rows times
	for (int j = 0; j < a_rows; j++) {
		for (int k = 0; k < b_columns; k++) {
				*(b_intermediate + b_intermediate_position++) = *(b + 4 + k);
				*(b_intermediate + b_intermediate_position++) = *(b + 4 + k+b_columns);
		}
	}
	// display(a_intermediate, a_intermediate_position);
	// printf("\n");
	// display(b_intermediate, b_intermediate_position);
	// multiply each intermediate element, add [0] and [1], [1] and [2] ...,  display as a_rows x b_columns matrix
	for (int i = 0; i < a_intermediate_position; i += 2) {
		*(c_output + c_output_position++) = *(a_intermediate + i) * *(b_intermediate + i) +
											*(a_intermediate + i + 1) * *(b_intermediate + i + 1);
	}

	free(a_intermediate);
	free(b_intermediate);
	return c_output;
}

int* matrix_transpose(int *a) {
	int *c_output = malloc(*a * sizeof(int));
	*c_output = *a;
	*(c_output + 1) = *(a + 2);
	*(c_output + 2) = *(a + 1);
	*(c_output + 3) = *(a + 3);
	int c_output_position = 4;
	// *(a+1)	// column count
	// *(a+2)	// row count
	// simple enough i guess
	for (int j = 0; j < *(a+1); j++) {
		for (int i = 0; i < *(a+2); i++) {
			*(c_output + c_output_position++) = *(a + 4 + j + i * *(a + 2));
		}
	}
	return c_output;
}

int* matrix_initialize_with_template(int *a) {
	int *c_output = malloc(*a*sizeof(int));
	for (int i = 0; i < 4; i++) {
		*(c_output + i) = *(a + i);
	}
	return c_output;
}

int* matrix_add(int *a, int *b) {
	int elements = *(a+1) * *(a+2);
	int *c_output = matrix_initialize_with_template(a);
	for (int i = 0; i < elements; i++) {
		*(c_output + i + 4) = *(a + i + 4) + *(b + i + 4);
	}
	return c_output;
}

int* matrix_subtract(int *a, int *b) {
	int elements = *(a+1) * *(a+2);
	int *c_output = matrix_initialize_with_template(a);
	for (int i = 0; i < elements; i++) {
		*(c_output + i + 4) = *(a + i + 4) - *(b + i + 4);
	}
	return c_output;
}

void matrix_toolbox(allowed_matrix_operations mode){
	if (mode == NONE) return;
	int *a = matrix_parser('A');
	int *b = NULL;
	int *c = NULL;
	if (mode != TRANSPOSE) {
		b = matrix_parser('B');
		if (mode == ADD || mode == SUBTRACT) {
			if (!matrix_is_matrix_pair_equal_dimensions(a, b)) {
				free(a);
				free(b);
				free(c);
				return;
			}
		}
	}
	if (mode == ADD) {
		c = matrix_add(a, b);
		matrix_display(c, 'C');
	}
	if (mode == SUBTRACT) {
		c = matrix_subtract(a, b);
		matrix_display(c, 'C');
	}
	if (mode == MULTIPLY) {
		if (!matrix_is_matrix_pair_multipliable(a, b)) {
			free(a);
			free(b);
			free(c);
			return;
		}
		c = matrix_multiply(a, b);
		matrix_display(c, 'C');
	}
	if (mode == TRANSPOSE) {
		c = matrix_transpose(a);
		matrix_display(c, 'T');
	}
	free(a);
	free(b);
	free(c);
}