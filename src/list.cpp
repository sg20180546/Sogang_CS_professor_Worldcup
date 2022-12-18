#include "list.h"

void list_init(struct list_t* list) {
	list->head.next = &list->tail;
	list->head.prev = nullptr;
	list->tail.next = nullptr;
	list->tail.prev = &list->head;
}

struct list_elem* list_begin(struct list_t* list) {
	return list->head.next;
}
struct list_elem* list_end(struct list_t* list) {
	return &list->tail;
}
struct list_elem* list_next(struct list_elem* elem) {
	return elem->next;
}
void list_insert(struct list_elem*before, struct list_elem* elem) {
	elem->next = before;
	elem->prev = before->prev;
	before->prev->next = elem;
	before->prev = elem;
}
void list_push_back(struct list_t*list, struct list_elem*elem) {
	list_insert(list_end(list), elem);
}

void list_insert_ordered(struct list_t* list, struct list_elem* elem, list_less_func less) {
	struct list_elem * e;
	for (e = list_begin(list);e!=list_end(list) ; e = list_next(e)) {
		if (less(e, elem)) {
			break;
		}
	}
	list_insert( e,elem);
}