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
int moistureCutoff = (DRY + WET) / 2;

const int lightPin = 1;
const int DARK = 130;
const int LIGHT = 650;
int lightCutoff = (DARK + LIGHT) / 2;
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
const int WATER_TIME = 100;

Servo servo;
int motorPin = 9;
int currentAngle = 0;
int angleCoefficient = 1;
const int NUM_STEPS = 2;
const int FIXED_ROTATION = 30;
const int ANGLE_MAX = (NUM_STPES - 1) * FIXED_ROTATION;
const int ANALOG_OUTPUT_MAX = 255;
// Time in seconds to wait before rotating again
unsigned long checkWaterTime = 5000;
auto waterTimer = timer_create_default();

// Sets up serial monitor, pins, and timers
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

// Ticks timers and checks for input
void loop() {
  waterTimer.tick();
  dataTimer.tick();
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil("\r\n");
    String dataString = getValue(data, "aaa");
    bool isWater = dataString == "1";
    moistureCutoff = getValue(data, "bbb").toInt();
    lightCutoff = getValue(data, "ccc").toInt();
    checkWaterTime = strtoul(getValue(data, "ddd").c_str(), NULL, 10);
    if (isWater) {
       water();
    }
  }
}

String getValue(String data, String separator) {
  int firstIndex = data.indexOf(separator) + separator.length();
  String unwrapped = data.substring(firstIndex, data.length() - 1);
  int secondIndex = unwrapped.indexOf(separator) + firstIndex;
  String value = data.substring(firstIndex, secondIndex);
  return value;
}

// Collects data from sensors and prints to serial monitor
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

// Checks if soil is dry and waters if dry
void checkWater() {
  int moisture = getMoisture();
  if (!isWet(moisture)) {
    water();
  }
  
  // Start another timer to check when to water after watering process is done
  waterTimer.in(checkWaterTime, checkWater);
}

// Waters soil
void water() {
  for (int i=0; i < NUM_STEPS; i++) {
    pumpWater();
    rotateChassis(FIXED_ROTATION);
  }
}

// Uses servomotor to rotate water tank
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

// Pumps water through piping
void pumpWater() {
  digitalWrite(pumpPin, HIGH);
  delay(WATER_TIME);
  digitalWrite(pumpPin, LOW);
  delay(WATER_TIME);
  collectData();
}

// Gets the moisture value from the soil moisture sensor
int getMoisture() {
  int moistureVal = analogRead(moisturePin);
  return moistureVal;
  
}

// Converts the moisture value to a humidity percentage
float getHumidity(int moistureVal) {
  float humidity = (float)(DRY - moistureVal) / (float)(DRY - WET) * PERCENT;
  return humidity;
}

// Returns true if wet
bool isWet(int moisture) {
  return moisture < moistureCutoff;
}

// Gets light value from photoresistor
int getLight() {
  int lightVal = analogRead(lightPin);
  return lightVal;
}

// Returns true if bright
int isBright(int light) {
  return light > lightCutoff;
}

// Gets the temperature from the temperature sensor
float getTemp() {
  // ACCURACY OF TEMP SENSOR HAS A VARIANCE OF ABOUT 4 C
  int tempVal = analogRead(tempPin);
  float temp = ((((float)tempVal / ANALOG_INPUT_MAX) * MAX_MILLIVOLTS) - TEMP_CONSTANT) / TEMP_COEFFICIENT;
  return temp;
}

// Prints all sensor data and other values to serial monitor
void printData(int moisture, float humidity, int light, float lightPercent, float temp, unsigned long timerTick) {
  Serial.println("!");
  Serial.print("aaa");
  Serial.print(moisture);
  Serial.println("aaa");
  Serial.print("bbb");
  Serial.print(humidity);
  Serial.println("bbb");
  Serial.print("ccc");
  Serial.print(moistureCutoff);
  Serial.println("ccc");
  Serial.print("ddd");
  Serial.print(light);
  Serial.println("ddd");
  Serial.print("eee");
  Serial.print(lightPercent);
  Serial.println("eee");
  Serial.print("fff");
  Serial.print(lightCutoff);
  Serial.println("fff");
  Serial.print("ggg");
  Serial.print(temp);
  Serial.println("ggg");
  Serial.print("hhh");
  Serial.print(timerTick);
  Serial.println("hhh");
  Serial.print("iii");
  Serial.print(checkWaterTime);
  Serial.println("iii");
  Serial.print("&");
}
