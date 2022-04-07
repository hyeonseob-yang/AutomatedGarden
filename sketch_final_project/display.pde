/* References:
/ Button: https://processing.org/examples/button.html
*/

import processing.serial.*;

Serial myPort;

PFont font;
Button updateButton = new Button(500, 600, "Update");
Button waterButton = new Button(600, 600, "Water");


// Sets up screen and serial monitor
void setup(){
  size(1000,1000);
  frameRate(10);
  printArray(Serial.list());
  String port = Serial.list()[2]; // <-- make sure its the right port
  myPort = new Serial(this, port, 9600);
  // if ur missing the font, on Processing, go to Tools tab, Create Font, and then find Consolas, size=48
  font = loadFont("Consolas-48.vlw");
  textFont(font);
  textSize(16);
}

// Reads input and draws elements
void draw(){ //<>//
  if (0 < myPort.available()){
    byte[] inBuffer = new byte[255]; //increase this number if issue is "buffer to small to ..."
    myPort.readBytesUntil('&', inBuffer);
    String dataString = new String(inBuffer);
    if (!dataString.trim().isEmpty()) {
      println("New"); //<>//
      println(new String(inBuffer));
      int moistureVal = (int)getValue(dataString, "aaa");
      float humidity = getValue(dataString, "bbb");
      int moistureCutoff = (int)getValue(dataString, "ccc");
      int lightVal = (int)getValue(dataString, "ddd");
      float lightPercent = getValue(dataString, "eee");
      int lightCutoff = (int)getValue(dataString, "fff");
      float temp = getValue(dataString, "ggg");
      int timeLeft = (int)getValue(dataString, "hhh");
      drawBackground();
      drawMoistureInfo(moistureVal, humidity, moistureCutoff);
      drawLightInfo(lightVal, lightPercent, lightCutoff);
      drawTempInfo(temp);
      drawTimeLeft(timeLeft);
    }
  }
  waterButton.draw();
}

// Checks if buttons are pressed
void mouseReleased() {
  if (waterButton.isOver()) {
    String msg = "1\r\n";
    myPort.write(msg);
  }
  if (updateButton.isOver()) {
    String msg = "aaa0aaabbb30bbb";
    myPort.write(msg);
  }
}

// Draws background
void drawBackground() {
  //example display thing
  background(220);
}

// Draws info related to soil moisture sensor
void drawMoistureInfo(int moistureVal, float humidity, int moistureCutoff) {
  fill(0);
  textAlign(LEFT, TOP);
  text("Moisture Value: " + moistureVal, 20, 45);
  text("Humidity: " + humidity + "%", 20, 65);
  text("Moisture Cutoff: " + moistureCutoff, 20, 85);
  drawBar(20, 100, humidity);
}

// Draws info related to photoresistor
void drawLightInfo(int lightVal, float lightPercent, int lightCutoff) {
  fill(0);
  textAlign(LEFT, TOP);
  text("Light Value: " + lightVal, 20, 200);
  text("Light Exposure: " + lightPercent + "%", 20, 220);
  text("Light Cutoff: " + lightCutoff, 20, 240);
  drawBar(20, 255, lightPercent);
}

// Draws info related to temperature sensor
void drawTempInfo(float temp) {
  fill(0);
  textAlign(LEFT, TOP);
  text("Temperature: " + temp + "C", 20, 350);
}

// Draws time left on water timer
void drawTimeLeft(int timeLeft) {
  fill(0);
  int secondsLeft = timeLeft / 1000;
  textAlign(LEFT, TOP);
  text("Seconds Until Water: " + secondsLeft, 20, 450);
}

// Draws bar that displays percents graphically
void drawBar(int x, int y, float percent) {
  float a = map(percent, 0, 100, 0, 460);
  stroke(0);
  strokeWeight(1);
  noFill();
  rect(x, y, 460, 75);
  noStroke();
  fill(0, 0, 205);
  rect(x, y, a, 75);
}

// Gets value from serial input and processes
float getValue(String dataString, String wrapping) { //<>//
  String[] tokens = splitTokens(dataString, "&"); //<>//
  String[] values = splitTokens(tokens[0], wrapping);
  float value = Float.valueOf(values[1]); //<>// //<>// //<>//
  return value;
}
