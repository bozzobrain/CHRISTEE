// Todd Locker
// This code is the base for the WatchDog.
// It simply will turn everything on and do nothing else.
// 5/7/16
#include <WatchDogTimers.h>

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

#define WAITTIME 1000
Timers sensorTimer(WAITTIME);
Timers naviTimer(WAITTIME);
Timers motorbTimer(WAITTIME);
Timers routerTimer(WAITTIME);
Timers ledTimer(WAITTIME);
Timers powerbTimer(WAITTIME);


void setup() {
  pinMode(sensor, INPUT);
  pinMode(navi, INPUT);
  pinMode(motorb, INPUT);
  pinMode(router, INPUT);
  pinMode(led, INPUT);
  pinMode(powerb, INPUT);
  
  pinMode(sensorOut, OUTPUT);
  pinMode(naviOut, OUTPUT);
  pinMode(motorbOut, OUTPUT);
  pinMode(routerOut, OUTPUT);
  pinMode(ledOut, OUTPUT);
  pinMode(powerbOut, OUTPUT);
  
  digitalWrite(sensorOut, HIGH);
  digitalWrite(naviOut, HIGH);
  digitalWrite(motorbOut, HIGH);
  digitalWrite(routerOut, HIGH);
  digitalWrite(ledOut, HIGH);
  digitalWrite(powerbOut, HIGH);
}

void loop() {
  
}

