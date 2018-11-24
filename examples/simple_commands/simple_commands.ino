#include <Neutrino.h>

/* Neutrino Arduino Command line system

 Thales Liu

 Change Log: 

 2018/11/24: 1. Every task can have an init part and a runtime part.
             2. Each command can take argument and task func can handle arguments.

*/

uint8_t greet() // Task "hello", the runtime part
{
  Serial.println("Just want to say hello!");
  return 0;
}

uint8_t init_greet() // Task "hello", the (trivial) init part
{
  Serial.println("Actually nothing needs initialization...");
  return 0;

}

uint8_t print_args(int argc, String *argv) // task "print", note the arguments
{
  Serial.print("\nargc = "); Serial.println(argc);
  Serial.println("arg list:");
  for (int i=0; i<argc; i++)
    Serial.println(argv[i]);
   return 0;
}

Neutrino neu = Neutrino(true); // true => auto add "_help" command

void setup()
{
  Serial.begin(115200);
  Serial.println();
  neu.add_command("hello", greet, init_greet); // register command "hello"
  neu.add_command("print", print_args); // register command "print" (init part default to NULL)
  bool inited = neu.init_commands(); // run all registered init parts
  if (inited)
    Serial.println("All tasks initialized.");
  neu.print_help_info();  // display all available commands


}

void loop()
{
  while(!Serial.available()); // wait for serial input (command)
  String command = Serial.readString(); // try input "hello", or "print hello world"
  command.trim();
  uint8_t retval = neu.parse_command(command); // parse the command and run corresponding task
  Serial.print("\nReturned value: "); Serial.println(retval);
}