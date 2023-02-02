#include <Servo.h>

#define servopin1 5
#define servopin2 6
#define ledpin 2
#define echoPin1 11
#define trigPin1 12
#define echoPin2 9
#define trigPin2 10

int number_of_people;
double predis1,postdis1;
double predis2,postdis2;
Servo servo1,servo2;
bool entering,leaving;

int timer,entertimer,leavetimer;
// 1 is ouside 2 is inside
//outside the door
double distance1(){
    digitalWrite(trigPin1,LOW);
    delay(2);
    digitalWrite(trigPin1,HIGH);
    delay(10);
    digitalWrite(trigPin1,LOW);
    long duration = pulseIn(echoPin1,HIGH);
    return duration*340/2/10000.; 
}

//inside the door
double distance2(){
    digitalWrite(trigPin2,LOW);
    delay(2);
    digitalWrite(trigPin2,HIGH);
    delay(10);
    digitalWrite(trigPin2,LOW);
    long duration = pulseIn(echoPin2,HIGH);
    return duration*340/2/10000.;
}

bool is_presence1(){
    if(15 < predis1  && postdis1 < 15){
        return true;
    }
    return false;
}

bool is_presence2(){
    if(5 < predis2  && postdis2 < 5){
        return true;
    }
    return false;
}


void printStatus1(){
    Serial.print("predis1 : ");
    Serial.println(predis1);
    Serial.print("postdis1 : ");
    Serial.println(postdis1);
}

void printStatus2(){
    Serial.print("predis2 : ");
    Serial.println(predis2);
    Serial.print("postdis2 : ");
    Serial.println(postdis2);
}

void openDoor(){
    servo1.write(0);
    delay(3000);
    servo1.write(80);
}

void setup(){
    Serial.begin((9600));
    servo1.attach(servopin1);
    servo2.attach(servopin2);
    pinMode(ledpin,OUTPUT);
    pinMode(trigPin1,OUTPUT);
    pinMode(echoPin1,INPUT);
    pinMode(trigPin2,OUTPUT);
    pinMode(echoPin2,INPUT);
    number_of_people = 0;
    entering = false;
    leaving = false;
    digitalWrite(ledpin,LOW);
    predis1 = 100;
    predis2 = 100;
    servo1.write(80);
    servo2.write(80);
}


void loop(){
    if( number_of_people < 0){
        number_of_people = 0;
    }
    timer = millis();
    postdis1 = distance1();
    printStatus1();
    postdis2 = distance2();
    printStatus2();
    //Serial.println(number_of_people);
    

    if(is_presence1()){
        entering = true;
    }
    if(is_presence2()){
        leaving = true;
    }

    if(entering){
        number_of_people += 1;
        openDoor();
        entering = false;
    }
    if(leaving){
        number_of_people -= 1;
        openDoor();
        leaving = false;
    }

    if(number_of_people <= 0){
        digitalWrite(ledpin,LOW);
    }
    else{
        digitalWrite(ledpin,HIGH);
    }
    predis1 = postdis1;
    predis2 = postdis2;
    delay(100);
}