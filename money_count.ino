

/*
 * TWG STEM course
 * Coin sorter demonstration program
 * Prepared by Brian Ho
 * www.Treasuremarktech.com
 * Date : 30/10/2019
 * 
 */
// include the necessary library
#include <Wire.h> // I2C library for I2C control
#include <LiquidCrystal_I2C.h> // The code converter and command control for LCD 1602
#include <TimerOne.h> // The game timer counter library
//#include <NewTone.h>


LiquidCrystal_I2C lcd(0x27,16,2);//setup liquidCrystal dispaly 1602 at address 0x27


//Declare the Motor control pins
const int motorA = 3;
const int motorB = 4;
// end declaration

// Declare the On/Off LED and interrupt pins
const byte Status_LED = 5;
const byte InterruptPin =2;
// End Declare

// Declare the IR counter input pins
const int pinOUT1 = 6;
const int pinOUT2 = 7;
const int pinOUT3 = 8;
const int pinOUT4 = 9;
// End Declare

//cDeclare the Buzzer pins
const int buz = 10;
// end declare

// Initialize the pins status
int IRvalue1 = 0;
int IRvalue2 = 0;
int IRvalue3 = 0;
int IRvalue4 = 0;


// setup the timer count to zero and the money lump sum

int Time_Rec = 0;
int money_count =0;

// end declare

//Declare the game status
bool Time_Over = false; // initial status the machine halt. so the game over is true
volatile bool tempstate = false;

void setup()
  {
  Serial.begin(9600);
  // setup the IR input pins for 4 types of coins
  pinMode(pinOUT1,INPUT);
  pinMode(pinOUT2,INPUT);
  pinMode(pinOUT3,INPUT);
  pinMode(pinOUT4,INPUT);
  // setup the Status LED output pins to show the status
  pinMode(Status_LED,OUTPUT);
  // The following pin 2 is the on off pin, we use interrupt to trigger
  pinMode(InterruptPin, INPUT_PULLUP);
  // timer interrupt setup
  attachInterrupt(digitalPinToInterrupt(InterruptPin),OnOff,CHANGE);
  Timer1.initialize(500000); // initialize the timer as 0.5 seconds a cycyle
  Timer1.attachInterrupt(Count_Timer);
  //the following set up the LCD display
  lcd.init(); // initialize the LCD display
  lcd.backlight(); // turn on the LCD backlight
  lcd.setCursor(0,0);
  lcd.print("TWGYKY college"); // display the school logo or team name
  
  }

void Write_LCD_Display()
  {
      lcd.backlight(); // turn on the LCD backlight
      lcd.setCursor(0,1);
      lcd.print("$$ ttl: ");
      lcd.setCursor(8,1);
      lcd.print(money_count); // display the total amount
      lcd.setCursor(12,1); // locate the second column end to display the status of On/Off
          if (tempstate == true)
            {
              lcd.print("ON ");
            }
          else
            {
              lcd.print("Off ");
            }
  }



void Motor_ON ()
{
  digitalWrite(motorA, HIGH);
  digitalWrite(motorB,LOW);
  
}
void Motor_OFF ()
{
  digitalWrite(motorA, LOW);
  digitalWrite(motorB,LOW);
  
}

void OnOff()
    {
      // Change the status 
      tempstate = true;
      // when TRUE turn motor on, when false stop the motor
      Time_Rec = 0;//set timer starting from zero
      money_count = 0;// reset the money count memory after restart
      Time_Over = false;//set the time loop not finished yet
     
    }

void Count_Timer()
{
  Time_Rec = Time_Rec+1;
  
  //Write_LCD_Display(); // every half seconds display one times
  
  if (Time_Rec >30) // in case the counter not reset, means no coins after 10seconds, the Game_Over to True, then and motor stop, until external interrupt reset the time_rec and game_Over status
  {
    Time_Over =true;
    Motor_OFF();
    digitalWrite(Status_LED,LOW);// turn off the status LEd show no working
    Time_Rec =0;
    tempstate = false;
  }
  

//Debug line display the status without assembly
  
  Serial.print("Status :");
  Serial.print("1D :");
  Serial.print(IRvalue1);
  Serial.print(" ;");
  Serial.print("2D :");
  Serial.print(IRvalue2); 
  Serial.print(" ;"); 
  Serial.print("5D :");
  Serial.print(IRvalue3);
  Serial.print(" ;");
  Serial.print("TD :");
  Serial.print(IRvalue4);
  Serial.print(" ;");
  Serial.print("GAME :");
      if (Time_Over==true)
          {
            Serial.print("Y");
          }
      else
          {
            Serial.print("N");
          }
  Serial.print(" ; ");        
  Serial.print("SWT :");
      if (tempstate==true)
          {
            Serial.print("Y");
          }
      else
          {
            Serial.print("N");
          }        
  Serial.print(" ; ");
  Serial.print("Tcnt:");
  Serial.print(Time_Rec);
  Serial.print(" ; ");
  Serial.print("Money_ttl :");
  Serial.println(money_count);
  
  

//end debug serial print routine  
  if (tempstate==true)  
  {
    digitalWrite(13,HIGH);
  }
  else
  {
    digitalWrite(13,LOW);
  }
  

}

void loop()
{
  
  if (tempstate==true)
  {
    Motor_ON(); // turn on the rotary box to screen out the coins
    digitalWrite(Status_LED, HIGH);//light on to show going to count
  }
  
  while ((Time_Over==false)&&(tempstate ==true)) // when the timer not count no operation up to 10seconds 
  {
    IRvalue1 = digitalRead(pinOUT1);
      if(IRvalue1==0)
          {
            money_count = money_count+1; // increase the money amount to 1, make sure the pins match with the coins value
             digitalWrite(buz,HIGH);
              delay(200);
              digitalWrite(buz,LOW);
            Time_Rec = 0;// reset the Timer counter, unless when timer over 20, the program stop. this make sure if keep counting, the machine will carry on
          }
  
    IRvalue2 = digitalRead(pinOUT2);
        if(IRvalue2==0)
          {
            money_count = money_count+2;// increase the money amount to 2, make sure the pins match with the coins value
             digitalWrite(buz,HIGH);
              delay(200);
              digitalWrite(buz,LOW);
            Time_Rec = 0; // reset the Timer counter, unless when timer over 20, the program stop. this make sure if keep counting, the machine will carry on
          }
  
  
    IRvalue3 = digitalRead(pinOUT3);
        if(IRvalue3==0)
          {
              money_count = money_count+5;// increase the money amount to 5, make sure the pins match with the coins value
              digitalWrite(buz,HIGH);
              delay(200);
              digitalWrite(buz,LOW);
              Time_Rec = 0;// reset the Timer counter, unless when timer over 20, the program stop. this make sure if keep counting, the machine will carry on
          }
    IRvalue4 = digitalRead(pinOUT4);
        if(IRvalue4==0)
          {
              money_count = money_count+10;// increase the money amount to 10, make sure the pins match with the coins value
              digitalWrite(buz,HIGH);
              delay(200);
              digitalWrite(buz,LOW);
              Time_Rec = 0;// reset the Timer counter, unless when timer over 20, the program stop. this make sure if keep counting, the machine will carry on
          }

   } 
    
    Write_LCD_Display(); // every half seconds display one times 
    
    delay(100);
  }
  
