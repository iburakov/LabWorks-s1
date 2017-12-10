#ifndef MENU_H
#define MENU_H
#include "globals.h"
#include "input.h"
#include "phonebook.h"

struct State;

typedef bool(*handlerp_t)(struct State*);

typedef struct MenuEntry {
	char* text;
	char key;
	handlerp_t handler;
	struct MenuEntry* parent;
	struct MenuEntry* child;
	struct MenuEntry* next;
} MenuEntry;

typedef struct State {
	MenuEntry* menu;
	BinFile* bfp;
} State;


/*
	Allocates memory for MenuEntry and initializes it.

	@return		NULL on failure, otherwise a pointer to the new MenuEntry.
*/
MenuEntry* menu_alloc(char* text, char key, handlerp_t handler, MenuEntry* parent);

/*
	Gets to the last sibling of a parent's child if any, calls menu_alloc(), checks for error and 
	writes a pointer to the new constructed MenuEntry to lastsibling->next or parent->child.
*/
bool menu_add_child(MenuEntry* parent, char* text, char key, handlerp_t handler);

/*
	Gets to the last sibling of a target, calls menu_alloc(), checks for error and writes a pointer to the new constructed MenuEntry
	to lastsibling->next.
*/
bool menu_add_next(MenuEntry* target, char* text, char key, handlerp_t handler);

/*
	Prints itself, then a child if exists and the next sibling recursively.

	@param	level		Default: 0
*/
bool menu_print(MenuEntry* me, int level);

/*
	Gets a MenuEntry to handle a key over its childs.
*/
bool menu_handle(MenuEntry * me, State * state, char key);

/*
	Searches for a sibling MenuEntry me with corresponding key recursively and run its handler passing given state pointer.
*/
bool menu_handle_siblings(MenuEntry* me, State* state, char key);

/*
	Prints a menu, waits for user input and gets menu to handle the key.
*/
bool menu_deploy(MenuEntry* me, State* state);



#endif 
