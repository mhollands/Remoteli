## Synopsis

This is the code repository for Remoteli, the world's smallest Infrared TV remote. This code runs on the ATtiny1634 processor.
PCB designs can be found at https://workspace.circuitmaker.com/Projects/6F5BD058-E187-437E-A402-97010F0CBC96.

# Read more at [my website](http://projects.matthollands.com/category/remoteli/)

## Installation

Downloading the whole project and opening Remoteli_v1.atsln in Atmel Studio should work, however this is untested. Otherwise import the .c files individually.
This code was written using Atmel Studio 7.0.594.

## Use
The device operation is relatively simple. Power is toggled using the switch on the top : Left = Off, Right = On. If one of the 11 buttons is held for 1 second immediately after power on, then the device will wait for a new Infrared code to be shon at it. Remoteli will then learn this new code and assign it to the button that was held at power on.
If a button is pressed at any other time then Remoteli will broadcast the command that it has in memory.

## Details
The ATtiny1634 has some quirks as specified in the Errata of it's datasheet. There is an error which prevents pin PB3 from operating correctly as an input if the Watch Dog Timer (WDT) has not been enabled. This is a bug in the silicon. Therefore there are a number of work arounds in the code. The WDT is only enabled in order to work around this bug, it's purpose can generally be ignored, however I explain further in the next paragraph.

The WDT times out every ~8 seconds (chosen by setting appropriate bits in the WDTCSR register). When the WDT times out, it either throws an interrupt or resets the entire device. Resetting the device is of course undesirable. Therefore you can set the WDT to throw an interrupt instead of resetting by setting appropriate bits in the WDTCSR register. However, everytime this interrupt is thrown, this selection is changed so that it will reset the code on the next interrupt. Therefore it is necessary to handle the WDT interrupt and reaffirm the selection to throw an interrupt rather than reset the device.
Furthermore, I found that if the WDT interrupt occured during the input of a new IR command, this caused unexpected results and therefore the watchdog is temporarily disabled while reading a new IR command.

The IR commands are stored in the EEPROM of the device so that they are not lost on power off.

## Pinout
PA2 - Button 1;
PC4 - Button 2;
PA3 - Button 3;
PB1 - Button 4;
PC1 - Button 5;
PA7 - Button 6;
PA1 - Button 7;
PB3 - Button 8 (This is the button with the bug!);
PB0 - Button 9;
PC5 - Button 10;
PB2 - Button 11;
PC0 - Infrared LED;
PA6 - Power Supply to the IR receiver;
PA5 - IR Receiver Signal;

![alt text](https://keepdevelopingprojects.files.wordpress.com/2016/02/hands.jpg)
