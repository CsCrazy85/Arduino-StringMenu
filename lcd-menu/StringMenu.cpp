#include "StringMenu.h"

// Set up buttons
int buttons[] = {UP,DOWN,LEFT,RIGHT};

// Needed for the timeout
volatile long millisNow;

// These are calculated automatically
int menuPositionMax = 0;
int menuParamsMax = 0;
volatile int *menuItemAddresses = NULL;
volatile int *menuParamAddresses = NULL;
volatile int *menuParamValueAddresses = NULL;

extern LiquidCrystal lcd;
extern const char *menuItems;
extern int (*function[])(int);
/*
  Runs the menu and the required button inputs here. Use the functions to do the magic (STEP 1-4).
*/
void runMenu()
{
  // static muuttujille tulee alustus vain kerran
  int menuReturnValue = 0;
  static int menuPosition = 0;
  static int paramPosition = 0;
  static bool menuOpen = false; 
  static bool buttonPressed = false;
  static int btn = 0;

  // Read user input
  btn = readButtons();
  if(btn != 0)
  {
    millisNow = millis();
    lcd.setCursor(0,0);
    lcd.print(F("-> "));
  }
  switch(btn)
  {
    case UP:
      if(menuOpen == true)
      {
        if(menuPosition>1)
        {
          menuPosition--;
        }
      }
    break;
    
    case DOWN:
      menuOpen = true;
      menuPosition++;
      if( menuPosition > menuPositionMax)
      {
        // limit incrementation over the maximum value
        menuPosition = menuPositionMax;
      }
    break;
    
    case LEFT:
      menuOpen = false;
      if(paramPosition>0)
      {
        paramPosition--;
      }
    break;
    
    case RIGHT:
        paramPosition++;
    break;

    case 0:
    break;
    
    default:
      // Timeout after button is pressed
      if((millisNow + MENUOPENTIME) > millis())
      { 
        menuOpen = true;
      }
      else
      {
        menuOpen = false;
      }
      break;
  }
  // Handle the menu
  if(menuOpen == true)
  {
    menuReturnValue = 0;
    menuReturnValue = navigateMenu(menuPosition-1,paramPosition);
    if(MENU_CANCELED == menuReturnValue)
    {
      menuOpen = false;
      menuPosition = 0;
      paramPosition = 0;
    }
    if(MENU_SELECTION_CHANGED == menuReturnValue)
    {
      printParamValues();
      // Here function is called which parameter was changed
      (*function[menuPosition-1])(menuParamValueAddresses[menuPosition-1]);
      menuOpen = false;
      menuPosition = 0;
      paramPosition = 0;
    }
  }
  else
  {
    lcd.clear();
    lcd.print(F("Press for menu"));
    delay(250);
    menuPosition = 0;
    paramPosition = 0;
  }
}
int navigateMenu(int menuPosition, int paramPosition)
{
  static char *menuPointer;
  char offset = 0;
  int i = 0;
  int currentParam = 0;
  int maxParams = 0;
  int charCount=0;
  long millisHere=0;
  bool currentParamSelected = false;
  if(menuPosition < 0)
  {
    return MENU_CANCELED;
  }
  // take the pointer to the main item in the menu 
  menuPointer = (char*)menuItemAddresses[menuPosition];

  // if OK button is pressed (right-button) then we are in the sub menu adjusting the parameters
  if(paramPosition > 0)
  {
    menuPointer = (char*)menuItemAddresses[menuPosition]; // we need to adjust the position because this is not a blocking function
    lcd.setCursor(0,0);
    while(*menuPointer != '.')
    {
      lcd.print(menuPointer[0]);
      charCount++;
      menuPointer++;
    }
    // Clean up the LCD
    lcd.setCursor(charCount,0);
    while(charCount<LCD_CHARS)
    {
      lcd.print(" ");
      charCount++;
    }
    Serial.println(menuPosition,DEC);
    menuPointer = (char*)menuItemAddresses[menuPosition]; // we need to adjust the position because this is not a blocking function
    lcd.setCursor(0,1);
    lcd.print("->");

    // Find out how many parameters we are dealing with and in which position they are
    while(*menuPointer != ';')
    {
      if(*menuPointer == '%')
      {
        maxParams++;
      }
      menuPointer++;
    }
    // reserve space from the memory
    menuParamAddresses = (int *) malloc((maxParams+1) * sizeof(int));

    // set pointer back to correct menuPosition 
    menuPointer = (char*)menuItemAddresses[menuPosition]; 
    
    // take the parameter addresses and use them to cycle around
    while(*menuPointer != ';')
    {
      if(*menuPointer == '.')
      {
        menuParamAddresses[i] = menuPointer + 1; // first parameter is after the dot always 
        if( (*menuPointer + 1 != '%') && (*menuPointer + 1 != '.'))
        {
          // check if the parameters actually exists
          maxParams = 1;
        }
      }
      if(*menuPointer == '%')
      {
        maxParams++;
        menuParamAddresses[++i] = menuPointer + 1; // other parameters start always after the % sign
      }
      menuPointer++;
    }
    // reset pointer
    menuPointer = (char*)menuItemAddresses[menuPosition-1];
    // now we know how many parameters we should scroll in the display

    // we should read the buttons and loop the parameters with up and down until right or left is pressed
    // right = OK, left = cancel
    millisHere = millis();

    menuPointer = (char*)menuParamAddresses[0]; // position of the first parameter
    while(1)
    {
      int button = readButtons();
      if(button != 0)
      {
        millisHere = millis();
      }
      lcd.setCursor(0,1);
      lcd.print("->");
      switch(button)
      {
        case UP:
            if(currentParam > 0)
            {
              currentParam--;
            }
        break;
        case DOWN:
            currentParam++;
            if(currentParam>(maxParams-1))
            {
              currentParam = (maxParams-1);
            }
        break;
        case LEFT: // CANCEL
            lcd.setCursor(0,1);
            lcd.print(F("Canceling!"));
            delay(MENUSELECTEDDELAY);
            lcd.clear();
            return MENU_CANCELED;
        break;
        case RIGHT:
            // selection changed do something... 
            lcd.setCursor(0,1);
            lcd.print(F("Selected!"));
            menuParamValueAddresses[menuPosition] = currentParam +1; // add one to the value so that nothing selected by the user can be distinguished
            delay(MENUSELECTEDDELAY);
            return MENU_SELECTION_CHANGED;
        break;
      }
      menuPointer = (char*)menuParamAddresses[currentParam]; // position of the selected parameter
      // print the parameter toggled
      charCount =2;
      while(*menuPointer != ';')
      {
        if(*menuPointer == '%')
        {
          break;
        }
        lcd.print(menuPointer[0]);
        menuPointer++;
        charCount++;
      }
      //charCount = (int) (menuPointer - (char*) menuParamAddresses[currentParam]);
      // Clean up the LCD
      lcd.setCursor(charCount,1);
      while(charCount<LCD_CHARS)
      {
        lcd.print(" ");
        charCount++;
      }
     
      if((millisHere + (MENUOPENTIME)) > millis())
      {
          delay(100);
      }
      else
      {
        // end the loop because of timeout
        break;
      }
    }
    Serial.println(F("Exit navi!"));
  }
  else
  {
    menuPointer = (char*)menuItemAddresses[menuPosition];
    // Scroll only through main menu level
    lcd.setCursor(0,0);
    lcd.print(F("-> "));
    i=3;
    while(*menuPointer != '.')
    {
      i++;
      lcd.print(menuPointer[0]);
      menuPointer++;
    }
    // Clean up the LCD
    while(++i<LCD_CHARS)
    {
      lcd.print(" ");
    }
  }

  return 0;
  
}
/*
  Constructs the menu from the string given in the beginning of this file
*/
void buildMenu()
{
  char *menuPointer;
  const char baseMenuSeparator[] = ".";
  char *token;
  char single;
  int ParamCount = 0;
  unsigned int i=0;
  unsigned int addr;
  menuPointer = menuItems;

  for(int i=0;i<(sizeof(buttons)/sizeof(buttons[0]));i++)
  { 
    // Init all the buttons
    pinMode(buttons[i],INPUT_PULLUP);
  }

  if(*menuPointer != ';')
  {
    Serial.println(F("Menu spelled incorrectly! Should start with ;."));
    return;
  }
  else
  {
    menuPointer++;
  }
  
  // Find out how many menus we have
  while(*menuPointer)
  {
    if(*menuPointer == ';')
    {
      menuPositionMax++;
      // Fix the amount due to the syntax
      ParamCount +=1;
    }
    if(*menuPointer == '%')
    {
      // Count total number of parameters
      ParamCount++;
    }
    menuPointer++;
  }
  menuParamsMax = ParamCount;
  // Memory allocation
  if(menuItemAddresses == NULL)
  {
      menuItemAddresses = (int *) malloc((menuPositionMax) * sizeof(int));  
      menuParamValueAddresses = (int *) malloc((menuPositionMax) * sizeof(int));  
  }
  
  if(menuItemAddresses != NULL)
  {
    Serial.print(F("Memory allocated. ParamCount = "));
    Serial.println(menuParamsMax,DEC);
  }
  else
  {
    Serial.println(F("Menu failed."));
  }
  Serial.print(F("\n\rOriginal Menu at address "));
  addr = (void *) &menuItems[0];
  Serial.println(addr,DEC);
  // Get the addresses
  menuPointer = menuItems;
  Serial.print(F("Searching from: "));
  Serial.println((char *)menuItems);
  while(*menuPointer)
  {
    if(*menuPointer == ';')
    {
      menuItemAddresses[i] = (menuPointer+1);
      if(*(menuPointer + 1) == '\0')
      {
        break;
      }
      token = (char*)menuItemAddresses[i];
      Serial.print(F("Found menu at address"));
      Serial.print((long)menuPointer,DEC);
      Serial.print(" which is: ");
      Serial.println(token[0]);
      
      while(*menuPointer != '.')
      {
        menuPointer++;
        if(*menuPointer != '.')
        {
          token = (char*)menuPointer;
          Serial.print(token[0]);
        }
      }
      Serial.println();
      i++;
    }
    menuPointer++;
  }

  memset((int*)menuParamValueAddresses,0x0000,(sizeof(int) * menuPositionMax));

}

/*
  Useful for checking the values in the memory
*/
void printParamValues()
{
  char numeric[4];
  Serial.println(F("Parameter values are:"));
  for(int i=0; i< menuPositionMax;i++)
  {
    Serial.print(F("Menu item "));
    Serial.print(i,DEC);
    Serial.print(F(": ["));
    sprintf(numeric,"%04X",menuParamValueAddresses[i]);
    Serial.print(numeric);
    Serial.print(F("], "));
  }
  Serial.println();
}

/*
  Reads the buttons here
*/
int readButtons()
{
  for(int i=0;i<(sizeof(buttons)/sizeof(buttons[0]));i++)
  {
    if(digitalRead(buttons[i]) == LOW)
    {
      // Nappi painettu, palautetaan mikä nappi oli painettu. Jos nappia pidetään pohjassa niin odotetaan sen vapautumista.
      while(digitalRead(buttons[i]) == LOW); // HUOM! Mahdollisuus jumi-luuppiin, jos nappi on oikosulussa.
      delay(100);
      return buttons[i];
    }
  }
  return 0; // mitään nappia ei painettu joten palautetaan nolla
}
