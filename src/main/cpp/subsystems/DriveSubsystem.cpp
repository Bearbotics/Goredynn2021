// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "subsystems/DriveSubsystem.h"

#include <units/angle.h>
#include <units/velocity.h>
#include <units/voltage.h>

#include "Constants.h"

using namespace DriveConstants;

DriveSubsystem::DriveSubsystem()
    : m_frontLeft{kFrontLeftMotorPort,rev::CANSparkMax::MotorType::kBrushless},
      m_rearLeft{kRearLeftMotorPort,rev::CANSparkMax::MotorType::kBrushless},
      m_frontRight{kFrontRightMotorPort,rev::CANSparkMax::MotorType::kBrushless},
      m_rearRight{kRearRightMotorPort,rev::CANSparkMax::MotorType::kBrushless},

      m_frontLeftEncoder{m_frontLeft,rev::CANEncoder::EncoderType::kHallSensor},
      m_rearLeftEncoder{m_rearLeft,rev::CANEncoder::EncoderType::kHallSensor},
      m_frontRightEncoder{m_frontRight,rev::CANEncoder::EncoderType::kHallSensor},
      m_rearRightEncoder{m_rearRight,rev::CANEncoder::EncoderType::kHallSensor},
      
      m_gyro{kPigeonIMUPort},

      m_odometry{kDriveKinematics, m_gyro., frc::Pose2d()} {
  // Set the distance per pulse for the encoders
  m_frontLeftEncoder.SetDistancePerPulse(kEncoderDistancePerPulse);
  m_rearLeftEncoder.SetDistancePerPulse(kEncoderDistancePerPulse);
  m_frontRightEncoder.SetDistancePerPulse(kEncoderDistancePerPulse);
  m_rearRightEncoder.SetDistancePerPulse(kEncoderDistancePerPulse);
}

void DriveSubsystem::Periodic() {
  // Implementation of subsystem periodic method goes here.
  m_odometry.Update(
      m_gyro.GetRotation2d(),
      frc::MecanumDriveWheelSpeeds{
          units::meters_per_second_t(m_frontLeftEncoder.GetRate()),
          units::meters_per_second_t(m_rearLeftEncoder.GetRate()),
          units::meters_per_second_t(m_frontRightEncoder.GetRate()),
          units::meters_per_second_t(m_rearRightEncoder.GetRate())});
}

void DriveSubsystem::Drive(double xSpeed, double ySpeed, double rot,
                           bool fieldRelative) {
  if (fieldRelative) {
    m_drive.DriveCartesian(ySpeed, xSpeed, rot, -m_gyro.GetAngle());
  } else {
    m_drive.DriveCartesian(ySpeed, xSpeed, rot);
  }
}

void DriveSubsystem::SetSpeedControllersVolts(units::volt_t frontLeftPower,
                                              units::volt_t rearLeftPower,
                                              units::volt_t frontRightPower,
                                              units::volt_t rearRightPower) {
  m_frontLeft.SetVoltage(frontLeftPower);
  m_rearLeft.SetVoltage(rearLeftPower);
  m_frontRight.SetVoltage(frontRightPower);
  m_rearRight.SetVoltage(rearRightPower);
}

// don't know if this function is needed or not
void DriveSubsystem::ResetEncoders() {
  m_frontLeftEncoder.Reset();
  m_rearLeftEncoder.Reset();
  m_frontRightEncoder.Reset();
  m_rearRightEncoder.Reset();
}

frc::Encoder& DriveSubsystem::GetFrontLeftEncoder() {
  return m_frontLeftEncoder;
}

frc::Encoder& DriveSubsystem::GetRearLeftEncoder() {
  return m_rearLeftEncoder;
}

frc::Encoder& DriveSubsystem::GetFrontRightEncoder() {
  return m_frontRightEncoder;
}

frc::Encoder& DriveSubsystem::GetRearRightEncoder() {
  return m_rearRightEncoder;
}

frc::MecanumDriveWheelSpeeds DriveSubsystem::getCurrentWheelSpeeds() {
  return (frc::MecanumDriveWheelSpeeds{
      units::meters_per_second_t(m_frontLeftEncoder.GetRate()),
      units::meters_per_second_t(m_rearLeftEncoder.GetRate()),
      units::meters_per_second_t(m_frontRightEncoder.GetRate()),
      units::meters_per_second_t(m_rearRightEncoder.GetRate())});
}

void DriveSubsystem::SetMaxOutput(double maxOutput) {
  m_drive.SetMaxOutput(maxOutput);
}

// Still not certain about this function...
frc::Rotation2d DriveSubsystem::GetPigeonRotation2d() {
  // also this is almost certainly incorrect
  return frc::Rotation2d(units::degree_t(m_gyro.GetAbsoluteCompassHeading()));
}

units::degree_t DriveSubsystem::GetHeading() {
  return DriveSubsystem::GetPigeonRotation2d().Degrees();
}

void DriveSubsystem::ZeroHeading() {
  m_gyro.Reset();
}

double DriveSubsystem::GetTurnRate() {
  return -m_gyro.GetRate();
}

frc::Pose2d DriveSubsystem::GetPose() {
  return m_odometry.GetPose();
}

void DriveSubsystem::ResetOdometry(frc::Pose2d pose) {
  m_odometry.ResetPosition(pose, m_gyro.GetRotation2d());
}
