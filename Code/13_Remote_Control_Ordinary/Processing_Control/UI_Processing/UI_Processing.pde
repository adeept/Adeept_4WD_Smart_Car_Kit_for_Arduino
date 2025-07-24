import controlP5.*;
import processing.net.*;
import java.awt.event.KeyEvent; // imports library for reading the data from the serial port
import java.io.IOException;

ControlP5 cp5;

controlP5.Button k1;
controlP5.Button k2;
controlP5.Button k3;
controlP5.Button k4;
controlP5.Button k5;
controlP5.Button k6;
controlP5.Button k7;

controlP5.Button policeButton;
controlP5.Button matrixScreenButton;
controlP5.Button buzzerButton;
controlP5.Button lightTrackingButton;
controlP5.Button keepDistanceButton;
controlP5.Button Stop;
controlP5.Button left;
controlP5.Button right;
controlP5.Button Home;
controlP5.Button StopFunction;

// defubes variables
String angle = "";
String distance = "";
String data = "";
String noObject;
float pixsDistance;
int iAngle = 181, iDistance, iOldAngle;
int index1 = 0;
int index2 = 0;

Client myClient;
int dataIn;
int dataIn0;
int dataIn1;
int dataIn2;
int dataIn3;
int dataIn4;
int dataIn5;
int dataIn6;
int dataIn7;
int dataNum = 0;
int Radarwidth = 1200;


void setup() {
  size(1400, 900); // ***CHANGE THIS TO YOUR SCREEN RESOLUTION***
  smooth();
  myClient = new Client(this, "192.168.4.1", 4000);

  cp5 = new ControlP5(this);
  PFont pfont = createFont("Arial", 20, true);
  ControlFont font = new ControlFont(pfont, 241);

  k1 = cp5.addButton("Forward")
    .setPosition(110, 130)
    .setSize(90, 60);
  k1.getCaptionLabel()
    .setFont(font)
    .setSize(15)
    .toUpperCase(false)
    .setText("Forward");

  k2 = cp5.addButton("Turn Left")
    .setPosition(10, 200)
    .setSize(90, 60);
  k2.getCaptionLabel()
    .setFont(font)
    .setSize(15)
    .toUpperCase(false)
    .setText("Turn Left");

  k3 = cp5.addButton("Turn Right")
    .setPosition(210, 200)
    .setSize(90, 60);
  k3.getCaptionLabel()
    .setFont(font)
    .setSize(15)
    .toUpperCase(false)
    .setText("Turn Right");

  k4 = cp5.addButton("Backward")
    .setPosition(110, 200)
    .setSize(90, 60);
  k4.getCaptionLabel()
    .setFont(font)
    .setSize(15)
    .toUpperCase(false)
    .setText("Backward");

  k5 = cp5.addButton("Radar Scanning")
    .setPosition(1230, 140)
    .setSize(150, 60);
  k5.getCaptionLabel()
    .setFont(font)
    .setSize(15)
    .toUpperCase(false)
    .setText("Radar Scanning");

  k6 = cp5.addButton("Tracking Line")
    .setPosition(1230, 220)
    .setSize(150, 60);
  k6.getCaptionLabel()
    .setFont(font)
    .setSize(15)
    .toUpperCase(false)
    .setText("Tracking Line");

  k7 = cp5.addButton("Obstacle Avoidance")
    .setPosition(1230, 300)
    .setSize(150, 60);
  k7.getCaptionLabel()
    .setFont(font)
    .setSize(15)
    .toUpperCase(false)
    .setText("Obstacle Avoidance");

  policeButton = cp5.addButton("Police")
    .setPosition(1230, 540)
    .setSize(150, 60);
  policeButton.getCaptionLabel() 
    .setFont(font)
    .setSize(15)
    .toUpperCase(false)
    .setText("Police");

  matrixScreenButton = cp5.addButton("Matrix Screen")
    .setPosition(1230, 620)
    .setSize(150, 60);
  matrixScreenButton.getCaptionLabel()
    .setFont(font)
    .setSize(15)
    .toUpperCase(false)
    .setText("Matrix Screen");

  //buzzerButton = cp5.addButton("Buzzer")
  //  .setPosition(1230, 700)
  //  .setSize(150, 60);
  //buzzerButton.getCaptionLabel()
  //  .setFont(font)
  //  .setSize(15)
  //  .toUpperCase(false)
  //  .setText("Buzzer");

  lightTrackingButton = cp5.addButton("Light Tracking")
    .setPosition(1230, 460)
    .setSize(150, 60);
  lightTrackingButton.getCaptionLabel()
    .setFont(font)
    .setSize(15)
    .toUpperCase(false)
    .setText("Light Tracking");

  keepDistanceButton = cp5.addButton("Keep Distance")
    .setPosition(1230, 380)
    .setSize(150, 60);
  keepDistanceButton.getCaptionLabel()
    .setFont(font)
    .setSize(15)
    .toUpperCase(false)
    .setText("Keep Distance");

  left = cp5.addButton("Look Left")
    .setPosition(800, 140)
    .setSize(90, 60);
  left.getCaptionLabel()
    .setFont(font)
    .setSize(15)
    .toUpperCase(false)
    .setText("Look Left");

  right = cp5.addButton("Look Right")
    .setPosition(1000, 140)
    .setSize(90, 60);
  right.getCaptionLabel()
    .setFont(font)
    .setSize(15)
    .toUpperCase(false)
    .setText("Look Right");

  Home = cp5.addButton("Home")
    .setPosition(900, 140)
    .setSize(90, 60);
  Home.getCaptionLabel()
    .setFont(font)
    .setSize(15)
    .toUpperCase(false)
    .setText("Home");

  StopFunction = cp5.addButton("StopFunction")
    .setPosition(1230, 700) 
    .setSize(150, 60);
  StopFunction.getCaptionLabel()
    .setFont(font)
    .setSize(15)
    .toUpperCase(false)
    .setText("StopFunction");

  background(0);
}


void draw() {
  fill(98, 245, 31);
  // simulating motion blur and slow fade of the moving line
  noStroke();
  fill(0, 4);
  //rect(0, 0, Radarwidth, height - height * 0.065);
  
  fill(98, 245, 31); // green color
  //// calls the functions for drawing the radar
  drawRadar();
  drawLine();
  drawObject();
  drawText();

  // Receive data from server
  if (myClient.available() > 0) {
    if (dataNum >= 8) {
      dataNum = 0;
    }
    if (dataNum == 0) {
      dataIn0 = myClient.read();
    }
    if (dataNum == 1) {
      dataIn1 = myClient.read();
    }
    if (dataNum == 2) {
      dataIn2 = myClient.read();
    }
    if (dataNum == 3) {
      dataIn3 = myClient.read();
    }
    if (dataNum == 4) {
      dataIn4 = myClient.read();
    }
    if (dataNum == 5) {
      dataIn5 = myClient.read();
    }
    if (dataNum == 6) {
      dataIn6 = myClient.read();
    }
    if (dataNum == 7) {
      dataIn7 = myClient.read();
    }
    if (dataNum == 7 && dataIn7 == 46) {   //unicode ./46  0/48
      iAngle = (dataIn0 - 48) * 100 + (dataIn1 - 48) * 10 + dataIn2 - 48;
      iDistance = (dataIn4 - 48) * 100 + (dataIn5 - 48) * 10 + dataIn6 - 48;
      iOldAngle = iAngle;
      iAngle = iAngle - 5;
    }
    dataNum++;
    delay(10);
  }
  if (iAngle < (iOldAngle + 10)) {
    iAngle++;
    //delay(5);
  }

  textSize(70);
  fill(169, 169, 169);
  text("4WD Smart Car Robot GUI", 300, 100);

  buttonColor();
  mouseCheck();
}

void drawRadar() {
  pushMatrix();
  translate(Radarwidth / 2, height - height * 0.074); // moves the starting coordinats to new location
  noFill();
  strokeWeight(2);
  stroke(98, 245, 31);
  // draws the arc lines
  arc(0, 0, (Radarwidth - Radarwidth * 0.0625), (Radarwidth - Radarwidth * 0.0625), PI, TWO_PI);
  arc(0, 0, (Radarwidth - Radarwidth * 0.27), (Radarwidth - Radarwidth * 0.27), PI, TWO_PI);
  arc(0, 0, (Radarwidth - Radarwidth * 0.479), (Radarwidth - Radarwidth * 0.479), PI, TWO_PI);
  arc(0, 0, (Radarwidth - Radarwidth * 0.687), (Radarwidth - Radarwidth * 0.687), PI, TWO_PI);
  // draws the angle lines
  line(-Radarwidth / 2, 0, Radarwidth / 2, 0);
  line(0, 0, (-Radarwidth / 2) * cos(radians(30)), (-Radarwidth / 2) * sin(radians(30)));
  line(0, 0, (-Radarwidth / 2) * cos(radians(60)), (-Radarwidth / 2) * sin(radians(60)));
  line(0, 0, (-Radarwidth / 2) * cos(radians(90)), (-Radarwidth / 2) * sin(radians(90)));
  line(0, 0, (-Radarwidth / 2) * cos(radians(120)), (-Radarwidth / 2) * sin(radians(120)));
  line(0, 0, (-Radarwidth / 2) * cos(radians(150)), (-Radarwidth / 2) * sin(radians(150)));
  line((-Radarwidth / 2) * cos(radians(30)), 0, Radarwidth / 2, 0);
  popMatrix();
}

void drawLine() {
  pushMatrix();
  strokeWeight(9);
  stroke(30,250,60);
  translate(Radarwidth/2,height-height*0.074); 
  
  float edgeDistance = Radarwidth * 0.495; 
  line(0,0, edgeDistance*cos(radians(iAngle)), -edgeDistance*sin(radians(iAngle))); 
  popMatrix();
}

void drawObject() {
  pushMatrix();
  translate(Radarwidth/2,height-height*0.074);
  strokeWeight(9);
  stroke(255,10,10); 
  
  float maxEdgeDistance = Radarwidth * 0.495;
  pixsDistance = iDistance * (maxEdgeDistance / 200); 
  float objectDistance = min(pixsDistance, maxEdgeDistance);
  
  if(iDistance<=200){
    line(objectDistance*cos(radians(iAngle)), -objectDistance*sin(radians(iAngle)),
         maxEdgeDistance*cos(radians(iAngle)), -maxEdgeDistance*sin(radians(iAngle)));
  }
  popMatrix();
}

void drawText() { 
  pushMatrix();
  if(iDistance>=200) {
    noObject = "Out of Range";
  }
  else {
    noObject = "In Range";
  }
  fill(0,0,0);
  noStroke();
  rect(0, height-height*0.0648, Radarwidth, height);
  fill(98,245,31);
  textSize(25);
  
  text("50cm",Radarwidth-Radarwidth*0.3854,height-height*0.0833);
  text("100cm",Radarwidth-Radarwidth*0.281,height-height*0.0833);
  text("150cm",Radarwidth-Radarwidth*0.177,height-height*0.0833);
  text("200cm",Radarwidth-Radarwidth*0.0729,height-height*0.0833);
  textSize(40);
  text("www.adeept.com ", Radarwidth-Radarwidth*0.875, height-height*0.0277);
  if(iAngle > 180){
    iAngle= 180;
  }
  if(iAngle < 0){
    iAngle= 0;
  }
  text("Angle: " + iAngle +" °", Radarwidth-Radarwidth*0.48, height-height*0.0277);
  text("Distance:", Radarwidth-Radarwidth*0.26, height-height*0.0277);
  if(iDistance<=200 && iDistance>= 0) {
    text("   " + iDistance +"cm", Radarwidth*0.85, height-height*0.0277);
  }
  textSize(25);
  fill(98,245,60);
  translate((Radarwidth-Radarwidth*0.4994)+Radarwidth/2*cos(radians(30)),(height-height*0.0907)-Radarwidth/2*sin(radians(30)));
  rotate(-radians(-60));
  text("30°",0,0);
  resetMatrix();
  translate((Radarwidth-Radarwidth*0.503)+Radarwidth/2*cos(radians(60)),(height-height*0.0888)-Radarwidth/2*sin(radians(60)));
  rotate(-radians(-30));
  text("60°",0,0);
  resetMatrix();
  translate((Radarwidth-Radarwidth*0.507)+Radarwidth/2*cos(radians(90)),(height-height*0.0833)-Radarwidth/2*sin(radians(90)));
  rotate(radians(0));
  text("90°",0,0);
  resetMatrix();
  translate(Radarwidth-Radarwidth*0.513+Radarwidth/2*cos(radians(120)),(height-height*0.07129)-Radarwidth/2*sin(radians(120)));
  rotate(radians(-30));
  text("120°",0,0);
  resetMatrix();
  translate((Radarwidth-Radarwidth*0.5104)+Radarwidth/2*cos(radians(150)),(height-height*0.0574)-Radarwidth/2*sin(radians(150)));
  rotate(radians(-60));
  text("150°",0,0);
  popMatrix(); 
}

void keyPressed(){
  if (key >= 'A' && key <= 'z') {

  } 
}

void mouseCheck(){
  if(k1.isPressed()){
    myClient.write("forward");
    delay(90);
    myClient.write("DS");
  }    
  if(k2.isPressed()){
    myClient.write("turn_left");
    delay(90);
    myClient.write("DS");
  }
  if(k3.isPressed()){
    myClient.write("turn_right");
    delay(90);
    myClient.write("DS");
  }
  if(k4.isPressed()){
    myClient.write("backward");
    delay(90); 
    myClient.write("DS");
  }         
  if(k5.isPressed()){
    background(0);
    myClient.write("scan");
    delay(10);
  } 
  if(k6.isPressed()){
    myClient.write("trackLine");
    delay(50);
  }
  if(k7.isPressed()){
    myClient.write("automatic");
    delay(50);
  }
  if(policeButton.isPressed()){
    myClient.write("police");
    delay(50);
  }
  if(matrixScreenButton.isPressed()){
    myClient.write("matrix");
    delay(50);
  }
  //if(buzzerButton.isPressed()){
  //  myClient.write("buzzerMusic"); 
  //  delay(50);
  //}
  if(lightTrackingButton.isPressed()){
    myClient.write("lightTracking"); 
    delay(50);
  }
  if(keepDistanceButton.isPressed()){
    myClient.write("KD"); 
    delay(50);
  }
  if(left.isPressed()){
    myClient.write("lookleft"); 
    delay(50);
    myClient.write("RLstop"); 
  }
  if(right.isPressed()){
    myClient.write("lookright"); 
    delay(50);
    myClient.write("RLstop"); 
  }
  if(Home.isPressed()){
    myClient.write("home"); 
    delay(50);
  }
  if(StopFunction.isPressed()){ 
    myClient.write("StopFunction");
    delay(50);
  }
}

void buttonColor(){
  if(keyPressed && key=='w'){//w
    k1.setColorBackground(color(255,0,0));
    myClient.write("forward");
    delay(100);
    myClient.write("DS");
  }
  else{
    k1.setColorBackground(color(2,72,90));
  }
  if(keyPressed && key=='a'){//a
    k2.setColorBackground(color(255,0,0));
    myClient.write("turn_left");
    delay(100);
    myClient.write("DS");
  }
  else{
    k2.setColorBackground(color(2,72,90)); 
  }
  if(keyPressed && key=='d'){//d
    k3.setColorBackground(color(255,0,0));
    myClient.write("turn_right");
    delay(100);
    myClient.write("DS");
  }
  else{
    k3.setColorBackground(color(2,72,90));
  }
  if(keyPressed && key=='s'){//s
    k4.setColorBackground(color(255,0,0));
    myClient.write("backward");
    delay(100);
    myClient.write("DS");
  }
  else{
    k4.setColorBackground(color(2,72,90));
  }

  k6.setColorBackground(color(2,72,90));
  k7.setColorBackground(color(2,72,90));
  policeButton.setColorBackground(color(2,72,90));
  matrixScreenButton.setColorBackground(color(2,72,90));
  //buzzerButton.setColorBackground(color(2,72,90));
  lightTrackingButton.setColorBackground(color(2,72,90));
  keepDistanceButton.setColorBackground(color(2,72,90));
  if(left.isPressed()){
    left.setColorBackground(color(255,0,0));
  } else {
    left.setColorBackground(color(2,72,90));
  }
  
  if(right.isPressed()){
    right.setColorBackground(color(255,0,0));
  } else {
    right.setColorBackground(color(2,72,90));
  }
  
  if(Home.isPressed()){
    Home.setColorBackground(color(255,0,0));
  } else {
    Home.setColorBackground(color(2,72,90));
  }
  
  if(StopFunction.isPressed()){ 
    StopFunction.setColorBackground(color(255,0,0));
  } else {
    StopFunction.setColorBackground(color(2,72,90));
  }
}
