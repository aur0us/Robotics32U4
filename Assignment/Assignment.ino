#include <Wire.h>
#include <Zumo32U4.h>
Zumo32U4LCD display;
Zumo32U4LineSensors lineSensors;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4Motors motors;
Zumo32U4Buzzer buzzer;
Zumo32U4ButtonA buttonA;

unsigned long previousMillis = 0;      
const long interval = 1000;            
const int detectionThreshHold = 400;  
const int MAX_TURNS = 200;  
const int turnTrigger = 7;


unsigned int lineSensorValues[3];
int reverseCounter = 0;
int turnCounter = 0;
int HouseCounter = 0;
int currentTurnIndex = 0; 

struct Turn {
  bool turnLeft;
  bool reverse;
  int duration;
  int reverseDuration;
};

Turn turnLog[MAX_TURNS];

void turn(int direction, int speed, int duration) {  
  if (direction == 0) {                              
    motors.setSpeeds(-speed, speed);
    delay(duration);
  }
  if (direction == 1) {  
    motors.setSpeeds(speed, -speed);
    delay(duration);
  }
}

void proxScanner()
{
  proxSensors.read();
  for(uint16_t i = 0; i < 60; i++)
  {
    if (i > 0 && i <= 30)
    {
      motors.setSpeeds(-100, 100);
    }
    else
    {
      motors.setSpeeds(100, -100);
    }
  }
  if (proxSensors.countsFrontWithLeftLeds() >= 5 || proxSensors.countsFrontWithRightLeds() >= 5)
    {
          motors.setSpeeds(0, 0);
          HouseCounter++;
          for(int i = 0; i < HouseCounter; i++)
          {
            buzzer.playFromProgramSpace(PSTR("!>>a32"));
            delay(100);
          }
          lineSensors.initThreeSensors();  
          turn(0, 150, 800);
     } 

  if(HouseCounter >= 3)
  {
    // homeFunction();
  }
}
void homeFunction() {
  motors.setSpeeds(0, 0);
  for (currentTurnIndex > 0; currentTurnIndex--;) {
    Turn currentTurn = turnLog[currentTurnIndex];
    int direction = currentTurn.turnLeft ? 0 : 1;
    if (currentTurn.reverse == false) {
      turn(direction , 125, currentTurn.duration);
    } else {
      reverse(currentTurn.reverseDuration);
      turn(direction, 125, currentTurn.duration);
    }
  }
}

void reverse(int duration) {
  motors.setSpeeds(-150, -150);
  delay(duration);
}

void calibrateSensors()
{
  delay(1000);
  for(uint16_t i = 0; i < 120; i++)
  {
    if (i > 30 && i <= 90)
    {
      motors.setSpeeds(-200, 200);
    }
    else
    {
      motors.setSpeeds(200, -200);
    }

    lineSensors.calibrate();
  }
  motors.setSpeeds(0, 0);
}

void setup() {
  
  buzzer.play(">g32>>c32");
  lineSensors.initThreeSensors();
  proxSensors.initThreeSensors();
  buttonA.waitForButton();
  calibrateSensors();
}

void loop() {
  lineSensors.read(lineSensorValues, QTR_EMITTERS_ON);
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    proxScanner();
    }

  motors.setSpeeds(75, 75);
  if (lineSensorValues[0] > detectionThreshHold || lineSensorValues[1] > detectionThreshHold || lineSensorValues[2] > detectionThreshHold) {
    delay(50);
    if (lineSensorValues[0] > detectionThreshHold)  
    {
    if(turnCounter < turnTrigger)
          {
          turn(1, 100, 125);  
          turnCounter++;
          }
          else
          {
            turnCounter = 0;
            reverse(150);
            turn(1, 100, 125);  
          }        
        if (currentTurnIndex < MAX_TURNS) {
        turnLog[currentTurnIndex].turnLeft = false;  
        turnLog[currentTurnIndex].duration = 125;
        turnLog[currentTurnIndex].reverse = false;
        turnLog[currentTurnIndex].reverseDuration = 0;
        currentTurnIndex++;
      }
    }

    if (lineSensorValues[2] > detectionThreshHold)  
    {
    if(turnCounter < turnTrigger)
          {
          turn(0, 100, 125);  
          turnCounter++;
          }
          else
          {
            turnCounter = 0;
            reverse(150);
            turn(0, 100, 125);  
          }       
        if (currentTurnIndex < MAX_TURNS) {
        turnLog[currentTurnIndex].turnLeft = true;  
        turnLog[currentTurnIndex].duration = 125;
        turnLog[currentTurnIndex].reverse = false;
        turnLog[currentTurnIndex].reverseDuration = 0;
        currentTurnIndex++;
      }

      if (lineSensorValues[2] > detectionThreshHold && lineSensorValues[1] > detectionThreshHold)  
      {
         if(turnCounter < turnTrigger)
          {
          reverse(150);
          turn(0, 100, 125);  
          turnCounter++;
          }
          else
          {
            turnCounter = 0;
            reverse(300);
            turn(0, 100, 125);  
          } 
        if (currentTurnIndex < MAX_TURNS) {
          turnLog[currentTurnIndex].turnLeft = true;  
          turnLog[currentTurnIndex].duration = 125;
          turnLog[currentTurnIndex].reverse = true;
          turnLog[currentTurnIndex].reverseDuration = 150;
          currentTurnIndex++;
        }

        if (lineSensorValues[0] > detectionThreshHold && lineSensorValues[1] > detectionThreshHold)  
        {
          if(turnCounter < turnTrigger)
          {
          reverse(150);
          turn(1, 100, 125);
          turnCounter++;
          }
          else
          {
            turnCounter = 0;
            reverse(300);
            turn(1, 100, 125);  
          }
          if (currentTurnIndex < MAX_TURNS) {
            turnLog[currentTurnIndex].turnLeft = false;  
            turnLog[currentTurnIndex].duration = 125;
            turnLog[currentTurnIndex].reverse = true;
            turnLog[currentTurnIndex].reverseDuration = 150;
            currentTurnIndex++;
          }
        }

        if (lineSensorValues[0] > detectionThreshHold && lineSensorValues[2] > detectionThreshHold)  
        {
            if(turnCounter < turnTrigger)
          {
          reverse(150);
          turn(1, 100, 125);
          turnCounter++;
          }
          else
          {
            turnCounter = 0;
            reverse(300);
            turn(1, 100, 125);  
          }
          if (currentTurnIndex < MAX_TURNS) {
            turnLog[currentTurnIndex].turnLeft = false;  
            turnLog[currentTurnIndex].duration = 125;
            turnLog[currentTurnIndex].reverse = true;
            turnLog[currentTurnIndex].reverseDuration = 150;
            currentTurnIndex++;
          }
        }

        if (lineSensorValues[1] > detectionThreshHold)  
        {
          reverseCounter++;
          if (reverseCounter < turnTrigger) {
            reverse(300);
            if (currentTurnIndex < MAX_TURNS) {
              turnLog[currentTurnIndex].turnLeft = false;  
              turnLog[currentTurnIndex].duration = 0;
              turnLog[currentTurnIndex].reverse = true;
              turnLog[currentTurnIndex].reverseDuration = 300;
              currentTurnIndex++;
            } else {
              reverse(500);
              reverseCounter = 0;
              if (currentTurnIndex < MAX_TURNS) {
                turnLog[currentTurnIndex].turnLeft = false;  
                turnLog[currentTurnIndex].duration = 0;
                turnLog[currentTurnIndex].reverse = true;
                turnLog[currentTurnIndex].reverseDuration = 500;
                currentTurnIndex++;
              }
            }
          }
        }
      }
    }
  }
  else{
    motors.setSpeeds(75, 75);
  }
}