#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <DHT.h>
#include <SoftwareSerial.h>

#define dhtPin 9
#define servo1Pin 10
#define servo2Pin 11
#define photoPin A0
#define LED1 13//ac
#define LED2 12

Servo servo1; 
Servo servo2; 
DHT myDHT (dhtPin, DHT11);
LiquidCrystal_I2C MyLCD(0x27,16,2);
SoftwareSerial BT(5, 6); 

bool acstatus_on;
bool acstatus_off;
int status;

void setup() {
    pinMode(photoPin, INPUT); 
    servo1.attach(servo1Pin);
    servo2.attach(servo2Pin); 
    myDHT.begin();
    MyLCD.init();  
    MyLCD.backlight();
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    Serial.begin(9600);
    BT.begin(9600);
    acstatus_on = false;
    acstatus_off = false;
    servo1.write(0);
    servo2.write(90);
}

void openWindow(){
    servo1.write(90);
    servo2.write(0);
}

void closeWindow(){
    servo1.write(0);
    servo2.write(90);
}
void loop() {
    if(myDHT.readHumidity() > 80 || analogRead(photoPin) >100){
       openWindow();
       delay(100);
    }
    else{
        closeWindow();         
        delay(100);
    }
    //air condition
    if(myDHT.readTemperature() > 25 && acstatus_off == false){
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, LOW);
    }
    else if(myDHT.readTemperature() < 25 && acstatus_on == false){
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, HIGH);
    }
    //bluetooth
    if(BT.available()){
        status = BT.read();
        Serial.println(status);
        switch(status){
            //open ac
            case 97:
                digitalWrite(LED1, HIGH);
                digitalWrite(LED2, LOW);
                acstatus_on = true;
                break;
            //close ac
            case 98:
                digitalWrite(LED1, LOW);
                digitalWrite(LED2, HIGH);
                acstatus_off = true;
                break;
            //open window
        }
            
    }

    MyLCD.clear();
    MyLCD.setCursor(0,0); 
    MyLCD.print(myDHT.readTemperature()); 
    MyLCD.print("C");
    MyLCD.setCursor(0,1); 
    MyLCD.print(myDHT.readHumidity());
    MyLCD.print("%");
    
}