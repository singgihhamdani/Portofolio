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

const int trigPin = 26;
const int echoPin = 25;
#define SOUND_SPEED 0.034 //kecepatan suara cm/uS
long duration;
float distanceCm;
float waterLevel, maxWaterLevel = 60;
int batas1 = 20;
int batas2 = 30;
int batas3 = 40;
int batas4 = 50;

#define relay1 5
#define relay2 17
#define relay3 16
#define relay4 4
boolean flag1, flag2, flag3, flag4 = false;
String status1, status2, status3, status4 = "OFF";

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

void myTimerEvent() {
  Blynk.virtualWrite(V0, waterLevel);
  Blynk.virtualWrite(V1, status1);
  Blynk.virtualWrite(V2, status2);
  Blynk.virtualWrite(V3, status3);
  Blynk.virtualWrite(V4, status4);
}

void setup() {
  pinMode(relay1, OUTPUT);
  digitalWrite(relay1, HIGH);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay2, HIGH);
  pinMode(relay3, OUTPUT);
  digitalWrite(relay3, HIGH);
  pinMode(relay4, OUTPUT);
  digitalWrite(relay4, HIGH);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  lcd.begin();
  lcd.setCursor(0, 0);
  lcd.print("Connecting...");

  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 8080);
  timer.setInterval(2000L, myTimerEvent);

  lcd.clear();
}

void loop() {
  Blynk.run();
  timer.run();

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * SOUND_SPEED / 2;
  waterLevel = maxWaterLevel - distanceCm;
  //  Serial.print("Tinggi Air (cm): ");
  //  Serial.println(waterLevel);

  if (waterLevel < batas1) {
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, HIGH);
    flag1 = false; flag2 = false; flag3 = false; flag4 = false;
    status1 = "OFF"; status2 = "OFF"; status3 = "OFF"; status4 = "OFF";
    delay(100);
  }

  if ((waterLevel >= batas1) && (waterLevel < batas2) && (flag1 == false)) {
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, HIGH);
    flag1 = true; flag2 = false; flag3 = false; flag4 = false;
    status1 = "ON"; status2 = "OFF"; status3 = "OFF"; status4 = "OFF";
    Serial.println("Batas1");
    delay(2000);
  }

  if ((waterLevel >= batas2) && (waterLevel < batas3) && (flag2 == false)) {
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, LOW);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, HIGH);
    flag1 = false; flag2 = true; flag3 = false; flag4 = false;
    status1 = "ON"; status2 = "ON"; status3 = "OFF"; status4 = "OFF";
    Serial.println("Batas2");
    delay(2000);
  }

  if ((waterLevel >= batas3) && (waterLevel < batas4) && (flag3 == false)) {
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, LOW);
    digitalWrite(relay3, LOW);
    digitalWrite(relay4, HIGH);
    flag1 = false; flag2 = false; flag3 = true; flag4 = false;
    status1 = "ON"; status2 = "ON"; status3 = "ON"; status4 = "OFF";
    Serial.println("Batas3");
    delay(2000);
  }

  if ((waterLevel >= batas4) && (flag4 == false)) {
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, LOW);
    digitalWrite(relay3, LOW);
    digitalWrite(relay4, LOW);
    flag1 = false; flag2 = false; flag3 = false; flag4 = true;
    status1 = "ON"; status2 = "ON"; status3 = "ON"; status4 = "ON";
    Serial.println("Batas4");
    delay(2000);
  }

  lcd.setCursor(0, 0);
  lcd.print("Water Level: " + String(waterLevel, 0) + " ");
  lcd.setCursor(0, 1);
  lcd.print(status1 + " " + status2 + " " + status3 + " " + status4 + "    ");
}
