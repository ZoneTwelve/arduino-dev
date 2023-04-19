#include <ESP8266WiFi.h>
#include <FS.h>

void setup(){
  Serial.begin(115200);
  SPIFFS.begin();
  delay(1000);
  setupWiFi();
}

void loop(){
  
}

void setupWiFi(){
  char ssid[32] = "";
  char pass[32] = "";
  String modes = "";
  
  File Config = SPIFFS.open("/config", "r+");
  if(!Config){
    Config.close();
    File Config = SPIFFS.open("/config", "w");
    Config.println("APSSID:WiFi_Module");
    Config.println("APPASS:ZoneTwelve");
    Config.println("APMODE:AP");
    Config.close();
    Serial.println("");
    Serial.println("place reboot your device, default config been apply");
  }else{
    Serial.println("");
    Serial.println("--------");
    Serial.println("config:{");
    while(Config.available()){
      String line = Config.readStringUntil('\n');
      String value[2] = {"", ""};
      splitValue(line, value, 2, ":");
      if(value[0]=="APSSID")
        value[1].toCharArray(ssid, 32);
      else if(value[0]=="APPASS")
          value[1].toCharArray(pass, 32);
      else if(value[0]=="APMODE")
          modes = value[1];
//      Serial.print("  ");
//      Serial.print(value[0].toCharArray(value[0]));
//      Serial.print(" | ");
//      Serial.println(toCharArray(value[1]));
//      Serial.println("  "+value[0]+" | "+value[1]);
//      Serial.println("  "+line);
    }
    Serial.print("  SSID = ");
    Serial.println(ssid);
    Serial.print("  Pass = ");
    Serial.println(pass);
    Serial.print("  Mode = ");
    Serial.println(modes);
    Serial.println("}");
    Serial.println("--------");
    Serial.println("|"+modes+"|");
//    modes = "AP";
    if(modes=="AP"){
      Serial.println("AP Mode");
      WiFi.mode(WIFI_AP);
      WiFi.softAP(ssid, pass);
    }else if(modes=="attack"){
      Serial.println("attackMode");
      WiFi.mode(WIFI_AP_STA);
      WiFi.softAP(ssid, pass);
    }else{
      Serial.println("The Mode is not legal");
    }
  }
}

void splitValue(String valueString, String *valueArray, int ArraySize, String splitp){
  int i=0;
  int pos;
  do{
    if(i>=ArraySize) break;
   
    pos = valueString.indexOf(splitp);
    if(pos != -1){
      //Serial << valueString.substring(0, pos) << ",";
      valueArray[i++] = valueString.substring(0, pos);
      valueString = valueString.substring(pos+1, valueString.length());
    }else{
      if(valueString.length()>0){
        //Serial << valueString << ",";
        valueArray[i++] = valueString;
      } 
    }
  }while(pos >=0);
  //Serial << "\n";
 
  while(i<ArraySize)
    valueArray[i++] = "";
}
