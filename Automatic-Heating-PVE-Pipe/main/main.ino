#include <Servo.h>

Servo servo1;
Servo servo2;

bool start, tmp;
String Class, manualClass;
const int states = 2, //控制開始與暫停
          genre = 4, //手動、自動
          aMercury = 7, //自動水汞 微動開關
          mMercury = 8, //手動水汞 按鈕開關 #手動噴水
          mServoControl1 = 5, //手動控制 #手動加熱
          mServoControl2 = 6, //手動控制 #手動彎管
          relay1 = 13, //熱風槍 控制繼電器
          relay2 = 12; //水汞控制用繼電器

void setup() {
  start = false;
  tmp = false;
  Class = "heating";
  manualClass = "none";

  servo1.attach(9);
  servo2.attach(10);

  pinMode(states, INPUT); // 開始與暫停
  pinMode(genre, INPUT); //手動與自動
  pinMode(aMercury, INPUT); //自動噴水 控制
  pinMode(mMercury, INPUT); //手動噴水控制
  pinMode(mServoControl1, INPUT);//手動開始加熱
  pinMode(mServoControl1, INPUT);//手動開始彎管

  pinMode(relay1, OUTPUT); //熱風槍控制
  pinMode(relay2, OUTPUT); //水汞控制
}

void loop() {
  int _state = digitalRead(states);
  int _genre = digitalRead(genre);
  if (_state && !tmp) {
    start = !start;
    tmp = true;
  } else {
    tmp = false;
  }

  if (start)
    switch (_genre) {
      case 0: return manual();
      case 1: return automatic();
        // default:return error("state is not found");
    }
  delay(1); //暫緩
}

void automatic() {
  if (Class == "heating") {
    // 啟動 relay1
    Serial.println("start heating");
    Class = "servo2";

    digitalWrite(relay1, HIGH);
    servo1.write(85);
    delay(60 * 1000);
    digitalWrite(relay1, LOW);
    servo1.write(90);
  } else if (Class == "servo2") {
    // 停止繼電器並開始彎管
    Serial.println("start control servo 2");

    servo2.write(90);
    if (digitalRead(aMercury) == 1)
      Class = "cooldown";
  } else if (Class == "cooldown") {
    // 停止彎管用伺服馬達並啟動relay2
    Serial.println("start cooldown");
    Class = "heating";
    start = false;
    digitalWrite(relay2, HIGH);
    delay(3000);
    digitalWrite(relay2, LOW);
  }
}

void manual() {
  int heating = digitalRead(mServoControl1);
  int _servo2 = digitalRead(mServoControl2);
  int _cd = digitalRead(mMercury);

  if (manualClass == "heating")
    return doheating();
  else if (manualClass == "servo2")
    return controlServo2();
  else if (manualClass == "cooldown")
    return cooldown();
  else
    Serial.println("Waiting...");

  // switch(manualClass){
  // case "heating":return heating();
  // case "servo2":return controlServo2();
  // case "cooldown":return cooldown();
  // default:Serial.println("Waiting...");
  // }

  if (heating == 1 && manualClass == "none") {
    manualClass = "heating";
  } else if (_servo2 == 1 && manualClass == "none") {
    manualClass = "servo2";
  } else if (_cd == 1 && manualClass == "none") {
    manualClass = "cooldown";
    // }else if((heating==1||_servo2==1||_cd==1)&&manualClass!="none"){
  } else if ((heating == 1 && manualClass == "heating") || (_servo2 == 1 && manualClass == "servo2") || (_cd == 1 && manualClass == "cooldown")) {
    resetAll();
    manualClass = "none";
  }
}

void resetAll() {
  servo1.write(90);
  servo2.write(0);
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
}

void doheating() {
  //控制繼電器與伺服馬達1
  digitalWrite(relay1, HIGH);
  servo1.write(85);
}

void controlServo2() {
  //控制伺服馬達2
  servo2.write(90);
}

void cooldown() {
  //控制水汞
  digitalWrite(relay2, LOW);
}

void error(String msg) {
  Serial.println(msg);
}
