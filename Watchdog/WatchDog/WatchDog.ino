// Todd Locker
// 5/7/16
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

#define WAITTIME 1000
Timers sensorTimer(WAITTIME);
Timers naviTimer(WAITTIME);
Timers motorbTimer(WAITTIME);
Timers routerTimer(WAITTIME);
Timers ledTimer(WAITTIME);
Timers powerbTimer(WAITTIME);

int state;
#define BOOTING 0
#define RUNNING 1
#define HANGING 2

bool oldValueSensor;
bool oldValueNavi;
bool oldValueMotorb;
bool oldValueRouter;
bool oldValueLed;
bool oldValuePowerb;

int pinToReboot;
#define ROUTER_BOOT_TIME 40000
#define NAVI_BOOT_TIME 20000
#define NORMAL_BOOT_TIME 5000


void setup()
{
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
  
  state = BOOTING;
}

void loop()
{
  if(state == BOOTING)
  {
    rebootRobot();
  }
  else if(state == RUNNING)
  {
    checkHanging(&sensorTimer, sensor, sensorOut, &oldValueSensor);
    checkHanging(&naviTimer, navi, naviOut, &oldValueNavi);
    checkHanging(&motorbTimer, motorb, motorbOut, &oldValueMotorb);
    checkHanging(&routerTimer, router, routerOut, &oldValueRouter);
    checkHanging(&ledTimer, led, ledOut, &oldValueLed);
    checkHanging(&powerbTimer, powerb, powerbOut, &oldValuePowerb);
  }
  else if(state == HANGING)
  {
    rebootPin(pinToReboot);
  }
}


void checkHanging(Timers *timer, const int &inPin, const int &outPin, bool *oldValue)
{
  // If the current state of the inPin equals the previous state of the inPin
  if(digitalRead(inPin) == *oldValue) {
    if(timer->timerDone()) {
      state = HANGING;
      pinToReboot = outPin;
    }
  }
  else {
    // The state of the inPin has changed so reset the timer
    timer->resetTimer();
  }
  *oldValue = digitalRead(inPin);
  return;
}


void rebootPin(const int &pin)
{
  int rebootTime;
  // If the router hangs, reboot entire robot
  // Otherwise reboot the pin for the given time depending on what pin
  if(pin == routerOut) {
    state = BOOTING;
    return;
  }
  else if(pin == naviOut) {
    rebootTime = NAVI_BOOT_TIME;
  }
  else {
   rebootTime = NORMAL_BOOT_TIME; 
  }
  
  digitalWrite(pin, LOW);
  Timers lowTimer(100);
  while(!lowTimer.timerDone());
  
  digitalWrite(pin, HIGH);
  Timers rebootTimer(rebootTime);
  while(!rebootTimer.timerDone());
  
  // Reset all the timers because they would have expired while rebooting current pin
  sensorTimer.resetTimer();
  naviTimer.resetTimer();
  motorbTimer.resetTimer();
  routerTimer.resetTimer();
  ledTimer.resetTimer();
  powerbTimer.resetTimer();
  
  state = RUNNING;
  return;
}


// This function completely reboots the robot
// Sets the output pins to LOW and holds for given timer amount of time
// Then the output pins are brought back to HIGH
// The watchDog continues watching after given amount of time
void rebootRobot()
{
  Timers lowTimer(200);
  digitalWrite(sensorOut, LOW);
  digitalWrite(naviOut, LOW);
  digitalWrite(motorbOut, LOW);
  digitalWrite(routerOut, LOW);
  digitalWrite(ledOut, LOW);
  digitalWrite(powerbOut, LOW);
  while(!lowTimer.timerDone());
  
  Timers rebootTimer(40000);
  digitalWrite(sensorOut, HIGH);
  digitalWrite(naviOut, HIGH);
  digitalWrite(motorbOut, HIGH);
  digitalWrite(routerOut, HIGH);
  digitalWrite(ledOut, HIGH);
  digitalWrite(powerbOut, HIGH);
  while(!rebootTimer.timerDone());
  
  oldValueSensor = digitalRead(sensor);
  oldValueNavi = digitalRead(navi);
  oldValueMotorb = digitalRead(motorb);
  oldValueRouter = digitalRead(router);
  oldValueLed = digitalRead(led);
  oldValuePowerb = digitalRead(powerb);
  
  return;
}
