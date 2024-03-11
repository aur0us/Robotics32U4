#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD display;
Zumo32U4LineSensors lineSensors;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4Motors motors;

unsigned int lineSensorValues[3];
int reverseCounter = 0;

void turn(int direction, int speed, int duration) { //TODO Perhaps add detection for sensors + duration if one is met then stop the turn
  if (direction == 0) { // Turn left
      motors.setSpeeds(-speed, speed);
      delay(duration); 
    }
  if (direction == 1) { // Turn right
        motors.setSpeeds(speed, -speed);
        delay(duration);
         } 
  else {
    // Handle invalid direction input (you could stop the robot, for example) 
  }
}


void reverse(int duration)
{
  motors.setSpeeds(-150, -150);
  delay(duration);
}

void LCDdisplay() {
  // lineSensors.read(lineSensorValues,QTR_EMITTERS_ON);
  // display.clear();
  // display.gotoXY(2, 0);
  // display.print(lineSensorValues[1]); // Middle Sensor
  // display.gotoXY(0, 1);
  // display.print(lineSensorValues[0]); // Left Sensor
  // display.gotoXY(4, 1);
  // display.print(lineSensorValues[2]); // Right Sensor
  // delay(20);
  // Serial.println(lineSensorValues[1]);
  // Serial.println(lineSensorValues[0]);
  // Serial.println(lineSensorValues[2]);
  // delay(100);
} 

void setup() {
  lineSensors.initThreeSensors();
  proxSensors.initThreeSensors();
}

void loop() {
  lineSensors.read(lineSensorValues, QTR_EMITTERS_ON);
  proxSensors.read();
  LCDdisplay();
  // Line Avoidance Logic
  if (lineSensorValues[0] > 500 || lineSensorValues[1] > 500 || lineSensorValues[2] > 500) { 
    // Line detected - Turn

    if(lineSensorValues[0] > 400) // Left sensor turn right
    {
        turn(1, 100, 125); // Turn right (adjust direction if needed)
    }

    if(lineSensorValues[2] > 400)  // Right sensor turn left
    {
      turn(0 ,100,125); // Turn Left
    }

     if(lineSensorValues[2] > 400 && lineSensorValues[1] > 400) // Right and middle, reverse and turn left
    {
      reverse(150);
      turn(0 ,100,125); // Turn left
    }

     if(lineSensorValues[0] > 400 && lineSensorValues[1] > 400) // Left and middle, reverse and turn right
    {
      reverse(150);
      turn(1 ,100,125); // Turn right
    }

    if(lineSensorValues[0] > 400 && lineSensorValues[2] > 400) // Right and left, reverse and turn 
    {
      reverse(150);
      turn(1,100,125);
    }
    
     if(lineSensorValues[1] > 400) // Middle sensor reverse
    {
      reverse(300);
    }
  } else {  
  if(proxSensors.countsFrontWithLeftLeds() >= 6
        || proxSensors.countsFrontWithRightLeds() >= 6)
        {
          motors.setSpeeds(0, 0);
          display.gotoXY(1, 0);
          display.print("HOUSE");
        }
        else{
        display.clear();  
        motors.setSpeeds(75, 75); 
        }
    // No line detected - Move forward
  }
}