#include <Bounce2.h>
#include <Keyboard.h>

const int ROW_NUM = 2;
const int COL_NUM = 2;
const int ROW_PIN[ROW_NUM] = { 3, 2 };
const int COL_PIN[COL_NUM] = { 4, 5 };

Bounce * debounce[ROW_NUM] = { new Bounce[COL_NUM], new Bounce[COL_NUM] };

const int KEY_MAP[ROW_NUM][COL_NUM] = {
  { KEY_A, KEY_B },
  { KEY_1, KEY_2 }
};

const int LED_PIN = 13;
int ledState = LOW;

void setup() {
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

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ledState);

  Serial.begin(9600);
  Keyboard.begin();
}

int delaycount = 0;
bool needToToggleLed = true;

void loop() {
  
  for (int row = 0; row < ROW_NUM; row++) {
    digitalWrite(ROW_PIN[row], LOW);

    for (int col = 0; col < COL_NUM; col++) {
      debounce[row][col].update();
      if (debounce[row][col].fell()) {
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
        Serial.println("Released.");
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

  if (delaycount > 100) {
    int data = touchRead(16);
    Serial.print("16: ");
    Serial.print(data);
    data = touchRead(22);
    Serial.print(", 22: ");
    Serial.print(data);
    data = touchRead(23);
    Serial.print(", 23: ");
    Serial.println(data);
    delaycount = 0;
  } else {
    delaycount++;
  }
}
