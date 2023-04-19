#include <Mouse.h>

const int rotationX = 9;
const int rotationY = 10;
const int btn = 11;
const int hack = 0;
int pressed = 0;

void setup() {
  Mouse.begin();
  pinMode(rotationX, INPUT);
  pinMode(rotationY, INPUT);
  pinMode(btn, INPUT_PULLUP);
  pinMode(hack, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
}

void loop() {
  int x = analogRead(rotationX)/50*5/10-5;
  int y = analogRead(rotationY)/50*5/10-5;
  int button = digitalRead(btn);
  Serial.println("control");
  Serial.println(x);
  Serial.println(y);
  Serial.println(pressed);
  if(digitalRead(1)==LOW){
    double middleX = analogRead(rotationY)/100;
    middleX = (middleX-5)/5;
    Mouse.move(x*5, 0, middleX);
    if(button==LOW)
      pressed+=9;
    delay(90);
  }else
    Mouse.move(x, -y, 0);
  if(digitalRead(hack)==HIGH){
    if(button==HIGH&&pressed>5&&pressed<20)
      Mouse.click();
    else if(button==LOW&&pressed>20)
      Mouse.press();
    else if(button==HIGH&&pressed==0)
      Mouse.release();
    if(button==LOW)
      pressed++;
    else
      pressed = 0;
  }else
    if(button==LOW)
      Mouse.click();
  delay(10);
}
