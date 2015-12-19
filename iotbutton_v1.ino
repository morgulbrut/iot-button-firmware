/*
Firmware for an internet button.

Features:
- four buttons
- one relay
- one RGB LED

Buttons:
Button presses trigger events button1 - button4,
event values are the the values on A0 - A3.

Cloud functions:
-set_relay: sets the relay on or off.
-set_colour: sets the RGB LED to the given colour.
*/

#include <sstream>


// definition of the RGB LED pins
int rpin = D0;
int gpin = D1;
int bpin = D2;

// definition of the relay pin
int rel = D7;

// definition of the button pins
int bu1 = D3;
int bu2 = D4;
int bu3 = D5;
int bu4 = D6;

int sens1 = A0;
int sens2 = A1;
int sens3 = A2;
int sens4 = A3;

// gets true in ISR
bool interrupt_occured;
int button_nr = 0;

int command_val;

void setup()
{

  // intitalization of the pins
  pinMode(rpin, OUTPUT);
  pinMode(gpin, OUTPUT);
  pinMode(bpin, OUTPUT);

  pinMode(rel, OUTPUT);

  pinMode(bu1, INPUT_PULLUP);
  pinMode(bu2, INPUT_PULLUP);
  pinMode(bu3, INPUT_PULLUP);
  pinMode(bu4, INPUT_PULLUP);

  // attaching the interupts
  attachInterrupt(bu1, button_handler_1, FALLING);
  attachInterrupt(bu2, button_handler_2, FALLING);
  attachInterrupt(bu3, button_handler_3, FALLING);
  attachInterrupt(bu4, button_handler_4, FALLING);

  // Cloud functions
  Particle.function("set_relay",cloud_rel);
  Particle.function("set_colour",cloud_colour);


  set_led_colour(0,0,0);
  interrupt_occured = false;

}

void loop(){
    if(interrupt_occured){
        switch(button_nr){
          case 1:
          Particle.publish("button1", static_cast<String>(analogRead(sens1)));
          break;
          case 2:
          Particle.publish("button2", static_cast<String>(analogRead(sens2)));
          break;
          case 3:
          Particle.publish("button3", static_cast<String>(analogRead(sens3)));
          break;
          case 4:
          Particle.publish("button4", static_cast<String>(analogRead(sens4)));
          break;
        }
        delay(200);
        interrupt_occured = false;
    }
    delay(10);
}

int cloud_rel(String command){
  command_val = command.toInt();
  digitalWrite(rel, command_val);
  return command_val;
}

int cloud_colour(String command){
  std::string command_s = static_cast<std::string>(command);
  int r, g, b;
  if(command_s.at(0) == '#') {
    command_s = command_s.erase(0, 1);
  }
  // ... and extract the rgb values.
  std::istringstream(command_s.substr(0,2)) >> std::hex >> r;
  std::istringstream(command_s.substr(2,2)) >> std::hex >> g;
  std::istringstream(command_s.substr(4,2)) >> std::hex >> b;
  set_led_colour(r,g,b);
  return r*1000000+g*1000+b;
}

void button_handler_1()
{
   set_led_colour(0,0,0);
   interrupt_occured = true;
   button_nr = 1;
}

void button_handler_2()
{
   set_led_colour(255,0,0);
   interrupt_occured = true;
   button_nr = 2;
}

void button_handler_3()
{
   set_led_colour(0,255,0);
   interrupt_occured = true;
   button_nr = 3;
}

void button_handler_4()
{
   set_led_colour(0,0,255);
   interrupt_occured = true;
   button_nr = 4;
}


void set_led_colour(int r, int g, int b){

    // since it is a common anode LED

    analogWrite(rpin,255-r);
    analogWrite(gpin,255-g);
    analogWrite(bpin,255-b);
}
