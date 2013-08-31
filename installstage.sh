#!/bin/bash
 
#64-bit Player stage installation script for UoA students.
#Use at your own risk.
 
#Install dependencies.
sudo apt-get -y install build-essential
sudo apt-get -y install libltdl-dev festival libgtk-3-0 libgtk-3-dev libgnomecanvas2-dev libjpeg-dev libfltk1.1-dev subversion cmake
sudo apt-get -y install libgs-dev libxmu-dev freeglut3-dev libpng12-0 libpng12-dev libboost-dev libboost-thread-dev libboost-signals-dev libdb5.1-stl
 
#Create working directory.
cd ~
mkdir playerstagesrc
cd playerstagesrc
 
#Download.
wget https://svn.ece.auckland.ac.nz/robotics-teaching/browser/cs726/player-3.0.2_ECE02112012.tar.gz?format=raw
 
wget https://svn.ece.auckland.ac.nz/robotics-teaching/browser/cs726/Stage-3.2.2-ECE02032012.tar.gz?format=raw
#Extract.
tar -xvf player-3.0.2_ECE02112012.tar.gz?format=raw
 
tar -xvf Stage-3.2.2-ECE02032012.tar.gz?format=raw
 
#Remove archives.
rm player-3.0.2_ECE02112012.tar.gz?format=raw
 
rm Stage-3.2.2-ECE02032012.tar.gz?format=raw
 
 
#Build and install player.
cd player-3.0.2
mkdir build
cd build
cmake ../
make
sudo make install
 
cd ~/playerstagesrc/Stage-3.2.2-Source/
 
#Player library path required for Stage compilation.
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib64/pkgconfig
 
#Build and install stage.
mkdir build
cd build
cmake ../
make
sudo make install
 
#Back up bashrc
cp  ~/.bashrc  ~/.bashrc.old
#Add to path.
echo "" >> ~/.bashrc
echo "export LD_LIBRARY_PATH=\${PATH}:/usr/local/lib:/usr/local/lib64" >> ~/.bashrc
echo "export PKG_CONFIG_PATH=\${PKG_CONFIG_PATH}:/usr/local/lib64/pkgconfig:/usr/local/lib/pkgconfig" >> ~/.bashrc
 
#Reload Path
source ~/.bashrc
 
 
cd ~