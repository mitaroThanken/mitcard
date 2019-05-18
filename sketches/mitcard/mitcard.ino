#include <TeensyView.h>
#include <Keyboard.h>
#include <Bounce2.h>

// for Keyswiches
const int ROW_NUM = 2;
const int COL_NUM = 2;
const int ROW_PIN[ROW_NUM] = { 3, 2 };
const int COL_PIN[COL_NUM] = { 4, 5 };

Bounce * debounce[ROW_NUM] = { new Bounce[COL_NUM], new Bounce[COL_NUM] };

const int KEY_MAP[ROW_NUM][COL_NUM] = {
  { KEY_A, KEY_B },
  { KEY_1, KEY_2 }
};

const String KEY_TITLE[ROW_NUM][COL_NUM] = {
  { "A?", "B!" },
  { "1.", "2.." }
};

// for Touch
const int TOUCH_B_PIN = 16;
const int TOUCH_L_PIN = 22;
const int TOUCH_R_PIN = 23;
const int TOUCH_DIFF = 500;

int touchB_avg = 0;
int touchR_avg = 0;
int touchL_avg = 0;
int touchB_th = 0;
int touchR_th = 0;
int touchL_th = 0;
bool touchB_touched = false;
bool touchL_touched = false;
bool touchR_touched = false;

void renew_touch_avg() {
  int t = touchRead(TOUCH_B_PIN);
  touchB_avg = (touchB_avg >> 1) + (touchB_avg >> 2) + (t >> 2);
  t = touchRead(TOUCH_L_PIN);
  touchL_avg = (touchL_avg >> 1) + (touchL_avg >> 2) + (t >> 2);
  t = touchRead(TOUCH_R_PIN);
  touchR_avg = (touchR_avg >> 1) + (touchR_avg >> 2) + (t >> 2);
}

// for LED
const int LED_PIN = 13;
int ledState = LOW;

// for TeensyView
// cf. https://learn.sparkfun.com/tutorials/teensyview-hookup-guide
#define PIN_RESET 15
#define PIN_DC    21
#define PIN_CS    10
#define PIN_SCK   13
#define PIN_MOSI  11

TeensyView oled(PIN_RESET, PIN_DC, PIN_CS, PIN_SCK, PIN_MOSI);
elapsedMillis sincePrint;

const String MY_NAME = "mitarothanken";
const String THIS_CARD = "mitcard!";

// Center and print a small title
// This function is quick and dirty. Only works for titles one
// line long.
void printTitle(String title, int font)
{
  int middleX = oled.getLCDWidth() / 2;
  int middleY = oled.getLCDHeight() / 2;

  oled.clear(PAGE);
  oled.setFontType(font);
  // Try to set the cursor in the middle of the screen
  oled.setCursor(middleX - (oled.getFontWidth() * (title.length() / 2)),
                 middleY - (oled.getFontWidth() / 2));
  // Print the title:
  oled.print(title);
  oled.display();
  //delay(1500);
  //oled.clear(PAGE);
  sincePrint = 0;
}

void setup(void) {
  // LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ledState);

  // TeencyView (splash screen)
  oled.begin();
  oled.clear(ALL);
  oled.display();

  // Keyswitches (matrix)
  for (int row = 0; row < ROW_NUM; row++) {
    pinMode(ROW_PIN[row], OUTPUT);
  }

  for (int col = 0; col < COL_NUM; col++) {
    pinMode(COL_PIN[col], INPUT_PULLUP);
  }

  for (int row = 0; row < ROW_NUM; row++) {
    for (int col = 0; col < COL_NUM; col++) {
      debounce[row][col].attach(COL_PIN[col]);
      debounce[row][col].interval(25);
    }
    digitalWrite(ROW_PIN[row], HIGH);
  }

  // Touch (init)
  int i = 0;
  int t = 0;
  for (; i < 4; i++) {
    t = touchRead(TOUCH_B_PIN);
    touchB_avg += (t >> 2);
    t = touchRead(TOUCH_L_PIN);
    touchL_avg += (t >> 2);
    t = touchRead(TOUCH_R_PIN);
    touchR_avg += (t >> 2);
  }
  for (; i < 100; i++) {
    renew_touch_avg();
  }

  // FIXME TOUCH_DIFF
  touchB_th = touchB_avg + (TOUCH_DIFF >> 2);
  touchL_th = touchL_avg + TOUCH_DIFF;
  touchR_th = touchR_avg + TOUCH_DIFF;

  // Random seed.
  randomSeed(A0);

  // for Debug
  Serial.begin(9600);

  // Keyboard begin.
  Keyboard.begin();

  // TeencyView (clear)
  sincePrint = 0;
}

int delaycount = 0;
bool needToToggleLed = true;

void scanKeyswitches(void) {
  for (int row = 0; row < ROW_NUM; row++) {
    digitalWrite(ROW_PIN[row], LOW);

    for (int col = 0; col < COL_NUM; col++) {
      debounce[row][col].update();
      if (debounce[row][col].fell()) {
        printTitle(KEY_TITLE[row][col], 1);
        Serial.print("[");
        Serial.print(row);
        Serial.print(", ");
        Serial.print(col);
        Serial.print("] ");
        Serial.println("Pushed.");
        Keyboard.press(KEY_MAP[row][col]);
        needToToggleLed = true;
      } else if (debounce[row][col].rose()) {
        Serial.print("[");
        Serial.print(row);
        Serial.print(", ");
        Serial.print(col);
        Serial.print("] ");
        Serial.println("released.");
        Keyboard.release(KEY_MAP[row][col]);
        needToToggleLed = true;
      }
      if (needToToggleLed) {
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState);
        needToToggleLed = false;
      }
    }
    digitalWrite(ROW_PIN[row], HIGH);
  }
}

void scanTouches(void) {
  renew_touch_avg();
  if (touchL_avg > touchL_th) {
    if ((!touchL_touched) && (touchB_avg > touchB_th)) {
      touchL_touched = true;
      Serial.println("L Touched.");
      Keyboard.print(MY_NAME);
      printTitle(MY_NAME, 0);
    }
  } else if (touchL_touched) {
    Serial.println("L released.");
    touchL_touched = false;
  }
  if (touchR_avg > touchR_th) {
    if ((!touchR_touched) && (touchB_avg > touchB_th)) {
      touchR_touched = true;
      Serial.println("R Touched.");
      Keyboard.print(THIS_CARD);
      printTitle(THIS_CARD, 1);
    }
  } else if (touchR_touched) {
    Serial.println("R released.");
    touchR_touched = false;
  }
}

const int oledInterval = 5000;

void oledClearOrDemo(void) {
  if (sincePrint > oledInterval * 6) {
    oled.clear(PAGE);
    oled.display();
    sincePrint = 0;
  } else if (sincePrint > oledInterval * 5) {
    for (int i = 0; i < 64; i++) {
      oled.pixel(random(oled.getLCDWidth()), random(oled.getLCDHeight()), WHITE, XOR);
    }
    oled.display();
  } else if (sincePrint > oledInterval * 4) {
    oled.clear(PAGE);
    oled.setFontType(1);
    oled.setCursor(0,0);
    oled.print("Touch");
    oled.setCursor(0, oled.getFontHeight());
    oled.print("white squares.");
    oled.display();
  } else if (sincePrint > oledInterval * 3) {
    oled.clear(PAGE);
    oled.setFontType(0);
    oled.setCursor(0, oled.getFontHeight());
    oled.print("https://github.com/");
    oled.setCursor(0, oled.getFontHeight() * 2);
    oled.print("mitaroThanken/mitcard");
    oled.display();
  } else if (sincePrint > oledInterval * 2) {
    int y = 0;
    oled.clear(PAGE);
    oled.setFontType(1);
    oled.setCursor(0,y);
    oled.print("mitcard");
    y += oled.getFontHeight();
    oled.setCursor(0, y);
    oled.setFontType(0);
    oled.print("designed by");
    y += oled.getFontHeight();
    oled.setCursor(0, y);
    oled.print("mitaroThanken.");
    oled.display();
  } else if (sincePrint > oledInterval) {
    oled.clear(PAGE);
    oled.setFontType(1);    // Set font to type 1
    oled.setCursor(73, 17); // move cursor
    oled.print("world!");   // Write a byte out as a character
    oled.setFontType(0);    // Set font to type 0
    oled.setCursor(67, 12); // move cursor
    oled.print("Hello");    // Write a byte out as a character
    oled.display();
  }
}

void loop(void) {
  scanKeyswitches();
  scanTouches();
  oledClearOrDemo();
}
