 #include "Adafruit_WS2801.h"
#include "SPI.h"

//Green wire to 11 and blue wire to 13 for LED SPI
Adafruit_WS2801 strip = Adafruit_WS2801(20);
int mode = 0;
int interrupt = 0;
const int sampleWindow = 50;
unsigned int sample;
unsigned int leds = strip.numPixels();
int ledArray[20];

void setup() {
  strip.begin();
  strip.show();
  attachInterrupt(1, modeShift, FALLING);
  pinMode(3, INPUT_PULLUP);
}

void modeShift() {
  noInterrupts();
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  if(interruptTime - lastInterruptTime > 300) {
    if(mode <5) {
      mode++;
    }else{
      mode = 0; 
    }
    interrupt = 1;
  }
  lastInterruptTime = interruptTime;
  interrupts();
}

void loop() {
  while (interrupt == 0) {
    if(mode == 2){
      cylon(255, 0, 0, 70);
    } else if(mode == 4) {
      cylon(0, 0, 255, 70);
    } else if(mode == 0) {
      int i, j;
      for (j=0; j < 256 * 5; j++) {
        if (interrupt != 0) {
          break;
        }
        for (i=0; i < strip.numPixels(); i++) {
          if (interrupt != 0) {
            break;
          }
          strip.setPixelColor(i, Wheel( ((i * 256 / strip.numPixels()) + j) % 256) );
        }  
        strip.show();
        delay(10);
      }
    } else if(mode == 1){
      unsigned long startMillis= millis();
      unsigned int peakToPeak = 0;
      unsigned int signalMax = 0;
      unsigned int signalMin = 1024;
      while (millis() - startMillis < sampleWindow) {
        sample = analogRead(0);
        if (sample < 1024) {
          if (sample > signalMax) {
            signalMax = sample;
          } else if (sample < signalMin){
            signalMin = sample;
          }
        }
      }
      peakToPeak = signalMax - signalMin;
      peakToPeak = map(peakToPeak, 0, 492, 0, 256);
      for(int i=leds-1; i > 0; i--){
        ledArray[i] = ledArray[i-1];
      }
      ledArray[0] = peakToPeak;
      for(int i=0; i<leds; i++){
        strip.setPixelColor(i,Wheel(ledArray[i]));
      }
      strip.show();
    } else if(mode == 3){
      unsigned long startMillis= millis();
      unsigned int peakToPeak = 0;
      unsigned int signalMax = 0;
      unsigned int signalMin = 1024;
      while (millis() - startMillis < sampleWindow) {
        sample = analogRead(0);
        if (sample < 1024) {
          if (sample > signalMax) {
            signalMax = sample;
          } else if (sample < signalMin){
            signalMin = sample;
          }
        }
      }
      peakToPeak = signalMax - signalMin;
      peakToPeak = map(peakToPeak, 0, 492, 0, 256);
      for(int i=leds-1; i > 0; i--){
        ledArray[i] = ledArray[i-1];
      }
      ledArray[0] = peakToPeak;
      for(int i=0; i<leds; i++){
        strip.setPixelColor(i,Color(map(10^ledArray[i],10^0,10^256,0,256),0,0));
      }
      strip.show();
    } else if(mode == 5){
      unsigned long startMillis= millis();
      unsigned int peakToPeak = 0;
      unsigned int signalMax = 0;
      unsigned int signalMin = 1024;
      while (millis() - startMillis < sampleWindow) {
        sample = analogRead(0);
        if (sample < 1024) {
          if (sample > signalMax) {
            signalMax = sample;
          } else if (sample < signalMin){
            signalMin = sample;
          }
        }
      }
      peakToPeak = signalMax - signalMin;
      peakToPeak = map(peakToPeak, 0, 492, 0, 256);
      for(int i=leds-1; i > 0; i--){
        ledArray[i] = ledArray[i-1];
      }
      ledArray[0] = peakToPeak;
      for(int i=0; i<leds; i++){
        strip.setPixelColor(i,Color(0,0,map(10^ledArray[i],10^0,10^256,0,256)));
      }
      strip.show();
    }
  }
  interrupt = 0;
}

void cylon(int r, int g, int b, uint8_t wait) {
  int i;
  for (i=0; i < strip.numPixels(); i++) {
      if (interrupt != 0) {
        break;
      }
      strip.setPixelColor(i, Color(r, g, b));
      strip.setPixelColor(i-1, Color(r/10, g/10, b/10));
      strip.setPixelColor(i-2, Color(r/20, g/20, b/20));
      strip.setPixelColor(i-3, 0);
      strip.show();
      delay(wait);
  }
  for (i=strip.numPixels()-1; i > 0; i--) {
      if (interrupt != 0) {
        break;
      }
      strip.setPixelColor(i, Color(r, g, b));
      strip.setPixelColor(i+1, Color(r/10, g/10, b/10));
      strip.setPixelColor(i+2, Color(r/20, g/20, b/20));
      strip.setPixelColor(i+3, 0);
      strip.show();
      delay(wait);
  }
}

uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85) {
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170; 
   return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
