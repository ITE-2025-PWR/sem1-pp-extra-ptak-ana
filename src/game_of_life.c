#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<unistd.h>
#include "../ananas.h"
#include<signal.h>

enum states {
	DEAD, ALIVE
};

void display_board(int *board, int rows, int columns, int generation){
	system("clear");
	printf("GEN: %d\n", generation);
	int output_index = 0;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			board[output_index] == ALIVE ? printf("O ") && output_index++ : 0;
			board[output_index] == DEAD ? printf("  ") && output_index++ : 0;
		}
		printf("\n");
	}
}
int keep_working = 1;
void sigint_handler(int sig) {
	keep_working = 0;
}

void looping_one(int *board, int rows, int columns) {
	int *one_position = &board[rows*columns/2];
	int *previous_one_position = one_position - 1;
	if (one_position == NULL) {
		free(one_position); free(previous_one_position); return;
	}
	unsigned int generation = 0;
	int key;
	while (1 && keep_working) {
		while (one_position < &board[rows*columns] && keep_working) {
			signal(SIGINT, sigint_handler);
			*one_position = ALIVE;
			*previous_one_position = DEAD;
			previous_one_position = one_position;
			one_position++;
			display_board(board, rows, columns, ++generation);
			printf("cur: %p\n", one_position);
			printf("prev: %p\n", previous_one_position);
			usleep(50000);
		}
		one_position = board;
	}
	system("clear");
}

int *create_board(int rows, int columns) {
	int *board = calloc(rows*columns, sizeof(int));
	return board;
}

void life() {
	keep_working = 1;
	const int rows = 1;
	const int columns = 50;
	int *board = create_board(rows, columns);
	looping_one(board, rows, columns);
	free(board);
}