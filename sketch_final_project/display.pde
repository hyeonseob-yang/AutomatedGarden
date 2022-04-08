/* References:
/ Button: https://processing.org/examples/button.html
*/

import processing.serial.*;
import controlP5.*;

ControlP5 p5;
int FIELD_WIDTH = 200;
int FIELD_HEIGHT = 40;

Serial myPort;

PFont font;
Button updateButton = new Button(680, 600, "Update");
Button waterButton = new Button(800, 600, "Water");

int moistureCutoff;
int lightCutoff;
int timeLeft;
int maxTime;
String moistureCutoffField = "moistureCutoff";
String lightCutoffField = "lightCutoff";
String timerField = "timer";


// Sets up screen and serial monitor
void setup(){
  size(1000,1000);
  frameRate(10);
  printArray(Serial.list());
  String port = Serial.list()[0]; // <-- make sure its the right port
  myPort = new Serial(this, port, 9600);
  
  // if ur missing the font, on Processing, go to Tools tab, Create Font, and then find Consolas, size=48
  font = loadFont("Consolas-48.vlw");
  textFont(font);
  textSize(16);
  
  p5 = new ControlP5(this);
  addTextField(20, 600, moistureCutoffField, "Moisture Cutoff: ", true);
  addTextField(240, 600, lightCutoffField, "Light Cutoff: ", false);
  addTextField(460, 600, timerField, "Timer: ", false); 
}

void addTextField(int x, int y, String name, String label, Boolean focus) {
  Textfield m = p5.addTextfield(name).setPosition(x, y).setSize(FIELD_WIDTH, FIELD_HEIGHT).setColor(0).setColorBackground(color(255,255,255)).setFocus(focus);
  m.setText("");
  Label mLabel = m.getCaptionLabel();
  mLabel.setText(label);
  mLabel.align(LEFT, ControlP5.TOP_OUTSIDE);
  mLabel.setColor(color(0, 0, 0));
}

// Reads input and draws elements
void draw(){ //<>//
  if (0 < myPort.available()){
    byte[] inBuffer = new byte[256]; //increase this number if issue is "buffer to small to ..."
    myPort.readBytesUntil('&', inBuffer);
    String dataString = new String(inBuffer);
    if (!dataString.trim().isEmpty()) {
      println("New"); //<>//
      println(new String(inBuffer));
      int moistureVal = (int)getValue(dataString, "aaa");
      float humidity = getValue(dataString, "bbb");
      moistureCutoff = (int)getValue(dataString, "ccc");
      int lightVal = (int)getValue(dataString, "ddd");
      float lightPercent = getValue(dataString, "eee");
      lightCutoff = (int)getValue(dataString, "fff");
      float temp = getValue(dataString, "ggg");
      timeLeft = (int)getValue(dataString, "hhh");
      maxTime = (int)getValue(dataString, "iii");
      drawBackground();
      drawMoistureInfo(moistureVal, humidity, moistureCutoff);
      drawLightInfo(lightVal, lightPercent, lightCutoff);
      drawTempInfo(temp);
      drawTime(timeLeft);
    }
  }
  waterButton.draw();
  updateButton.draw();
}

// Checks if buttons are pressed
void mouseReleased() {
  if (waterButton.isOver()) {
    String msg = "aaa1aaabbb" + moistureCutoff + "bbbccc" + lightCutoff + "cccddd" + maxTime + "ddd\r\n";
    println(msg);
    myPort.write(msg);
  }
  if (updateButton.isOver()) {
    update();
  }
}

// Updates Arduino values based on text input fields
void update() {
  String mCutoff = p5.get(Textfield.class, moistureCutoffField).getText();
  if (mCutoff.equals("")) {
    mCutoff = Integer.toString(moistureCutoff);
  }
  String lCutoff = p5.get(Textfield.class, lightCutoffField).getText();
  if (lCutoff.equals("")) {
    lCutoff = Integer.toString(lightCutoff);
  }
  String t = p5.get(Textfield.class, timerField).getText();
  if (t.equals("")) {
    t = Integer.toString(maxTime);
  }
  String msg = "aaa0aaabbb" + mCutoff + "bbbccc" + lCutoff + "cccddd" + t + "ddd\r\n";
  println(msg);
  myPort.write(msg);
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
void drawTime(int timeLeft) {
  fill(0);
  int secondsLeft = timeLeft / 1000;
  textAlign(LEFT, TOP);
  text("Seconds Until Water: " + secondsLeft, 20, 450);
  text("Seconds Between Cycles: " + maxTime, 20, 470);
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
