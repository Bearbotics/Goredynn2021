# This file should be uploaded to the 
# Raspberry Pi Coprocessor to work
from cscore import CameraServer,UsbCamera
from networktables import NetworkTables, NetworkTablesInstance

import cv2
# import json
import numpy as np
import time

# Using the __main__ caller so that a bunch
# of stuff is already handled, reference:
# https://robotpy.readthedocs.io/en/latest/vision/other.html#vision-other-runcustom
if __name__ == "__main__":
   # Width and height for both cameras
   width = 320
   height = 240
   # Teamnumber
   team = 2498

   # start NetworkTables
   ntinst = NetworkTablesInstance.getDefault()
   print("Setting up NetworkTables client for team {}".format(team))
   ntinst.startClientTeam(team)

   ntVision = ntinst.getTable("Shuffleboard").getSubTable("Vision")
   ntDashboard = ntinst.getTable("Shuffleboard").getSubTable("Dashboard")

   # Initiate CameraServer instance
   cs = CameraServer.getInstance()

   # Initiate first camera to automatically send to dashboard 
   # https://robotpy.readthedocs.io/en/latest/vision/code.html#vision-code
   #                                \/ - preferably use paths? ref above
   cam1 = cs.startAutomaticCapture(dev=0)
   cam1.setResolution(80, 45)
    
   # Initiate second camera to process and then send to dashboard
   # TODO: find what path               \/ is
   cam2 = UsbCamera(name="Aim Cam Unprocessed?", dev=2)
   cam2.setResolution(width, height)

   # Create input stream for the second camera
   input_stream = cs.getVideo(camera = cam2)

   # Create output Stream for Shuffleboard to receive video
   output_stream = cs.putVideo("Aim Cam Processed", width, height)

   # Allocating new images is very expensive, 
   # always try to preallocate space first
   img = np.zeros(shape=(height, width, 3), dtype=np.uint8)

   # Wait for NetworkTables to start
   time.sleep(0.5)

   # An infinite loop that will get images, process,
   # and then send them to Shuffleboard as fast as possible
   while True:
      # Get the time of start
      start_time = time.time()

      # Grabs a frame from the camera
      frame_time, input_img = input_stream.grabFrame(img)

      # Copies the input img to an output_img
      output_img = np.copy(input_img)

      # This detects errors, notify's output of error
      # and skips the current iteration
      if frame_time == 0:
         output_stream.notifyError(input_stream.getError())
         continue
      
      # Get x and y coordinates from Shuffleboard.
      # THE ORIGIN OF COORDS ARE CENTERED AT THE MIDDLE
      x = ntVision.getNumber(key='Crosshair - X',defaultValue=0.0)
      y = ntVision.getNumber(key='Crosshair - Y',defaultValue=0.0)
      rad = int(round(ntVision.getNumber(key='Crosshair - Radius',defaultValue=3.0)))

      # Draw a circle centered at x and y coords
      # with arbitrary radius, color, and thickness?
      # I'm pretty certain this will draw a dot
      # in the middle
      cv2.circle(
         output_img, 
         (round(width/2 + x), round(height/2 + y)), 
         radius = rad, # this probably needs to change to
                     # something smaller
         color = (0, 0, 255), 
         thickness = 3) # negative thickness might mean
                          # to fill in the circle?
      
      # sends the processed image to Shuffleboard
      output_stream.putFrame(output_img)

      # Calculates processing time
      processing_time = time.time() - start_time

      # Calculates an estimate for how many frames
      # can be calculated if all frames spent this 
      # long to be processed
      fps = 1 / processing_time

      # Send the fps data to Shuffleboard
      ntVision.putNumber('FPS for Aimer', fps)