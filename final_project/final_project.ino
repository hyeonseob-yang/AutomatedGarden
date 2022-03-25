/* References:
 *  Soil Moisture: https://www.youtube.com/watch?v=gWDzS5guXzk&t=239s
 *  Temperature: https://learn.adafruit.com/tmp36-temperature-sensor
 *  Relay Info: https://www.youtube.com/watch?v=kfPzXbhTQQk
 *  Water Pump: https://www.youtube.com/watch?v=Z0SZ-jzu_q8
 *  Servo Motor: https://docs.arduino.cc/learn/electronics/servo-motors
 */
#include <Servo.h>

int moisturePin = 0;
const int DRY = 932;
const int WET = 603;
const int MOISTURE_CUTOFF = (DRY + WET) / 2;

int lightPin = 1;
const int DARK = 5;
const int LIGHT = 135;
const int MID_LIGHT = (DARK + LIGHT) / 2;
int lightTime = 0;
int totalTime = 0;

int tempPin = 2;
const int MAX_MILLIVOLTS = 5000;
const int TEMP_CONSTANT = 500;
const int TEMP_COEFFICIENT = 10;
const int ANALOG_INPUT_MAX = 1023;

int pumpPin = 7;

Servo servo;
int motorPin = 9;
int currentAngle = 0;
int angleCoefficient = 1;
const int FIXED_ROTATION = 30;
const int ANGLE_MAX = 180;
const int ANALOG_OUTPUT_MAX = 255;
// Time in seconds to wait before rotating again
const int rotatePeriod = 5;
int rotateTimer = 0;

void setup() {
  Serial.begin(9600);
  pinMode(pumpPin, OUTPUT);
  servo.attach(9);
  servo.write(0);
  // Wait for motor to get into starting position
  delay(1000);
}

void loop() {
  delay(1000);
  int humidity = getMoisture();
  int light = getLight();
  if (isBright(light)) {
    lightTime++;
  }
  totalTime++;
  int lightPercent = (float) lightTime / totalTime * 100;
  float temp = getTemp();
  printData(humidity, lightPercent, temp);

  bool willRotate = shouldRotate();
  if (willRotate) {
//    if (!isWet(humidity)) {
//      pumpWater();
//    } else {
      rotateChassis(FIXED_ROTATION);
//    }
  } else {
    rotateTimer++;
  }
  
}

bool shouldRotate() {
  // Use + 1 because rotate code will increment once
  bool willRotate = rotateTimer % (rotatePeriod + 1) <= 0;
  return willRotate;
}

void rotateChassis(int angle) {
  angle *= angleCoefficient;
  currentAngle += angle;
  servo.write(currentAngle);
  delay(100);
  if (currentAngle == ANGLE_MAX) {
    // Set so next call to rotate chassis will go to 0
    currentAngle = angle * -1;
    // Set so motor will wait for rotatePeriod to pass
    rotateTimer++;
  }
}

void pumpWater() {
  digitalWrite(pumpPin, HIGH);
  delay(1000);
  digitalWrite(pumpPin, LOW);
}

int getMoisture() {
  int moistureVal = analogRead(moisturePin);
  int percentageHumidity = 100 * (float)(DRY - moistureVal) / (float)(DRY - WET);
  return percentageHumidity;
}

bool isWet(int humidity) {
  return humidity > MOISTURE_CUTOFF;
}

int getLight() {
  int lightVal = analogRead(lightPin);
  return lightVal;
}

int isBright(int light) {
  return light > MID_LIGHT;
}

float getTemp() {
  // ACCURACY OF TEMP SENSOR HAS A VARIANCE OF ABOUT 4 C
  int tempVal = analogRead(tempPin);
  float temp = ((((float)tempVal / ANALOG_INPUT_MAX) * MAX_MILLIVOLTS) - TEMP_CONSTANT) / TEMP_COEFFICIENT;
  return temp;
}

void printData(int humidity, int lightPercent, float temp) {
  Serial.print("aaa");
  Serial.print(humidity);
  Serial.println("aaa");
  Serial.print("bbb");
  Serial.print(lightPercent);
  Serial.println("bbb");
  Serial.print("ccc");
  Serial.print(temp);
  Serial.println("ccc");
  Serial.print("&");
}
