/* This code test the AB1805 real-time clock from Abracon Corporation
   code written by J.W.Chia @ 29/3/2017
*/

#include <Arduino.h>
#include <DWire.h>
#include <DSerial.h>
#include "AB1805.h"

DWire wire;
DSerial serial;

AB1805 AB_1(wire);

//Parameters
unsigned char hundredth_sec;
unsigned char sec;
unsigned char mins;
unsigned char hr;
unsigned char day_of_week;
unsigned char date_1;
unsigned char month;
unsigned char year;
unsigned char mode;   //time format 0: 12-hour mode(AM) 1: 12-hour mode(PM) 2: 24-hour mode

unsigned char init_time[8];

void setup()
{
  wire.begin();          // initialize I2C master
  serial.begin();       //initialize debug UART

  delay(20);           // ensure the serial port is initialized

  //Initialise control register
  /*24-hour format*/
  AB_1.init(HOUR_MODE_24);
  /*12-hour format*/
  //  AB_1.init(HOUR_MODE_12);

  delay(20);

  //Initialise interrupt mask register
  AB_1.init_int();
  delay(20);

  //time initialisation
  /*24-hour format*/
  init_time[0] = 17;
  init_time[1] = 12;
  init_time[2] = 31;
  init_time[3] = 6;
  init_time[4] = 23;
  init_time[5] = 59;
  init_time[6] = 59;
  init_time[7] = 0;
  AB_1.init_time(init_time);

  /*12-hour format*/
  //  init_time[0] = 17;
  //  init_time[1] = 12;
  //  init_time[2] = 31;
  //  init_time[3] = 6;
  //  init_time[4] = 11 | AB_1.hex2dec(PM);
  //  init_time[5] = 59;
  //  init_time[6] = 59;
  //  init_time[7] = 0;
  //  AB_1.init_time(init_time);

  delay(20);
  AB_1.init_time_done();  //prevent other source to overwrite time register

  delay(20);

  serial.println();
  serial.println("---------------------------------------------------------");
  serial.println("-----------------   AB1805 RTC tester    ----------------");
  serial.println("---------------------------------------------------------");
  serial.println();

  delay(20);

  //print the header of the table
  serial.println();
  serial.println("----------------------------------------------------------------------------------------------------");
  serial.print("Year ");
  serial.print("\t");
  serial.print("Month ");
  serial.print("\t");
  serial.print("Date ");
  serial.print("\t");
  serial.print("Day ");
  serial.print("\t");
  serial.print("Hour ");
  serial.print("\t");
  serial.print("Minutes");
  serial.print("\t");
  serial.print("Seconds");
  serial.print("\t");
  serial.print("100th Seconds");
  serial.print("\t");
  serial.print("Time Format");
  serial.println();
  serial.println("----------------------------------------------------------------------------------------------------");
}

void loop()
{
  mode = AB_1.get_time();   //Get the time from RTC

  hundredth_sec = AB_1.date[7];
  sec = AB_1.date[6];
  mins = AB_1.date[5];
  hr = AB_1.date[4];
  day_of_week = AB_1.date[3];
  date_1 = AB_1.date[2];
  month = AB_1.date[1];
  year = AB_1.date[0];

  serial.print(year, DEC);
  serial.print("\t");
  serial.print(month, DEC);
  serial.print("\t");
  serial.print(date_1, DEC);
  serial.print("\t");
  serial.print(day_of_week, DEC);
  serial.print("\t");
  serial.print(hr, DEC);
  serial.print("\t");
  serial.print(mins, DEC);
  serial.print("\t");
  serial.print(sec, DEC);
  serial.print("\t");
  serial.print(hundredth_sec, DEC);
  serial.print("\t");
  serial.print("\t");

  if (mode == 0)
  {
    serial.print("AM");
  }
  else if (mode == 1)
  {
    serial.print("PM");
  }
  else
  {
    serial.print("24-FORMAT");
  }
  serial.println();

  delay(96);
}
