# Neutrino: build your own mini "operating system" on Arduino/ESP8266

Well, it's not really an operating system, strictly speaking. It's even oversimplified as a commandline interface. You write a function to do something. Instead of putting that function into a sketch and make your Arduino a single task runner, you register several functions and give each one a command name. You can then call one task by keying in its name in the serial monitor, and, Bang! Your function is running, and you can run different tasks with one codes uploading!

Seriously, that's all this library is about. When I use this library myself, however, I felt it very handy because I can run different tasks without constantly burning codes of each task into the flash again and again. For example, I can register a "save_ir_codes" command to store some newly-recorded ir codes for my TV, and use a "send_ir_instruction" command to control the TV. (Previously, I had to burn the "save_ir_codes" sketch to Arduino, record and store the IR codes, and then burn the "send_ir_instruction" sketch to Arduino to control the TV. When I need another button's code, I burned "save_ir_codes" again to save, and "send_ir_instruction" again to send...)

What's more, it has triggered my imagination because I can treat each task as a building block, and build various complicated tools from serial. If you have played **Minecraft**&copy;, you know what I mean.

## Orientation

Here are images showing Neutrino in action. See below ("**how to use Neutrino**") for the corresponding codes.

The first image below is when Arduino restarted. The `neu.init_commands` initialized all commands (who has an `init` function) and printed the help infomation. The user then issued the "hello" command.

![init_commands when restarted (see "init_greet" function below)](https://liutheprogrammer.files.wordpress.com/2018/11/neutrino_hello.png)

In the second image below, the user firstly issued `_help` command, which caused the help info to be printed. Next the user entered "led on". This called the "led" command with arguments. Consequently, the "led" function was called with `argc=2, argv=["led", "on"]`. Then the user entered "led off". Check the `led` function below to see how the arguments are handled.

![user entered "led on" and "led off". See the `led` function below](https://liutheprogrammer.files.wordpress.com/2018/11/neutrino_led.png)

## What can you do with Neutrino?

It all depends on what command functions you register to it! Here are some examples I can think of, and I will implement some myself in a directory called "building_blocks". Please give me your feedback and tell me more!

### Your own "DOS system"

If your Arduino has an SD card connection or a filesystem (I'm using an ESP8266, which comes with an SPIFFS filesystem), you can build your own "DOS system" by adding some functions navigating directories, reading and writing files, etc. Here are some commands you may want to implement:

1. **pwd** Print your current directory. (For flat filesystem, one can mimic directory by carefully designing the filename. For example, you can't create a real folder called "home" and store a file "hello.txt" in it, but you can create a file called "home/hello.txt". )
2. **ls** (or **dir**) List files in the working directory.
2. **cd** Change directory.
3. **cat** Print all content of a file to the serial monitor.
4. **write** Write data to a file.
5. and many more

In this way, you got a small document operating system to navigate.

### A diary manager

Similar to the above example, implement commands like **create_diary**, **view_diary**, **list_diaries**, etc, you get a small diary manager.

### A control console

You have a sketch which blinks an LED, a sketch which controls a servo, and a sketch which connects to WiFi. Now you can register each task as a command, and give it a command name. Then sending commands like "light_on", "servo"... let you control each component without burning each sketch over and over again.

## Install the Neutrino library

1. Download the library: https://github.com/liuzheng1990/Neutrino/archive/master.zip
2. (In the Arduino IDE) Sketch -> Include Library -> Add .ZIP Library...
3. Select the downloaded file.
4. Click "Open".
5. In the menu bar, click "Sketch -> Include Library", you'll see the list of all installed libraries, where "Neutrino" should be among them. Click it to include this library to your sketch.
6. To view and try the examples, in the menu bar, click "File -> Examples" and you should see "Neutrino" there.

## How to use Neutrino

Using Neutrino is simple. Here is a chunk of sample codes. The step-by-step explanations are below the codes.
```
#include <Neutrino.h> // you can either enter this line or 
                      // click "Neutrino" in "Sketch->Include Library->Neutrino".
              
// include other libraries you may use

#define PIN_LED 4

Neutrino neu = Neutrino(true); // create a global object, "true" to auto add "_help" command

uint8_t greet() // a sample task function with no argument, and return uint8_t.
{
  Serial.println("Just to say hello!");
  return 0; // return 0 to indicate successful exit.
}

uint8_t init_greet() // a task can optionally have a "init" function. 
{
  Serial.println("For this task, nothing to init...");
  return 0;
}

uint8_t led(int argc, String argv[]) // another sample task, with arguments
{
  if (argc > 1) // check at least one argument(first arg is always command name)
  {
    if (argv[1] == "on")
  	  digitalWrite(PIN_LED, LOW);
      Serial.println("LED is on!");
    else
      digitalWrite(PIN_LED, HIGH);
      Serial.println("LED is off!");
    return 0;
  }
  else
    return 1; non-zero return value => returned with error
}

// ... More sample tasks

void setup()
{
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT); //some setup you need
  
  neu.add_command("greet", greet); // register a command.
  neu.add_command("led", led); // 1st arg: command name, 2nd arg: function name binding to the command
  // ... add more commands
  bool i = neu.init_commands(); // run init functions of all tasks
  neu.print_help_info(); // print an auto generated help info
                         // note the "_help" command auto registered.
  
  
}

void loop()
{
  string command;
  uint8_t retval; // used to store the return value of each command later
  while (!Serial.available()); //wait for user input at serial port
  
  command = Serial.readString();
  retval = neu.parse_command(command);
  // You can do something depending on the return value, or simply ignore.
}
```
Step-by-step explanations:

1. Include the library. See the last section.
2. Define a global "Neutrino" object. `Neutrino neu = Neutrino();`
3. Define your task functions. Each function should take no arguments and return an `uint8_t` number to indicate an exit status. If you are not using the returned number, simply `return 0;`.
4. In the `setup` function, call the `add_command` to register each task function to a command name. You can choose any command name you like. The second arument of `add_command` is the function name you want to bind to the command name.
5. After you initialize the `Serial` (i.e., after you call `Serial.begin(<some_baudrate>);`) you can call `neu.print_help_info();` to print an automatically generated help message, listing all commands you have registered. You may notice the `_help` command. You didn't register this command. The library registered it so that when you want to see this help message later on, you can just send `_help` command in the Serial monitor to review what are available commands. (If you want to define your own customized help command, please use `help` instead of `_help`.
6. Anywhere in your codes, when you receive the command name from Serial, you can pass it to the `neu.parse_command` method. This method will check whether the command name is properly registered, and run it if it is. It will return the same number returned by the task function, so if you like, you can store the return value into a `uint8_t` variable and do whatever you like with it.

That's it! Define a Neutrino object -> define your task function -> register your task (add_command) -> call it by `parse_command`.

## Playground for Arduino Beginners

I myself is an Arduino beginner. If you are also new to Arduino and find this library interesting, I encourage you to modify the lirary codes and play with it. I'd be very glad to know your own journey to use, modify and improve it!

## More to come

Here are some new features I plan to add.

1. I'll distinguish one-short tasks to loop tasks. At this moment, the library is really only handling the one-short tasks. (Of course, you can create an infinite loop in your own task function. But once such a task starts, it'll never give other tasks a chance to run.) For a loop task, I'll wrap its loop part so that after each loop, Neutrino can get control for a short while and do some message handling. This is a possible way to evolve Neutrino to a crude operating system.

2. For now, the task functions take no argument. All input has to be passed to the function either through global variables or through serial input within the task. I'll think about adding a simple argument parsing mechanism so that when users enter the command name in the Serial, they can also pass arguments. The arg parser then parses them and pass them to the task functions when calling them. 

I don't intend to make Neutrino a full-featured operating system like `RTOS`, or a run-time interpretator like `MicroPython`. They are already very powerful and reliable tools to use. This library will always be a simple playground for beginners to gain some hands-on intuition about task control. However, if you have a more ambitious goal and would like to use Neutrino as a stepping stone to build it, you are very welcome to fork it, and do any modification you like!

Happy coding!


