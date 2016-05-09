// Reed J.
// Todd L.
// 5/7/16
#include <Adafruit_NeoPixel.h>
#include <FastTransfer.h>
#include <Timers.h>

#define PINCr 2
#define PINRht 3
#define PINMtr 4
#define PINLft 5
#define PINCntrl 6

#define RIGHTMAIN_MAX  27
#define LEFTMAIN_MAX   27
#define MOTORBOX_MAX   8
#define CONTROLBOX_MAX 8
#define CORE_MAX       3

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel LeftMain = Adafruit_NeoPixel(LEFTMAIN_MAX, PINLft, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel RightMain = Adafruit_NeoPixel(RIGHTMAIN_MAX, PINRht, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ControlBox = Adafruit_NeoPixel(CONTROLBOX_MAX, PINCntrl, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel MotorBox = Adafruit_NeoPixel(MOTORBOX_MAX, PINMtr, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Core = Adafruit_NeoPixel(CORE_MAX, PINCr, NEO_GRB + NEO_KHZ800);


#define PIC_ADDRESS                1
#define LED_ADDRESS                2
#define POWER_ADDRESS	           3
#define NAVIGATION_ADDRESS         4
#define CONTROL_ADDRESS            5
#define MOTOR_ADDRESS              6


FastTransfer LED;
int received[4];
#define STATE 1
#define COLOR 2
#define UPDATE_FLAG 3

int state;
boolean initialized;

//assorted constants
#define STROBE_DELAY 100  //Changes both strobe() and party()
#define POLICE_DELAY 200
// 0 is start up
// 1 is autonomous-UA
// 2 is macro-UA
// 3 is normal-UA
// 4 is christmas
// 5 is party
// 6 is override
// 7 is error
#define GETTING_COMMS   0
#define AUTONOMOUS      1
#define MACRO           2
#define NORMAL          3
#define CHRISTMAS       4
#define PARTY           5
#define COLOR_OVERRIDE  6
#define STROBE          7
#define POLICE          8
#define RAINBOW         9
#define ZIPS             10


// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

// safetyTimer is how long to wait before switching to gettingComms()
Timers safetyTimer(2000);

void setup()
{
  // begin easy transfer communications.
  Serial.begin(115200);
  LED.begin(Details(received), LED_ADDRESS, false, &Serial);
  pinMode(13, OUTPUT);
  
  LeftMain.begin();
  LeftMain.show();
  RightMain.begin();
  RightMain.show();
  ControlBox.begin();
  ControlBox.show();
  MotorBox.begin();
  MotorBox.show();
  Core.begin();
  Core.show();
  
  state = 0;
  initialized = false;
}

uint8_t red;
uint8_t green;
uint8_t blue;

void loop()
{
  if(LED.receiveData())
  {
    digitalWrite(13, !digitalRead(13)); //equivalent to blink
    safetyTimer.resetTimer();
    initialized = false;
    state = received[STATE];
    blue = (uint8_t)((received[COLOR] & 0x001F) << 3); //0000 0000 0001 1111 -> 0000 0000 1111 1000
    green = (uint8_t)((received[COLOR] & 0x07E0) >> 3); //0000 0111 1110 0000 -> 0000 0000 1111 1100
    red  = (uint8_t)((received[COLOR] & 0xF800) >> 8); //1111 1000 0000 0000 -> 0000 0000 1111 1000
  }
  else if(safetyTimer.timerDone())
  {
    state = 0;
    blue = 0;
    green = 0;
    red = 0;
  }
  
  if((state == AUTONOMOUS || state == MACRO || state == NORMAL) && !initialized)
  {
    UALogo();
  }
  
  if(state == GETTING_COMMS)
  {
    gettingComms();
  }
  else if(state == AUTONOMOUS && !initialized)
  {
    colorWipe(Core.Color(0, 0, 255));
    initialized = true;
  }
  else if(state == MACRO && !initialized)
  {
    colorWipe(Core.Color(255, 0, 255));
    initialized = true;
  }
  else if(state == NORMAL && !initialized)
  {
    colorWipe(Core.Color(0, 255, 0));
    initialized = true;
  }
  else if(state == COLOR_OVERRIDE && !initialized)
  {
    colorWipe(Core.Color(red, green, blue));
    initialized = true;
  }
  else if(state == RAINBOW && !initialized)
  {
    rainbow();
    initialized = true;
  }
  else if(state == CHRISTMAS && !initialized)
  {
    christmas();
    initialized = true;
  }
  else if(state == STROBE && !initialized)
  {
    strobe();
    initialized = true;
  }
  else if(state == PARTY && !initialized) 
  {
    //strobe in multi color
    party();
    initialized = true;
  }
  else if(state == POLICE && !initialized)
  {
    //left side lights up red, right side lights up blue, then they switch
    police();
    initialized = true;
  }
  else if(state == ZIPS && !initialized)
  {
    //battery boxes go blue, all else goes yellow
    zips();
    initialized = true;
  }
  
  
  delay(2);
}


void blink()
{
   static boolean state = false;
   static long previousTime = 0;
   if((millis() - 500) > previousTime) {
      previousTime = millis();
      digitalWrite(13, state);
      state = !state;
   }
}


void allOff()
{
  for (int i = 0; i < 27; i++)
  {
    if (i < RIGHTMAIN_MAX-6)     RightMain.setPixelColor(i, 0);
    if (i < LEFTMAIN_MAX-6)     LeftMain.setPixelColor(i, 0);
    if (i < MOTORBOX_MAX)     MotorBox.setPixelColor(i, 0);
    if (i < CONTROLBOX_MAX)   ControlBox.setPixelColor(i, 0);
    if (i < CORE_MAX)         Core.setPixelColor(i, 0);
  }

  LeftMain.show();
  RightMain.show();
  ControlBox.show();
  MotorBox.show();
  Core.show();
}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c)
{
  for (uint16_t i = 0; i < 27; i++)
  {
    if (i < RIGHTMAIN_MAX-6)  RightMain.setPixelColor(i, c);
    if (i < LEFTMAIN_MAX-6)   LeftMain.setPixelColor(i, c);
    if (i < MOTORBOX_MAX)     MotorBox.setPixelColor(i, c);
    if (i < CONTROLBOX_MAX)   ControlBox.setPixelColor(i, c);
    if (i < CORE_MAX)         Core.setPixelColor(i, c);
  }
  RightMain.show();
  LeftMain.show();
  MotorBox.show();
  ControlBox.show();
  Core.show();
}


void gettingComms()
{
  static Timers commTimer(500);
  static boolean marker = false;
  if (commTimer.timerDone()) {
    if(marker) {
      colorWipe(Core.Color(255, 0, 0));
      marker = false;
    }else {
      colorWipe(Core.Color(0, 0, 255));
      marker = true;
    }
  }
}


void UALogo() 
{
  static long previousTime = 0;
  if(millis()-20 >= previousTime) 
  {
    blueParadeFade();
    yellowParadeFade();
  }
}


void blueParadeFade() 
{
  static int dirrection = 1;
  static int j = 0;
  for (int i = 21; i < 27; i++) 
  {
    if((i % 2) == 0)
    {
      RightMain.setPixelColor(i, RightMain.Color(0, 0, j));
    }
    else
    {
      RightMain.setPixelColor(i, RightMain.Color(0, 0, 255-j));
    }
    RightMain.show();
    j = j + dirrection;
    if(j > 254)
    {
      dirrection = -1;
    }
    if(j < 1)
    {
      dirrection = 1;
    }
  }
}


void yellowParadeFade() 
{
  static int dirrection = 1;
  static int j = 0;
  for (int i = 21; i < 27; i++) 
  {
    if((i % 2) == 0) {
      LeftMain.setPixelColor(i, LeftMain.Color(j, j, 0));
    }else {
      LeftMain.setPixelColor(i, LeftMain.Color(255-j, 255-j, 0));
    }
    LeftMain.show();
    j = j + dirrection;
    if(j > 254)
    {
      dirrection = -1;
    }
    if(j < 1)
    {
      dirrection = 1;
    }
  }
}


void rainbow() 
{
  static uint16_t i = 0;
  static uint16_t j = 0;
  
  RightMain.setPixelColor(i, Wheel((i + j) & 255));
  LeftMain.setPixelColor(i, Wheel((i + j) & 255));
  MotorBox.setPixelColor(i, Wheel((i + j) & 255));
  ControlBox.setPixelColor(i, Wheel((i + j) & 255));
  Core.setPixelColor(i, Wheel((i + j) & 255));
  
  RightMain.show();
  LeftMain.show();
  MotorBox.show();
  ControlBox.show();
  Core.show();
  delay(20);
  
  if(i>25) {
    i = 0;
  }else {
   i++; 
  }
  if(j>254) {
    j = 0;
  }else {
    j++;
  }
}


// This is used in rainbow()
uint32_t Wheel(byte WheelPos)
{
  if(WheelPos < 85) {
    return Core.Color(WheelPos * 3, 255 - (WheelPos * 3), 0);
  }else if (WheelPos < 170) {
    WheelPos -= 85;
    return Core.Color(255 - (WheelPos * 3), 0, WheelPos * 3);
  }else {
    WheelPos -= 170;
    return Core.Color(0, WheelPos * 3, 255 - (WheelPos * 3));
  }
}


void christmas()
{
  static long previousTime = 0;
  if(millis() - 20 >= previousTime) 
  {
    static int j = 0;
    static int dirrection = 1;
    for(int i = 0; i < 27; i++) 
    {
      if((i % 5) == 0) 
      {
        RightMain.setPixelColor(i, Core.Color(j, j, j));
        LeftMain.setPixelColor(i, Core.Color(j, j, j));
        MotorBox.setPixelColor(i, Core.Color(j, j, j));
        ControlBox.setPixelColor(i, Core.Color(j, j, j));
        Core.setPixelColor(i, Core.Color(j, j, j));
      }
      if((i % 2) == 0) 
      {
        RightMain.setPixelColor(i, Core.Color(255, 0, 0));
        LeftMain.setPixelColor(i, Core.Color(255, 0, 0));
        MotorBox.setPixelColor(i, Core.Color(255, 0, 0));
        ControlBox.setPixelColor(i, Core.Color(255, 0, 0));
        Core.setPixelColor(i, Core.Color(255, 0, 0));
      }
      else 
      {
        RightMain.setPixelColor(i, Core.Color(0, 255, 0));
        LeftMain.setPixelColor(i, Core.Color(0, 255, 0));
        MotorBox.setPixelColor(i, Core.Color(0, 255, 0));
        ControlBox.setPixelColor(i, Core.Color(0, 255, 0));
        Core.setPixelColor(i, Core.Color(0, 255, 0));
      }
      if(j > 254) 
      {
        dirrection = -1;
      }
      if(j < 1) 
      {
        dirrection = 1;
      }
    }
  }
}


void strobe()
{
  colorWipe(Core.Color(0, 0, 0));
  delay(STROBE_DELAY);
  colorWipe(Core.Color(255, 255, 255));
  delay(STROBE_DELAY);
}


void party()
{
  static int ColorState = 0; 
  colorWipe(Core.Color(0, 0, 0));
  switch(ColorState)
  {
    case 0:
      delay(STROBE_DELAY);
      colorWipe(Core.Color(255, 0, 0));
      delay(STROBE_DELAY);
      ColorState++;
      break;
    case 1:
      delay(STROBE_DELAY);
      colorWipe(Core.Color(0, 255, 0));
      delay(STROBE_DELAY);
      ColorState++;
      break;
    case 2:
      delay(STROBE_DELAY);
      colorWipe(Core.Color(0, 0, 255));
      delay(STROBE_DELAY);
      ColorState = 0;
      break;
    }
}


void police()
{
  //left side lights up red, right side lights up blue, then they switch
  for (int i = 0; i < 27; i++)
  {
    if (i < RIGHTMAIN_MAX)    RightMain.setPixelColor(i, Core.Color(255, 0, 0));
    if (i < LEFTMAIN_MAX)     LeftMain.setPixelColor(i, Core.Color(0, 0, 255));
    if (i < MOTORBOX_MAX)     MotorBox.setPixelColor(i, Core.Color(255, 0, 0));
    if (i < CONTROLBOX_MAX)   ControlBox.setPixelColor(i, Core.Color(0, 0, 255));
    if (i < CORE_MAX)         Core.setPixelColor(i, Core.Color(255, 0, 0));
  }
  LeftMain.show();
  RightMain.show();
  ControlBox.show();
  MotorBox.show();
  Core.show();
  delay(POLICE_DELAY); 
  for (int i = 0; i < 27; i++)
  {
    if (i < RIGHTMAIN_MAX)    RightMain.setPixelColor(i, Core.Color(0, 0, 255));
    if (i < LEFTMAIN_MAX)     LeftMain.setPixelColor(i, Core.Color(255, 0, 0));
    if (i < MOTORBOX_MAX)     MotorBox.setPixelColor(i, Core.Color(0, 0, 255));
    if (i < CONTROLBOX_MAX)   ControlBox.setPixelColor(i, Core.Color(255, 0, 0));
    if (i < CORE_MAX)         Core.setPixelColor(i, Core.Color(0, 0, 255));
  }
  LeftMain.show();
  RightMain.show();
  ControlBox.show();
  MotorBox.show();
  Core.show();
  delay(POLICE_DELAY);
}


void zips()
{
  //battery boxes go blue, all else goes yellow
  for (int i = 0; i < 27; i++)
  {
    if (i < RIGHTMAIN_MAX)     RightMain.setPixelColor(i, Core.Color(0, 0, 255));
    if (i < LEFTMAIN_MAX)     LeftMain.setPixelColor(i, Core.Color(0, 0, 255));
    if (i < MOTORBOX_MAX)     MotorBox.setPixelColor(i, Core.Color(255, 255, 0));
    if (i < CONTROLBOX_MAX)   ControlBox.setPixelColor(i, Core.Color(255, 255, 0));
    if (i < CORE_MAX)         Core.setPixelColor(i, Core.Color(255, 255, 0));
  }
  LeftMain.show();
  RightMain.show();
  ControlBox.show();
  MotorBox.show();
  Core.show();
  delay(POLICE_DELAY); //delay just so not to be constantly writing to LEDs
}
