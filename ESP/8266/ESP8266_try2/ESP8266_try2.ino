#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <FS.h>

void setup() {
  Serial.begin(115200);
  SPIFFS.begin();
  delay(1000);
  setupWiFi();
}

void loop(){}

void setupWiFi(){
  StaticJsonBuffer<200> jsonBuffer;
  File _config = SPIFFS.open("/config.json", "r+");
  if(!_config){
    _config.close();
    JsonObject& root = jsonBuffer.createObject();
    root["APSSID"] = "WiFi_Modules";
    root["APPASS"] = "ZoneTwelve";
    root["APMODE"] = "AP";
    File _config = SPIFFS.open("/config.json", "w");
//    _config.println("APSSID:WiFi_Module");
//    _config.println("APPASS:ZoneTwelve");
//    _config.println("APMODE:AP");
//    _config.close();
    Serial.println("");
    Serial.println("place reboot your device, default config been apply");
    root.printTo(_config);
    root.printTo(Serial);
  }else{
    while(_config.available()){
      char line[] = toCharArray(_config.readStringUntil("\n"), 100);
      JsonObject& root = jsonBuffer.parseObject(line);
      if(!root.success()){
        Serial.println("parse failed");
      }
    }
  }
}
