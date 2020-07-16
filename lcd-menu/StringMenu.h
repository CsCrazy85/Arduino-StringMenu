#include "Arduino.h"
#include <LiquidCrystal.h>

// Some helpful defines
#define LED     13
#define UP      7

// Change buttons if needed
#define DOWN    6
#define LEFT    9
#define RIGHT   8

// LCD properties
#define LCD_CHARS   16
#define LCD_ROWS    2

// Timeouts and menu-related 
#define MENUOPENTIME        3000 // in ms
#define MENUSELECTEDDELAY   800 // in ms
#define MENU_CANCELED           -1
#define MENU_SELECTION_CHANGED  1

void runMenu();
int navigateMenu(int menuPosition, int paramPosition);
void buildMenu();
void printParamValues();
int readButtons();
