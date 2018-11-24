#include "Neutrino.h"

uint8_t greet()
{
	Serial.println("Just want to say hello!");
	return 0;
}

uint8_t init_greet()
{
	Serial.println("Actually nothing needs initialization...");
	return 0;

}

uint8_t print_args(int argc, String *argv)
{
  Serial.print("\nargc = "); Serial.println(argc);
  Serial.println("arg list:");
  for (int i=0; i<argc; i++)
    Serial.println(argv[i]);
   return 0;
}

Neutrino neu = Neutrino(true);

void setup()
{
	Serial.begin(115200);
  Serial.println();
	neu.add_command("hello", greet, init_greet);
  neu.add_command("print", print_args);
	bool inited = neu.init_commands();
	if (inited)
		Serial.println("All tasks initialized.");
	neu.print_help_info();


}

void loop()
{
	while(!Serial.available());
	String command = Serial.readString();
	command.trim();
	uint8_t retval = neu.parse_command(command);
	Serial.print("\nReturned value: "); Serial.println(retval);
}
