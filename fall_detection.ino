#include <Wire.h>
#include <MPU6050.h>
#include <SoftwareSerial.h>
#include <math.h>

// Pins
const int ledPin = 8;
const int buzzerPin = 9;
const int simRx = 2;
const int simTx = 3;
const int gpsRx = 5;
const int gpsTx = 6;

const float impactThreshold = 1.4;

MPU6050 mpu;
SoftwareSerial sim800l(simRx, simTx);
SoftwareSerial gpsSerial(gpsRx, gpsTx);

const char phoneNumber[] = "xxxxxxxxx";

float lat_dd = 0.0;
float lon_dd = 0.0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();

  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  sim800l.begin(9600);
  gpsSerial.begin(9600);
  delay(1500);

  sim800l.println("AT");
  delay(500);
  sim800l.println("AT+CMGF=1");
  delay(500);
}

void loop() {
  readGPS();

  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  float ax_g = ax / 16384.0;
  float ay_g = ay / 16384.0;
  float az_g = az / 16384.0;
  float totalAccel = sqrt(ax_g * ax_g + ay_g * ay_g + az_g * az_g);

  if (totalAccel >= impactThreshold) {

    String link = "https://maps.google.com/?q=" + String(lat_dd,6) + "," + String(lon_dd,6);
    String msg = "Accident detected.\nAccel: " + String(totalAccel,2) + "g\nLocation:\n" + link;

    triggerAlert(msg);
    delay(8000);
  }

  delay(1000);
}

void readGPS() {
  static String line = "";

  while (gpsSerial.available()) {
    char c = gpsSerial.read();
    if (c == '\n') {
      if (line.startsWith("$GPGGA")) {
        parseGPGGA(line);
      }
      line = "";
    } else {
      line += c;
    }
  }
}

void parseGPGGA(String data) {
  int firstComma = data.indexOf(',');
  int secondComma = data.indexOf(',', firstComma + 1);
  int thirdComma = data.indexOf(',', secondComma + 1);
  int fourthComma = data.indexOf(',', thirdComma + 1);

  String latRaw = data.substring(secondComma + 1, thirdComma);
  String latDir = data.substring(thirdComma + 1, thirdComma + 2);

  String lonRaw = data.substring(fourthComma + 1, data.indexOf(',', fourthComma + 1));
  String lonDir = data.substring(data.indexOf(',', fourthComma + 1) + 1, data.indexOf(',', fourthComma + 2));

  if (latRaw.length() > 5 && lonRaw.length() > 5) {
    lat_dd = convertToDecimal(latRaw);
    lon_dd = convertToDecimal(lonRaw);

    if (latDir == "S") lat_dd = -lat_dd;
    if (lonDir == "W") lon_dd = -lon_dd;
  }
}

float convertToDecimal(String val) {
  float raw = val.toFloat();
  int degrees = int(raw / 100);
  float minutes = raw - (degrees * 100);
  return degrees + minutes / 60.0;
}

void triggerAlert(String msg) {
  digitalWrite(ledPin, HIGH);
  digitalWrite(buzzerPin, HIGH);

  sendSMS(msg);
  makeCall();

  delay(3000);
  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);
}

void sendSMS(String msg) {
  sim800l.println("AT+CMGF=1");
  delay(300);
  sim800l.print("AT+CMGS=\"");
  sim800l.print(phoneNumber);
  sim800l.println("\"");
  delay(1000);
  sim800l.print(msg);
  delay(200);
  sim800l.write(26);
  delay(3000);
}

void makeCall() {
  sim800l.print("ATD");
  sim800l.print(phoneNumber);
  sim800l.println(";");
  delay(10000);
  sim800l.println("ATH");
  delay(500);
}
