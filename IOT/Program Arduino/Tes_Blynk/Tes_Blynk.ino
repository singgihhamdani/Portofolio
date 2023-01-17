#define BLYNK_TEMPLATE_ID "TMPLPqyDRfqk"
#define BLYNK_DEVICE_NAME "Monitoring Air Sungai"
#define BLYNK_AUTH_TOKEN "gstPXZxQ9jr8DuHEIecQTm3jrYsRZMtz"
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "SAWALA Office";
char pass[] = "sawala28";
BlynkTimer timer;

void myTimerEvent() {
  Blynk.virtualWrite(V0, millis() / 1000);
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 8080);
  timer.setInterval(2000L, myTimerEvent);
}

void loop() {
  Blynk.run();
  timer.run();
}
