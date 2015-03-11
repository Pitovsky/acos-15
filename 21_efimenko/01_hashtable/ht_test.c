#include"ht11.h"


int main(){
	Table* t1 = createTable(MAX_SIZE);
	Node* p = insertNode("aaaa", t1);
	p = findNode("aaaa", t1);
	deleteNode("aaaa", t1);
//	clearTable(t1);

	return 0;
}
