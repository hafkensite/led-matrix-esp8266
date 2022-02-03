#include <FastLED.h>

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
WiFiManager wm;

#define LED_PIN  D3

#define COLOR_ORDER GRB
#define CHIPSET     WS2811

#define BRIGHTNESS 64

// Params for width and height
const uint8_t kMatrixWidth = 16;
const uint8_t kMatrixHeight = 16;

// Param for different pixel layouts
const bool    kMatrixSerpentineLayout = true;
const bool    kMatrixVertical = false;

WiFiUDP Udp;
char incomingPacket[kMatrixWidth * kMatrixHeight * 3];

uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;

  if ( kMatrixSerpentineLayout == false) {
    if (kMatrixVertical == false) {
      i = (y * kMatrixWidth) + x;
    } else {
      i = kMatrixHeight * (kMatrixWidth - (x + 1)) + y;
    }
  }

  if ( kMatrixSerpentineLayout == true) {
    if (kMatrixVertical == false) {
      if ( y & 0x01) {
        // Odd rows run forwards
        i = (y * kMatrixWidth) + x;

      } else {
        // Even rows run backwards
        uint8_t reverseX = (kMatrixWidth - 1) - x;
        i = (y * kMatrixWidth) + reverseX;
      }
    } else { // vertical positioning
      if ( x & 0x01) {
        i = kMatrixHeight * (kMatrixWidth - (x + 1)) + y;
      } else {
        i = kMatrixHeight * (kMatrixWidth - x) - (y + 1);
      }
    }
  }

  return i;
}

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds[ NUM_LEDS ];

void loop() {

  int packetSize = Udp.parsePacket();
  if (packetSize)  {
    //    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, NUM_LEDS * 3);
    if (len == NUM_LEDS * 3) {
      //      Serial.printf("UDP packet contents: %d\n", len);

      for ( uint8_t y = 0; y < kMatrixHeight; y++) {
        for ( uint8_t x = 0; x < kMatrixWidth; x++) {
          int start = (y * kMatrixHeight + x) * 3;
          leds[ XY(x, y)]  = CRGB(
                               incomingPacket[start + 0],
                               incomingPacket[start + 1],
                               incomingPacket[start + 2]);
        }
      }
    }
  }
  FastLED.show();
  
}

void DrawOneFrame( uint8_t startHue8, int8_t yHueDelta8, int8_t xHueDelta8)
{
  uint8_t lineStartHue = startHue8;
  for ( uint8_t y = 0; y < kMatrixHeight; y++) {
    lineStartHue += yHueDelta8;
    uint8_t pixelHue = lineStartHue;
    for ( uint8_t x = 0; x < kMatrixWidth; x++) {
      pixelHue += xHueDelta8;
      leds[ XY(x, y)]  = CHSV( pixelHue, 255, 255);
    }
  }
}


void setup() {
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setCorrection(TypicalSMD5050);
  FastLED.clear();
  DrawOneFrame(1, 32, 192);
  for (uint8_t x = 0; x < 10; x++) {
    FastLED.show();
  }

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  //  wm.resetSettings();

  if (wm.autoConnect("AutoConnectAP")) {
    Serial.println("connected...yeey :)");
    Udp.begin(5060);
  } else {
    Serial.println("Failed to connect");
    wm.reboot();
  }
}
