SimpleWSServer server;

void setup(){
  // set up web socket server on some port
  // you will be able to access it at: http://your-comp-name.local:8888
  try {
    server = new SimpleWSServer( this, 8888 );
  } catch ( UnknownHostException e ){
  }
}

// called automagically from server
void onMessage( String message ){
  println( "got message "+message );

  // do stuff here when you get a message!

}
