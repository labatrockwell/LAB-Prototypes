#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    port.listDevices();
    port.setup();
    // setup a server with default options on port 9092
    // - pass in true after port to set up with SSL
    bConnected = server.setup( 9094 );
    
    // Uncomment this to set up a server with a protocol
    // Right now, clients created via libwebsockets that are connecting to servers
    // made via libwebsockets seem to want a protocol. Hopefully this gets fixed, 
    // but until now you have to do something like this:

    /*
    ofxLibwebsockets::ServerOptions options = ofxLibwebsockets::defaultServerOptions();
    options.port = 9092;
    options.protocol = "of-protocol";
    bConnected = server.setup( options );
    */
    
    // this adds your app as a listener for the server
    server.addListener(this);
    
    ofSetFrameRate(60);
    
    colors[0] = "<CA>";
    colors[1] = "<CB>";
    colors[2] = "<CC>";
    colors[3] = "<CD>";
    colors[4] = "<CE>";
    colors[5] = "<CF>";
    colors[6] = "<CG>";
    colors[7] = "<CH>";
    colors[8] = "<CI>";
    colors[9] = "<CJ>";
    colors[10] = "<CK>";
    colors[11] = "<CL>";
    colors[12] = "<CM>";
    colors[13] = "<CN>";
    colors[14] = "<CO>";
    colors[15] = "<CP>";

    string msg = "<ID01><PA><FB>start\r\n";
    port.writeBytes((unsigned char *)msg.c_str(), msg.length());  
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
}

//--------------------------------------------------------------
void testApp::onConnect( ofxLibwebsockets::Event& args ){
    cout<<"on connected"<<endl;
    args.conn.send("{\"string\":\"" + stringValue + "\",\"bool\":\"" + ofToString(boolValue) + "\",\"int\":\"" + ofToString(intValue) + "\"}");
}

//--------------------------------------------------------------
void testApp::onOpen( ofxLibwebsockets::Event& args ){
    cout<<"new connection open"<<endl;
    messages.push_back("New connection from " + args.conn.getClientIP() + ", " + args.conn.getClientName() );
}

//--------------------------------------------------------------
void testApp::onClose( ofxLibwebsockets::Event& args ){
    cout<<"on close"<<endl;
    messages.push_back("Connection closed");
}

//--------------------------------------------------------------
void testApp::onIdle( ofxLibwebsockets::Event& args ){
    cout<<"on idle"<<endl;
}

//--------------------------------------------------------------
void testApp::onMessage( ofxLibwebsockets::Event& args ){
    cout<<"got message "<<args.message<<endl;
    
    // trace out string messages or JSON messages!
    if ( args.json != NULL){
        string type = args.json["type"].asString();
        if ( type == "string" ){
            stringValue = args.json["data"].asString();
        } else if ( type == "boolean" ){
            if ( args.json["data"].isInt() ){
                boolValue = args.json["data"].asInt();
            } else if ( args.json["data"].isString() ){
                string val = args.json["data"].asString();
                boolValue = ofToBool(val);                
            }
        } else if ( type == "number" ){
            if ( args.json["data"].isInt() ){
                intValue = args.json["data"].asInt();
            } else if ( args.json["data"].isString() ){
                string val = args.json["data"].asString();
                intValue = ofToInt(val);                
            }
        }
        
        // construct message
        int where = floor(intValue / 64);
        
        string msg;
        if (boolValue){
            msg = "<ID01><PA><FN>" + colors[where] + stringValue+"\r\n";            
        } else {
            msg = "<ID01><PA>" + colors[where] + stringValue+"\r\n";            
        }
        port.writeBytes((unsigned char *)msg.c_str(), msg.length());
    } else {
        //args.message
        string msg = "<ID01><PA>"+args.message+"\r\n";
        port.writeBytes((unsigned char *)msg.c_str(), msg.length());      
    }
}

//--------------------------------------------------------------
void testApp::onBroadcast( ofxLibwebsockets::Event& args ){
    cout<<"got broadcast "<<args.message<<endl;    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    // do some typing!
    if ( key != OF_KEY_RETURN ){
        if ( key == OF_KEY_BACKSPACE ){
            if ( toSend.length() > 0 ){
                toSend.erase(toSend.end()-1);
            }
        } else {
            toSend += key;
        }
    } else {
        // send to all clients
        server.send( toSend );
        messages.push_back("Sent: '" + toSend + "' to "+ ofToString(server.getConnections().size())+" websockets" );
        toSend = "";
    }
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
    string url = "http";
    if ( server.usingSSL() ){
        url += "s";
    }
    url += "://localhost:" + ofToString( server.getPort() );
    ofLaunchBrowser(url);
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