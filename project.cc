/*
* Project Main
* Interprets command line arguments, starts connection to server
* Updates robot data and control class [collision].
* 
* Also interprets control signals to perform close operation.
* 
* CS726 Project 1
* Ryan Kurte, rkur014
*/


#include <libplayerc++/playerc++.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>

#include "args.h"
#include "collision.h"

void sigint_handler(int signum);

bool running = true;

int main(int argc, char **argv) {
    //parse arguments from command line. 
    parse_args(argc,argv);

    // Register signal handler.
    signal(SIGINT, sigint_handler);

    // Throw exception on failure.
    try{
        using namespace PlayerCc;

        //Perform Setup.
        //Attach robot.
        PlayerClient robot(gHostname, gPort);
        //Create control object.
        Collision c = Collision(&robot);
        c.Start();

        //The 'endless' while loop.
        while(running) {
            //Update values from the robot.
            robot.Read();
            //Call controller.
            c.Update();
        }

        std::cout << std::endl << "Exiting" << std::endl;        
        c.Stop();

    }catch (PlayerCc::PlayerError e){
        std::cerr << "Robot object disconnected." << std::endl;
        return -1;
    }

    return 0;
}

//Signal callback to catch ctrl+c
void sigint_handler(int signum){
    // Signal process end here.
    //If the process has not been warned.
    if(running) {
        //Request graceful shutdown.
        running = false;
    //Otherwise.
    } else {
        //Kill process.
        std::exit(0);
    }
}











