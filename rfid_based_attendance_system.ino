#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
// WiFi parameters
#define WLAN_SSID       "HGW-72DA11"
#define WLAN_PASS       "22222222"

// Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "Vivek2407"
#define AIO_KEY         "aio_cnLO823aDKG449dnbKaTrUHF1BZV"

constexpr uint8_t RST_PIN = D3;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = D4;     // Configurable, see typical pin layout above


WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Publish Attendance = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/ATTENDANCE");

char ID;

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;


String tag;


void setup() {
  Serial.begin(115200);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  pinMode(D8, OUTPUT);
  Serial.println(F("Adafruit IO Example"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());

  // connect to adafruit io
  connect();

}

// connect to adafruit io via MQTT
void connect() {
  Serial.print(F("Connecting to Adafruit IO... "));
  int8_t ret;
  while ((ret = mqtt.connect()) != 0) {
    switch (ret) {
      case 1: Serial.println(F("Wrong protocol")); break;
      case 2: Serial.println(F("ID rejected")); break;
      case 3: Serial.println(F("Server unavail")); break;
      case 4: Serial.println(F("Bad user/pass")); break;
      case 5: Serial.println(F("Not authed")); break;
      case 6: Serial.println(F("Failed to subscribe")); break;
      default: Serial.println(F("Connection failed")); break;
    }

    if(ret >= 0)
      mqtt.disconnect();

    Serial.println(F("Retrying connection..."));
    delay(5000);
  }
  Serial.println(F("Adafruit IO Connected!"));
}

void loop() {
  // ping adafruit io a few times to make sure we remain connected
  if(! mqtt.ping(3)) {
    // reconnect to adafruit io
    if(! mqtt.connected())
      connect();
  }
   if ( ! rfid.PICC_IsNewCardPresent())
    return;
  if (rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }
    Serial.println(tag);
    if (tag == "1193818595") {
      Serial.println("Access Granted!");
      digitalWrite(D8, HIGH);
      delay(100);
      digitalWrite(D8, LOW);
      delay(100);
      digitalWrite(D8, HIGH);
      delay(100);
      digitalWrite(D8, LOW);
      delay(100);
      digitalWrite(D8, HIGH);
      delay(100);
      digitalWrite(D8, LOW);
      delay(100);
    } else if (tag == "1356479181") {
      Serial.println("1-RG MISHRA : ATTENDANCE RECORDED SUCCESSFULLY");
      
      digitalWrite(D8, HIGH);
      delay(100);
      digitalWrite(D8, LOW);
      delay(100);
      digitalWrite(D8, HIGH);
      delay(100);
      digitalWrite(D8, LOW);
      delay(100);
      digitalWrite(D8, HIGH);
      delay(100);
      digitalWrite(D8, LOW);
      delay(100);
    } else if (tag == "250174219128") {
      Serial.println("2-VIVEK MISHRA : ATTENDANCE RECORDED SUCCESSFULLY");
      
      digitalWrite(D8, HIGH);
      delay(1000);
      digitalWrite(D8, LOW);
      delay(1000);
     
    } else {
      Serial.println("Access Denied!");
      digitalWrite(D8, HIGH);
      delay(2000);
      digitalWrite(D8, LOW);
    }
    tag = "";
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
  if (! Attendance.publish(ID)) {                     //Publish to Adafruit
      Serial.println(F("Failed"));
    } else {
      Serial.println(F("Sent!"));
    }
}
