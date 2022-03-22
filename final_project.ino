/* References:
 *  Soil Moisture: https://www.youtube.com/watch?v=gWDzS5guXzk&t=239s
 *  Temperature: https://learn.adafruit.com/tmp36-temperature-sensor
 *  Relay Info: https://www.youtube.com/watch?v=kfPzXbhTQQk
 *  Water Pump: https://www.youtube.com/watch?v=Z0SZ-jzu_q8
 */

int moisturePin = 0;
const int DRY = 932;
const int WET = 603;

int lightPin = 1;

int tempPin = 2;
const int ANALOG_MAX = 1023;
const int MAX_MILLIVOLTS = 5000;
const int TEMP_CONSTANT = 500;
const int TEMP_COEFFICIENT = 10;

int pumpPin = 7;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pumpPin, OUTPUT);
}

void loop() {
  printMoisture();
  printLight();
  printTemp();
  digitalWrite(pumpPin, HIGH);

  delay(1000);

  digitalWrite(pumpPin, LOW);
}

void printMoisture() {
  int moistureVal = analogRead(moisturePin);
  int percentageHumidity = 100 * (float)(DRY - moistureVal) / (float)(DRY - WET);
  Serial.print("MOISTURE: ");
  Serial.print(percentageHumidity);
  Serial.println("%");
}

void printLight() {
  int lightVal = analogRead(lightPin);
  Serial.print("LIGHT: ");
  Serial.println(lightVal);
}

void printTemp() {
  // ACCURACY OF TEMP SENSOR HAS A VARIANCE OF ABOUT 4 C
  int tempVal = analogRead(tempPin);
  float temp = ((((float)tempVal / ANALOG_MAX) * MAX_MILLIVOLTS) - TEMP_CONSTANT) / TEMP_COEFFICIENT;
  Serial.println(tempVal);
  Serial.print("TEMP: ");
  Serial.print(temp);
  Serial.println(" C");
}
