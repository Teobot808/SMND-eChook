#include <Arduino.h>
#include <Speed.hpp>
#include <Bluetooth.hpp>

int Speed::rotations = 0;
int Speed::rotationsPerMinute = 0;



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