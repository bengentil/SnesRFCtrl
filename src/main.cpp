#include <Arduino.h>
#include <VirtualWire.h>

#define RF_POWER_PIN 5
#define RF_TX_PIN 6

/*
SNES controller connector:
__________________
| 1 2 3 4 | x x 5 )

1. 5v (white)
2. CLK (blue)
3. Latch (yellow)
4. Data (red)
5. GND (brown)
*/

#define SNES_CLK_PIN 7
#define SNES_LATCH_PIN 8
#define SNES_DATA_PIN 9

//#define DEBUG_ENABLED

#ifdef DEBUG_ENABLED
#define DBG(x) x
#else
#define DBG(x)
#endif

uint8_t _keys[2] = {0xFF, 0xFF};

uint8_t skip;
uint8_t last_keys[2]={0xFF, 0xFF};

#define DF3_BTN_R      0, 1
#define DF3_BTN_START  0, 2
#define DF3_BTN_HOME   0, 3
#define DF3_BTN_SELECT 0, 4
#define DF3_BTN_L      0, 5
#define DF3_BTN_DOWN   0, 6
#define DF3_BTN_RIGHT  0, 7

#define DF3_BTN_UP     1, 0
#define DF3_BTN_LEFT   1, 1
#define DF3_BTN_ZR     1, 2
#define DF3_BTN_X      1, 3
#define DF3_BTN_A      1, 4
#define DF3_BTN_Y      1, 5
#define DF3_BTN_B      1, 6
#define DF3_BTN_ZL     1, 7

#define DF3_PRESS_KEY_ARGS(x, y) bitClear(_keys[x], y)
#define DF3_PRESS_KEY(...) DF3_PRESS_KEY_ARGS(__VA_ARGS__)

void readNesController()
{

  _keys[0] = 0xFF;
  _keys[1] = 0xFF;

  digitalWrite(SNES_LATCH_PIN, HIGH);
  digitalWrite(SNES_LATCH_PIN, LOW);

  // A
  if (digitalRead(SNES_DATA_PIN) == LOW)
    DF3_PRESS_KEY(DF3_BTN_A);

  // B
  digitalWrite(SNES_CLK_PIN, HIGH);
  digitalWrite(SNES_CLK_PIN, LOW);
  if (digitalRead(SNES_DATA_PIN) == LOW)
    DF3_PRESS_KEY(DF3_BTN_B);

  // SELECT
  digitalWrite(SNES_CLK_PIN, HIGH);
  digitalWrite(SNES_CLK_PIN, LOW);
  if (digitalRead(SNES_DATA_PIN) == LOW)
    DF3_PRESS_KEY(DF3_BTN_SELECT);

  // START
  digitalWrite(SNES_CLK_PIN, HIGH);
  digitalWrite(SNES_CLK_PIN, LOW);
  if (digitalRead(SNES_DATA_PIN) == LOW)
    DF3_PRESS_KEY(DF3_BTN_START);

  // UP
  digitalWrite(SNES_CLK_PIN, HIGH);
  digitalWrite(SNES_CLK_PIN, LOW);
  if (digitalRead(SNES_DATA_PIN) == LOW)
    DF3_PRESS_KEY(DF3_BTN_UP);

  // DOWN
  digitalWrite(SNES_CLK_PIN, HIGH);
  digitalWrite(SNES_CLK_PIN, LOW);
  if (digitalRead(SNES_DATA_PIN) == LOW)
    DF3_PRESS_KEY(DF3_BTN_DOWN);

  // LEFT
  digitalWrite(SNES_CLK_PIN, HIGH);
  digitalWrite(SNES_CLK_PIN, LOW);
  if (digitalRead(SNES_DATA_PIN) == LOW)
    DF3_PRESS_KEY(DF3_BTN_LEFT);

  // RIGHT
  digitalWrite(SNES_CLK_PIN, HIGH);
  digitalWrite(SNES_CLK_PIN, LOW);
  if (digitalRead(SNES_DATA_PIN) == LOW)
    DF3_PRESS_KEY(DF3_BTN_RIGHT);

}

void setup()
{

  pinMode(RF_POWER_PIN, OUTPUT);
  digitalWrite(RF_POWER_PIN, HIGH);

  // Set appropriate pins to inputs
  pinMode(SNES_DATA_PIN, INPUT);

  // Set appropriate pins to outputs
  pinMode(SNES_CLK_PIN, OUTPUT);
  pinMode(SNES_LATCH_PIN, OUTPUT);

  // Set initial states
  digitalWrite(SNES_CLK_PIN, LOW);
  digitalWrite(SNES_LATCH_PIN, LOW);

  DBG(Serial.begin(115200);)
  DBG(while (!Serial);)

  vw_set_tx_pin(RF_TX_PIN); // Setup transmit pin
  vw_setup(2600); // Transmission speed in bits per second.
}

void loop()
{
  readNesController();

  //DBG(uint32_t t1=millis();)
  if(!vw_send(&_keys[0], 2)) Serial.println("error");
  vw_wait_tx();
  delay(1);

}
