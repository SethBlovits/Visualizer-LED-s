#include "FastLED.h"
#include "SoftwareSerial.h"
#define NUM_LEDS 150 
#define DATA_PIN 3       
#define updateLEDS 2 
#define LED_TYPE    WS2811
int count=0;
//#define INPUT_PIN 3
SoftwareSerial serial_connection(10,11);

CRGB leds[NUM_LEDS]; 
int val=0;
int color=0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  serial_connection.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
   for(int i = 0; i < NUM_LEDS ; i++) {
      leds[i] = CRGB(0,0,0);
    }
    FastLED.show();
}

void loop() {
  // put your main code here, to run repeatedly:
    color=0;
    count=serial_connection.available();
    Serial.println(count);
    if (count==1)
    {
        Serial.println("Dumb Shit");
        int a=serial_connection.available();
        int b = serial_connection.read();
        //Serial.println(b);
        val=b;
        //Serial.println(val);
        //if (val>=1){
        color= map(val,0,160,0,255);
        val=7;
        //}
        //else{
          
          //color=0;
        //}
        //Serial.print("Received Value: ");
        //Serial.println(color);
    }
    else{
      Serial.println("Bitch");
      val=0;
    }
    
  VisualizerLights();
  
  
}
void VisualizerLights(){
  for(int i = NUM_LEDS - 1; i >= updateLEDS; i--) {
    leds[i] = leds[i - updateLEDS];
  }

  if(val>=7){
  //distance=spectrumValue[1];
  for(int i = 0; i < updateLEDS; i++) {
    leds[i] = CHSV(color, 255, 175);
  }
  }
  else{
    for(int i = 0; i < updateLEDS; i++) {
    leds[i] = CHSV(0,0,0);
  }
  }
  delay(10); 
  FastLED.show();
}
