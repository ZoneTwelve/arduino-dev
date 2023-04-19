#include <DigiKeyboard.h>

const int btn[] = { 0 };
const int ins[] = { HIGH };
const int btn_len = sizeof( btn ) / sizeof( int );

void setup() {
  for(int i = 0 ; i < btn_len ; i++){
    pinMode( btn[i], INPUT );
  }
  
  pinMode(1, OUTPUT);
}

void loop() {
  for(int i = 0 ; i < btn_len; i++){
    int data = digitalRead( btn[i] );
    if( data == ins[i] ){
      DigiKeyboard.println( "fuck" );
      digitalWrite( 1, HIGH );
      DigiKeyboard.delay(500);
    }
  }
  digitalWrite( 1, LOW );
  delay( 500 );
}
