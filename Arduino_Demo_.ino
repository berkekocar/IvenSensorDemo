#include <IvenCloudESP.h>
#include <SimpleDHT.h>

#define SENSOR_1 4
#define SENSOR_2 A2
#define SENSOR_3 6
#define SENSOR_4 A0
#define SENSOR_5 A1



#define SECRET_KEY "a0b5e7487d6f166c272182301cdc5b83ced04000"
#define DEVICE_UID "BERKEKOCAR"

SimpleDHT11 dht;
bool isActive = false;

IvenCloudESP client(2, 3, 9600);
void setup() {
  Serial.begin(115200);
  Serial.println("start");
  //  SoftwareSerial esp(2, 3);
  //  esp.begin(9600);
  //  esp.println("AT+CWJAP=\"IVEN_TEST\",\"Iven2016\"");
  //  while (true) {
  //    while (Serial.available())
  //      esp.print((char)Serial.read());
  //    while (esp.available())
  //      Serial.print((char)esp.read());
  //    delay(99);
  //  }
  //  Serial.println("ok");
}
void loop() {


  if (!isActive) {
    IvenResponse resp = client.activateDevice(SECRET_KEY, DEVICE_UID);
    if (resp.error == IR_OK) {
      isActive = true;
      Serial.println("activated");
    } else {
      Serial.println(resp.error);
    }
  } else {
    byte temp, hum;
    byte ldr, snd, vbr, gas;
    IvenData data;

    dht.read(SENSOR_1, &temp, &hum, NULL);
    ldr = analogRead(SENSOR_4);
    gas = analogRead(SENSOR_5);
    vbr = digitalRead(SENSOR_3);
    snd = analogRead(SENSOR_2);

    if (temp > 30) {
      digitalWrite(7, HIGH);
    }
    Serial.println("TEMP : " + String(temp));
    Serial.println("HUMIDITY : " + String(hum));
    Serial.println("LIGHT : " + String(ldr));
    Serial.println("SOUND : " + String(snd));
    Serial.println("VIBRATION : " + String(vbr));
    Serial.println("GAS : " + String(gas));
    delay(1000);

    data.add("temp", temp);
    data.add("hum", hum);
    data.add("ldr", ldr);
    data.add("snd", snd);
    data.add("vbr", vbr);
    data.add("gas", gas);

    IvenResponse result = client.sendData(data);
    delay(333);
    Serial.println(result.ivenCode);
  }
}
