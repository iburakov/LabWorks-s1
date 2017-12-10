#include "menu.h"

MenuEntry* menu_alloc(char * text, char key, handlerp_t handler, MenuEntry* parent){
	MenuEntry* newme = (MenuEntry*)malloc(sizeof(MenuEntry));
	if (!newme) {
		assert(FALSE);
		ERROR = errFatal;
		ERRSTR = "Could not allocate memory for new MenuEntry!";
		return NULL;
	}

	newme->text = text;
	newme->key = key;
	newme->handler = handler;
	newme->parent = parent;
	newme->child = NULL;
	newme->next = NULL;
	return newme;
}

bool menu_add_child(MenuEntry * parent, char * text, char key, handlerp_t handler) {
	if (parent->child) return menu_add_next(parent->child, text, key, handler);

	parent->child = menu_alloc(text, key, handler, parent);
	if (parent->child) return SUCCESS;
	else return FAILURE;
}

bool menu_add_next(MenuEntry * target, char * text, char key, handlerp_t handler) {
	while (target->next) target = target->next;
	target->next = menu_alloc(text, key, handler, target->parent);
	if (target->next) return SUCCESS;
	else return FAILURE;
}

bool menu_print(MenuEntry* me, int level) {
	if (!me->key || !me->text) {
		assert(FALSE);
		ERROR = errFatal;
		ERRSTR = "Attempted to print corrupted MenuEntry.";
		return FAILURE;
	}

	for (int i = 0; i < level; ++i) printf("\t");
	printf("%c - %s\n", me->key, me->text);
	//	if (me->child)	menu_print(me->child, level + 1);
	if (me->next)	menu_print(me->next, level);
	return SUCCESS;
}

bool menu_handle(MenuEntry* me, State* state, char key) {
	if (!me->child) {
		assert(FALSE);
		ERROR = errFatal;
		ERRSTR = "Attempted to handle a key for a leaf MenuEntry (no childs) as a parent";
		return FAILURE;
	}

	return menu_handle_siblings(me->child, state, key);
}

bool menu_handle_siblings(MenuEntry * me, State * state, char key) {
	if (!me->key) {
		assert(FALSE);
		ERROR = errFatal;
		ERRSTR = "Attempted to handle a key for a corrupted MenuEntry";
		return FAILURE;
	}

	if (me->key == key) {
		if (me->child) {
			state->menu = me;
			return SUCCESS;
		}
		else {
			return me->handler(state);
		}
	}

	if (me->next) return menu_handle_siblings(me->next, state, key);

	// haven't found a sibling yet and there's no next sibling, so this key can't be handled in me->parent menu
	ERROR = errTechnical;
	ERRSTR = "Invalid input.";
	return FAILURE;
}

bool menu_deploy(MenuEntry * me, State * state) {
	printf("%s\n", me->text);
	menu_print(me->child, 1);

	char key;
	char next;
	if ((key = getchar_after_spaces()) != EOF && key != '\n' && (next = getchar_after_spaces()) == '\n') {
		return menu_handle(me, state, key);
	} 

	// wrong input
	if (key != '\n') {
		ungetc(next, stdin);
		cleanbuf();
	}	
	ERROR = errTechnical;
	ERRSTR = "Invalid input.";
	return FAILURE;
}
