Demonstration video: https://drive.google.com/file/d/1nlmRzYamGFdrQn9zjMDGmw6A-YF_utnO/view?usp=sharing

Overview 

This project is a fully functional Arduino-based electronic door lock that combines embedded software, electronics, and mechanical actuation. 

The system uses a keypad for authentication, an LCD for user feedback, and a stepper motor with a gear-rack mechanism to physically lock and unlock a door.

The lock is controlled using a finite-state machine to ensure reliable behavior across locked, unlocked, and passcode-change states. The passcode is stored in EEPROM, allowing it to persist even after power loss.

This project was built as a hands-on exercise in embedded systems design and hardware–software integration.

Features

4×4 keypad input for passcode entry

16×2 LCD for real-time system feedback

Stepper motor–driven mechanical locking mechanism

Gear-rack actuation for increased torque and reliability

Secure passcode verification

Passcode change functionality

EEPROM-based passcode storage (non-volatile memory)

Finite-state machine architecture

Reset-to-default passcode option

Error handling for incorrect input and invalid states

System States

The firmware is organized using a finite-state machine with the following states:

LOCKED
Awaits correct 4-digit passcode input to unlock

UNLOCKED
Allows the door to be locked, passcode to be changed, or system reset

CHANGE_PASSWORD
Allows the user to securely enter and save a new passcode

This structure improves code clarity, reliability, and ease of future expansion.

Hardware Used

Arduino Mega (ELEGOO)

4×4 Matrix Keypad

16×2 LCD Display

Stepper Motor with Driver

Gear and Rack Mechanism

Breadboard and Jumper Wires

External Power Supply (for motor)

Software & Libraries

Arduino IDE

C/C++ (Arduino)

Keypad library

LiquidCrystal library

Stepper library

EEPROM library

EEPROM Passcode Storage

The system stores the passcode in Arduino EEPROM so it is retained after power loss.

A magic byte is written to EEPROM to indicate valid initialization. On startup:

If EEPROM is uninitialized or corrupted, the system safely resets to the default passcode (1234)

If valid data exists, the saved passcode is restored

EEPROM writes use EEPROM.update() to minimize memory wear.

How to Use
Unlocking the Door

Enter the 4-digit passcode

Press #

The motor actuates to unlock the door

Locking the Door

Press A while unlocked

Changing the Passcode

Unlock the door

Press C four times (CCCC)

Enter a new 4-digit passcode

Press D to save

Resetting to Default Passcode

Press B while unlocked

The passcode resets to 1234 and is saved to EEPROM

Schematic & Build

A wiring schematic is included in this repository.
Due to simulator limitations, the schematic only represents the logical wiring used for the keypad and LCD and not the stepper motor.

Photos and a demonstration video of the working prototype are included to show the physical build and operation.

What I Learned

Designing embedded systems using finite-state machines

Persisting data using EEPROM

Integrating mechanical, electrical, and software components

Managing user input and error handling

Debugging real hardware systems

Designing for reliability rather than just functionality

Future Improvements

Add Bluetooth (HC-05) for remote access control

Implement lockout after repeated failed attempts

Improve power management and enclosure design
