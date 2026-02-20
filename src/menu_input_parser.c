#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>

#include"../ananas.h"

char *get_arg(char *user_null_buffer, int argn) {
	char *value = user_null_buffer + 1;
	char *p1, *p2;
	p1 = user_null_buffer;
	p2 = p1 + 1;
	for (int i = 0; i <= argn; ++i) {
		if (i > 0) {
			p1 = p2;
			p2 = p1 + 1;
		}
		while (!(*p1 == '\0' && *p2 != '\0')) {
			++p1;
			++p2;
		}
		value = p2;
	}
	return value;
}

c_data_type determine_data_type(char *str) {
	// 0xADD							< inferred pointer
	// 0x and ascii [0x30 ; 0x39] and [0x41; 0x47]
	// 5								< inferred int
	// ONLY [0x30; 0x39]
	// [allowed_matrix_operations]		< inferred matrix op
	// [0x41 ; 0x5A] and [0x61 ; 0x7A]
	char *p = str;
	while (*p++ != '\0') {
		// i hate this
	}
	return INVALID;
}

argument_package *function_selector_input_parser (char *user_input_buffer) {
	// whitespace or tabs are separators
	int argc = 0;
	char *p1 = user_input_buffer;
	char *p2 = p1 + 1;
	*p1 = '\0';
	// "tokenizer"
	while (!(*p1 == '\0' && *p2 == '\0')) {
		if (user_input_buffer[1] == '\n') break;
		if (*p1 != ' ' && (*p2 == ' ' || *p2 == '\n')){
			++argc;
			*p2 = '\0';
		}
		++p1;
		++p2;
	}
	// buf now is a null-separated list of args
	// printf("%s", get_arg(user_input_buffer, 2)); // second arg
	argument_package *arguments_pack = malloc(sizeof(argument_package));
	arguments_pack->arguments = NULL;
	arguments_pack->count = argc;
	if (argc != 0) {
		argument_definition *arguments = malloc(argc*sizeof(argument_definition));
		if (arguments == NULL) {
			free(arguments);
			exit(ENOMEM);
		}
		// move args from user_input_buffer to array of argument_definition structs
		// and determine their type
		char argument_value[16];
		for (int i = 0; i < argc; i++) {
			strcpy(argument_value, get_arg(user_input_buffer, i));
			arguments[i].type = determine_data_type(argument_value);
			strcpy(arguments[i].value, argument_value);
		}
		arguments_pack->arguments = arguments;
	}
	return arguments_pack;
}
