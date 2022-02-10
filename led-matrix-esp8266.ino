#include <FastLED.h>

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
WiFiManager wm;

#define LED_PIN D3

#define COLOR_ORDER GRB
#define CHIPSET WS2811

#define BRIGHTNESS 16

// Params for width and height
#define MATRIX_WIDTH 16
#define MATRIX_HEIGHT 16

#define NUM_LEDS (MATRIX_WIDTH * MATRIX_HEIGHT)
CRGB leds[NUM_LEDS];

WiFiUDP Udp;
char incomingPacket[MATRIX_WIDTH * MATRIX_HEIGHT * 3];

uint16_t XY(uint8_t x, uint8_t y)
{
  if (y & 0x01)
  {
    // Odd rows run forwards
    return (y * MATRIX_WIDTH) + x;
  }

  // Even rows run backwards
  uint8_t reverseX = (MATRIX_WIDTH - 1) - x;
  return (y * MATRIX_WIDTH) + reverseX;
}

void loop()
{

  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    //    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, NUM_LEDS * 3);
    if (len == NUM_LEDS * 3)
    {
      for (uint8_t y = 0; y < MATRIX_HEIGHT; y++)
      {
        for (uint8_t x = 0; x < MATRIX_WIDTH; x++)
        {
          int start = (y * MATRIX_HEIGHT + x) * 3;
          leds[XY(x, y)] = CRGB(
              incomingPacket[start + 0],
              incomingPacket[start + 1],
              incomingPacket[start + 2]);
        }
      }
      FastLED.show();
    }
  }
}

void DrawOneFrame(uint8_t startHue8, int8_t yHueDelta8, int8_t xHueDelta8)
{
  uint8_t lineStartHue = startHue8;
  for (uint8_t y = 0; y < MATRIX_HEIGHT; y++)
  {
    lineStartHue += yHueDelta8;
    uint8_t pixelHue = lineStartHue;
    for (uint8_t x = 0; x < MATRIX_WIDTH; x++)
    {
      pixelHue += xHueDelta8;
      leds[XY(x, y)] = CHSV(pixelHue, 255, 255);
    }
  }
}

void setup()
{
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setCorrection(TypicalSMD5050);
  WiFi.setSleepMode(WIFI_MODEM_SLEEP);
  FastLED.clear();
  DrawOneFrame(1, 32, 192);
  FastLED.show();

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  //  wm.resetSettings();

  if (wm.autoConnect("AutoConnectAP"))
  {
    Serial.println("connected...yeey :)");
    Udp.begin(5060);
  }
  else
  {
    Serial.println("Failed to connect");
    wm.reboot();
  }
}