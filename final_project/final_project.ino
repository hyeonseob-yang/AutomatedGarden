/* References:
 *  Soil Moisture: https://www.youtube.com/watch?v=gWDzS5guXzk&t=239s
 *  Temperature: https://learn.adafruit.com/tmp36-temperature-sensor
 *  Relay Info: https://www.youtube.com/watch?v=kfPzXbhTQQk
 *  Water Pump: https://www.youtube.com/watch?v=Z0SZ-jzu_q8
 *  Servo Motor: https://docs.arduino.cc/learn/electronics/servo-motors
 */
#include <Servo.h>
#include <arduino-timer.h>

const int moisturePin = 0;
const int DRY = 1023;
const int WET = 0;
const int MOISTURE_CUTOFF = 50;

const int lightPin = 1;
const int DARK = 130;
const int LIGHT = 650;
const int MID_LIGHT = (DARK + LIGHT) / 2;
int lightTime = 0;
int totalTime = 0;
const int PERCENT = 100;

const int tempPin = 2;
const int MAX_MILLIVOLTS = 5000;
const int TEMP_CONSTANT = 500;
const int TEMP_COEFFICIENT = 10;
const int ANALOG_INPUT_MAX = 1023;

const int DATA_TIME = 1000;
auto dataTimer = timer_create_default();

const int pumpPin = 7;
const int WATER_TIME = 1000;

Servo servo;
int motorPin = 9;
int currentAngle = 0;
int angleCoefficient = 1;
const int FIXED_ROTATION = 30;
const int ANGLE_MAX = 180;
const int ANALOG_OUTPUT_MAX = 255;
// Time in seconds to wait before rotating again
unsigned long checkWaterTime = 5000;
auto waterTimer = timer_create_default();
const int NUM_STEPS = 7;

void setup() {
  Serial.begin(9600);
  pinMode(pumpPin, OUTPUT);
  servo.attach(motorPin);
  servo.write(0);
  // Wait for motor to get into starting position
  delay(1000);
  // Have to use timer.in over timer.every because water process is blocking code
  waterTimer.in(checkWaterTime, checkWater);
  dataTimer.every(DATA_TIME, collectData);
}

void loop() {
  waterTimer.tick();
  dataTimer.tick();
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\r\n');
    bool isWater = (bool)data.substring(0, data.length() - 2);
    if (isWater) {
      water();
    }
  }
}

void collectData() {
  int moisture = getMoisture();
  float humidity = getHumidity(moisture);
  int light = getLight();
  if (isBright(light)) {
    lightTime++;
  }
  totalTime++;
  float lightPercent = (float) lightTime / totalTime * PERCENT;
  float temp = getTemp();
  printData(moisture, humidity, light, lightPercent, temp, waterTimer.ticks());
}

void checkWater() {
  int moisture = getMoisture();
  float humidity = getHumidity(moisture);
  if (!isWet(humidity)) {
    water();
  }
  
  // Start another timer to check when to water after watering process is done
  waterTimer.in(checkWaterTime, checkWater);
}

void water() {
  for (int i=0; i < NUM_STEPS; i++) {
    pumpWater();
    rotateChassis(FIXED_ROTATION);
  }
}


void rotateChassis(int angle) {
  angle *= angleCoefficient;
  currentAngle += angle;
  servo.write(currentAngle);
  delay(1000);
  collectData();
  if (currentAngle == ANGLE_MAX) {
    currentAngle = angle * -1;
  }
}

void pumpWater() {
  digitalWrite(pumpPin, HIGH);
  delay(500);
  digitalWrite(pumpPin, LOW);
  delay(500);
  collectData();
}

int getMoisture() {
  int moistureVal = analogRead(moisturePin);
  return moistureVal;
  
}

float getHumidity(int moistureVal) {
  float humidity = (float)(DRY - moistureVal) / (float)(DRY - WET) * PERCENT;
  return humidity;
}

bool isWet(float humidity) {
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

void printData(int moisture, float humidity, int light, float lightPercent, float temp, unsigned long timerTick) {
  Serial.println("!");
  Serial.print("aaa");
  Serial.print(moisture);
  Serial.println("aaa");
  Serial.print("bbb");
  Serial.print(humidity);
  Serial.println("bbb");
  Serial.print("ccc");
  Serial.print(light);
  Serial.println("ccc");
  Serial.print("ddd");
  Serial.print(lightPercent);
  Serial.println("ddd");
  Serial.print("eee");
  Serial.print(temp);
  Serial.println("eee");
  Serial.print("fff");
  Serial.print(timerTick);
  Serial.println("fff");
  Serial.print("&");
}
