# Arduino StringMenu for character LCDs
Somewhat easy way to create simple LCD menu system with only text.

With this code you can easily create customized menus without writing too much code for the menus itself. 

When creating the menu, there's a few steps involved after which the menus are created on the fly automatically by the processor.

## Step 1
Use the following syntax to add your own menu-system (note only one level of menus is supported so no submenus). 

* Start with semicolon ; 
* then add your menuItem name first, ending with period .
* then add desired parameters separated with % (percent) and end parameters with a semicolon ;
* Create another menu if needed following the same princible.

For example:

";Led.On%Off%param1;"

This string is stored in a variable called menuItems.

## Step 2
Create the functions for the main menu items. When parameter has changed, this function is called. Please note that the order is relevant in here, since function addresses are stored in a table. You should have at least as many functions declared here than what there's declared in menuItems. If you have more functions, it doesn't matter, but if you have less the program will not work properly.

## Step 3
Update or check that the function pointer table, which holds the names of the menus are correct and matches the actual function names.

## Step 4
Write your functions what you need and how you like them to work
Note that when the first parameter is selected from the menu, number 1 is returned. Second parameter is number 2 etc...
Value 0 in parameter value only indicates that no changes have been made.
