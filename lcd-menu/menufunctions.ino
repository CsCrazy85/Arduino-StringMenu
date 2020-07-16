// STEP 4
// Write your functions what you need and how you like them to work
// Note that when the first parameter is selected from the menu, number 1 is returned. Second parameter is number 2 etc...
// Value 0 in parameter value only indicates that no changes have been made.

// Please note that all these are blocking functions and run only once.

// Turns the led on or off
int menuLedFunction(int led)
{
  if(led == 0)
  {
    // No change!
  }
  else if(led == 1)
  {
    digitalWrite(LED,HIGH);
  }
  else if(led == 2)
  {
    digitalWrite(LED,LOW);
  }
}
// Blinks the led
int menuSpeedFunction(int speed)
{
  long flashTime = millis() +5000;
  byte toggle = HIGH;
  while(flashTime > millis())
  {
    switch(speed)
    {
      case 1: // Low
        digitalWrite(LED,toggle);
        delay(1000);
      break;
      case 2: // Medium
        digitalWrite(LED,toggle);
        delay(800);
      break;
      case 3: // Fast
        digitalWrite(LED,toggle);
        delay(200);
      break;
    }
    toggle = !toggle;
  }

  digitalWrite(LED,LOW);
}
int menuBlinkFunction(int toggle)
{
  // Empty on purpose
}
