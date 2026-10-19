/*
  Happy / sad face on a 16x2 LCD Keypad Shield
  The shield plugs straight onto the Arduino Uno, so there is no wiring.
  Pins used by the shield (fixed): RS=D8, EN=D9, D4=D4, D5=D5, D6=D6, D7=D7,
  backlight=D10, buttons=A0.

  A 16x2 LCD can only draw text, but it allows 8 custom 5x8-pixel characters.
  Each face is drawn with all 8 of them: 4 characters wide x 2 rows, centred.
  Switching faces just reloads those 8 characters, so the screen changes
  instantly without flicker.

  Nothing on the screen? Turn the small blue contrast screw on the shield.
*/

#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

constexpr unsigned long SWITCH_MS = 2000;   // change face every 2 seconds
constexpr uint8_t FACE_COL = 6;             // first column of the 4-wide face

const byte HAPPY_FACE[8][8] PROGMEM = {
  { 0b00000, 0b00000, 0b00001, 0b00010, 0b00100, 0b01000, 0b01000, 0b10000 },   // top 0
  { 0b00111, 0b11000, 0b00000, 0b00000, 0b01100, 0b01100, 0b00000, 0b00000 },   // top 1
  { 0b11100, 0b00011, 0b00000, 0b00000, 0b00110, 0b00110, 0b00000, 0b00000 },   // top 2
  { 0b00000, 0b00000, 0b10000, 0b01000, 0b00100, 0b00010, 0b00010, 0b00001 },   // top 3
  { 0b10000, 0b10001, 0b01000, 0b01000, 0b00100, 0b00010, 0b00001, 0b00000 },   // bottom 0
  { 0b00000, 0b00000, 0b10000, 0b01100, 0b00011, 0b00000, 0b10000, 0b01111 },   // bottom 1
  { 0b00000, 0b00000, 0b00001, 0b00110, 0b11000, 0b00000, 0b00001, 0b11110 },   // bottom 2
  { 0b00001, 0b10001, 0b00010, 0b00010, 0b00100, 0b01000, 0b10000, 0b00000 },   // bottom 3
};

const byte SAD_FACE[8][8] PROGMEM = {
  { 0b00000, 0b00000, 0b00001, 0b00010, 0b00100, 0b01000, 0b01000, 0b10000 },   // top 0
  { 0b00111, 0b11000, 0b00000, 0b00000, 0b01100, 0b01100, 0b00000, 0b00000 },   // top 1
  { 0b11100, 0b00011, 0b00000, 0b00000, 0b00110, 0b00110, 0b00000, 0b00000 },   // top 2
  { 0b00000, 0b00000, 0b10000, 0b01000, 0b00100, 0b00010, 0b00010, 0b00001 },   // top 3
  { 0b10000, 0b10000, 0b01000, 0b01000, 0b00101, 0b00010, 0b00001, 0b00000 },   // bottom 0
  { 0b00000, 0b00011, 0b01100, 0b10000, 0b00000, 0b00000, 0b10000, 0b01111 },   // bottom 1
  { 0b00000, 0b11000, 0b00110, 0b00001, 0b00000, 0b00000, 0b00001, 0b11110 },   // bottom 2
  { 0b00001, 0b00001, 0b00010, 0b00010, 0b10100, 0b01000, 0b10000, 0b00000 },   // bottom 3
};

// Loads one face into the LCD's 8 custom-character slots
void loadFace(const byte face[8][8]) {
  byte rows[8];
  for (uint8_t i = 0; i < 8; i++) {
    memcpy_P(rows, face[i], 8);
    lcd.createChar(i, rows);
  }
}

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  loadFace(HAPPY_FACE);

  // Place characters 0-3 on the top row and 4-7 on the bottom row. Only the
  // character shapes change later, so this is printed just once.
  for (uint8_t i = 0; i < 4; i++) {
    lcd.setCursor(FACE_COL + i, 0);
    lcd.write((uint8_t)i);
    lcd.setCursor(FACE_COL + i, 1);
    lcd.write((uint8_t)(i + 4));
  }
}

void loop() {
  static bool happy = true;
  static unsigned long lastSwitch = 0;

  if (millis() - lastSwitch >= SWITCH_MS) {
    lastSwitch = millis();
    happy = !happy;
    loadFace(happy ? HAPPY_FACE : SAD_FACE);
  }
}
