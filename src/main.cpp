#include <Arduino.h>

#ifndef __WIRE_H
#include <Wire.h>
#endif

#include <LiquidCrystal.h>

#define AnalogPin A0

#define TimeStep 1000 // Update bounds every 1000 ms

// Limit declarations; these get updated in SweepLow() and SweepHigh() Functions
#define MID_RANGE 512
int lowerlimit = 250;
int upperlimit = 750;

int highs = 0;
int lows = 0;

// Initialisation for LCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Function declarations
int ReadOut();
void Diffs();
void SetLimits();

///
void setup() {
  Serial.begin(9600);

  lcd.begin(16, 2);
  //lcd.print("hello, world!");
}

void loop()
{
  uint32_t start = millis(); // Starts a Timer for every loop

  SetLimits(); // Sets or updates the limits
  do {
    Diffs(); // Counts low and high waves
  } while(millis() - start < TimeStep); // loops every 1 second

  int bpm = (lows + highs)/2 * 60/TimeStep; // calculates the BPM and prints to LCD
  lcd.clear();
  lcd.print(bpm);
}

// Reads the Signal from AnalogPin
int ReadOut()
{
  int sig = analogRead(AnalogPin);
  return sig;
}


// Sweeps the low range
int SweepLow()
{
  for (int i = MID_RANGE; i > 0; i--)
  {
    if (i > ReadOut())
    {
      return i; // Return i as low bound
      break;
    }
  }
  return 0;
}

// Sweeps the high range
int SweepHigh()
{
  for(int i = MID_RANGE; i < 1023; i++)
  {
    if(i > ReadOut())
    {
      return i;
      break;
    }
  }
  return 0;
}

// Sets the limits
void SetLimits()
{
  lowerlimit = SweepLow();
  upperlimit = SweepHigh();
}

// Counts the high and low Waves
void Diffs()
{
  if (ReadOut() < lowerlimit)
    lows++;

  if (ReadOut() > upperlimit)
    highs++;
}

