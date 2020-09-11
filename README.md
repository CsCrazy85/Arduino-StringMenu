# Arduino StringMenu for character LCDs
Somewhat easy way to create simple LCD menu system with only text.

With this code you can easily create customized menus without writing too much code for the menus itself. 

When creating the menu, there's a few steps involved after which the menus are created on the fly automatically by the processor.

## Step 1
Use the following syntax to add your own menu-system (note only one level of menus is supported so no submenus). 

Start with semicolon ; 

then add your menuItem name first, ending with period .

then add desired parameters separated with % (percent) and end parameters with a semicolon ;

Create another menu if needed following the same princible.

For example:
'const char *menuItems = ";Led.On%Off%param1;"'
