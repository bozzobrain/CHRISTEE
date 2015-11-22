#include <Timers.h>

#define sensor 1
#define navi 2
#define motorb 3
#define router 4
#define led 5
#define powerb 6

#define sensorOut  A0
#define naviOut A1
#define motorbOut A2
#define routerOut A3
#define ledOut A4
#define powerbOut A5

#define enable_sensor 1
#define enable_navi 1
#define enable_motorb 1
#define enable_router 1
#define enable_led 1
#define enable_powerb 1

Timers sensorTimer (1000);
Timers naviTimer (1000);
Timers motorbTimer (1000);
Timers routerTimer (1000);
Timers ledTimer (1000);
Timers powerbTimer (1000);

bool oldValueSensor=0;
bool oldValueNavi=0;
bool oldValueMotorb=0;
bool oldValueRouter=0;
bool oldValueLed=0;
bool oldValuePowerb=0;

void checkHanging(Timers *timer, bool *oldValue, int inPin, int outPin);
void setup() {        
  pinMode(sensor,INPUT);
  pinMode(navi,INPUT);
  pinMode(motorb,INPUT);
  pinMode(router,INPUT);
  pinMode(led,INPUT);
  pinMode(powerb,INPUT);
  if(enable_sensor){
    oldValueSensor = digitalRead(sensor);
  }
  if(enable_navi){ 
    oldValueNavi = digitalRead(navi);
  }
  if(enable_motorb){ 
    oldValueMotorb = digitalRead(motorb);
  }
  if(enable_router){ 
    oldValueRouter = digitalRead(router);
  }
  if(enable_led){ 
    oldValueLed = digitalRead(led);
  }
  if(enable_powerb){ 
    oldValuePowerb = digitalRead(powerb);
  }
}

void loop() {    
  if(enable_sensor){
    checkHanging(&sensorTimer, &oldValueSensor, sensor , sensorOut);
  }
  if(enable_navi){
    checkHanging(&naviTimer, &oldValueNavi, navi , naviOut);
  }
  if(enable_motorb){
    checkHanging(&motorbTimer, &oldValueMotorb, motorb , motorbOut);
  }
  if(enable_router){
    checkHanging(&routerTimer, &oldValueRouter, router , routerOut);
  }
  if(enable_led){
    checkHanging(&ledTimer, &oldValueLed, led , ledOut);
  }
  if(enable_powerb){
    checkHanging(&powerbTimer, &oldValuePowerb, powerb , powerbOut);
  }



}
// need to add a hanging timer to turn board back on
void checkHanging(Timers *timer, bool *oldValue, int inPin, int outPin){

  if(digitalRead(inPin) == *oldValue)
  {
    if(timer->timerDone())
    {
      digitalWrite(outPin,!digitalRead(outPin));
    }
  }
  else{
    timer->resetTimer();
    digitalWrite(outPin,HIGH);
  } 
  *oldValue =digitalRead(inPin);

  return;
}

