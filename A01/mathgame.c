/***************************************************
 * mathgame.c
 * Author: Nora Steil
 * Implements a math game
 */

#include <stdio.h>
#include<stdlib.h>

void game(int a) {
	int correct = 0;
	for (int i = 0; i < a; i++) {
		int num1 = rand() % 10;
		int num2 = rand() % 10;
		int userAns;
		printf("%d + %d = ? ", num1, num2);
		scanf("%d", &userAns);
		if (userAns == (num1 + num2)) {
			printf("Correct!\n");
			correct++;
		}
		else {
			printf("Incorrect.\n");
		}
	}
	printf("You correctly answered %d/%d\n", correct, a);
}

int main() {
	printf("Welcome to Math Game!\n");
	int a;
	printf("How many rounds do you want to play? ");
	scanf("%d", &a);
	game(a);
	return 0;
}
