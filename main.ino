#include "header.h"
#include <esp_wifi.h>

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  15        /* Time ESP32 will go to sleep (in seconds) */

const unsigned long AnalogIn  = 34; //34 for wishTree-ESP32;   // A0 for ESP8266, works in nodeMCU directly, Arduino needs voltage divider
int readingIn = 0;
//unsigned long       Sleeptime = 15*1000000 ;  // 15 sec. deepsleep

const char* id = "TP-LINK";   // removed "const" specifier- otherwise not compiling -> then added "const" in .h and .cpp -> then worked
const char* pw = "bappaditya";

const char* deviceName = "wishTree-ESP32";

void setup() {
  Serial.begin(115200);
  ConnectToWiFi(id, pw);
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);   // needed for ESP32 only
}

void loop() {
  readingIn = analogRead(AnalogIn);
  Serial.println(readingIn);
  delay(200);

  String Url = "/Dev/thingsapi?thingname=";
  Url += deviceName;
  Url += "&";
  Url += "a";
  Url += "=";
  Url += readingIn;

  request(Url);
  
  Serial.println("Going to sleep");
  delay(1000);
  Serial.flush();
  //ESP.deepSleep(Sleeptime);   // for ESP8266 sleep
  //esp_bluedroid_disable();    // don't compile
  //esp_bt_controller_disable();    // don't compile
  esp_wifi_stop();
  esp_deep_sleep_start();       // for ESP32 sleep
  Serial.println("This will never be printed");
}
