#ifndef MENUHANDLERS_H
#define MENUHANDLERS_H
#include "menu.h"

bool menuhandler_exit(State * state);

bool menuhandler_file_open(State * state);

bool menuhandler_file_rebuild(State * state);

bool menuhandler_file_close(State * state);

bool menuhandler_records_find(State * state);

bool menuhandler_records_add(State * state);

bool menuhandler_records_delete(State * state);

#endif