#include <SerialCli.h>

SerialCli sc = SerialCli(true); // if you put true, you can use '_help' command for free.

uint8_t greet() // a sample callback function
{ 
  Serial.println("Just to say hello!");
  return 0;
}

void setup() {
  // put your setup code here, to run once:
  sc.add_command("hello", greet); // register function greet to command name "hello".
  sc.print_help_info();
}

void loop() {
  String command;
  uint8_t retval; // a variable to store the returned value of the command tasks.
  while (!Serial.available()); // empty loop, wait for serial command

  command = Serial.readString(); //store the command request to command
  retval = sc.parse_command(command);
  Serial.print("Command finished running. Returned value: ");
  Serial.println(retval);
}
