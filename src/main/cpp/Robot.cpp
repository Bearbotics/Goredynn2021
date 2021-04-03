// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.


// Include header file
#include "Robot.h"


void Robot::RobotInit() {}
void Robot::RobotPeriodic() {}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {}
void Robot::TeleopPeriodic()
{
  //define variable for absolute heading from pigegon
  double gyroAngle = pigeon_.GetAbsoluteCompassHeading();
  
  // not certain about these controls
  // and whether field-oriented drive is a good idea or not?
  // mecanumDrive_.DriveCartesian(pow(firstJoystick_.GetX(),3)*-1, pow(firstJoystick_.GetY(),3),pow(secondJoystick_.GetX(),3) * .7, gyroAngle);
  // Strafing is reversed for x-axis above soooooooo.......
  
  //construct mecanum drive with inputs from the 2 joysticks & gyroangle for
  //field oreiented control
  mecanumDrive_.DriveCartesian(
    pow(firstJoystick_.GetX() , 3)      , 
    pow(firstJoystick_.GetY() , 3)      , 
    pow(secondJoystick_.GetX(), 3) * .7 , 
    gyroAngle
  );
  
  // Starts both shooter motors when sideButton is pressed
  firstJoystick_.GetRawButton(sideButton)                   // condition for if/else
  ? shooterMotors_.Set(shooterSpeed_.GetDouble(0.25))        // runs if condition is true
  : shooterMotors_.Set(shooterSpeed_.GetDouble(0.0));       // runs condition is else (if not true)

  // activates the feeder when trigger button is pressed
  // this checks if the trigger button is held down
  firstJoystick_.GetRawButton(triggerButton)                // condition for if/else
  ? feederMotor_.Set(feederSpeed_.GetDouble(0.5))           //what runs if true
  : feederMotor_.Set(feederSpeed_.GetDouble(0.0));          //what runs if else (not true)
}

void Robot::DisabledInit() {}
void Robot::DisabledPeriodic() {}

void Robot::TestInit() {}
void Robot::TestPeriodic() 
{
  // just run teleop, because the built in widget
  // kSpeedController should be editable only in test mode
  TeleopPeriodic();
}

#ifndef RUNNING_FRC_TESTS
int main()
{
  return frc::StartRobot<Robot>();
}
#endif
