#include "list_expand.h"

Node *nodeWithInt(int m_int) {
	Node *p_node;
	int *p_int = (int *)malloc(sizeof(int));
    if(p_int == NULL){
        showError(pushError(INT, STANDARD, initInfo("nodeWithInt()", "Error in getting the memory of int.")));
        return NULL;
    }
	*p_int = m_int;
	p_node = initNode();
	initMallocValueForNode(p_node, INT, (void *)p_int);
	return p_node;
}

Node *nodeWithUInt(unsigned int m_uint){
    Node *p_node;
    unsigned int *pu_int = (unsigned int *)malloc(sizeof(unsigned int));
    if(pu_int == NULL){
        showError(pushError(INT, STANDARD, initInfo("nodeWithUInt()", "Error in getting the memory of int.")));
        return NULL;
    }
    *pu_int = m_uint;
    p_node = initNode();
    initMallocValueForNode(p_node, INT, (void *)pu_int);
    return p_node;
}

Node *nodeWithDouble(double m_double) {
	Node *p_node;
	double *p_double = (double *)malloc(sizeof(double));
    if(p_double == NULL){
        showError(pushError(DOUBLE, STANDARD, initInfo("nodeWithDouble()", "Error in getting the memory of double.")));
        return NULL;
    }
	*p_double = m_double;
	p_node = initNode();
	initMallocValueForNode(p_node, DOUBLE, (void *)p_double);
	return p_node;
}

Node *nodeWithString(const char *m_string) {
	Node *p_node;
	char *p_string = (char *)malloc(sizeof(char)*(strlen(m_string) + 1));
    if(p_string == NULL){
        showError(pushError(STRING, STANDARD, initInfo("initWithString()", "Error in getting the memory of string.")));
        return NULL;
    }
	strcpy(p_string, m_string);
	p_node = initNode();
	initMallocValueForNode(p_node, STRING, (void *)p_string);
	return p_node;
}

Node *nodeWithPointer(const void *m_pointer) {
	Node *p_node = initNode();
	initMallocValueForNode(p_node, POINTER, m_pointer);
	return p_node;
}

Node *nodeWithComplex(void) {
	Node *p_node = initNode();
	p_node->type = LIST;
	p_node->value = initList();
	p_node->if_malloc = 1;
	return p_node;
}

Node *findByIndexForNode(List *p_list, unsigned long long m_index) {
	Node *p_node = p_list->head;
	unsigned long long i;
	for (i = 0; i < m_index; i++) {
		p_node = p_node->next;
	}
	return p_node;
}

List *listThrough(List *p_list, List *(*p_func)(unsigned int, void *, List *), List *expand_resources) {
	Node *p_node = p_list->head;
    List *m_rtnlst = NULL;
	while (p_node != NULL) {
		if (p_node->if_malloc == 1) {
			List *m_rtnlst = (*p_func)(p_node->type, p_node->value, expand_resources);
            int status = getByIntForNode(findByIndexForNode(m_rtnlst, 0));
			if (status == -1) break;
			else if (status == 1) {
				p_node = p_node->last;
                releaseList(m_rtnlst);
                m_rtnlst = NULL;
				continue;
			}
			else {
                releaseList(m_rtnlst);
                m_rtnlst = NULL;
			}
		}
		p_node = p_node->next;
	}
	return m_rtnlst;
}

int getByIntForNode(Node *p_node) {
	if (p_node->type == INT) return *(int *)(p_node->value);
	else return -1;
}

unsigned int getByUIntForNode(Node *p_node){
    if (p_node->type == UINT) return *(unsigned int *)(p_node->value);
    else return -1;
}

char *getByStringForNode(Node *p_node) {
	if (p_node->type == STRING) return (char *)(p_node->value);
	else return NULL;
}

double getByDoubleForNode(Node *p_node) {
	if (p_node->type == DOUBLE) return *(double *)(p_node->value);
	else return -1;
}

void *getByPointerForNode(Node *p_node) {
	if (p_node->type == POINTER) return (void *)(p_node->value);
	else return NULL;
}

void printListInfo(List *p_list, int priority) {
	int i = 0;
	Node *p_node;
	for (i = 0; i < priority; i++) printf("   ");
	printf("###LIST(location:%p, id:%s){\n", p_list, s_idToASCIIString(p_list->s_id));
	for (i = 0; i < priority + 1; i++) printf("   ");
	printf("HEAD->%p / Tail->%p / Length:%llu\n", p_list->head, p_list->tail, p_list->length);
	p_node = p_list->head;
	while (p_node != NULL) {
		for (i = 0; i < priority + 1; i++) printf("   ");
		printf("%d.... \n", i);
		printNodeInfo(p_node, priority + 1);
		p_node = p_node->next;
		i++;
	}
	for (i = 0; i < priority; i++) printf("   ");
	printf("}\n");

}

void printList(List *p_list) {
	int if_nearLast = 0;
	Node *p_node = p_list->head;
	printf("[");
	while (p_node != NULL) {
		if (!if_nearLast && p_node->next == NULL) if_nearLast = 1;
		if (p_node->type == INT) {
			printf("%d", *(int *)(p_node->value));
		}
		else if (p_node->type == DOUBLE) {
			printf("%a", *(double *)(p_node->value));
		}
		else if (p_node->type == STRING) {
			printf("%s", (char *)(p_node->value));
		}
		else if (p_node->type == POINTER) {
			printf("%p", (char *)(p_node->value));
		}
		else if (p_node->type == LIST) {
			printList((List *)p_node->value);
		}
		if (!if_nearLast) {
			printf(", ");
		}
		p_node = p_node->next;
	}
	printf("]");
}

void printNodeInfo(Node *p_node, int priority) {
	int i;
	for (i = 0; i < priority; i++) printf("   ");
	printf("#NODE(location:%p, id:%s){\n", p_node, s_idToASCIIString(p_node->s_id));
	for (i = 0; i < priority + 1; i++) printf("   ");
	printf("NEXT->%p / LAST->%p / MALLOC:%d\n", p_node->next, p_node->last, p_node->if_malloc);
	if (p_node->type == INT) {
		for (i = 0; i < priority + 1; i++) printf("   ");
		printf("VALUE(int):%d\n", *(int *)(p_node->value));
	}
	else if (p_node->type == DOUBLE) {
		for (i = 0; i < priority + 1; i++) printf("   ");
		printf("VALUE(double):%a\n", *(double *)(p_node->value));
	}
	else if (p_node->type == STRING) {
		for (i = 0; i < priority + 1; i++) printf("   ");
		printf("VALUE(string):%s\n", (char *)(p_node->value));
	}
	else if (p_node->type == POINTER) {
		for (i = 0; i < priority + 1; i++) printf("   ");
		printf("VALUE(pointer):%s\n", (char *)(p_node->value));
	}
	else if (p_node->type == LIST) {
		for (i = 0; i < priority + 1; i++) printf("   ");
		printf("VALUE(List):\n");
		printListInfo((List *)p_node->value, priority + 2);
	}
	for (i = 0; i < priority; i++) printf("   ");
	printf("}\n");
}

void printNode(Node *p_node) {
	int i;
	printf("#NODE(location:%p, id:%s){\n", p_node, s_idToASCIIString(p_node->s_id));
	printf("   ");
	printf("NEXT->%p / LAST->%p\n", p_node->next, p_node->last);
	for (i = 0; i < 1; i++) printf("   ");
	printf("ifMalloc: ");
	if (p_node->if_malloc) {
		printf("YES\n");
		for (i = 0; i < 1; i++) printf("   ");
		printf("Value(type: %d): ", p_node->type);
		if (p_node->type == INT) {
			printf("%d", *(int *)(p_node->value));
		}
		else if (p_node->type == DOUBLE) {
			printf("%a\n", *(double *)(p_node->value));
		}
		else if (p_node->type == STRING) {
			printf("%s\n", (char *)(p_node->value));
		}
		else if (p_node->type == POINTER) {
			printf("%p\n", (char *)(p_node->value));
		}
		else if (p_node->type == LIST) {
			printList((List *)p_node->value);
		}
	}
	else printf("NO\n");
	
	printf("}\n");
}


Node *findByIntForNode(List *p_list, int target) {
	Node *t_node;
	int *p_target = (int *)malloc(sizeof(int));
	*p_target = target;
	t_node = findByValue(p_list, INT, p_target);
	free(p_target);
	return t_node;
}

Node *findByDoubleForNode(List *p_list, double target) {
	Node *t_node;
	double *p_target = (double *)malloc(sizeof(double));
	*p_target = target;
	t_node = findByValue(p_list, DOUBLE, p_target);
	free(p_target);
	return t_node;
}

Node *findByStringForNode(List *p_list, char *target) {
	Node *t_node;
	char *p_temp = (char *)malloc(sizeof(char)*(strlen(target) + 1));
	strcpy(p_temp, target);
	t_node = findByValue(p_list, STRING, p_temp);
	free(p_temp);
	return t_node;
}

Node *findByPointerForNode(List *p_list, void *target) {
	Node *t_node = findByValue(p_list, POINTER, target);
	return t_node;
}

int addValueForComplex(Node * p_node, int type, void *value) {
	List *c_list;
	Node *c_node;
	if (p_node->type == LIST) {
		c_list = (List *)p_node->value;
		c_node = initNode();
		initMallocValueForNode(c_node, type, value);
		insertInTail(c_list, c_node);
		return  0;
	}
	return  -1;
}

int addIntForComplex(Node *p_node, int temp) {
	if (p_node->type == LIST) {
		int *p_temp = (int *)malloc(sizeof(int));
        if(p_temp == NULL){
            showError(pushError(INT, STANDARD, initInfo("addIntForComplex()", "Error in getting the memory of int.")));
            return -1;
        }
		*p_temp = temp;
		addValueForComplex(p_node, INT, p_temp);
		return 0;
	}
	return -1;
}

int addDoubleForComplex(Node *p_node, double temp) {
	if (p_node->type == LIST) {
		double *p_temp = (double *)malloc(sizeof(double));
        if(p_temp == NULL){
            showError(pushError(DOUBLE, STANDARD, initInfo("addDoubleForComplex()", "Error in getting the memory of double.")));
            return -1;
        }
		*p_temp = temp;
		addValueForComplex(p_node, DOUBLE, p_temp);
		return 0;
	}
	return -1;
}

int addStringForComplex(Node *p_node, char *temp) {
    if (p_node->type == LIST) {
		char *p_temp = (char *)malloc(sizeof(strlen(temp) + 1));
        if(p_temp == NULL){
            showError(pushError(STRING, STANDARD, initInfo("addStringForComplex()", "Error in getting the memory of string.")));
            return -1;
        }
		strcpy(p_temp, temp);
		addValueForComplex(p_node, STRING, p_temp);
		return 0;
	}
	return -1;
}

int addPointerForComplex(Node *p_node, void *temp) {
	if (p_node->type == LIST) {
		addValueForComplex(p_node, POINTER, temp);
		return 0;
	}
	return -1;
}

List *m_findByInt(List* p_list, int temp) {
	int *p_temp = (int *)malloc(sizeof(int));
    if(p_temp == NULL){
        showError(pushError(INT, STANDARD, initInfo("m_findByInt()", "Error in getting the memory of int.")));
        return NULL;
    }
	List *t_list;
	*p_temp = temp;
	t_list = mply_findByValue(p_list, INT, (void *)p_temp);
	free(p_temp);
	return t_list;
}

List *m_findByDouble(List* p_list, double temp) {
	List *t_list;
	double *p_temp = (double *)malloc(sizeof(double));
    if(p_temp == NULL){
        showError(pushError(DOUBLE, STANDARD, initInfo("m_findByDouble()", "Error in getting the memory of double.")));
        return NULL;
    }
	*p_temp = temp;
	t_list = mply_findByValue(p_list, DOUBLE, (void *)p_temp);
	free(p_temp);
	return t_list;
}

List *m_findByString(List* p_list, char *temp) {
	List *t_list;
	char *p_temp = (char *)malloc(sizeof(char)*(strlen(temp) + 1));
    if(p_temp == NULL){
        showError(pushError(STRING, STANDARD, initInfo("m_findByString()", "Error in getting the memory of string.")));
        return NULL;
    }
	strcpy(p_temp, temp);
	t_list = mply_findByValue(p_list, STRING, (void *)p_temp);
	free(p_temp);
	return t_list;
}

List *m_findByPointer(List* p_list, void *temp) {
	List *t_list = mply_findByValue(p_list, DOUBLE, (void *)temp);
	return t_list;
}

unsigned long long getIndexByNode(List *p_list, Node *p_node) {
	Node *t_node = p_list->head;
	unsigned long long index = 0;
	while (t_node != NULL) {
		if (p_node->s_id == t_node->s_id) return index;
		index++;
		t_node = t_node->next;
	}
	return 0;
}

List *m_findByIntForNode(List* p_list, int temp) {
    int *p_temp = (int *)malloc(sizeof(int));
    if(p_temp == NULL){
        showError(pushError(INT, STANDARD, initInfo("m_findByIntForNode()", "Error in getting the memory of int.")));
        return NULL;
    }
	*p_temp = temp;
	return mply_findByValue(p_list, INT, (void *)p_temp);
}

List *m_findByDoubleForNode(List* p_list, double temp) {
	double *p_temp = (double *)malloc(sizeof(double));
    if(p_temp == NULL){
        showError(pushError(DOUBLE, STANDARD, initInfo("m_findByDoubleForNode()", "Error in getting the memory of double.")));
        return NULL;
    }
	*p_temp = temp;
	return mply_findByValue(p_list, DOUBLE, (void *)p_temp);
}

List *m_findByStringForNode(List* p_list, char *temp) {
	char *p_temp = (char *)malloc(sizeof(char) * (strlen(temp) + 1));
    if(p_temp == NULL){
        showError(pushError(STRING, STANDARD, initInfo("m_findByStringForNode()", "Error in getting the memory of string.")));
        return NULL;
    }
	strcpy(p_temp, temp);
	return mply_findByValue(p_list, STRING, (void *)p_temp);
}

List *m_findByPointerForNode(List* p_list, void *temp) {
	return mply_findByValue(p_list, POINTER, (void *)temp);
}

unsigned long long calListMemory(List * p_list){
    Node *p_node = p_list->head;
    unsigned long long nodes_size = 0LL;
    unsigned long long list_size = sizeof(p_list);
    while(p_node != NULL){
        nodes_size += sizeof(p_node);
        p_node = p_node->next;
    }
    return  list_size + nodes_size;
}

int updateValueWithInt(Node *p_node,int value){
    int *p_value = (int *)malloc(sizeof(int));
    if(p_value == NULL){
        showError(pushError(INT, STANDARD, initInfo("updateValueWithInt()", "Error in getting the memory of int.")));
        return -1;
    }
    *p_value = value;
    free(p_node->value);
    p_node->value = p_value;
    return 0;
}

int updateValueWithDouble(Node *p_node, double value){
    double *p_value = (double *)malloc(sizeof(double));
    if(p_value == NULL){
        showError(pushError(DOUBLE, STANDARD, initInfo("updateValueWithDouble()", "Error in getting the memory of double.")));
        return -1;
    }
    *p_value = value;
    free(p_node->value);
    p_node->value = p_value;
    return 0;
}

int updateValueWithString(Node *p_node, char *string){
    char *p_value = (char *)malloc(sizeof(strlen(string)) + 1);
    if(p_value == NULL){
        showError(pushError(STRING, STANDARD, initInfo("updateValueWithString()", "Error in getting the memory of string.")));
        return -1;
    }
    strcpy(p_value, string);
    free(p_node->value);
    p_node->value = p_value;
    return 0;
}

int updateValueWithPointer(Node *p_node, void *pointer){
    free(p_node->value);
    p_node->value = pointer;
    return 0;
}
