#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../ananas.h"

#define MAX_DISK_LENGTH 6
#define BASE_LENGTH (18+1)
#define BASE_MIDDLE_POINT ((BASE_LENGTH-1)/2)
#define DISK_CHARACTER '#'

typedef enum {
	REDRAW_BOARD, UPDATE_SELECTOR
} hanoi_draw_ui_mode;

typedef enum {
	LEFT_STACK, CENTER_STACK, RIGHT_STACK
} hanoi_selector_arrow_position;

typedef enum {
	WHITE, RED
} hanoi_cursor_colors;

void hanoi_draw_ui(int *left_int, int *center_int, int *right_int, hanoi_selector_arrow_position arrow_pos, int held) {
	// # - 1 (1)
	// ### - 2 (3)
	// ##### - 3 (5)
	// ####### - 4 (7)
	// ######### - 5 (9)
	// ########### - 6 (11)
	// @@@@@@@@@@@@@@@@@ - base 8 (17)
	system("clear");
	printf("a, d - move cursor\t\tspace bar - pick up/put down disk\t\to - re-gen board\t\tx - exit\n");
	char left[MAX_DISK_LENGTH][BASE_LENGTH] = {};
	char center[MAX_DISK_LENGTH][BASE_LENGTH] = {};
	char right[MAX_DISK_LENGTH][BASE_LENGTH] = {};
	char arrow_position[3];
	char arrow_held[3][BASE_LENGTH];
	char* p1, *p2;
	for (int i = 0; i < MAX_DISK_LENGTH /* no. of disks */; ++i) {
		memset(&left[i][0], 32, BASE_LENGTH-2);
		memset(&center[i][0], 32, BASE_LENGTH-2);
		memset(&right[i][0], 32, BASE_LENGTH-2);
		p1 = &left[i][BASE_MIDDLE_POINT-1];
		*p1 = '|';
		p2 = p1;
		for (int j = 0; j < left_int[i]; ++j) {
			*p1-- = DISK_CHARACTER;
			*p2++ = DISK_CHARACTER;
		}
		p1 = &center[i][BASE_MIDDLE_POINT-1];
		*p1 = '|';
		p2 = p1;
		for (int j = 0; j < center_int[i]; ++j) {
			*p1-- = DISK_CHARACTER;
			*p2++ = DISK_CHARACTER;
		}
		p1 = &right[i][BASE_MIDDLE_POINT-1];
		*p1 = '|';
		p2 = p1;
		for (int j = 0; j < right_int[i]; ++j) {
			*p1-- = DISK_CHARACTER;
			*p2++ = DISK_CHARACTER;
		}

	}
	// arrow
	memset(&arrow_position, 32, 3);
	memset(&arrow_held, 32, 3*BASE_LENGTH);
	arrow_position[arrow_pos] = 'V';
	p1 = &arrow_held[arrow_pos][BASE_MIDDLE_POINT-1];
	p2 = p1;
	for (int j = 0; j < held; ++j) {
		*p1-- = DISK_CHARACTER;
		*p2++ = DISK_CHARACTER;
	}
	for (int i = 0; i < 3; ++i) {
		arrow_held[i][BASE_LENGTH-1] = '\0';
	}
	left[MAX_DISK_LENGTH-1][BASE_LENGTH-1] = '\0';
	center[MAX_DISK_LENGTH-1][BASE_LENGTH-1] = '\0';
	right[MAX_DISK_LENGTH-1][BASE_LENGTH-1] = '\0';

	printf("\n");
	printf("        %c        \t\t        %c        \t\t        %c        \n", arrow_position[0], arrow_position[1], arrow_position[2]);
	printf("%s\t\t%s\t\t%s\n", arrow_held[0], arrow_held[1], arrow_held[2]);
	printf("\n");
	printf("        |        \t\t        |        \t\t        |        \n");
	for (int i = MAX_DISK_LENGTH-1; i >= 0; --i) {
		printf("%s\t\t%s\t\t%s\n", left[i], center[i], right[i]);
	}
	printf("@@@@@@@@@@@@@@@@@\t\t@@@@@@@@@@@@@@@@@\t\t@@@@@@@@@@@@@@@@@");
}

int hanoi_validate_stack(int *stack, int stck_h) {
	for (int i = 1; i < stck_h; ++i) {
		if (stack[i] != 0) {
			if (stack[i-1] <= stack[i]) return 0;
			continue;
		}
		return 1;
	}
	return 1;
}

void hanoi() {
	srand(time(NULL));
	int left[MAX_DISK_LENGTH] = {2, 1, 0, 0, 0, 0};
	int left_height = 2;
	int center[MAX_DISK_LENGTH] = {6, 5, 0, 0, 0, 0};
	int center_height = 2;
	int right[MAX_DISK_LENGTH] = {4, 3, 0, 0, 0, 0};
	int right_height = 2;
	int *stacks[3] = {&left[0], &center[0], &right[0]};
	int *stacks_height[3] = {&left_height, &center_height, &right_height};
	hanoi_selector_arrow_position arrow_position = 0;
	int held = 0;
	hanoi_draw_ui(left, center, right, arrow_position, held);
	int key;
	while (1) {
		assert(hanoi_validate_stack(left, MAX_DISK_LENGTH) && "left stack invalid");
		assert(hanoi_validate_stack(center, MAX_DISK_LENGTH) && "middle stack invalid");
		assert(hanoi_validate_stack(right, MAX_DISK_LENGTH) && "right stack invalid");
		// works on macos and *nix
		system("stty raw");
		key = getchar();
		system("stty sane");
		// arrow right
		if (key == 'a') {
			if (arrow_position > LEFT_STACK) {
				arrow_position--;
			}
			else {
				arrow_position = RIGHT_STACK;
			}
		}
		// arrow left
		if (key == 'd') {
			if (arrow_position < RIGHT_STACK) {
				arrow_position++;
			}
			else {
				arrow_position = LEFT_STACK;
			}
		}
		// exit
		if (key == 'x') {
			system("clear");
			return;
		}
		// moving logic
		if (key == ' ' || key == 'w') {
			// pick up
			if (held == 0) {
				if (*stacks_height[arrow_position] != 0) {
					held = *(stacks[arrow_position] + *stacks_height[arrow_position] - 1);
					*(stacks[arrow_position] + *stacks_height[arrow_position] - 1) = 0;
					*stacks_height[arrow_position] = *stacks_height[arrow_position] - 1;
					hanoi_draw_ui(left, center, right, arrow_position, held);
					continue;
				}
			}
			// put down
			if (held != 0) {
				if (*stacks_height[arrow_position] != 0) {
					if (held > *(stacks[arrow_position] + *stacks_height[arrow_position] - 1)) {
						continue;
					}
				}
				*(stacks[arrow_position] + *stacks_height[arrow_position]) = held;
				held = 0;
				*stacks_height[arrow_position] = *stacks_height[arrow_position] + 1;
				hanoi_draw_ui(left, center, right, arrow_position, held);
				continue;
			}
		}
		// re-generate board
		if (key == 'o') {
			held = 0;
			memset(left, 0, MAX_DISK_LENGTH*sizeof(int));
			left_height = 0;
			memset(center, 0, MAX_DISK_LENGTH*sizeof(int));
			center_height = 0;
			memset(right, 0, MAX_DISK_LENGTH*sizeof(int));
			right_height = 0;
			int select;
			for (int i = MAX_DISK_LENGTH; i >= 0; --i) {
				select = rand() % 3;
				*(stacks[select] + *stacks_height[select]) = i;
				*stacks_height[select] += 1;
			}
		}
		hanoi_draw_ui(left, center, right, arrow_position, held);
	}
}