// Todd Locker
// 5/19/16
#include <WatchDogTimers.h>

#define navi 6  //needs longer boot time
#define sensor 7
#define router 2
#define led 3
//#define motorb 2
//#define powerb 3

/*
* sensorOut  A1
* naviOut    A0
* motorbOut  A5
* routerOut  A4
* ledOut     A2
* powerbOut  A3
*/
#define sensorOut A1
#define naviOut A0
#define motorbOut A5
#define routerOut A4
#define ledOut A2
#define powerbOut A3

int pinToReboot;
#define COMPLETE_BOOT_TIME 45000 // Time given to boot up entire robot
#define NAVI_BOOT_TIME 6500 // Time given to boot up the navi
#define OTHER_BOOT_TIME 4000 // Time given to boot up a pin other than router or navi

#define LOW_TIME 2000  // time to hold pins low while resetting
#define WAIT_TIME 5000 // time to wait until it is determined a processor is hanging
Timers sensorTimer(WAIT_TIME);
Timers naviTimer(WAIT_TIME);
//Timers motorbTimer(WAIT_TIME);
Timers routerTimer(WAIT_TIME);
Timers ledTimer(WAIT_TIME);
//Timers powerbTimer(WAIT_TIME);
Timers rebootTimer(COMPLETE_BOOT_TIME);
Timers pinBootTimer();
Timers lowTimer(LOW_TIME);

int state;
#define BOOTING 0
#define RUNNING 1
#define HANGING 2

bool oldValueSensor;
bool oldValueNavi;
//bool oldValueMotorb;
bool oldValueRouter;
bool oldValueLed;
//bool oldValuePowerb;
bool mustRebootPin;
bool lowWaiting;
bool bootWaiting;
bool allPinsLow;
bool allPinsHigh;


void setup() {
  pinMode(sensor, INPUT);
  pinMode(navi, INPUT);
  //pinMode(motorb, INPUT);
  pinMode(router, INPUT);
  pinMode(led, INPUT);
  //pinMode(powerb, INPUT);
  
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
  
  oldValueSensor = digitalRead(sensor);
  oldValueNavi = digitalRead(navi);
  //oldValueMotorb = digitalRead(motorb);
  oldValueRouter = digitalRead(router);
  oldValueLed = digitalRead(led);
  //oldValuePowerb = digitalRead(powerb);
  
  // Reset all the timers because they would have expired while waiting on boot timer
  sensorTimer.resetTimer();
  naviTimer.resetTimer();
  //motorbTimer.resetTimer();
  routerTimer.resetTimer();
  ledTimer.resetTimer();
  //powerbTimer.resetTimer();
  rebootTimer.resetTimer();
  bootWaiting = true;
  lowWaiting = false;
  mustRebootPin = false;
  allPinsLow = false;
  allPinsHigh = false;
}


void loop() {
  if(!mustRebootPin && !lowWaiting && !bootWaiting) {
    checkHanging(&sensorTimer, sensor, sensorOut, &oldValueSensor);
  }
  if(!mustRebootPin && !lowWaiting && !bootWaiting) {
    checkHanging(&naviTimer, navi, naviOut, &oldValueNavi);
  }
  if(!mustRebootPin && !lowWaiting && !bootWaiting) {
    checkHanging(&routerTimer, router, routerOut, &oldValueRouter);
  }
  if(!mustRebootPin && !lowWaiting && !bootWaiting) {
    checkHanging(&ledTimer, led, ledOut, &oldValueLed);
  }
  
  if(mustRebootPin) {
    pinDown(pinToReboot);
    mustRebootPin = false;
  }
  
  if(allPinsLow) {
    setAllLow();
    allPinsLow = false;
  }
  
  if(lowWaiting) {
    if(lowTimer.timerDone()) {
      lowTimer.resetTimer();
      lowWaiting = false;
      if(allPinsHigh) {
        setAllHigh();
      }
      else {
        pinUp(pinToReboot);
      }
    }
  }
  
  if(bootWaiting) {
    if(rebootTimer.timerDone()) {
      rebootTimer.resetTimer(); 
      bootWaiting = false;
      
      oldValueSensor = digitalRead(sensor);
      oldValueNavi = digitalRead(navi);
      //oldValueMotorb = digitalRead(motorb);
      oldValueRouter = digitalRead(router);
      oldValueLed = digitalRead(led);
      //oldValuePowerb = digitalRead(powerb);
      
      // Reset all the timers because they would have expired while rebooting the robot
      sensorTimer.resetTimer();
      naviTimer.resetTimer();
      //motorbTimer.resetTimer();
      routerTimer.resetTimer();
      ledTimer.resetTimer();
      //powerbTimer.resetTimer();
    }
  }

}



void checkHanging(Timers *timer, int inPin, int outPin, bool *oldValue)
{
  // If the current state of the inPin equals the previous state of the inPin
  if(digitalRead(inPin) == *oldValue) {
    if(timer->timerDone()) {
      mustRebootPin = true;
      pinToReboot = outPin;
    }
  }
  else {
    // The state of the inPin has changed so reset the timer
    timer->resetTimer();
    *oldValue = digitalRead(inPin);
  }
  return;
}



void pinDown(int pin)
{
  // If the led hangs, reboot entire robot
  // Otherwise reboot the pin for the given time depending on what pin
  if(pin == ledOut) {
    allPinsLow = true;
    return;
  }
  
  digitalWrite(pin, LOW);
  lowTimer.resetTimer();
  lowWaiting = true;
  
  return;
}



void pinUp(int pin)
{
  int rebootTime;
  
  // determine how long to wait for reboot before watchdog begins checking again
  if(pin == routerOut) {
    rebootTime = COMPLETE_BOOT_TIME;
  }
  else if(pin == naviOut) {
    rebootTime = NAVI_BOOT_TIME;
  }
  else {
    rebootTime = OTHER_BOOT_TIME;
  }
  
  digitalWrite(pin, HIGH);
  rebootTimer.setInterval(rebootTime);
  rebootTimer.resetTimer();
  bootWaiting = true;
  
  oldValueSensor = digitalRead(sensor);
  oldValueNavi = digitalRead(navi);
  //oldValueMotorb = digitalRead(motorb);
  oldValueRouter = digitalRead(router);
  oldValueLed = digitalRead(led);
  //oldValuePowerb = digitalRead(powerb);
  
  // Reset all the timers because they would have expired while rebooting current pin
  sensorTimer.resetTimer();
  naviTimer.resetTimer();
  //motorbTimer.resetTimer();
  routerTimer.resetTimer();
  ledTimer.resetTimer();
  //powerbTimer.resetTimer();
  
  return;
}



void setAllLow()
{
  digitalWrite(sensorOut, LOW);
  digitalWrite(naviOut, LOW);
  digitalWrite(motorbOut, LOW);
  digitalWrite(routerOut, LOW);
  digitalWrite(ledOut, LOW);
  digitalWrite(powerbOut, LOW);
  lowTimer.resetTimer();
  lowWaiting = true;
  allPinsHigh = true;
  
  return;
}



void setAllHigh()
{
  digitalWrite(sensorOut, HIGH);
  digitalWrite(naviOut, HIGH);
  digitalWrite(motorbOut, HIGH);
  digitalWrite(routerOut, HIGH);
  digitalWrite(ledOut, HIGH);
  digitalWrite(powerbOut, HIGH);
  rebootTimer.resetTimer();
  bootWaiting = true;
  
  return;
}
