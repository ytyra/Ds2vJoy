﻿# Ds2vJoy
DualShock™ and DualSense™ to vJoy or ViGEm for Windows 64 bits only
- Bluetooth compatible
- Vibration compatible
  (If you select vJoy or ViGEm from the game, the operation of DS4 will be reflected)
- Combinations, macros and time based operations allowed
- Control complexe mouse operations
- HidGuardian included to let the game only see the needed vJoy or ViGEm joystick

## How to use
1. Install vJoy (https://sourceforge.net/projects/vjoystick/)
   or not if you plan to use ViGEm
2. Open 'Configure vJoy' and select Axes, up to 128 Buttons and up to 4 continuous POV and effects
   It may result in a PC restart.
3. Launch Ds2vJoy.exe
   an .ini file will be created, to save parameters, with some examples already filled
   and needed .dll, whose will be destroyed when you'll exit
4. Connect your PC and DS4/DS5 via USB or Bluetooth 
5. Configure as you wish (you have 3 profiles accessible by system tray)

- It is okay to connect DS4 after starting the game. VJoy and ViGEm are always visible in the game
- When you change a setting, Ds2vJoy.ini is automatically and instantly saved
- Ds2vJoy is portable, you an use multiple copy in diffrent directories with differents parameters
- Install for ViGEm, HidGuardian and HidCerberus are included,
  they will use subdirectories of C:\Program Files\Ds2vJoy
  removed as soon as tou chose to uninstall them from Ds2vJoy menu tabs,
  the same for C:\Program Files\Ds2vJoy itself when no required anymore
- You can see which button is pressed by launching Monitor vJoy
  but when under vJoy and ViGEm tabs,   you can see them directly in status bar
  to easily test quick configurations

## Log
![Log](Doc/1.png)

## Log
![Settings](Doc/2.png)
- Choose between prefered Sony® joypad, vJoy device in use
  but for example if you chosed DS5 and only DS4 is conneected
  this one will be in used
  You may have to restart the program to change apply
- Usual Configurations
- Triggers only works with DS5
  May be used only for touch pleasure
  But in ocnjonction with L2, L2LOW, L2HIGH, L2FULL (same for R2),
  you may use distinctly 2 or 3 separate actions on the same trigger
- You can specify a serial if you have multiples DS connected
  or/and multiple copies of Ds2vJoy running
- You can fix thresold of sticks, and set times more acurate to your senses
  Simultaneous is R1L1 or R2L2 the same time (with this 40ms as default delta)
- Multiple touchpad divide in multiple buttons,
  But for more simple uses, there is multiple special buttons (see vJoy)
- Led is calm wave fluctuating, and can color be stopped on exit

## vJoy
![vJoy](Doc/3.png)
Each line correspond to a different mapping

Order, add, delete, copy or edit (double click or menu)
Mappings are treated in order,
  Then you can decide for compelexe actions or shifts
  For example you can use L1, and even L1R1 or more complexe situations
  to determine which action square button will launch

- DS Button:
    + Indicate combination
	- Indicate this mapping won't be lauched if this button is pressed
	Already configured vJoy buttons can be used as source, in conjunctions with DS buttons
- vJoy Button:
    See a brief of what will be launched like vJoy, mouse action (different from mouse) 
- Tags:
	I: if mouse is actually in use
	F: force this mapping even if a source button has been disabled by another
	S: short press
	D: double press
	L: long press
	T: this is a toggle mapping
	R: an action will be effectued on release
	M: if Mouse will be in used

![vJoy Editing](Doc/4.png)
First row is source
Solo is central led and battery status
Height ones are destination
Group of checkboxes is for mouse
Group of numbers is either for special mouse actions,
  or to use mouse in a grid (divided or not) and not the full screen

Over first source you can find special requirments
- If mouse, if a mouse is already in use
- And Force, to ignore if a button has been disabled by a precedent mapping
    double, this will work only if this mapping is already working

Over two next sources you'll find or/xor conditions instead of simple combination

Over two alst sources you'll find not condition,
  the mapping won't be lauched if this button is pressed
  double, except if this mapping is already running

Desitinations can be effectued with time stamps for tricky actions, macros or combos
  Enter start (if not as soon as) or/and end (of not when release)
- If you enter an odd number, 0-9 millisesonds will be added, different each time
- Middle check box, is to begin the action on release
   double, only the up press, even if some other timestamps are finished
  
Under each source and destination, you'll find disabling
  this button will be disabled for next mappings if this mapping is running (except with Force)
  double (only destination), if time stamp is in use,
    disabling will be effective until whole mapping is finished instead this destination only

Under first source you'll find 3 checkboxes which determine the method
- None: Simple, start when pressed, stop when release
- First: Short*, you have to press and release quickly
- Second: Double, you'll have to double press in a limited time
- Third: Long, you have to stay pressed from a certain time
- 1 & 2: Double short*, second press acting like short
- 2 & 2: Double long, second press acting like long
- 1 & 3: Medium long, like long but after double long
- all: Very long

  * you'll have to use timestamp, otherwize you can simply use on release or go to timestamp too

In terms of time:
- Simple		Immediate
- Short			found in settings
- Double		~first press and second press < long
- Long			found in settings
- Double		~first press and second press < long + second press duration < short
- Double short*	~first press and second press < long + second press duration > long
- Medium long	~press > twice the time as long
- Very long		found in settings

Under central led, you'll find 2 checkboxes
- Macro: Interrupt macros on release (even if timestamp not finsihed)
    double, pause this mapping while a not condition istead of interrupting
- Toggle: satisfy mapping conditions one time to start, second one to stop
    double, destinations states begin activated at program launch
	can be used to time based mouse actions too,
	and for mouse if ACTIVE_MOUSE mous eaction is used (see below)

At the left of first 4 destinations, you'll find special mouse actions
- ACTIVE_MOUSE: use to bring the chosen mouse
    (from the bottom group of checkboxes)
    into the timestamp or make it responsive to the toggle
- SAVE_POSITION: Save the mouse position
- MOVE_BACK: return to saved point
- MOVE_TO_XY/MOVE_TO_WH: Go to x,y/w,h (two left/central numbers of the grid)
- SAVE_AND_MOVE_TO_XY/SAVE_AND_MOVE_TO_WH: both
- LEFT_CLICK, MIDDLE_CLICK, RIGHT_CLICK, X1_CLICK, X2_CLICK, CROLL_UP, SCROLL_DOWN
- LEFT_DOUBLE_CLICK, MIDDLE_DOUBLE_CLICK, RIGHT_DOUBLE_CLICK, X1_DOUBLE_CLICK, X2_DOUBLE_CLICK, SCROLL_UP_x5, SCROLL_DOWN_x5
- LEFT_DOWN, MIDDLE_DOWN, RIGHT_DOWN, X1_DOWN, X2_DOWN
    (long press, can use timestamp and on release)
- SCROLL_UP_VARIABLE, SCROLL_DOWN_VARIABLE
    if on trigger, variable speed following the pressure force, otherwise exponential
	
The mouse group of checkboxes at the bottom, elt you use mouse
- With left stick (left column)
- With the touchpad (central column)
- With the right stick (right column)
There is different modes for each
- absolute, you move at a point which correspond to the coordinates of the stick or touchpad
- mouse, as a normal mouse with acceleration
- move (stick) or slow (touchpad), the same but adapted for game movements, or precise mouse
- sniper (stick) or accuracy (touchpad), extreme precision
- raid (stick) or grid (touchpad), move inside a zone whom coordinates are entered in numbers group at the right
  left ones are for first points, central ones for second point,
  two last are to divide the zone in a grid, then you are always docked to a point of this grid
Last solo checkbox is for when you have together raid/grid and another mouse mode active
  which one is the default one

  2nd mapping: DS L2R2 -(simple) vJoy 128 active right stick mouse move
 
An example of a simple advanced script for mouse:
  1st mapping: DS L2R2 + DS R2 activate right stick mouse
  2nd mapping: DS L2R2 - DS R2 activate right stick snipper
  Then as soon as you enter L2R2 your right stick is activated for mouse
    if you release only R2 you enter snipper mode,
    if you press back R2 you get again quick mouse
    until both L2 and R2 are released,
	which can therefore be used for usual actions
    (you may differ them 40ms and add -(simple)L2R2

An example of advanced script for targeting:
  1st mapping: DS L2R2 -(double) DS R2 active vJoy 128
    when you first press L2R2 nothing happen as R2 is pressed
	as soon as you realease only R2, the conditions is respected, and then button 128 is pressed
	as R2 is double checked, it doesn't stop the mapping while already running,
	so until you release both L2 and R2, 128 stay pressed
	(you could even press R2 and release only L2 for other actions and come back after to only L2)
  2nd mapping: vJoy 128 + DS R2 do target forward target (you place on the button you wish)
  Do the same for the left, with button 127 and target backward target (you place on the button you wish)
    and then until you release Both L2 & R2, you can easily swap target
    and individual L2 and R2 stay free to be affected to usual actions
    (you may differ them 40ms and add -(simple)L2R2

![vJoy Help](Doc/5.png)
- You can find this help directly in Ds2vJoy by pressing help below grid entries

## RapidFire
![RapidFire](Doc/6.png)
You can set repeated hits with the assigned vJoy button
Order, add, delete, copy or edit (double click or menu)
- After the first time has passed, release and press repeatedly
- If you specify the simultaneous press condition,
    you can also enable repeated hits of the cross key and □ △ × ○,
	for example, only when L2 or R2 is pressed
![RapidFire Editing](Doc/7.png)
Removed DS4vJoy/DS5vJoy limits of time

## Keymap
![Keymap](Doc/8.png)
You can send a keydown when the button is pressed and a keyup when it is released
Order, add, delete, copy or edit (double click or menu)
![Keymap Editing](Doc/9.png)
You can enter directly keys in the Enter Keycode part, or use Send Keycode choices (for advanced use)
Send to, permit you to specify the process, and will ungrey (disabled)Find button
If you check last checkbox, this will activate the window first (if post method is unsuported)
![Keymap Window Post](Doc/10.png)
You will find.

## ViGEm
![ViGEm](Doc/11.png)
Install, remove and use Nefarius&co ViGEm separately or jointly to vJoy
Then you can have a fully remaped DS4 or X360
Or a simple copy of DS4/DS5 to X360, to simplify compatibility problems
Checkboxes are made to swap betweeen DS or vJoy as source

## Guardian
![Guardian](Doc/12.png)
Install, remove and use Nefarius&co HidGuardian and HidCerberus (and stop/start this service).
A restart may be necessaire after installing Guardian to have it properly running
You can find link to manual configuration of Cerberus,
In which you can verify the HID\ to enter in the blacklist section
Then you can hide your DS4/DS5 and let see only the remaped DS4/X360 or vJoy to the system
Whitelist section, bypass the gardian for programs you need to achieve this

## Links
![Links](Doc/13.png)
You'll find source inspiration
And you can enter from 1 to 5 programs name and location
which can be launched aftre that is the systray menu

## Tray
![Tray](Doc/14.png)
Picture talk by itself

## Efficiency
![Efficiency](Doc/15.png)
There is no resize but some other stuff

## Thanks
https://github.com/090 (a lot)
https://sourceforge.net/projects/vjoystick/
https://github.com/ViGEm/ViGEmBus
https://github.com/ViGEm/ViGEmClient
https://github.com/ViGEm/HidGuardian
https://github.com/ViGEm/HidCerberus
https://github.com/Jays2Kings/DS4Windows

## Development environment
- Win10 64bit + Visual Studio 2019 (Community is fine)
- Sony® DualShock™ and DualSense™
- Intel® Wireless Bluetooth from Asus ROG
