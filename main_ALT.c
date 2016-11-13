task main()
{
	// Declare Variables
	int start = 0;
	int heading;
	int angle;
  //Completely clear out any previous sensor readings by setting the port to "sensorNone"
  SensorType[in8] = sensorNone;
  wait1Msec(1000);
  //Reconfigure Analog Port 8 as a Gyro sensor and allow time for ROBOTC to calibrate it
  SensorType[in8] = sensorGyro;
  wait1Msec(2000);

  heading = SensorValue[in8];
  angle = SensorValue[in8];
	// Makes the robot not do anything until 8D button is pressed
	while (start==0) {
		// Checks heading and outputs desired angle and current heading to debug stream
		heading = SensorValue[in8];
		writeDebugStreamLine("Desired Angle is : %d\n", angle);
		writeDebugStreamLine("Current Heading is is : %d\n", heading);
		if (vexRT[Btn8D] == 1) {
			start = start + 1;
		}
	}
	// Control Loop
	while (start == 1){
		// Checks heading and outputs desired angle and current heading to debug stream
		heading = SensorValue[in8];
		writeDebugStreamLine("Desired Angle is : %d\n", angle);
		writeDebugStreamLine("Current Heading is : %d\n", heading);

		//Rotation and angle-setting code
		while (abs(vexRT[Ch4]) > 0) {
			angle = SensorValue[in8];
			heading = SensorValue[in8];
			motor[port2] =   vexRT[Ch1] - vexRT[Ch2] + vexRT[Ch4] * 0.75;
			motor[port3] = - vexRT[Ch1] - vexRT[Ch2] + vexRT[Ch4] * 0.75;
			motor[port4] = - vexRT[Ch1] + vexRT[Ch2] + vexRT[Ch4] * 0.75;
			motor[port5] =   vexRT[Ch1] + vexRT[Ch2] + vexRT[Ch4] * 0.75;
// Attempts at fixing overcorrection after rotation, FIX AND REIMPLEMENT THIS AFTER A LARGE DEGREE OF FUCKING WITH
			if (SensorValue[in8] + vexRT[Ch4]*14 > 3600) {
				angle = (SensorValue[in8] + vexRT[Ch4]*14) %  3600;
			}
			else if (SensorValue[in8] + vexRT[Ch4]*14 > 3600) {
				angle = (SensorValue[in8] + vexRT[Ch4]*14) % -3600;
			}
			else {
				angle = SensorValue[in8] + vexRT[Ch4]*14;
			}
			wait1Msec(10);
		}

		// Translates joystick control to motor actuation
		// Currently the most basic implementation of course correction, works well, overcorrects after yawing
		motor[port2] =   vexRT[Ch1] - vexRT[Ch2] + (angle - heading)/3;
		motor[port3] = - vexRT[Ch1] - vexRT[Ch2] + (angle - heading)/3;
		motor[port4] = - vexRT[Ch1] + vexRT[Ch2] + (angle - heading)/3;
		motor[port5] =   vexRT[Ch1] + vexRT[Ch2] + (angle - heading)/3;
	}
}