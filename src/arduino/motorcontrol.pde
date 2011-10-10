// Sweep
// by BARRAGAN <http://barraganstudio.com> 
// This example code is in the public domain.


#include <Servo.h> 

Servo myservox;  // create servo object to control a servo 
// a maximum of eight servo objects can be created 
Servo myservoy;

int posx = 140;    // variable to store the servo position 
int posy = 140;

byte inByte;

void setup() 
{ 
  Serial.begin(9600);
  myservox.attach(9,600,2400);  // attaches the servo on pin 9 to the servo object 
  myservoy.attach(10,600,2400);
} 


void loop() 
{ 
  if(Serial.available()>0)
  {
    inByte = Serial.read();
    switch(inByte)
    {
    case '1':
      if(posx<180)
        posx = posx+20;
      break;
    case '2':
      if(posx>0)
        posx = posx-20;
      break;
    case '3':
      if(posy>90)
        posy = posy-20;
      break;
    case '4':
      if(posy<180)
        posy = posy+20;
      break;
    default:
      break;
    }
    Serial.print("x: ");
    Serial.print(posx);
    Serial.print("y: ");
    Serial.println(posy);
  } 

  myservox.write(posx);              // tell servo to go to position in variable 'pos'
  myservoy.write(posy);
} 





