//
//  Route.h
//  EchoServer
//
//  Created by Brett Renfer on 7/31/12.
//
//

#pragma once

class Patch {
public:
    
    ofPoint from, to;
    
    int width;
    bool bConnected, bPressed;
    
    Patch(){
        bConnected  = false;
        bPressed    = false;
    }
    
    void draw(){
        
        ofPushStyle(); {
            ofSetRectMode(OF_RECTMODE_CENTER);
            ofNoFill();
            ofSetColor(255);
            ofEllipse(from.x, from.y, width, width);
            if ( bConnected){
                ofLine( from, to );
                
                ofFill();
                ofSetColor(255);
                ofEllipse(from.x, from.y, width-2, width-2);
                //ofEllipse(to.x, to.y, width-2, width-2);
                //ofNoFill();
                //ofEllipse(to.x, to.y, width, width);
            }
        } ofPopStyle();
    }
    
    bool mousePressed( int x, int y ){
        if ( x > from.x - (width/2) && x < from.x + width/2 &&
             y > from.y - (width/2) && y < from.y + width/2 ){
            bPressed = true;
            bConnected = true;
        } else {
            bPressed = false;
        }
        return bPressed;
    };
    
    void mouseReleased(){
        bPressed = false;
        bConnected = false;
    }
    
    void mouseDragged( int x, int y){
        to.set(x,y);
    }
    
};

class Route : public ofRectangle {
public:
    
    Route(){
        connection = NULL;
        lastMessage = "";
        setupPatches();
        width = 400;
        height = 200;
    }
    
    Route( ofxLibwebsockets::Connection & connection ){
        setConnection(connection);
        lastMessage = "";
        setupPatches();
        width = 400;
        height = 200;
    }
    
    void draw( bool left=true){
        ofPushMatrix();{
            if ( left ) ofTranslate(x,y);
            else ofTranslate( x + ofGetWidth() - 10 - width, y);
            ofPushStyle(); {
                ofSetColor(bg);
                ofRect( 0,0,width,height );
                ofSetColor(255);
                ofDrawBitmapString( lastMessage, 10, 10 );
                if ( left ){
                    for (int i=0; i<patches_right.size(); i++){
                        patches_right[i].draw();
                    }
                } else {
                    for (int i=0; i<patches_left.size(); i++){
                        patches_left[i].draw();
                    }
                }
                
            } ofPopStyle();
        } ofPopMatrix();
    }
    
    void onMessage( string message ){
        lastMessage = message;
        for ( int i=0; i<routes.size(); i++){
            routes[i]->send( message );
        }
    }
    
    void send( string message ){
        connection->send(message);
    }
    
    // these should eventually be tied to string val / bool val / int val
    void addRoute( Route & r ){
        routes.push_back( &r );
    }
    
    void removeRoute( Route & r ){
        for ( int i=0, len = routes.size(); i < len; i++){
            if ( routes[i] == &r ){
                routes.erase( routes.begin() + i );
            }
        }
    }
    
    ofxLibwebsockets::Connection * getConnection(){
        return connection;
    }
    void setConnection( ofxLibwebsockets::Connection & conn ){
        connection = &conn;
    }
    
    bool mousePressed( int mx, int my ){
        if ( my > y && my < y + height ){
                
            // left?
            if ( mx > x && mx < x + width ){
                for (int i=0; i<patches_left.size(); i++){
                    patches_left[i].mousePressed(mx - x,my - y);
                }
            // right?
            } else if ( mx > ( x + ofGetWidth() - 10 - width) && mx < ( x + ofGetWidth() - 10) ){
                for (int i=0; i<patches_right.size(); i++){
                    patches_right[i].mousePressed(mx - x,my - y);
                }
            }
            
        }
    }
    
    void mouseDragged(int mx, int my){
        for (int i=0; i<patches_left.size(); i++){
            patches_left[i].mouseDragged(mx - x,my - y);
        }
        for (int i=0; i<patches_right.size(); i++){
            patches_right[i].mouseDragged(mx - x,my - y);
        }
    }
    
    void mouseReleased(){
        for (int i=0; i<patches_left.size(); i++){
            patches_left[i].mouseReleased();
        }
        for (int i=0; i<patches_right.size(); i++){
            patches_right[i].mouseReleased();
        }
    }
    
private:
    
    ofColor bg;
    
    void setupPatches(){
        bg.set(ofRandom(150),ofRandom(150),ofRandom(150));
        for (int i=0; i<4; i++){
            patches_left.push_back(Patch());
            patches_left.back().from.set(10, 10 + i*25);
            
            patches_right.push_back(Patch());
            patches_right.back().from.set(380, 10 + i*25);
        }
    }
    
    vector<Patch>   patches_left, patches_right;
    vector<Route*> routes;
    ofxLibwebsockets::Server    * server;
    ofxLibwebsockets::Connection * connection;
    
    string lastMessage;
    
};