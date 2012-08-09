 
 char val; // Data received from the serial port
 int ledPin = 13; // Set the pin to digital I/O 4
 
 #include <Servo.h>      // include the servo library

Servo servoMotor;       // creates an instance of the servo object to control a servo
int servoPin = 2;       // Control pin for servo motor

 
 void setup() {
 //pinMode(ledPin, OUTPUT); // Set pin as OUTPUT
 servoMotor.attach(servoPin);  // attaches the servo on pin 2 to the servo object
 Serial.begin(9600); // Start serial communication at 9600 bps
 }
 
 void loop() {
 if (Serial.available()) { // If data is available to read,
 val = Serial.read(); // read it and store it in val
 }
 

 if (val == 'H') { // If H was received
   servoMotor.write(120); //rotate to 120 degree angle
   delay(500);
   servoMotor.write(0); //rotate to 0 degree angle
   delay(500);
   val = 0;
 } else {
   //servoMotor.write(0); //rotate to 0 degree angle
 }
 

 /*
 digitalWrite(ledPin, HIGH); // turn the LED on
 } else {
 digitalWrite(ledPin, LOW); // Otherwise turn it OFF
 }*/
 
 
 delay(10); // Wait 100 milliseconds for next reading
 }
