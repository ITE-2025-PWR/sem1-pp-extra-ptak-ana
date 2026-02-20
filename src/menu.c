#include<assert.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/errno.h>
#include "../ananas.h"

void function_selector_init_array(function_description_struct *function_array) {
	// matrix_toolbox()
	strncpy(function_array[0].function_name, "matrix_toolbox", strlen("matrix_toolbox"));
	function_array[0].function_ptr = &matrix_toolbox;
	function_array[0].function_arg_count = 1;
	function_array[0].function_arg_list[0] = MATRIX;
	function_array[0].return_type = VOID;

	// life()
	strncpy(function_array[1].function_name, "life", strlen("life"));
	function_array[1].function_ptr = &life;
	function_array[1].function_arg_count = 1;
	function_array[1].function_arg_list[0] = VOID;
	function_array[1].return_type = VOID;

	// func_example()
	strncpy(function_array[2].function_name, "func_example", strlen("func_example"));
	function_array[2].function_ptr = &func_example;
	function_array[2].function_arg_count = 1;
	function_array[2].function_arg_list[0] = INT;
	function_array[2].return_type = INT;

	// malloc()
	strncpy(function_array[3].function_name, "malloc", strlen("malloc"));
	function_array[3].function_ptr = &malloc;
	function_array[3].function_arg_count = 1;
	function_array[3].function_arg_list[0] = SIZE_T;
	function_array[3].return_type = PTR_INT;

	// func_example_2()
	strncpy(function_array[4].function_name, "add", strlen("add"));
	function_array[4].function_ptr = &add;
	function_array[4].function_arg_count = 2;
	function_array[4].function_arg_list[0] = INT;
	function_array[4].function_arg_list[1] = INT;
	function_array[4].return_type = INT;

	// exit()
	strncpy(function_array[5].function_name, "stdlib_exit", strlen("stdlib_exit"));
	function_array[5].function_ptr = &exit;
	function_array[5].function_arg_count = 1;
	function_array[5].function_arg_list[0] = INT;
	function_array[5].return_type = VOID;

	// hanoi()
	strncpy(function_array[6].function_name, "hanoi", strlen("hanoi"));
	function_array[6].function_ptr = &hanoi;
	function_array[6].function_arg_count = 1;
	function_array[6].function_arg_list[0] = VOID;
	function_array[6].return_type = VOID;

	strncpy(function_array[7].function_name, "?", strlen("?"));
	function_array[7].function_ptr = &menu;
	function_array[7].function_arg_count = 1;
	function_array[7].function_arg_list[0] = VOID;
	function_array[7].return_type = VOID;

	strncpy(function_array[8].function_name, "clear", strlen("clear"));
	function_array[8].function_ptr = &clear;
	function_array[8].function_arg_count = 1;
	function_array[8].function_arg_list[0] = VOID;
	function_array[8].return_type = VOID;

	strncpy(function_array[9].function_name, "exit", strlen("exit"));
	function_array[9].function_ptr = &ana_exit;
	function_array[9].function_arg_count = 1;
	function_array[9].function_arg_list[0] = VOID;
	function_array[9].return_type = VOID;

}

void shell_sig_handler(int sig) {
	ana_exit();
}

void mock_shell() {
	const int selector_func_count = 10;
	char *user_input_buffer = malloc(ARG_PARSER_BUF_SIZE);
	argument_package *arguments_pack;
	int argc;
	argument_definition *argv;
	function_description_struct *function_array = malloc(selector_func_count*sizeof(function_description_struct));
	function_selector_init_array(function_array);
	for (;;) {
		signal(SIGINT, shell_sig_handler);
		printf(">> ");
		fgets(user_input_buffer+1, ARG_PARSER_BUF_SIZE-1, stdin);
		arguments_pack = function_selector_input_parser(user_input_buffer);
		argc = arguments_pack->count;
		argv = arguments_pack->arguments;
		if (argc == 0) continue;
		for (int i = 0; i < selector_func_count; ++i) {
			if (!strcmp(argv[0].value, CUR_FUNC_IDX.function_name)) {
				//
				if (CUR_FUNC_IDX.function_ptr == &menu) {
					((void(*)(function_description_struct*, int))CUR_FUNC_IDX.function_ptr)(function_array, selector_func_count);
					continue;
				}
				//
				if (CUR_FUNC_IDX.function_arg_count		== 1 &&
					argc								== 1 &
					CUR_FUNC_IDX.return_type			== VOID) {
					((void(*)(void))CUR_FUNC_IDX.function_ptr)();
				}
				//
				if (CUR_FUNC_IDX.function_arg_count == 1 && argc == 2) {
					//
					if (CUR_FUNC_IDX.return_type			== INT &&
						CUR_FUNC_IDX.function_arg_list[0]	== INT) {
						printf("Output of %s: %d\n", CUR_FUNC_IDX.function_name, ((int(*)(int))CUR_FUNC_IDX.function_ptr)(atoi(argv[1].value)));
					}
					if (CUR_FUNC_IDX.return_type			== VOID &&
						CUR_FUNC_IDX.function_arg_list[0]	== INT) {
						((void(*)(int))CUR_FUNC_IDX.function_ptr)(atoi(argv[1].value));
					}
					if (CUR_FUNC_IDX.return_type			== VOID &&
						CUR_FUNC_IDX.function_arg_list[0]	== MATRIX) {
						((void(*)(allowed_matrix_operations))CUR_FUNC_IDX.function_ptr)(matrix_toolbox_str_to_enum(argv[1].value));

						}
				}
				//
				if (CUR_FUNC_IDX.function_arg_count == 2 && argc == 3) {
					//
					if (CUR_FUNC_IDX.return_type			== INT &&
						CUR_FUNC_IDX.function_arg_list[0]	== INT &&
						CUR_FUNC_IDX.function_arg_list[1]	== INT) {
						printf("Output of %s: %d\n", CUR_FUNC_IDX.function_name,
						((int(*)(int, int))CUR_FUNC_IDX.function_ptr)(atoi(argv[1].value), atoi(argv[2].value)));

					}
				}
			}
		}
	}
}

int main(void) {
	mock_shell();
}


