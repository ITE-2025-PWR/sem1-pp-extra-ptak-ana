#include <stdio.h>
#include <stdlib.h>

#include"../ananas.h"

void menu(function_description_struct *function_array, int selector_func_count) {
	printf("Available functions: \n");
	for (int i = 0; i < selector_func_count; i++) {
		printf("%*s(), ",14, function_array[i].function_name);
		printf("(%s)(", function_type_enum_to_str(function_array[i].return_type));
		for (int j = 0; j < function_array[i].function_arg_count; ++j) {
			printf("%s ", function_type_enum_to_str(function_array[i].function_arg_list[j]));
		}
		printf(")\n");
	}
}

void clear(void) {
	system("clear");
}
void ana_exit(void) {
	exit(0);
}
int add(int b, int c) {
	return b + c;
}
int func_example(int b) {
	return b+2;
}