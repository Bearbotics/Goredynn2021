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
  wpi::StringMap<std::shared_ptr<nt::Value>> defaultRadiusRange {
    std::make_pair("min", nt::Value::MakeDouble(0)),
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
  frc::Shuffleboard::GetTab(visionTabName)
    .AddPersistent("Crosshair - Radius", 0)
    .WithWidget(frc::BuiltInWidgets::kNumberSlider)
    .WithProperties(defaultRadiusRange);
  // frc::Shuffleboard::GetTab(visionTabName)
    
  // frc::Shuffleboard::GetTab(tabName)
  //   .Add("Sensor", false)
  //   .WithWidget(frc::BuiltInWidgets::kBooleanBox);
}
void Robot::RobotPeriodic() {}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() 
{
  switchSpeedToGreen_ = true;
  switchSpeedToGreenNTBool_.SetBoolean(true);
}
void Robot::TeleopPeriodic()
{
  //define variable for absolute heading from pigegon
  // double gyroAngle = pigeon_.GetAbsoluteCompassHeading();
  // printf("Value of gyroAngle = %f",gyroAngle);
  
  //construct mecanum drive with inputs from the 2 joysticks & gyroangle for
  //field oreiented control
  mecanumDrive_.DriveCartesian(
      pow(firstJoystick_.GetY() , 3) * .5 , 
      pow(firstJoystick_.GetX() , 3) * .5 , 
      pow(secondJoystick_.GetX(), 3) * .5 
      // ,gyroAngle
  );
  if (secondJoystick_.GetRawButton(sideButton))
  {
    intakeSignMultiplier_ = -1;
  }
  else
  {
    intakeSignMultiplier_ = 1;
  }
  
  if (firstJoystick_.GetRawButton(3))
  {
    switchSpeedToGreen_ = false;
    switchSpeedToGreenNTBool_.SetBoolean(false);
  }
  switchSpeedToGreen_ = switchSpeedToGreenNTBool_.GetBoolean(false);
  
  
  // Starts both shooter motors when sideButton is pressed
  // firstJoystick_.GetRawButton(sideButton)                   // condition for if/else
  // ? shooterMotors_.Set(shooterSpeed_.GetDouble(0.25))       // runs if condition is true; sets speed to 0.25 as default
  // : shooterMotors_.Set(0);                                  // runs if condition is else (if not true); sets speed to 0
  if (switchSpeedToGreen_)
  {
      firstJoystick_.GetRawButton(sideButton)
      ? shooterMotorOne_.Set(shooterSpeedGreen_.GetDouble(-0.17))
      : shooterMotorOne_.Set(0);
  }
  else
  {
      firstJoystick_.GetRawButton(sideButton)
      ? shooterMotorOne_.Set(shooterSpeed_.GetDouble(0.25))
      : shooterMotorOne_.Set(0);
  }
  


  // activates the feeder when trigger button is pressed
  // this checks if the trigger button is held down
  firstJoystick_.GetRawButton(triggerButton)                // condition for if/else
  ? feederMotor_.Set(feederSpeed_.GetDouble(0.5))           // what runs if true; 0.5 default
  : feederMotor_.Set(0);                                    // what runs if else (not true); 0 because off
  
  if (secondJoystick_.GetRawButton(triggerButton))
  {
    escalatorMotor_.Set(intakeSignMultiplier_ * escalatorSpeed_.GetDouble(0.5));
    intakeMotor_.Set(intakeSignMultiplier_ * intakeSpeed_.GetDouble(0.5));
  } 
  else 
  {
    escalatorMotor_.Set(0);
    intakeMotor_.Set(0);
  }

  // // Update intake motor
  // intakeMotor_.Set(intakeSpeed_.GetDouble(0.0));
  // // Update escalator motor
  // escalatorMotor_.Set(escalatorSpeed_.GetDouble(0.0));
}

void Robot::DisabledInit() {}
void Robot::DisabledPeriodic() {}

void Robot::TestInit() {
}
void Robot::TestPeriodic() 
{
  double gyroAngle = pigeon_.GetAbsoluteCompassHeading();
  printf("Value of gyroAngle = %f",gyroAngle);
  // just run teleop
  // see if something could be implemented
  // to lock networktableentry values / not receive
  // input from Shuffleboard during teleop, but only during testing?
  TeleopPeriodic();
  actualIntakeSpeed_.SetDouble(intakeMotor_.Get());
  actualEscalatorSpeed_.SetDouble(escalatorMotor_.Get());
  actualFeederSpeed_.SetDouble(feederMotor_.Get());
  actualShooterSpeed1_.SetDouble(shooterMotorOne_.Get());
  actualShooterSpeed2_.SetDouble(shooterMotorTwo_.Get());
  if (intakeSignMultiplier_ == 1)
  {
    actualIntakeReversal_.SetBoolean(false);
  }
  else if (intakeSignMultiplier_ == -1)
  {
    actualIntakeReversal_.SetBoolean(true);
  }
}

#ifndef RUNNING_FRC_TESTS
int main()
{
  return frc::StartRobot<Robot>();
}
#endif
