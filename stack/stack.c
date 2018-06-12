#include "./stack_expand.h"
int stack(void) {
	Stack *t_stack = initStack();
	for (int i = 0; i < 10; i++) {
		pushStack(t_stack, snodeWithInt(i));
	}
	for (int i = 0; i < 10; i++) {
		printf("%d", getValueByIntForSNode(popStack(t_stack)));
	}
	releaseStack(t_stack);
	return 0;
}
