#include "Neutrino.h"




//////////////////////////////////////////////////////////////

// Public functions

//////////////////////////////////////////////////////////////
Neutrino::Neutrino(bool ah)
{
	this->auto_help = ah;
}

uint8_t Neutrino::print_help_info()
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

// trim the command before calling this function, and no space in command
bool Neutrino::add_command(const String & command, cbfunc_t runtime_cb, cbfunc_t init_cb)
{
	int i = this->_search_command(command);
	if (i>=0 || this->number_commands == MAX_COMMANDS) // names taken or two much commands
		return false;

	this->init_callbacks[this->number_commands] = init_cb;
	this->commands_list[this->number_commands] = command;
	this->runtime_callbacks[this->number_commands] = runtime_cb;
	this->number_commands++;
	return true;
}

// trim the command before calling this function, and no space in command
bool Neutrino::add_command(const String & command, cbfunc_arg_t runtime_cb_arg, cbfunc_t init_cb)
{
	int i = this->_search_command(command);
	if (i>=0 || this->number_commands == MAX_COMMANDS) // names taken or two much commands
		return false;

	this->init_callbacks[this->number_commands] = init_cb;
	this->commands_list[this->number_commands] = command;
	this->runtime_callbacks_arg[this->number_commands] = runtime_cb_arg;
	this->number_commands++;
	return true;
}

bool Neutrino::init_commands()
{
	bool all_well = true;
	for (int i=0; i<this->number_commands; i++)
	{
		if (this->init_callbacks[i] == NULL) continue;
		if (this->init_callbacks[i]())
		{
			all_well = false;
			Serial.print("Command ");
			Serial.print(this->commands_list[i]);
			Serial.println("not initialized properly (non-zero exit code)!");
		}
	}
	return all_well;

}

uint8_t Neutrino::parse_command(String command)
{
	command.trim();
	String argv[10];
	if (command == "") return 0;
	if (this->auto_help && command == "_help")
	{
		this->print_help_info();
		return 0;
	}
  	// take the command head for the match
  	int i_space = command.indexOf(' ');
  	int i_next = -1;
  	argv[0] = (i_space==-1)?command:command.substring(0, i_space);

	int i = this->_search_command(argv[0]);
	int idx = 1; //store the current position of available vacancy in argv
	while (i_space > 0) // assemble all arguments into argv array
	{
		i_next = command.indexOf(' ', i_space+1);
		if (idx == 10) return 255; // failed to parse: too many args
		if (i_next >= 0)
			argv[idx] = command.substring(i_space+1, i_next);
		else
			argv[idx] = command.substring(i_space+1);
		idx++;
		i_space = i_next;
	} // after the loop ends, 'idx' is nothing but argc


	if (i<0 || i>=number_commands)
	{
		Serial.print("Error in parsing command: ");
		Serial.println(command);
		this->print_help_info();
		return 1; // exit codes other than 0 means failure.
	}
	// run the callback funcs.
	if (idx > 1)
		return this->runtime_callbacks_arg[i](idx, argv);
	else if (this->runtime_callbacks[i] == NULL)
		return this->runtime_callbacks_arg[i](0, NULL);
	else
		return this->runtime_callbacks[i](); // learn exception mechanism in Arduino

}


//////////////////////////////////////////////////////////////

// Priviate functions

//////////////////////////////////////////////////////////////

int Neutrino::_search_command(const String & command)
{
	// For now, let's use sequential search
	for (int i=0; i<this->number_commands; i++)
		if (this->commands_list[i] == command)
			return i;

	return -1;

}
