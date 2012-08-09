SimpleWSServer server;

import processing.serial.*;

Serial myPort;  // Create object from Serial class
String lastmessage = "";
PFont f;

int lastPoked = 0;

boolean poking;

void setup(){
  // set up web socket server on some port
  // you will be able to access it at: http://your-comp-name.local:8888

  size(200, 200);
  f = createFont("Arial",16,true);
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 9600);
  
  try {
    server = new SimpleWSServer( this, 8888 );
  } catch ( UnknownHostException e ){
  }   
}

void draw(){
  background(0);
  textFont(f,36);
  text(lastmessage, 10, 36 + 10);
}


// called automagically from server
void onMessage( String message ){
  println( "got message "+message );
  lastmessage = message;
  
  // only allow poking once a second
  if ( millis() - lastPoked > 1000 ){
    println("poke?");
      lastPoked = millis();    
    // do stuff here when you get a message!
      myPort.write('H');
  }
}
