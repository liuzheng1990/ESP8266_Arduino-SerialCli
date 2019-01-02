/* Neutrino Arduino Command line system

 Thales Liu

 Change Log: 

 2018/11/24: 1. Every task can have an init part and a runtime part.
             2. Each command can take argument and task func can handle arguments.

*/


#ifndef NEUTRINO_H_INNER
#define NEUTRINO_H_INNER


#include <Arduino.h>
#define MAX_COMMANDS 50

using cbfunc_t = uint8_t (*)();
using cbfunc_arg_t = uint8_t (*)(int, String *);


class Neutrino
{

private:
	String commands_list[MAX_COMMANDS]; // for now, let's just use a list of 10 commands.
	bool auto_help;
	cbfunc_t init_callbacks[MAX_COMMANDS] = {NULL};
	cbfunc_t runtime_callbacks[MAX_COMMANDS] = {NULL};
	cbfunc_arg_t runtime_callbacks_arg[MAX_COMMANDS] = {NULL};
	size_t number_commands = 0;

	int _search_command(const String & command);

public:
	Neutrino(bool add_help=false);
	uint8_t print_help_info();
	bool add_command(const String & command, cbfunc_t runtime_cb, cbfunc_t init_cb = NULL);
	bool add_command(const String & command, cbfunc_arg_t runtime_cb_arg, cbfunc_t init_cb = NULL);
	bool init_commands();
	uint8_t parse_command(String command);

};



#endif // NEUTRINO_H
