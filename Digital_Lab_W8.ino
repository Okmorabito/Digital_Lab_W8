#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S i2s1;                 //xy=295,339
AudioEffectBitcrusher bitcrusher1;  //xy=409,461
AudioEffectDelay delay1;            //xy=474,585
AudioMixer4 mixer1;                 //xy=650,469
AudioOutputI2S i2s2;                //xy=835,455
AudioConnection patchCord1(i2s1, 0, delay1, 0);
AudioConnection patchCord2(i2s1, 0, bitcrusher1, 0);
AudioConnection patchCord3(bitcrusher1, 0, mixer1, 0);
AudioConnection patchCord4(delay1, 0, mixer1, 1);
AudioConnection patchCord5(delay1, 1, mixer1, 2);
AudioConnection patchCord6(delay1, 2, mixer1, 3);
AudioConnection patchCord7(mixer1, 0, i2s2, 0);
AudioConnection patchCord8(mixer1, 0, i2s2, 1);
AudioControlSGTL5000 sgtl5000_1;  //xy=391,209
// GUItool: end automatically generated code

int bitPot = A14;
int crossFader = A15;
float mappedPotVal = 0;
float mixBitCrush = 0;
float mixDelay = 0;
int delaySwtich = 37;
int lastSwtichState = LOW;
int switchState = LOW;

void setup() {
  pinMode(bitPot, INPUT);
  pinMode(crossFader, INPUT);
  pinMode(delaySwtich, INPUT);
  bitcrusher1.bits(16);
  bitcrusher1.sampleRate(44100);
  delay1.delay(0, 500);
  delay1.delay(1, 500);
  delay1.delay(2, 500);
  AudioMemory(500);
  Serial.begin(9600);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  sgtl5000_1.inputSelect(AUDIO_INPUT_MIC);
  sgtl5000_1.micGain(30);
}

void loop() {
  checkBitCrush();
  checkDelay();
  checkCrossfade();
}

void checkBitCrush() {
  bitPot = analogRead(A14);
  mappedPotVal = map(bitPot, 0, 1023, 0, 16);
  bitcrusher1.bits(mappedPotVal);
}

void checkDelay() {
  lastSwtichState = switchState;
  switchState = digitalRead(delaySwtich);
  if (lastSwtichState == LOW and switchState == HIGH) {
    delay1.delay(0, 500);
    delay1.delay(1, 600);
    delay1.delay(2, 700);
    delay(5);
  }
  if (lastSwtichState == HIGH and switchState == LOW) {
    delay1.delay(0, 0);
    delay1.delay(1, 0);
    delay1.delay(2, 0);
    delay(5);
  }
}
void checkCrossfade() {
  crossFader = analogRead(A15);
  mixBitCrush = map(crossFader, 0, 1023, 0, 100);
  float gainBitCrush = mixBitCrush/100;
  mixDelay = map(crossFader, 0, 1023, 100, 0);
  float gainDelay = mixDelay/100;
  mixer1.gain(0, gainBitCrush);
  mixer1.gain(1, gainDelay);
  mixer1.gain(2, gainDelay);
  mixer1.gain(3, gainDelay);
}
