player-person-tracker
=====================
Project developed for CS726 course at the University of Auckland
Basic leg tracking algorithm and implementation using the Player-Stage framework for execution on Pioneer robots.


Instructions.
==========
Build project using make.
All builds dependant on Player Stage packages.
make project builds with renderer, requires Allegro5.0 on system path.
make norenderer builds the project without renderer dependencies.

World and configuration files are located in the Worlds directory.

Run the project with ./project [-h hostname]

The project main is located in project.cc
All functionality implemented in collision.cc

Code.
==========
args.h parses command line arguments for the program.

blob.h defines a blob structure for use in the program.

blobdetect.cc and .h provide blob detection methods using matrix type.

collision.cc and .h provide person tracking and collision avoidance.

gridrenderer.cc and .c provide rendering of useful data types.

matrix.cc and .h encapsulate a matrix class to allow for accessors and dynamic assignment.

person.h provides a person structure.

persondetect.cc and .h provide methods for detecting people from a vector of blobs.

position.h provides a position structure.

project.cc is the program entry point.

test.cc contains test methods [not required for compilation]

Scripts.
==========
run.sh transfers and connects to robots to streamline testing.
installstage.sh installs the UoA Player Stage packages and dependancies automatically on a home machine.

Robot controller developed for the CS726 course for person tracking using a low mounted laser scanner and leg detection algorithms. The algorithm attempts to identify legs (or pairs of legs) to follow a person. Local collision avoidance is maintained using the standard VFH+ algorithm.
Developed using the Player-Stage framework for execution on a Pioneer robot.

Algorithm
==========
The algorithm consists of two major parts. Person tracking and control logic. The motion and control logic utilizes the person-tracking algorithm to plan paths and effect robot state. Basic state updates are printed to the console to assist in demonstrating the robot.
Person Tracking

The project uses an occupancy grid based detection algorithm for person tracking. A 2d grid is constructed every execution cycle based on the laser data available. Laser values are mapped to a grid square, which has the effect of smoothing and averaging the available laser data.  Additionally, laser data is only considered within a 3.5m window to minimize disparities resulting from distant readings.  Sliding window based blob detection is then used to identify “blobs” in the image, corresponding to legs in the robots field of view.
These blobs are then passed through a person-identifying algorithm, which looks for pairs of legs within a provided threshold distance.

Movement
==========
Robot movement is based on a state machine utilizing inputs from the person-tracking algorithm. The robot starts in an initialization state, and advances to a following state when a person is identified. In this state, the robot tracks the person and updates local go to commands to follow them. Hysteresis has been implemented so that if a person is not identified, the robot can continue moving to the previous location for a number of cycles. This gives the robot an opportunity to reacquire the target and navigate around objects. After this, the robot checks whether the person has been lost out of the field of view of the laser, and reorients to resolve this if valid, or reverts to the idle state if unaware.
From here, this is repeated as the robot reacquires a target to follow.

Additionally, as the robot follows a person it narrows the field of view for person recognition. This means that the person the robot is following will be preferentially followed over another person.

Rendering
==========
To better view the data and operations occurring on the robot, I build a lightweight renderer based on the Allegro Framework. This allows the user to view the data received from the robot, as well as the current blob and person detection outputs.

Limitations
==========
The tracking algorithm uses laser data returned from the legs of a person, some materials can interfere with the operation of the laser and increase the difficulty of this detection.  As the system looks for a pair of legs, floor length dresses will not be detected. The detection window can detect blobs along the edge of the corridor at the far extent of the laser range. Decreasing the range of valid data from the laser sensor has reduced this error, however integration with a line detection algorithm, to ignore those blobs that are part of a wall, may be more effective.



