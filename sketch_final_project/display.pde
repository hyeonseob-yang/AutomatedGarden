import processing.serial.*;

Serial myPort;

PFont font;
Button waterButton = new Button(600, 600, "Water");


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

void draw(){ //<>//
  if (0 < myPort.available()){
    byte[] inBuffer = new byte[96]; //increase this number if issue is "buffer to small to ..."
    myPort.readBytesUntil('&', inBuffer);
    String dataString = new String(inBuffer);
    if (!dataString.trim().isEmpty()) {
      println("New"); //<>//
      println(new String(inBuffer));
      int moistureVal = (int)getValue(dataString, "aaa");
      float humidity = getValue(dataString, "bbb");
      int lightVal = (int)getValue(dataString, "ccc");
      float lightPercent = getValue(dataString, "ddd");
      float temp = getValue(dataString, "eee");
      int timeLeft = (int)getValue(dataString, "fff");
      drawBackground();
      drawMoistureInfo(moistureVal, humidity);
      drawLightInfo(lightVal, lightPercent);
      drawTempInfo(temp);
      drawTimeLeft(timeLeft);
    }
  }
  waterButton.draw();
}

void mouseReleased() {
  if (waterButton.isOver()) {
    String msg = "1\r\n";
    myPort.write(msg);
  }
}

void drawBackground() {
  //example display thing
  background(220);
}

void drawMoistureInfo(int moistureVal, float humidity) {
  fill(0);
  textAlign(LEFT, TOP);
  text("Moisture Value: " + moistureVal, 20, 45);
  text("Humidity: " + humidity + "%", 20, 65);
  drawBar(20, 80, humidity);
}

void drawLightInfo(int lightVal, float lightPercent) {
  fill(0);
  textAlign(LEFT, TOP);
  text("Light Value: " + lightVal, 20, 200);
  text("Light Exposure: " + lightPercent + "%", 20, 220);
  drawBar(20, 235, lightPercent);
}

void drawTempInfo(float temp) {
  fill(0);
  textAlign(LEFT, TOP);
  text("Temperature: " + temp + "C", 20, 350);
}

void drawTimeLeft(int timeLeft) {
  fill(0);
  int secondsLeft = timeLeft / 1000;
  textAlign(LEFT, TOP);
  text("Seconds Until Water: " + secondsLeft, 20, 450);
}

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

float getValue(String dataString, String wrapping) { //<>//
  String[] tokens = splitTokens(dataString, "&"); //<>//
  String[] values = splitTokens(tokens[0], wrapping);
  float value = Float.valueOf(values[1]); //<>// //<>// //<>//
  return value;
}
