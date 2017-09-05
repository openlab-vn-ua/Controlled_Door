#include "door.hpp"

Door   door;
Button button_door;
Sensor sensor_open, sensor_close;
Motor  motor_A, motor_B;

#define BUTTON_DOOR_PIN  8
#define SENSOR_OPEN_PIN  10
#define SENSOR_CLOSE_PIN 9

#define MOTOR_A_PIN_A 2
#define MOTOR_A_PIN_B 3
#define MOTOR_B_PIN_A 4
#define MOTOR_B_PIN_B 5

void setup()
{
	Serial.begin(115200);
	
	button_door.init( BUTTON_DOOR_PIN);
	sensor_open.init( SENSOR_OPEN_PIN);
	sensor_close.init(SENSOR_CLOSE_PIN);
	
	motor_A.init(MOTOR_A_PIN_A, MOTOR_A_PIN_B);
	motor_B.init(MOTOR_B_PIN_A, MOTOR_B_PIN_B);
	
	door.bindDoorButton(&button_door);
	door.bindCloseSensor(&sensor_close);
	door.bindOpenSensor(&sensor_open);
	door.bindMotorA(&motor_A);
	door.bindMotorB(&motor_B);
	
	door.init();
}

#define LOOP_DELAY 100

void loop()
{
	door.loop();
	delay(LOOP_DELAY);
}










