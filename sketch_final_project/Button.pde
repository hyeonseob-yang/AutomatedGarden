class Button {
  int x;
  int y;
  String text;
  
  int bWidth = 100;
  int bHeight = 40;
  int margin = 5;
  
  color regular = color(50, 50, 50);
  color hover = color(100, 100, 100);
  
  // Initializes Button fields
  Button(int x, int y, String text) {
     this.x = x;
     this.y = y;
     this.text = text;
  }
  
  // Draws button
  void draw() {
    noStroke();
    fill(regular);
    if (this.isOver()) {
      fill(hover);
    }
    textAlign(LEFT, TOP);
    rect(this.x, this.y, this.bWidth, this.bHeight);
    fill(255);
    textAlign(CENTER, CENTER);
    text(text, x + (this.bWidth / 2), y + (this.bHeight / 2));
  }
  
  // Returns true if mouse is over button
  Boolean isOver() {
    return mouseX > this.x && mouseX < this.x + this.bWidth &&
      mouseY > this.y && mouseY < this.y + this.bHeight;
  }
}
