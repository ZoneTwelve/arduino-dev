#include <Keyboard.h>

int btnPin[4] = {4,7,6,5};
int btnKey[4] = {0x63, 0xD8, 0xD7, 0x78};
int pressCounter[4];
void setup() {
  Keyboard.begin();
  Keyboard.releaseAll();
  for(int i=0;i<4;i++)
    pinMode(btnPin[i], INPUT_PULLUP);
}

void loop() {
  for(int i=0;i<4;i++){
    int s = digitalRead(btnPin[i]);
//    Serial.print(String(pressCounter[i])+" ");
    if(s==0){
      pressCounter[i]++;
      if(pressCounter[i]==20){
        Serial.print("press: ");
        Serial.println(btnKey[i]);
        Keyboard.press(btnKey[i]);
      }
    }else{
      if(pressCounter[i]<=20&&pressCounter[i]>0){
        Serial.print("write: ");
        Serial.println(btnKey[i]);
        Keyboard.write(btnKey[i]);
      }else if(pressCounter[i]>20){
        Serial.println("release key:"+String(btnKey[i]));
        Keyboard.release(btnKey[i]);
      }
      pressCounter[i] = 0;
    }
  }
//  Serial.println();
  delay(10);
//  Serial.print("["+String(digitalRead(btnPin[0])));
//  for(int i=1;i<4;i++)
//    Serial.print(", "+String(digitalRead(btnPin[i])));
//  Serial.println("]");
//  delay(100);
}
