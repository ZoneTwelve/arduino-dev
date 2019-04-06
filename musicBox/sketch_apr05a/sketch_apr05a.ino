int btnLen = 4;
int debugModePin= 2;
int btnPin[4] = {11,10,9,8};
int ledPin[4] = {4,5,6,7};
int modePin = 12, analogPin = A0, bzPin = 3;
int musicNote[16];
int toneCount = 0;
int timeCounter = 0;
#define Do  523
#define Re  587
#define Mi  659
#define Fa  698
#define So  784
#define La  880
#define Si  988
int melody[7] = {Do, Re, Mi, Fa, So, La, Si};
const int toneTable[7][5]={
    { 66, 131, 262, 523, 1046},  // C Do
    { 74, 147, 294, 587, 1175},  // D Re
    { 83, 165, 330, 659, 1318},  // E Mi
    { 88, 175, 349, 698, 1397},  // F Fa
    { 98, 196, 392, 784, 1568},  // G So
    {110, 220, 440, 880, 1760},  // A La
    {124, 247, 494, 988, 1976}   // B Si
};
boolean mode = true;
boolean wait = false;
int counter = 0;
void setup() {
  pinMode(bzPin, OUTPUT);
  pinMode(modePin, INPUT_PULLUP);
  pinMode(debugModePin, INPUT_PULLUP);
  for(int i=0;i<btnLen;i++)
    pinMode(btnPin[i], INPUT_PULLUP);
  for(int i=0;i<btnLen;i++)
    pinMode(ledPin[i], OUTPUT);
  tone(bzPin, 1000);
  for(int i=1;i<=16;i*=2){
    setLED(i);
    delay(50);
  }
  noTone(bzPin);
}

void loop(){
  Serial.println(digitalRead(debugModePin));
  if(digitalRead(modePin)==0){
    mode = mode?false:true;
    counter = 0;
    setLED(counter);
    delay(1000);
  }
  mode?createNote():(digitalRead(debugModePin)==1?playMusic():debugMode());
}

void createNote(){
  boolean releaseWait = true;
  int note = -1;
  for(int i=0;i<4;i++){
    if(digitalRead(btnPin[i])==0){
      if(!wait){
        tone(bzPin, melody[i]);
        note = i;
        wait = true;
        musicNote[counter] = i+1;
        toneCount = 0;
        counter = (counter+1)%16;
        setLED(counter);
      }
      releaseWait = false;
    }
  }
  Serial.print("["+String(musicNote[0]));
  for(int i=1;i<16;i++)
    Serial.print(", "+String(musicNote[i]));
  Serial.println("]");
//  Serial.println("status: "+String(wait?1:0)+", note:"+String(note)+", realse: "+(releaseWait?"O":"X"));
  if(releaseWait){
    wait = false;
    timeCounter = 0;
    noTone(bzPin);
  }else{
    timeCounter++;
    Serial.println(timeCounter);
    if(timeCounter==50){
      for(int i=0;i<16;i++)
        musicNote[i] = 0;
      counter = 0;
      setLED(counter);
      noTone(bzPin);
    }else if(timeCounter==15){
      counter--;
      setLED(counter);
      musicNote[counter] = 0;
      delay(50);
      counter++;
      setLED(counter);
    }
  }
  delay(50);
}

void setLED(int c){
  for(int i=0;i<4;i++){
    digitalWrite(ledPin[i], c%2==1);
    c/=2;
  }
}

void playMusic(){
  int delayTime = 50*(analogRead(analogPin)/100);
//  analogWrite(bzPin, delayTime);
//  int t = 1000*musicNote[toneCount];
  int tofn = musicNote[toneCount];
  int t = melody[tofn];
  if(tofn!=0){
    tone(bzPin, melody[musicNote[toneCount]-1]);
    delay(delayTime/2);
    noTone(bzPin);
  }
  toneCount = (toneCount+1)%16;
  setLED(toneCount);
  Serial.print(delayTime);
  Serial.println(", playing sound...");
  delay(delayTime/2);
//  analogWrite(bzPin, 0);
}

void debugMode(){
//  for(int i=0;i<100;i/++){
    Serial.println(String(digitalRead(modePin))+", "+String(analogRead(analogPin)));
    if(digitalRead(modePin)==1){
      tone(bzPin, analogRead(analogPin)*10);
      delay(250);
      noTone(bzPin);
    }
//  }/
//  for(int i=0;i<7;i++)
//    for(int j=0;j<5;j++){
//      tone(bzPin, toneTable[i][j]);
//      if(digitalRead(debugModePin)==0)
//        delay(100);
//      Serial.println(String(i)+","+String(j)+","+String(toneTable[i][j]));
//    }
//  noTone(bzPin);
}
