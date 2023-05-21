

// Plotclock
// cc - by Johannes Heberlein 2014
// v 1.02
// thingiverse.com/joo   wiki.fablab-nuernberg.de
// units: mm; microseconds; radians
// origin: bottom left of drawing surface
// time library see http://playground.arduino.cc/Code/time 
// RTC  library see http://playground.arduino.cc/Code/time 
//               or http://www.pjrc.com/teensy/td_libs_DS1307RTC.html  
// Change log:
// 1.01  Release by joo at https://github.com/9a/plotclock
// 1.02  Additional features implemented by Dave:
//       - added ability to calibrate servofaktor seperately for left and right servos
//       - added code to support DS1307, DS1337 and DS3231 real time clock chips
//       - see http://www.pjrc.com/teensy/td_libs_DS1307RTC.html for how to hook up the real time clock
//
// 1.03  Remix/mod by Kjetil Egeland

// delete or mark the next line as comment if you don't need these
//#define CALIBRATION      // enable calibration mode
//#define REALTIMECLOCK    // enable real time clock
#include <SoftwareSerial.h>
SoftwareSerial BLU(6, 7);   //TX, TX   //RAFA00
//#include <String.h>

String voice;
int respondeu=0;
int x;
// When in calibration mode, adjust the following factor until the servos move exactly 90 degrees
#define SERVOFAKTORLEFT 580       //610
#define SERVOFAKTORRIGHT 540      //620
// Zero-position of left and right servo
// When in calibration mode, adjust the NULL-values so that the servo arms are at all times parallel
// either to the X or Y axis
//#define SERVOLEFTNULL 2250
//#define SERVORIGHTNULL 920
#define SERVOLEFTNULL 1650
#define SERVORIGHTNULL 400

#define SERVOPINLIFT  2
#define SERVOPINLEFT  3
#define SERVOPINRIGHT 4

// lift positions of lifting servo
//#define LIFT0 1080 // on drawing surface
//#define LIFT1 925  // between numbers
//#define LIFT2 725  // going towards sweeper
#define LIFT0 1130   // on drawing surface
#define LIFT1 825  // between numbers
#define LIFT2 605  // going towards sweeper

// speed of liftimg arm, higher is slower
//#define LIFTSPEED 1500
#define LIFTSPEED 3000

// length of arms
#define L1 35
#define L2 57.1
#define L3 13.2

// origin points of left and right servo 
//#define O1X 22
//#define O1Y -25
//#define O2X 47
//#define O2Y -25
#define O1X 21
#define O1Y -25
#define O2X 48
#define O2Y -25

#define PARKX 76
#define PARKY 44
#define ERASEMAXX 60

#include <Time.h> // see http://playground.arduino.cc/Code/time 
#include <Servo.h>

#ifdef REALTIMECLOCK
// for instructions on how to hook up a real time clock,
// see here -> http://www.pjrc.com/teensy/td_libs_DS1307RTC.html
// DS1307RTC works with the DS1307, DS1337 and DS3231 real time clock chips.
// Please run the SetTime example to initialize the time on new RTC chips and begin running.

#include <Wire.h>
#include <DS1307RTC.h> // see http://playground.arduino.cc/Code/time    
#endif

int servoLift = 600;
long Number;
int randNumber1;
int randNumber2;
char resposta[50];
//int resposta2;            //RAFA01
String resposta2;
int tentativas = 0;

Servo servo1;  // 
Servo servo2;  // 
Servo servo3;  // 

volatile double lastX = PARKX;
volatile double lastY = PARKY;

void setup() 
{ 

#ifdef REALTIMECLOCK
  Serial.begin(9600);
  //while (!Serial) { ; } // wait for serial port to connect. Needed for Leonardo only

  // Set current time only the first to values, hh,mm are needed  
  tmElements_t tm;
  if (RTC.read(tm)) 
  {
    setTime(tm.Hour,tm.Minute,tm.Second,tm.Day,tm.Month,tm.Year);
    Serial.println("DS1307 time is set OK.");
  } 
  else 
  {
    if (RTC.chipPresent())
    {
      Serial.println("DS1307 is stopped.  Please run the SetTime example to initialize the time and begin running.");
    } 
    else 
    {
      Serial.println("DS1307 read error!  Please check the circuitry.");
    } 
    // Set current time only the first to values, hh,mm are needed
    setTime(15,05,0,0,0,0);
  }
  #else  
  Serial.begin(9600);
  BLU.begin(9600);  //RAFA00
  // Set current time only the first to values, hh,mm are needed
//  setTime(19,45,0,0,0,0);     //HORA PADRAO SEM RELOGIO   RAFA00
#endif

  randomSeed(analogRead(A0));
  drawTo(PARKX, PARKY);
  lift(1);
  servo1.attach(SERVOPINLIFT);  //  lifting servo
  servo2.attach(SERVOPINLEFT);  //  left servo
  servo3.attach(SERVOPINRIGHT);  //  right servo
  delay(1000);
  Serial.println("Bem-Vindo(a)");
  Serial.println("Voce sabe fazer contas?");

} 

void loop() 
{ 
#ifdef CALIBRATION

  drawTo(-3, 29.2);
  delay(500);
  drawTo(74.1, 28);
  delay(500);

#else 


    if (!servo1.attached()) servo1.attach(SERVOPINLIFT);
    if (!servo2.attached()) servo2.attach(SERVOPINLEFT);
    if (!servo3.attached()) servo3.attach(SERVOPINRIGHT);

  // imprime um número aleatório entre 0 e 9
  randNumber1 = random(10);  
  //randNumber1 = 4;  

  // imprime um número aleatório entre 0 e 9
  randNumber2 = random(10);
  //randNumber2 = 6;  
  
  number(0,25,randNumber1,1.2);
  //number(25, 25, 12, 1.2);          //Sinal X-Multiplicacao
  number(25, 25, 13, 1.2);            //Sinal +
  number(48,25,randNumber2,1.2);

  //number(25,25,randNumber1,1.2);
  //number(32, 25, 12, 1.2);
  //number(38,15,randNumber2,1.2);

    servo1.detach();
    servo2.detach();
    servo3.detach();
  //resposta2 = randNumber1 * randNumber2;    //*
  resposta2 = randNumber1 + randNumber2;      //++++++++
  
  Serial.print(randNumber1);
  //Serial.print(" x ");
  Serial.print(" + ");
  Serial.println(randNumber2);
  Serial.println("resposta:::::");
  //Serial.println(resposta2);    //RAFA00
    //sprintf(resposta, "", Number);
  tentativas = 3;
  
   Serial.println("Qual sua resposta? ");
   while(tentativas > 0){
   //while(Serial.available()==0){}
   //x = Serial.parseInt();
  while(true){
        if (BLU.available()){
        delay(10);                   //Delay added to make thing stable 
      
        char c = BLU.read();      //Conduct a serial read
      
        if (c == '#') 
        {
          respondeu = 1;
          //Serial.println("RECEBEU");            //RAFA01
          break;                     //Exit the loop when the # is detected after the word
     
        }
        if (c != '*'){
          voice += c;
          }
        }
        //Serial.println("dentro while");
      }

   //Serial.println(voice.length());

   if (respondeu == 1){
    //Serial.print("recebeu voice::::::");
    //Serial.println(voice);
    //Serial.print("length do voice:::");
    //Serial.println(voice.length());
    ////////Serial.print("resposta normal:::");
    //Serial.println(resposta2);
    //Serial.print("length da respo.:::");
    //Serial.println(resposta2.length());
   
   Serial.print("Sua resposta foi ");
   //Serial.print(x);                 //RAFA01
   Serial.print(voice);
   Serial.print(".");
   //delay(1000);

    //if (x == resposta2){            //RAFA01
    if (voice == resposta2){
       acertou();
       Serial.println("Parabens!\n");
       tentativas = 0;
       //for (int i = 0; i < 18; i++){  // This is where I want to flush any remaing data
        //byte discard = Serial.read();
       //}
       //x = 0;
       voice = "";
       respondeu = 0;
       
    } 
      else
        {
          errou();
          Serial.println(" Acho que nao!");
          //x = 0;
          //for (int i = 0; i < 18; i++){  // This is where I want to flush any remaing data
            //byte discard = Serial.read();
          //}
          tentativas -= 1;
          if(tentativas != 0){Serial.println("Tente novamente.");}
          else{Serial.println("Nao foi dessa vez...\n");}
          voice = "";
          respondeu =0;
        }
   }
 } 
     

#endif

}

void acertou(){
  digitalWrite(13, HIGH);
  digitalWrite(12, HIGH);
  delay(700);
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  delay(200);
  digitalWrite(13, HIGH);
  digitalWrite(12, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  delay(200);
  }

void errou(){
  digitalWrite(13, HIGH);
  delay(700);
  digitalWrite(13, LOW);
  delay(200);
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  delay(200);
  }
// Writing numeral with bx by being the bottom left originpoint. Scale 1 equals a 20 mm high font.
// The structure follows this principle: move to first startpoint of the numeral, lift down, draw numeral, lift up
void number(float bx, float by, int num, float scale) {
  switch (num) {

  case 0:
    drawTo(bx + 12 * scale, by + 6 * scale);
    lift(0);
    bogenGZS(bx + 7 * scale, by + 10 * scale, 10 * scale, -0.8, 6.7, 0.5);
    lift(1);
    break;
  case 1:

    drawTo(bx + 3 * scale, by + 15 * scale);
    lift(0);
    drawTo(bx + 10 * scale, by + 20 * scale);
    drawTo(bx + 10 * scale, by + 0 * scale);
    lift(1);
    break;
  case 2:
    drawTo(bx + 2 * scale, by + 12 * scale);
    lift(0);
    bogenUZS(bx + 8 * scale, by + 14 * scale, 6 * scale, 3, -0.8, 1);
    drawTo(bx + 1 * scale, by + 0 * scale);
    drawTo(bx + 12 * scale, by + 0 * scale);
    lift(1);
    break;
  case 3:
    drawTo(bx + 2 * scale, by + 17 * scale);
    lift(0);
    bogenUZS(bx + 5 * scale, by + 15 * scale, 5 * scale, 3, -2, 1);
    bogenUZS(bx + 5 * scale, by + 5 * scale, 5 * scale, 1.57, -3, 1);
    lift(1);
    break;
  case 4:
    drawTo(bx + 10 * scale, by + 0 * scale);
    lift(0);
    drawTo(bx + 10 * scale, by + 20 * scale);
    drawTo(bx + 2 * scale, by + 6 * scale);
    drawTo(bx + 12 * scale, by + 6 * scale);
    lift(1);
    break;
  case 5:
    drawTo(bx + 2 * scale, by + 5 * scale);
    lift(0);
    bogenGZS(bx + 5 * scale, by + 6 * scale, 6 * scale, -2.5, 2, 1);
    drawTo(bx + 5 * scale, by + 20 * scale);
    drawTo(bx + 12 * scale, by + 20 * scale);
    lift(1);
    break;
  case 6:
    drawTo(bx + 2 * scale, by + 10 * scale);
    lift(0);
    bogenUZS(bx + 7 * scale, by + 6 * scale, 6 * scale, 2, -4.4, 1);
    drawTo(bx + 11 * scale, by + 20 * scale);
    lift(1);
    break;
  case 7:
    drawTo(bx + 2 * scale, by + 20 * scale);
    lift(0);
    drawTo(bx + 12 * scale, by + 20 * scale);
    drawTo(bx + 2 * scale, by + 0);
    lift(1);
    break;
  case 8:
    drawTo(bx + 5 * scale, by + 10 * scale);
    lift(0);
    bogenUZS(bx + 5 * scale, by + 15 * scale, 5 * scale, 4.7, -1.6, 1);
    bogenGZS(bx + 5 * scale, by + 5 * scale, 5 * scale, -4.7, 2, 1);
    lift(1);
    break;

  case 9:
    drawTo(bx + 9 * scale, by + 11 * scale);
    lift(0);
    bogenUZS(bx + 7 * scale, by + 15 * scale, 5 * scale, 4, -0.5, 1);
    drawTo(bx + 5 * scale, by + 0);
    lift(1);
    break;

  case 111:

    lift(0);
    drawTo(70, 46);
    drawTo(ERASEMAXX, 43);

    drawTo(ERASEMAXX, 49);
    drawTo(5, 49);
    drawTo(5, 45);
    drawTo(ERASEMAXX, 45);
    drawTo(ERASEMAXX, 40);

    drawTo(5, 40);
    drawTo(5, 35);
    drawTo(ERASEMAXX, 35);
    drawTo(ERASEMAXX, 30);

    drawTo(5, 30);
    drawTo(5, 25);
    drawTo(ERASEMAXX, 25);
    drawTo(ERASEMAXX, 20);

    drawTo(5, 20);
    drawTo(60, 44);

    drawTo(PARKX, PARKY);
    lift(2);

    break;

  case 11:
    drawTo(bx + 5 * scale, by + 15 * scale);
    lift(0);
    bogenGZS(bx + 5 * scale, by + 15 * scale, 0.1 * scale, 1, -1, 1);
    lift(1);
    drawTo(bx + 5 * scale, by + 5 * scale);
    lift(0);
    bogenGZS(bx + 5 * scale, by + 5 * scale, 0.1 * scale, 1, -1, 1);
    lift(1);
    break;


  case 12:  // X multiplicacao sinal
    drawTo(bx + 3 * scale, by + 15 * scale);
    lift(0);
    drawTo(bx + 15 * scale, by + 0 * scale);
    lift(1);
    drawTo(bx + 3 * scale, by + 0 * scale);
    lift(0);
    drawTo(bx + 15 * scale, by + 15 * scale);
    lift(1);
    break;
     
  case 13:  // + soma sinal
    drawTo(bx + 10 * scale, by + 20 * scale);
    lift(0);    //desceu
    drawTo(bx + 10 * scale, by + 0 * scale);
    lift(1);    //subiu
    drawTo(bx + 2 * scale, by + 10 * scale);
    lift(0);
    drawTo(bx + 16 * scale, by + 10 * scale);
    lift(1);
    
    break;


  }
}



void lift(char lift) {
  switch (lift) {
    // room to optimize  !

  case 0: //850

      if (servoLift >= LIFT0) {
      while (servoLift >= LIFT0) 
      {
        servoLift--;
        servo1.writeMicroseconds(servoLift);        
        delayMicroseconds(LIFTSPEED);
      }
    } 
    else {
      while (servoLift <= LIFT0) {
        servoLift++;
        servo1.writeMicroseconds(servoLift);
        delayMicroseconds(LIFTSPEED);

      }

    }

    break;

  case 1: //150

    if (servoLift >= LIFT1) {
      while (servoLift >= LIFT1) {
        servoLift--;
        servo1.writeMicroseconds(servoLift);
        delayMicroseconds(LIFTSPEED);

      }
    } 
    else {
      while (servoLift <= LIFT1) {
        servoLift++;
        servo1.writeMicroseconds(servoLift);
        delayMicroseconds(LIFTSPEED);
      }

    }

    break;

  case 2:

    if (servoLift >= LIFT2) {
      while (servoLift >= LIFT2) {
        servoLift--;
        servo1.writeMicroseconds(servoLift);
        delayMicroseconds(LIFTSPEED);
      }
    } 
    else {
      while (servoLift <= LIFT2) {
        servoLift++;
        servo1.writeMicroseconds(servoLift);        
        delayMicroseconds(LIFTSPEED);
      }
    }
    break;
  }
}


void bogenUZS(float bx, float by, float radius, int start, int ende, float sqee) {
  float inkr = -0.05;
  float count = 0;

  do {
    drawTo(sqee * radius * cos(start + count) + bx,
    radius * sin(start + count) + by);
    count += inkr;
  } 
  while ((start + count) > ende);

}

void bogenGZS(float bx, float by, float radius, int start, int ende, float sqee) {
  float inkr = 0.05;
  float count = 0;

  do {
    drawTo(sqee * radius * cos(start + count) + bx,
    radius * sin(start + count) + by);
    count += inkr;
  } 
  while ((start + count) <= ende);
}


void drawTo(double pX, double pY) {
  double dx, dy, c;
  int i;

  // dx dy of new point
  dx = pX - lastX;
  dy = pY - lastY;
  //path lenght in mm, times 4 equals 4 steps per mm
  c = floor(4 * sqrt(dx * dx + dy * dy));

  if (c < 1) c = 1;

  for (i = 0; i <= c; i++) {
    // draw line point by point
    set_XY(lastX + (i * dx / c), lastY + (i * dy / c));

  }

  lastX = pX;
  lastY = pY;
}

double return_angle(double a, double b, double c) {
  // cosine rule for angle between c and a
  return acos((a * a + c * c - b * b) / (2 * a * c));
}

void set_XY(double Tx, double Ty) 
{
  delay(1);
  double dx, dy, c, a1, a2, Hx, Hy;

  // calculate triangle between pen, servoLeft and arm joint
  // cartesian dx/dy
  dx = Tx - O1X;
  dy = Ty - O1Y;

  // polar lemgth (c) and angle (a1)
  c = sqrt(dx * dx + dy * dy); // 
  a1 = atan2(dy, dx); //
  a2 = return_angle(L1, L2, c);

  servo2.writeMicroseconds(floor(((a2 + a1 - M_PI) * SERVOFAKTORLEFT) + SERVOLEFTNULL));

  // calculate joinr arm point for triangle of the right servo arm
  a2 = return_angle(L2, L1, c);
  Hx = Tx + L3 * cos((a1 - a2 + 0.621) + M_PI); //36,5°
  Hy = Ty + L3 * sin((a1 - a2 + 0.621) + M_PI);

  // calculate triangle between pen joint, servoRight and arm joint
  dx = Hx - O2X;
  dy = Hy - O2Y;

  c = sqrt(dx * dx + dy * dy);
  a1 = atan2(dy, dx);
  a2 = return_angle(L1, (L2 - L3), c);

  servo3.writeMicroseconds(floor(((a1 - a2) * SERVOFAKTORRIGHT) + SERVORIGHTNULL));

}


