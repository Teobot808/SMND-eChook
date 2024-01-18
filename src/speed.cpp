#include <Arduino.h>
#include <Speed.hpp>
#include <Bluetooth.hpp>
//#include <constants.hpp>
//#include <Bounce2.h>

int Speed::rotations = 0;
int Speed::rotationsPerMinute = 0;
float WheelCircumference      = 160;  // in cm


void Speed::init(){
    pinMode(A1, INPUT);
    pinMode(A6, INPUT);
  //these pins will be changed for acutal data reading pins when I get to echook (I dont have it next to me right now)
};

void Speed::MeasureRPM(){
if(A1 == HIGH){
    Speed::rotations++;
}

};

float Speed::readRPM(){
   Speed::rotationsPerMinute =  Speed::rotations;
    Speed::rotations = 0;
    Speed::rotationsPerMinute = (Speed::rotationsPerMinute / 5000); //divide number of rotations by number of milisecond. Now we have rotations per second
    Speed::rotationsPerMinute = (Speed::rotationsPerMinute * 60); // multiply number of rotations per second by 60 to get rotations per minute
    return(Speed::rotationsPerMinute);
};

float Speed::KmpH(float RPM){
// RPM * wheelcircumference = meters/minute wheelcircumference is in cm so we divide it by 100 to get meters
// to km/hour we need to convert from minutes to hours //// meters/minute * 60  = meters/hour
// divide by 1000 to get KmpH //// meters/hour  /  1000   = Km/H
RPM = RPM * (WheelCircumference / 100);
RPM = RPM * 60;
RPM = RPM / 1000;
return(RPM);
};
