import processing.serial.*;

Serial myPort;

int moisture_val;
int light_val;
int temp_val;
PFont font;

byte[] inBuffer = new byte[36]; //increase this number if issue is "buffer to small to ..."

void setup(){
  size(500,400);
  frameRate(10);
  printArray(Serial.list());
  String port = Serial.list()[2]; // <-- make sure its the right port
  myPort = new Serial(this, port, 9600);
}

void draw(){
  if (0 < myPort.available()){
    myPort.readBytesUntil('&', inBuffer);
    if (inBuffer !=null){
      String myString = new String(inBuffer);
      String[] p = splitTokens(myString, "&");
      String[] moisture_sensor = splitTokens(p[0], "aaa");
      moisture_val = int(moisture_sensor[1]);
      print("Moisture: "); // the prints are here just for testing
      print(moisture_val);
      println("%");
      
      String[] light_sensor = splitTokens(p[0], "bbb");
      light_val = int(light_sensor[1]);
      print("Light: "); // the prints are here just for testing
      print(light_val);
      println(" ");
      
      String[] temp_sensor = splitTokens(p[0], "ccc");
      temp_val = int(temp_sensor[1]);
      print("Temperature: "); // the prints are here just for testing
      print(temp_val);
      println(" C");
      
      //example display thing
      background(220);
      noFill();
      stroke(0);
      strokeWeight(10);
      rect(20,75,460,75);
      rect(20,225,460,75);
      
      //kinda like a health bar, not sure what to have for the temperature
      float a = map(moisture_val, 0, 100, 0, 450); // change the first and second number maybe for other displays
      noStroke();
      fill(0,0,205);
      rect(25,80,a,65);
      
      float b = map(light_val, 0, 100, 0, 450);
      fill(255);
      rect(25,231,b,64);
       // if ur missing the font, on Processing, go to Tools tab, Create Font, and then find Consolas, size=48
      font = loadFont("Consolas-48.vlw");
      fill(0);
      textFont(font);
      textSize(32);
      text("Moisture: " + a + "%", 20, 45);
      text("Light Exposure: " + light_val + "%", 20, 200);
      text("Temperature: " + temp_val + "C", 20, 350);
    }
  }
}
