  Arduino Keypad Door Lock
  Overview:

  This project is a fully functional Arduino-based electronic door lock combining embedded software, electronics, and mechanical actuation.

  The system uses a 4×4 keypad for authentication, a 16×2 LCD for user feedback, and a stepper motor with a gear-rack mechanism to physically lock and unlock the door. 
  A finite-state machine controls system behavior, and the passcode is stored in EEPROM to persist across power loss.

  Features:

- 4×4 keypad passcode entry

- 16×2 LCD real-time feedback

- Stepper motor–driven locking mechanism

- Gear-rack actuation for increased torque

- Secure passcode verification

- Passcode change and reset functionality

- EEPROM-based passcode persistence

- Finite-state machine architecture

- Error handling for invalid input
  

  System States:

- The firmware operates using three states:

- LOCKED – Awaits correct 4-digit passcode

- UNLOCKED – Allows locking, passcode change, or reset

- CHANGE_PASSWORD – Securely updates the passcode

  This structure improves reliability, readability, and future scalability.


  Hardware Used:

- Arduino Mega (ELEGOO)

- 4×4 Matrix Keypad

- 16×2 LCD Display

- Stepper Motor with Driver

- Gear and Rack Mechanism

- Breadboard and Jumper Wires

- External Motor Power Supply


  Software & Libraries:

- Arduino IDE

- C/C++ (Arduino)

- Keypad

- LiquidCrystal

- Stepper

- EEPROM


  EEPROM Passcode Storage

  The passcode is stored in EEPROM so it remains after power loss.

  On startup:

- If EEPROM is uninitialized or invalid, the system resets to the default passcode (1234)

- If valid data exists, the stored passcode is loaded

  EEPROM.update() is used to reduce memory wear.


  How to Use:
  Unlocking:

- Enter the 4-digit passcode

- Press #

- Motor actuates to unlock

  Locking:

- Press A while unlocked

  Changing the Passcode:

- Unlock the door

- Press C four times (CCCC)

- Enter a new 4-digit passcode

- Press D to save

  Reset to Default:

- Press B while unlocked

- Passcode resets to 1234 and is saved to EEPROM

  Schematic & Build:

  A wiring schematic is included in this repository. Due to simulator limitations, it represents the logical wiring for the keypad and LCD only.

  Photos and a demo video show the physical build and operation.


  What I Learned

- Finite-state machine design in embedded systems

- EEPROM data persistence

- Hardware–software integration

- User input validation and error handling

- Debugging real embedded hardware

- Designing for reliability
