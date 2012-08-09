SimpleWSServer server;

import processing.serial.*;

Serial myPort;  // Create object from Serial class

void setup(){
  // set up web socket server on some port
  // you will be able to access it at: http://your-comp-name.local:8888
  size(200, 200);
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 9600);
  
  try {
    server = new SimpleWSServer( this, 8888 );
  } catch ( UnknownHostException e ){
  }   
}

void draw(){
}


// called automagically from server
void onMessage( String message ){
  println( "got message "+message );
  
  // do stuff here when you get a message!
    myPort.write('H');
    delay(1000);
    myPort.write('L');     
}
