#!/bin/bash

#Installs and runs shit on a robot.

ADDRESS=$1

echo Running.

#Copy files.
scp World/rkur014.cfg robot@$ADDRESS:

#Create connection.
ssh robot@$ADDRESS

#Remove files.
ssh robot@$ADDRESS "rm rkur014.cfg lodo_driver.so"
