#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include "SerialMP3Player.h"
#include "pinku.h"

//SSL BYWAHJOE
#define url "https://igos.bywahjoe.com/parse.php"
const uint8_t fingerprint[20] = {0x1E, 0xBE, 0x7B, 0x83, 0xFC, 0x68, 0x9F, 0xFB, 0x4D, 0x6C, 0xCB, 0xC4, 0xCA, 0xC8, 0x3B, 0xA5, 0xDB, 0x27, 0x44, 0x5F};

ESP8266WiFiMulti WiFiMulti;
int lastTimer = 0, lastSong = 0, lastSpeed = 0;
String lastParsing = "";

//Timer
unsigned long nows = 0, stops = 0;
int detik = 60;

SerialMP3Player mp3(RX, TX);

void setup() {

  Serial.begin(9600);
  // Serial.setDebugOutput(true);

  pinMode(pinIN1, OUTPUT);
  pinMode(pinIN2, OUTPUT);
  pinMode(pinIN3, OUTPUT);
  pinMode(pinIN4, OUTPUT);
  setMotor(0);

  mp3.begin(9600);        // start mp3-communication
  delay(500);             // wait for init

  mp3.sendCommand(CMD_SEL_DEV, 0, 2);   //select sd-card
  delay(500);             // wait for init

  setMusic(0);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("SUKSES TERSAMBUNG");
}

void loop() {
  // wait for WiFi connection
  loadWEB();
  delay(15000);
  nows = millis();

  if (nows >= stops) {
    Serial.println("STOP ALL");
    setMusic(0);
    setMotor(0);
    delay(500);
  }

}
bool isNewMode(int parseTimer, int parseSong, int parseSpeed) {
  bool result = false;
  if (parseTimer != lastTimer || parseSong != lastSong || parseSpeed != lastSpeed) result = true;

  return result;
}
void setMusic(int option) {
  //0 - Stop
  //1-5 Song

  if (option == 0)mp3.stop();
  else mp3.play(option);
}
void setMotor(int option) {
  //Stop,Slow,Medium,Fast

  switch (option) {
    case 0:
      digitalWrite(pinIN1, LOW);
      analogWrite(pinIN2, 0);
      digitalWrite(pinIN3, LOW);
      analogWrite(pinIN4, 0);
      break;
    case 1:
      digitalWrite(pinIN1, LOW);
      analogWrite(pinIN2, 100);
      digitalWrite(pinIN3, LOW);
      analogWrite(pinIN4, 100);
      break;
    case 2:
      digitalWrite(pinIN1, LOW);
      analogWrite(pinIN2, 170);
      digitalWrite(pinIN3, LOW);
      analogWrite(pinIN4, 170);
      break;
    case 3:
      digitalWrite(pinIN1, LOW);
      analogWrite(pinIN2, 255);
      digitalWrite(pinIN3, LOW);
      analogWrite(pinIN4, 255);
      break;
    default:
      break;
  }
}
void prosesData(String data) {
  String mydata[3];
  int parseTimer = 0, parseSong = 0, parseSpeed = 0;
  int counter = 0;

  for (int i = 0; i < data.length(); i++) {
    if ( data[i] == ',') {
      counter++;
      continue;
    }
    else {
      mydata[counter] += data[i];
    }
  }
  //Store;
  parseTimer = mydata[0].toInt();
  parseSong = mydata[1].toInt();
  parseSpeed = mydata[2].toInt();

  lastTimer = parseTimer;
  lastSong = parseSong;
  lastSpeed = parseSpeed;

  Serial.println("New Data is Parsing!");
  Serial.print("Select Timer:"); Serial.println( lastTimer);
  Serial.print("Select Song :"); Serial.println( lastSong);
  Serial.print("Select Speed:"); Serial.println( lastSpeed);

  //New Setting
  setMusic(lastSong);
  setMotor(lastSpeed);

  //Program Real
  //stops=millis()+(lastTimer*detik*1000);

  //Program Test
  stops=millis()+(lastTimer*1000);
}
void loadWEB() {
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

  client->setFingerprint(fingerprint);
  // Or, if you happy to ignore the SSL certificate, then use the following line instead:
  client->setInsecure();

  HTTPClient https;

  Serial.print("[HTTPS] begin...\n");
  if (https.begin(*client, url)) {  // HTTPS

    Serial.print("[HTTPS] GET...\n");
    // start connection and send HTTP header
    int httpCode = https.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        String payload = https.getString();
        int data = payload.length();
        Serial.println(payload);

        if (payload != lastParsing && data <= 10 && data > 5) {
          lastParsing = payload;
          prosesData(payload);
        }
      } else {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }

      https.end();

      Serial.println("Wait.....");
    }
  }
}
