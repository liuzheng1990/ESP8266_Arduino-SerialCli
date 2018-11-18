#include "SerialCli.h"




//////////////////////////////////////////////////////////////

// Public functions

//////////////////////////////////////////////////////////////
SerialCli::SerialCli(bool ah)
{
	this->auto_help = ah;
}

uint8_t SerialCli::print_help_info()
{
	uint8_t idx;
	Serial.println("Please begin by keying in one of the following commands:\n");
	for (idx=0; idx<(this->number_commands); idx++)
	{
		Serial.print("\t");
		Serial.println((this->commands_list)[idx]);
	}
	if (this->auto_help)
	Serial.println("\t_help (this help information.)");
	return 0;

}

bool SerialCli::add_command(String command, uint8_t (*func)())
{
	int i = this->_search_command(command);
	if (i>=0 || this->number_commands == MAX_COMMANDS) // names taken or two much commands
		return false;

	this->commands_list[this->number_commands] = command;
	this->callback_funcs[this->number_commands] = func;
	this->number_commands++;
	return true;

}

uint8_t SerialCli::parse_command(String command)
{
	command.trim();

	if (command == "_help")
	{
		this->print_help_info();
		return 0;
	}
  
	int i = this->_search_command(command);

	if (i<0 || i>=number_commands)
	{
		Serial.print("Error in parsing command: ");
		Serial.println(command);
		this->print_help_info();
		return 1; // exit codes other than 0 means failure.
	}

	return this->callback_funcs[i]();

}


//////////////////////////////////////////////////////////////

// Priviate functions

//////////////////////////////////////////////////////////////

int SerialCli::_search_command(String command)
{
	// For now, let's use sequential search
	for (int i=0; i<this->number_commands; i++)
		if (this->commands_list[i] == command)
			return i;

	return -1;

}
