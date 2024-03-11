#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4LineSensors lineSensors;
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

void display() {
  lineSensors.read(lineSensorValues,QTR_EMITTERS_ON);
  lcd.clear();
  lcd.gotoXY(2, 0);
  lcd.print(lineSensorValues[1]); // Middle Sensor
  lcd.gotoXY(0, 1);
  lcd.print(lineSensorValues[0]); // Left Sensor
  lcd.gotoXY(4, 1);
  lcd.print(lineSensorValues[2]); // Right Sensor
  delay(20);
  Serial.println(lineSensorValues[1]);
  Serial.println(lineSensorValues[0]);
  Serial.println(lineSensorValues[2]);
  delay(100);
} 

void setup() {
  lineSensors.initThreeSensors();
}

void loop() {
  lineSensors.read(lineSensorValues, QTR_EMITTERS_ON);
  display();
  // Line Avoidance Logic
  if (lineSensorValues[0] > 500 || lineSensorValues[1] > 500 || lineSensorValues[2] > 500) { 
    // Line detected - Turn

    if(lineSensorValues[0] > 400)
    {
        turn(1, 100, 200); // Turn right (adjust direction if needed)
    }

    if(lineSensorValues[1] > 400)
    {
      // int duration = reverseCounter >= 3 ? 300 : 150 ;
      reverse(300);
    }
    if(lineSensorValues[2] > 400)
    {
      turn(0 ,100,200); // Turn Left
    }

     if(lineSensorValues[2] > 400 && lineSensorValues[1] > 400)
    {
      reverse(150);
      turn(0 ,100,200); // Turn left
    }

     if(lineSensorValues[0] > 400 && lineSensorValues[1] > 400)
    {
      reverse(150);
      turn(1 ,100,200); // Turn right
    }

    if(lineSensorValues[0] > 400 && lineSensorValues[2] > 400)
    {
      reverse(150);
      turn(1,100,200);
    }
    
  } else {  
    // No line detected - Move forward
    motors.setSpeeds(75, 75); 
  }
}