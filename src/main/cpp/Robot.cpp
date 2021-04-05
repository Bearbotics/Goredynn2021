// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.


// Include header file
#include "Robot.h"


void Robot::RobotInit() 
{
  // Creates the two ranges for where the center of the crosshair
  // could be
  wpi::StringMap<std::shared_ptr<nt::Value>> defaultXRange {
    std::make_pair("min", nt::Value::MakeDouble(-camWidth/2)),
    std::make_pair("max", nt::Value::MakeDouble(camWidth/2))
  };
  wpi::StringMap<std::shared_ptr<nt::Value>> defaultYRange {
    std::make_pair("min", nt::Value::MakeDouble(-camHeight/2)),
    std::make_pair("max", nt::Value::MakeDouble(camHeight/2))
  };
  // Code for the fancy new Camera Crosshair stuff mebe?
  // Which really just needs to add in some widgets
  // Shouldn't need to actually interface with those widgets
  // but having a nice min max would be cool

  // 0 is just a default value
  // Doing this is just so that the pi
  // can change the network table, and
  // the user will just be able to see
  // the slider
  frc::Shuffleboard::GetTab(visionTabName)
    .AddPersistent("Crosshair - X", 0)
    .WithWidget(frc::BuiltInWidgets::kNumberSlider)
    .WithProperties(defaultXRange);
  frc::Shuffleboard::GetTab(visionTabName)
    .AddPersistent("Crosshair - Y", 0)
    .WithWidget(frc::BuiltInWidgets::kNumberSlider)
    .WithProperties(defaultYRange);
  
}
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
  ? shooterMotors_.Set(shooterSpeed_.GetDouble(0.25))       // runs if condition is true; sets speed to 0.25 as default
  : shooterMotors_.Set(0);                                  // runs if condition is else (if not true); sets speed to 0

  // activates the feeder when trigger button is pressed
  // this checks if the trigger button is held down
  firstJoystick_.GetRawButton(triggerButton)                // condition for if/else
  ? feederMotor_.Set(feederSpeed_.GetDouble(0.5))           // what runs if true; 0.5 default
  : feederMotor_.Set(0);                                    // what runs if else (not true); 0 because off
}

void Robot::DisabledInit() {}
void Robot::DisabledPeriodic() {}

void Robot::TestInit() {}
void Robot::TestPeriodic() 
{
  // just run teleop
  // see if something could be implemented
  // to lock networktableentry values / not receive
  // input from Shuffleboard during teleop, but only during testing?
  TeleopPeriodic();
}

#ifndef RUNNING_FRC_TESTS
int main()
{
  return frc::StartRobot<Robot>();
}
#endif
