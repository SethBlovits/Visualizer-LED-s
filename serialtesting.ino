#include "FastLED.h"
#include "SoftwareSerial.h"
#define NUM_LEDS 150 
#define DATA_PIN 3       
#define updateLEDS 2 
#define colorShiftUpdate 7
#define LED_TYPE    WS2811
int count=0;
int pick=0;
//#define INPUT_PIN 3
SoftwareSerial serial_connection(10,11);

CRGB leds[NUM_LEDS]; 
int val=0;
int color=0;
int temp=0;
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
  count=serial_connection.available();
  if (count>=1){
    int val= serial_connection.read();
    temp=val;
    if(val==1){
      pick=1;
    }
    else if(val==2){
      pick=2;
    }
    else if(val==3){
      pick=3;
    }
  }
  if(pick==1){
    ColorShift(color);
    if(color<10000){
      color+=5;
    }
    else{
      color=0;
    }
  }
  else if(pick==2){
      SerialRecieve(count,temp);
  }
  else if(pick==3){
    BlockColor(temp);
  }
}
void SerialRecieve(int count,int b){
    color=0;
    //count=serial_connection.available();
    //Serial.println(count);
    if (count>=1)
    {
        //int a=serial_connection.available();
        //int b = serial_connection.read();
        val=b;
        color= map(val,5,180,0,255);
        Serial.println(color);
        val=7;
    }
    else{
      val=0;
    }
    
  VisualizerLights();
}
void ColorShift(int color){
  int mappedColor;
  for(int i = NUM_LEDS - 1; i >= colorShiftUpdate; i--) {
    leds[i] = leds[i - colorShiftUpdate];
  }
  for(int i = 0; i < colorShiftUpdate; i++) {
    mappedColor=map(color,0,10000,10,255);
    leds[i] = CHSV(mappedColor, 255, 135);
  }
  delay(10);
  FastLED.show();
}
void BlockColor(int val){
  for(int i=0;i<NUM_LEDS;i++){
    leds[i]=CHSV(val,255,100);
  }
  FastLED.show();
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
