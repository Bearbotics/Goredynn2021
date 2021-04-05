#!/usr/bin/env python3

# Copyright (c) FIRST and other WPILib contributors.
# Open Source Software; you can modify and/or share it under the terms of
# the WPILib BSD license file in the root directory of this project.

import json
import time
import numpy as np
import sys

from cscore import CameraServer, VideoSource, UsbCamera, MjpegServer
from networktables import NetworkTablesInstance

def startCamera(camName, camPath):
    """Start running the camera."""
    print("Starting camera '{}' on {}".format(camName, campath))
    inst = CameraServer.getInstance()
    camera = UsbCamera(camName, camPath)
    server = inst.startAutomaticCapture(camera=camera, return_server=True)

    # camera.setConfigJson(json.dumps(config.config))
    # camera.setConnectionStrategy(VideoSource.ConnectionStrategy.kKeepOpen)

    return camera

if __name__ == "__main__":

    # start NetworkTables
    ntinst = NetworkTablesInstance.getDefault()
    print("Setting up NetworkTables client for team 2498")
    ntinst.startClientTeam(2498)
    # ntinst.startDSClient()

    # Table for vision output information
    nt = NetworkTables.getTable('Dashboard')

    # Allocating new images is very expensive, always try to preallocate
    img = np.zeros(shape=(240, 320, 3), dtype=np.uint8)

    # start cameras
    startCamera("Aim Cam", "/dev/0")
    startCamera("Monitor Cam", "/dev/1")



    # loop forever
    while True:
        time.sleep(10)
        

    # use grip + contours
        # for later ig
