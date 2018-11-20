#ifndef NEUTRINO_H
#define NEUTRINO_H


#include <Arduino.h>
#define MAX_COMMANDS 10


class Neutrino
{

private:
	String commands_list[MAX_COMMANDS]; // for now, let's just use a list of 10 commands.
	bool auto_help;
	uint8_t (*callback_funcs[MAX_COMMANDS])();
	size_t number_commands = 0;

	int _search_command(String command);

public:
	Neutrino(bool add_help=false);
	uint8_t print_help_info();
	bool add_command(String command, uint8_t (*func)());
	uint8_t parse_command(String command);

};



#endif // NEUTRINO_H
