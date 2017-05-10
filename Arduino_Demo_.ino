#include <IvenCloudESP.h>
#include <SimpleDHT.h>

#define SENSOR_1 4
#define SENSOR_2 A2
#define SENSOR_3 6
#define SENSOR_4 A0
#define SENSOR_5 A1


#define SECRET_KEY "93bb4aee6fc9ba9502a4022931a59ea1f6421250"
#define DEVICE_UID "demo1234"

#define SSID "IVEN_DEVICE"
#define PASSWORD "Iven2016"

#define FREQUENCY 1000


SimpleDHT11 dht;
bool isActive = false;
IvenCloudESP client(2, 3, 9600);
void setup() {
  Serial.begin(115200);
  Serial.println("start");

  SoftwareSerial esp(2, 3);
  esp.begin(9600);
  
//   while (true) {
//       while (Serial.available())
//         esp.print((char)Serial.read());
//       while (esp.available())
//         Serial.print((char)esp.read());
//       delay(9);
//     }
    
  delay(3000);
  esp.println("AT+CWJAP=\"" SSID "\",\"" PASSWORD "\"");
  delay(1000);
  Serial.print(esp.readString());

  Serial.println("ok");
}
void loop() {

  if (!isActive) {
    IvenResponse resp = client.activateDevice(SECRET_KEY, DEVICE_UID);
    if (resp.error == IR_OK) {
      isActive = true;
      Serial.println("activated");
    } else {
      Serial.print("Activate Error: code ");
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
    Serial.print("Send Data iven code: ");
    Serial.println(result.ivenCode);
    delay(FREQUENCY);
  }
}
