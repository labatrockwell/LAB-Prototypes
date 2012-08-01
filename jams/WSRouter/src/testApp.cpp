#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    // setup a server with default options on port 9092
    // - pass in true after port to set up with SSL
    bConnected = server.setup( 9093 );
    
    // this adds your app as a listener for the server
    server.addListener(this);
    
    ofBackground(100);
    ofSetFrameRate(60);
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
    map<ofxLibwebsockets::Connection *, Route *>::iterator it;
    
    for ( it=connections.begin() ; it != connections.end(); it++ ){
        (*it).second->draw();
    }

    for ( it=connections.begin() ; it != connections.end(); it++ ){
        (*it).second->draw(false);
    }
}

//--------------------------------------------------------------
void testApp::onConnect( ofxLibwebsockets::Event& args ){
    cout<<"on connected"<<endl;
}

//--------------------------------------------------------------
void testApp::onOpen( ofxLibwebsockets::Event& args ){
    cout<<"new connection open"<<endl;
    Route * route = new Route(args.conn);
    route->x = 10;
    route->y = connections.size() * 210;
    connections.insert( std::make_pair( &args.conn, route));
}

//--------------------------------------------------------------
void testApp::onClose( ofxLibwebsockets::Event& args ){
    cout<<"on close"<<endl;
    
    int y = 10;
    
    map<ofxLibwebsockets::Connection *, Route *>::iterator it;
    for ( it=connections.begin() ; it != connections.end(); it++ ){
        if ( (*it).first != &args.conn){
            (*it).second->removeRoute( *connections[&args.conn] );
            (*it).second->y = y;
            y += 210;
        }
    }
    connections.erase( &args.conn );
}

//--------------------------------------------------------------
void testApp::onIdle( ofxLibwebsockets::Event& args ){
    cout<<"on idle"<<endl;
}

//--------------------------------------------------------------
void testApp::onMessage( ofxLibwebsockets::Event& args ){
    connections[&args.conn]->onMessage(args.message);
}

//--------------------------------------------------------------
void testApp::onBroadcast( ofxLibwebsockets::Event& args ){
    cout<<"got broadcast "<<args.message<<endl;    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}