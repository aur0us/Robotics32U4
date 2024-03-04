#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4LineSensors lineSensors;
Zumo32U4Motors motors;

unsigned int lineSensorValues[3];

void turn(int direction, int speed, int duration) { //TODO Fix me you idiot the duration doesnt work
  if (direction == 0) { // Turn left
    motors.setSpeeds(-speed, speed); 
  } else if (direction == 1) { // Turn right
    motors.setSpeeds(speed, -speed);
  } else {
    // Handle invalid direction input (you could stop the robot, for example) 
  }
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
} 


void setup() {
  lineSensors.initThreeSensors();
}

void loop() {
  lineSensors.read(lineSensorValues, QTR_EMITTERS_ON);
  display();
  // Line Avoidance Logic
  if (lineSensorValues[0] > 600 || lineSensorValues[1] > 600 || lineSensorValues[2] > 600) { 
    // Line detected - Turn
    motors.setSpeeds(-100, -100);
    delay(100);
    if(lineSensorValues[0] > 600)
    {
        turn(1, 200, 300); // Turn right (adjust direction if needed)
    }
    if(lineSensorValues[2] > 600)
    {
      turn(0 ,200,300);
    }
  } else {  
    // No line detected - Move forward
    motors.setSpeeds(100, 100); 
  }
}
