/*
:Project:Alarm_Clock
:Author: Tiziano Bianchettin
:Date: 10/02/2017
:Revision: 2
:License: Public Domain
thanks to:
  http://arduinoenonsolo.blogspot.it/2012/12/orologio-con-arduino-e-il-ds1307.html
  http://www.mauroalfieri.it/
  http://www.danielealberti.it/
  http://www.maffucci.it/
  My electronics laboratory professor "Perito Carli"  
*/
//Orange is method (or other things), Vomit green are keywords,
//************libraries**************//
#include <Wire.h>
#include <RTClib.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c LCD i/o class header

//************************************//
hd44780_I2Cexp lcd; // Display  I2C 16 x 2
RTC_DS1307 RTC;

//************Button*****************//
int P1=6; // Button SET MENU'
int P2=7; // Button +
int P3=8; // Button -
int P4=9; // SWITCH Alarm

//**************Alarm***************//
int LED=3; //arduino pin controlling LED strip
int duty=0;
int steps=64;
//this is the human readable number of minutes for duration of sunrise
int minutes_of_sunrise = 1;
//187.5 is the multiplier necessary to get the math of the PWM and number of light levels to equate to minutes in real life 
int numberofpulses = minutes_of_sunrise * 187.5;
int i;
int j;

int lookup[64] = {1,2,4,6,9,12,16,20,25,30,36,
42,49,56,64,72,81,90,100,110,121,132,
144,156,169,182,196,210,225,240,256,272,289,
306,324,342,361,380,400,420,441,462,484,506,
529,552,576,600,625,650,676,702,729,756,784,
812,841,870,900,930,961,992,992,992};

//************Variables**************//
int hourupg;
int minupg;
int yearupg;
int monthupg;
int dayupg;
int menu =0;
int setAlarm =0;

uint8_t alarmHours = 0, alarmMinutes = 0;  // Holds the current alarm time


void setup()
{

  lcd.begin(16,2);
  lcd.clear();

  pinMode(P1,INPUT); // https://www.arduino.cc/en/Tutorial/InputPullupSerial
  pinMode(P2,INPUT);
  pinMode(P3,INPUT);
  pinMode(P4,INPUT);
  pinMode(LED, OUTPUT); // Set arduino pin controlling LED strip as an output
  printAlarmOff();
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();

  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // Set the date and time at compile time
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  // RTC.adjust(DateTime(__DATE__, __TIME__)); //removing "//" to adjust the time
    // The default display shows the date and time
  int menu=0;
}
 
void loop()
{ 

// check if you press the SET button and increase the menu index
  if(digitalRead(P1)== HIGH) 
  {
   menu=menu+1;
  }
// in which subroutine should we go?
  if (menu==0)
    {
     DisplayDateTime(); // void DisplayDateTime
     Alarm(); // Alarm control
          }
  if (menu==1)
    {
    DisplaySetHour();
    }
  if (menu==2)
    {
    DisplaySetMinute();
    }
  if (menu==3)
    {
    DisplaySetHourAlarm();
    }
  if (menu==4)
    {
    DisplaySetMinuteAlarm();
    } 
  if (menu==5)
    {
    StoreAgg(); 
    delay(500);
    menu=0;
    }
    delay(100);
}

void DisplayDateTime ()
{
// We show the current time
  DateTime now = RTC.now();

  lcd.setCursor(0, 0);
  lcd.print("Hour : ");
  
  if (now.hour()<=9)
  {
    lcd.print("0");
  }
  lcd.print(now.hour(), DEC);
  hourupg=now.hour();
  lcd.print(":");
  if (now.minute()<=9)
  {
    lcd.print("0");
  }
  lcd.print(now.minute(), DEC);
  minupg=now.minute();
  lcd.print(":");
  if (now.second()<=9)
  {
    lcd.print("0");
  }
  lcd.print(now.second(), DEC);

 }

void DisplaySetHour()
{
// time setting
  lcd.clear();
  DateTime now = RTC.now();
  if(digitalRead(P2)==HIGH)
  {
    if(hourupg==23)
    {
      hourupg=0;
    }
    else
    {
      hourupg=hourupg+1;
    }
  }
   if(digitalRead(P3)==HIGH)
  {
    if(hourupg==0)
    {
      hourupg=23;
    }
    else
    {
      hourupg=hourupg-1;
    }
  }
  lcd.setCursor(0,0);
  lcd.print("Set Hour:");
  lcd.setCursor(0,1);
  lcd.print(hourupg,DEC);
  delay(200);
}

void DisplaySetMinute()
{
// Setting the minutes
  lcd.clear();
  if(digitalRead(P2)==HIGH)
  {
    if (minupg==59)
    {
      minupg=0;
    }
    else
    {
      minupg=minupg+1;
    }
  }
   if(digitalRead(P3)==HIGH)
  {
    if (minupg==0)
    {
      minupg=59;
    }
    else
    {
      minupg=minupg-1;
    }
  }
  lcd.setCursor(0,0);
  lcd.print("Set Minutes:");
  lcd.setCursor(0,1);
  lcd.print(minupg,DEC);
  delay(200);
}
void StoreAgg()
{
// Variable saving
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SAVING IN");
  lcd.setCursor(0,1);
  lcd.print("PROGRESS");
  RTC.adjust(DateTime(yearupg,monthupg,dayupg,hourupg,minupg,0));
  delay(200);
}
void DisplaySetHourAlarm()// Setting the alarm hours
{
// set alarm hours
  lcd.clear();

  if(digitalRead(P2)==HIGH)
  {
    if(alarmHours==23)
    {
      alarmHours=0;
    }
    else
    {
      alarmHours=alarmHours+1;
    }
  }
   if(digitalRead(P3)==HIGH)
  {
    if(alarmHours==0)
    {
      alarmHours=23;
    }
    else
    {
      alarmHours=alarmHours-1;
    }
  }
  lcd.setCursor(0,0);
  lcd.print("Set Hour Alarm:");
  lcd.setCursor(0,1);
  lcd.print(alarmHours,DEC);
  delay(200);
}


void DisplaySetMinuteAlarm()// Setting the alarm minutes
 {
  lcd.clear();
  
  if(digitalRead(P2)==HIGH)
  {
    if (alarmMinutes==59)
    {
      alarmMinutes=0;
    }
    else
    {
      alarmMinutes=alarmMinutes+1;
    }
  }
   if(digitalRead(P3)==HIGH)
  {
    if (alarmMinutes==0)
    {
      alarmMinutes=59;
    }
    else
    {
      alarmMinutes=alarmMinutes-1;
    }
  }
  lcd.setCursor(0,0);
  lcd.print("Set MIN. Alarm:");
  lcd.setCursor(0,1);
  lcd.print(alarmMinutes,DEC);
  delay(200);
 
}
void printAlarmOn(){
   lcd.setCursor(0,1);
  lcd.print("Alarm: ");

  
  
  if (alarmHours <= 9)
  {
    lcd.print("0");
  }
  lcd.print(alarmHours, DEC);
  
  lcd.print(":");
  if (alarmMinutes <= 9)
  {
    lcd.print("0");
  }
  lcd.print(alarmMinutes, DEC); 

}
//Changed set cursor from 0,3 to 0,1//
void printAlarmOff() {
  lcd.setCursor(0, 1);
  lcd.print("Alarm: Off  ");  
}
void Alarm(){
   if(digitalRead(P4)== HIGH)
  {
   setAlarm=setAlarm+1;
  }
  if (setAlarm==0)
    {
     printAlarmOff();
     digitalWrite (LED, LOW);
     }
  if (setAlarm==1)
    {

     printAlarmOn();    
  
     DateTime now = RTC.now();
     if ( now.hour() == alarmHours && now.minute() == alarmMinutes )
        {
         DateTime now = RTC.now();
            
            {
              for (i=0; i<steps; i++)
              {
                duty = lookup[i] * 5;
                for (j=0; j<numberofpulses; j++)
                {
                  // one pulse of PWM
                  digitalWrite(LED, HIGH);
                  delayMicroseconds(duty);
                  digitalWrite(LED, LOW);
                  delayMicroseconds(5000-duty);
                }
              }
            }
        }
    else{
         digitalWrite (5, LOW);
        }
    
    } 
  if (setAlarm==2)
    {
     setAlarm=0;
    }
    delay(200);
}
