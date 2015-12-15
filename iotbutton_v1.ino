// definition of the RGB LED pins
int rpin = D0;
int gpin = D1;
int bpin = D2;

// definition of the button pins
int bu1 = D3;
int bu2 = D4;
int bu3 = D5;
int bu4 = D6;

// gets true in ISR
bool interrupt_occured;
int button_nr = 0;

void setup()
{

  // intitalization of the pins

  pinMode(rpin, OUTPUT);
  pinMode(gpin, OUTPUT);
  pinMode(bpin, OUTPUT);

  pinMode(bu1, INPUT_PULLUP);
  pinMode(bu2, INPUT_PULLUP);
  pinMode(bu3, INPUT_PULLUP);
  pinMode(bu4, INPUT_PULLUP);

  // attaching the interupts

  attachInterrupt(bu1, button_handler_1, FALLING);
  attachInterrupt(bu2, button_handler_2, FALLING);
  attachInterrupt(bu3, button_handler_3, FALLING);
  attachInterrupt(bu4, button_handler_4, FALLING);

  set_led_colour(0,0,0);

  interrupt_occured = false;

}

void loop(){
    if(interrupt_occured){
        Particle.publish("Button pressed", static_cast<String>(button_nr));
        delay(200);
        interrupt_occured = false;
    }
    delay(10);
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
