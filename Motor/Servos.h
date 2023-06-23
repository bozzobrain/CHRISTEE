//servos.h
#define UP 254
#define DOWN 253
#define STOP 255
#define UPOVERRIDE 252
#define DOWNOVERRIDE 251
#define lowerLIMIT 1 //6
#define upperLIMIT 91  //90
#define VIBRATING_MOTOR_ON_HIGH 80
#define VIBRATING_MOTOR_ON_LOW 15
//// attage names to servos
static Servo leftMotor;
static Servo rightMotor;
static Servo actuator;

static int currentLeftMotorSpeed = 0;
static int currentRightMotorSpeed = 0;
static int currentActuatorSpeed = 0;

// atteches pins to the servo commands
void initializeServos()
{
  leftMotor.attach(5);
  rightMotor.attach(6);
  actuator.attach(3);
}

inline static void updateRamp()
{
  static unsigned long currentMotorLeft = 0;
  static unsigned long previousMotorLeft = 0;
  static unsigned long currentMotorRight = 0;
  static unsigned long previousMotorRight = 0;
  static unsigned long currentActuator = 0;
  static unsigned long previousActuator = 0;



  unsigned long curTime = millis();
  currentMotorLeft = curTime;
  currentMotorRight = curTime;
  currentActuator = curTime;




  if((currentMotorLeft - previousMotorLeft) > time[(currentLeftMotorSpeed + 90)])
  {
    previousMotorLeft = currentMotorLeft;
    if(currentLeftMotorSpeed < leftMotorSpeed)
    {
      currentLeftMotorSpeed += 1;
    }
    else if(currentLeftMotorSpeed > leftMotorSpeed)
    {
      currentLeftMotorSpeed -= 1;
    }
    else
    {
      currentLeftMotorSpeed = leftMotorSpeed;
    }

  }

  if((currentMotorRight - previousMotorRight) > time[(currentRightMotorSpeed + 90)])
  {
    previousMotorRight = currentMotorRight;

    if(currentRightMotorSpeed < rightMotorSpeed)
    {
      currentRightMotorSpeed += 1;
    }
    else if(currentRightMotorSpeed > rightMotorSpeed)
    {
      currentRightMotorSpeed -= 1;
    }
    else
    {
      currentRightMotorSpeed = rightMotorSpeed;
    }
  }

  if(currentActuator - previousActuator > 5)
  {
    previousActuator = currentActuator;

    if(currentActuatorSpeed < actuatorSpeed)
    {
      currentActuatorSpeed += 1;
    }
    else if(currentActuatorSpeed > actuatorSpeed)
    {
      currentActuatorSpeed -= 1;
    }
    else
    {
      currentActuatorSpeed = actuatorSpeed;
    }
  }

  if(actuatorSpeed == 255 || leftMotorSpeed == 255 || rightMotorSpeed == 255)
  {
    currentLeftMotorSpeed = 0;
    currentRightMotorSpeed = 0;
    currentActuatorSpeed = 0;
  }
}


inline static void updateSpeeds()
{
  leftMotor.write((currentLeftMotorSpeed + 90));
  rightMotor.write((currentRightMotorSpeed + 90));
  actuator.write((currentActuatorSpeed + 90));
}



inline void updateServos()
{
  updateRamp();
  updateSpeeds();
  setAct(actuatorValue);
}


#define ACTUATOR_DEADBAND 3
#define ACTUATOR_SPEED_LOWER_LIMIT 35
#define ACTUATOR_SPEED_UPPER_LIMIT 90
#define SLOW_LIMIT_ANGLE_LOW 35
#define SLOW_LIMIT_ANGLE_HIGH 75
#define ACTUATOR_SPEED_LOWER_LIMIT_SLOW 20
#define ACTUATOR_SPEED_UPPER_LIMIT_SLOW 25

static Timers t1(20);
float error, prev_error, deriv, integ;
float kp=0.01 , kd=1000 , ki=0.1 , dt=0.02 , output=0;

inline int setAct(int act)
{
  if(t1.timerDone())
  {
    prev_error=error;
    bucketAngle = updateAngle();
    switch(act)
    {
      case UP:
        if (bucketAngle<upperLIMIT){
          error=upperLIMIT-bucketAngle;
          deriv=(error-prev_error)/dt;
          integ+=(error)*dt;
          output=error*(kp)+deriv*kd+integ*ki;
          if(bucketAngle>SLOW_LIMIT_ANGLE_HIGH)
          {
            actuatorSpeed= constrain(output,ACTUATOR_SPEED_LOWER_LIMIT_SLOW,ACTUATOR_SPEED_UPPER_LIMIT_SLOW); 
          }
          else
            actuatorSpeed= constrain(output,ACTUATOR_SPEED_LOWER_LIMIT,ACTUATOR_SPEED_UPPER_LIMIT);  
        }
        else
          actuatorSpeed=0;
        break;
      case DOWN:
        if (bucketAngle>lowerLIMIT){          
          error=bucketAngle-lowerLIMIT;
          deriv=(error-prev_error)/dt;
          //integ+=(error)*dt;
          output=error*kp+deriv*kd;//+integ*ki;
          if(bucketAngle<SLOW_LIMIT_ANGLE_LOW)
          {
            
          actuatorSpeed= constrain(-output,-ACTUATOR_SPEED_UPPER_LIMIT_SLOW,-ACTUATOR_SPEED_LOWER_LIMIT_SLOW);
          }
          else         
            actuatorSpeed= constrain(-output,-ACTUATOR_SPEED_UPPER_LIMIT,-ACTUATOR_SPEED_LOWER_LIMIT); 
        }
        else
         actuatorSpeed=0;
        break;
      case STOP:
        actuatorSpeed=0;
        integ=0;
        break;
      case UPOVERRIDE:      
        actuatorSpeed= ACTUATOR_SPEED_UPPER_LIMIT;  
        break;
      case DOWNOVERRIDE:      
        actuatorSpeed= -ACTUATOR_SPEED_UPPER_LIMIT;  
        break;
      default:
        if((act<=93)&&(act>=0)){
          if(!((bucketAngle>(act-ACTUATOR_DEADBAND))  &&   (bucketAngle< (act+ACTUATOR_DEADBAND)))){
            
            if (bucketAngle>act){
              actuatorSpeed= constrain(-(act-bucketAngle),-ACTUATOR_SPEED_UPPER_LIMIT,-ACTUATOR_SPEED_LOWER_LIMIT);
            }
            else if(bucketAngle<act){
              actuatorSpeed= constrain((act-bucketAngle),ACTUATOR_SPEED_LOWER_LIMIT,ACTUATOR_SPEED_UPPER_LIMIT);        
            }
           
          }
          else {
            actuatorValue=255;
            actuatorSpeed=0;
          }
        }
        break;
    }
    
  }
}

