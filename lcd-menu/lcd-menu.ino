#include <LiquidCrystal.h>
#include "StringMenu.h"

// ARDUINO UNO basic connections for the LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); 

extern int buttons[];
// STEP 1 
// Use the following syntax to add your own menu-system (note only one level of menus is supported so no submenus)
// start with semicolon ;
// then add your menuItem name first, ending with period .
// add desired parameters separated with % (percent) and end parameters with a semicolon ;
// Create another menu if needed following the same princible.

const char *menuItems = 
  ";Led.On%Off%parametri;"
  "Speed.Low%Medium%Fast;"
  "Blink.On%Off;";

// STEP 2
// Create the functions for the main menu items. When parameter has changed, this function is called.
// Please note that the order is relevant in here, since function addresses are stored in a table. 
// You should have at least as many functions declared here than what there's declared in menuItems. If you have more
// functions, it doesn't matter, but if you have less the program will not work properly.
int menuLedFunction(int led);
int menuSpeedFunction(int speed);
int menuBlinkFunction(int toggle);

// STEP 3
// Update or check that the following statement is correct and matches the above order
int (*function[])(int) = {
  menuLedFunction,
  menuSpeedFunction,
  menuBlinkFunction
  };

// STEP 4 
// Write your functions what you need and how you like them to work
// --> go to menufunctions -TAB!

void setup() {
  Serial.begin(115200);
  lcd.begin(LCD_CHARS,LCD_ROWS); 
  lcd.clear();

  pinMode(LED,OUTPUT);
  digitalWrite(LED,LOW);
  lcd.setCursor(0,0);
  lcd.print("Press for menu");
  //lcd.setCursor(2,0);
  buildMenu();
  printParamValues();
  delay(1000);
  
}

void loop() 
{
  runMenu();
}
