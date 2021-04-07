import pyfrc as frc
from networktables import NetworkTables, NetworkTablesInstance
import time

team = 2498
testing = True

ntinst = NetworkTablesInstance.getDefault()
ntinst.startDSClient()
# ntinst.startClientTeam(team)
nt = ntinst.getTable("Dashboard")

print("Starting wizard ... please wait...")
# print("Detected 17 errors on startup, proceeding to run")
# print("Daemon detected 15 viruses ... sending reports to Microsoft and Facebook")
# print("Proceeding with startup")

while True:
    # do something
    print(nt.getEntry("Escalator Motor").getDouble(defaultValue=0))
    time.sleep(1)

