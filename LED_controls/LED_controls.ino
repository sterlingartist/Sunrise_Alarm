int duty = 0;
int steps = 64;
//this is the human readable number of minutes for duration of sunrise
int minutes_of_sunrise = 30;
//187.5 is the multiplier necessary to get the math of the PWM and number of light levels to equate to minutes in real life 
int numberofpulses = minutes_of_sunrise * 187.5;
int i;
int j;
int LED = 5;

int lookup[64] = {1,2,4,6,9,12,16,20,25,30,36,
42,49,56,64,72,81,90,100,110,121,132,
144,156,169,182,196,210,225,240,256,272,289,
306,324,342,361,380,400,420,441,462,484,506,
529,552,576,600,625,650,676,702,729,756,784,
812,841,870,900,930,961,992,992,992};


// there are freq steps in the phasing
void setup()
{
  pinMode(LED, OUTPUT);
}

void loop()
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
