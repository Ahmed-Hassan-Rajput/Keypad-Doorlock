#include <Stepper.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <EEPROM.h>

const int UNLOCK_STEPS = 550; 
const int LOCK_STEPS   = 550; 

int motorSpeed = 10;
Stepper myStepper(2048, 50, 52, 51, 53); // IN1, IN3, IN2, IN4

// LCD: rs=49, en=47, d4=45, d5=43, d6=41, d7=39
LiquidCrystal lcd(49, 47, 45, 43, 41, 39);

// Keypad 4x4
const byte ROW = 4, COLM = 4;

char values[ROW][COLM] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte pinsROW[ROW]   = {37, 35, 33, 31};
byte pinsCOLM[COLM] = {29, 27, 25, 23};

Keypad myKeypad = Keypad(makeKeymap(values), pinsROW, pinsCOLM, ROW, COLM);

enum Mode { UNLOCKED, CHANGE_PWRD, LOCKED };
Mode mode = LOCKED;

String input = "";
String setPwrd = "";
String newPwrd = "";

String change = "CCCC";

// EEPROM settings
const String DEFAULT_PASS = "1234";
const int EEPROM_MAGIC_ADDR = 0;   // 1 byte
const int EEPROM_PASS_ADDR  = 1;   // 4 bytes after magic
const byte EEPROM_MAGIC     = 0xA7;

String password = DEFAULT_PASS;

bool isFourDigits(const String &s) {
  if (s.length() != 4) return false;
  for (int i = 0; i < 4; i++) {
    if (s[i] < '0' || s[i] > '9') return false;
  }
  return true;
}

void savePasswordToEEPROM(const String &pw) {
  EEPROM.update(EEPROM_MAGIC_ADDR, EEPROM_MAGIC);
  for (int i = 0; i < 4; i++) {
    EEPROM.update(EEPROM_PASS_ADDR + i, pw[i]);
  }
}

String loadPasswordFromEEPROM() {
  if (EEPROM.read(EEPROM_MAGIC_ADDR) != EEPROM_MAGIC) {
    savePasswordToEEPROM(DEFAULT_PASS);
    return DEFAULT_PASS;
  }

  char buf[5];
  for (int i = 0; i < 4; i++) {
    buf[i] = (char)EEPROM.read(EEPROM_PASS_ADDR + i);
    if (buf[i] < '0' || buf[i] > '9') {
      savePasswordToEEPROM(DEFAULT_PASS);
      return DEFAULT_PASS;
    }
  }
  buf[4] = '\0';
  return String(buf);
}

void setup() {
  myStepper.setSpeed(motorSpeed);

  lcd.begin(16, 2);

  // Load saved password from EEPROM (or initialize to default)
  password = loadPasswordFromEEPROM();

  lcd.setCursor(0, 0);
  lcd.print("Keypad Doorlock");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ready");
  delay(800);
  lcd.clear();
}

void loop() {
  char key = myKeypad.getKey();
  if (key == NO_KEY) return;

  // -------------------------
  // CHANGE PASSWORD MODE
  // -------------------------
  if (mode == CHANGE_PWRD) {

    if (key >= '0' && key <= '9') {
      if (newPwrd.length() < 4) {
        newPwrd += key;
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(16 - newPwrd.length(), 1);
        lcd.print(newPwrd);
      }
      return;
    }

    if (key == 'D') {
      if (newPwrd.length() == 4) {
        password = newPwrd;
        savePasswordToEEPROM(password); // save persistently

        lcd.clear();
        lcd.print("Password Saved");
        delay(1500);
        lcd.clear();

        mode = UNLOCKED;
        newPwrd = "";
        setPwrd = "";
        input = "";
      } else {
        lcd.clear();
        lcd.print("Need 4 digits");
        delay(1500);
        lcd.clear();
        lcd.print("Enter New Code");
        lcd.setCursor(0, 1);
        lcd.print("Press D to save");
        newPwrd = "";
      }
      return;
    }

    if (key == '*') {
      mode = UNLOCKED;
      newPwrd = "";
      setPwrd = "";
      lcd.clear();
      lcd.print("Cancelled");
      delay(1000);
      lcd.clear();
      return;
    }

    return;
  }

  // -------------------------
  // UNLOCKED MODE
  // -------------------------
  if (mode == UNLOCKED) {

    // Enter change mode trigger: CCCC
    if (key == 'C') {
      setPwrd += 'C';
      lcd.setCursor(0, 0);
      lcd.print("                ");
      lcd.setCursor(16 - setPwrd.length(), 0);
      lcd.print(setPwrd);

      if (setPwrd == change) {
        mode = CHANGE_PWRD;
        newPwrd = "";
        setPwrd = "";

        lcd.clear();
        lcd.print("Enter New Code");
        lcd.setCursor(0, 1);
        lcd.print("Press D to save");
      }
      return;
    }

    // reset password to default while unlocked using B
    if (key == 'B') {
      password = DEFAULT_PASS;
      savePasswordToEEPROM(password);

      lcd.clear();
      lcd.print("Reset to 1234");
      delay(1200);
      lcd.clear();

      input = "";
      setPwrd = "";
      newPwrd = "";
      return;
    }

    // Lock with A
    if (key == 'A') {
      lcd.clear();
      lcd.print("Locking");

      myStepper.step(LOCK_STEPS);
      mode = LOCKED;

      lcd.clear();
      lcd.print("Locked");
      delay(1000);
      lcd.clear();

      input = "";
      setPwrd = "";
      newPwrd = "";
      return;
    }

    // Clear trigger buffer
    if (key == '*') {
      setPwrd = "";
      lcd.clear();
      return;
    }

    return;
  }

  // -------------------------
  // LOCKED MODE
  // -------------------------

  // Digits for unlock attempt (only when locked)
  if (key >= '0' && key <= '9') {
    if (input.length() < 4) {
      input += key;
      lcd.setCursor(0, 0);
      lcd.print("                ");
      lcd.setCursor(16 - input.length(), 0);
      lcd.print(input);
    }
    return;
  }

  // Submit unlock
  if (key == '#') {
    lcd.clear();

    if (input == password) {
      lcd.print("Unlocking");

      myStepper.step(-UNLOCK_STEPS);
      mode = UNLOCKED;

      lcd.clear();
      lcd.print("Unlocked");
    } else {
      lcd.print("Incorrect");
    }

    delay(1000);
    lcd.clear();
    input = "";
    return;
  }

  // Clear input
  if (key == '*') {
    input = "";
    lcd.clear();
    return;
  }
}
