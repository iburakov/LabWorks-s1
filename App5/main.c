#include "globals.h"
#include "menu.h"
#include "menuhandlers.h"
#include "phonebook.h"

error_t ERROR = errNo;
char* ERRSTR = "";

int main() {
	State st;
	
	// INITIALIZING MENU
	if (!(st.menu = menu_alloc("PhoneBook collection operating terminal. Choose an option:", 0, NULL, NULL)) ||
			!menu_add_child(st.menu, "File: operations on .bin data storages.", 'f', NULL) ||
				!menu_add_child(st.menu->child,				"Open",		'o', &menuhandler_file_open) ||
				!menu_add_next(st.menu->child->child,		"Defrag",	'd', &menuhandler_file_rebuild) ||
				!menu_add_next(st.menu->child->child,		"Close",	'c', &menuhandler_file_close) ||
				!menu_add_next(st.menu->child->child,		"Back",		'b', &menuhandler_exit) ||
			!menu_add_child(st.menu, "Records: operations on records in a database.", 'r', NULL) ||
				!menu_add_child(st.menu->child->next,		"Add",		'a', menuhandler_records_add) ||
				!menu_add_next(st.menu->child->next->child,	"Find",		'f', menuhandler_records_find) ||
				!menu_add_next(st.menu->child->next->child,	"Delete",	'd', menuhandler_records_delete) ||
				!menu_add_next(st.menu->child->next->child,	"Back",		'b', &menuhandler_exit) ||
			!menu_add_child(st.menu, "Exit", 'e', &menuhandler_exit)
		) {
		fprintf(stderr, "Couldn't allocate memory for a menu");
		return EXIT_FAILURE;
	}
	while (TRUE) {
		if (!menu_deploy(st.menu, &st)) {
			switch (ERROR)
			{
			case errFatal: {
				printf("FATAL ERROR! %s\n", ERRSTR);
				return EXIT_FAILURE;
			} break;
			case errExit: {
				printf("%s\n", ERRSTR);
				return EXIT_SUCCESS;
			} break;
			case errTechnical: {
				printf("%s\n", ERRSTR);
				system("pause");
			} break;
			default: break;
			}
		}
		system("cls");
	}
}