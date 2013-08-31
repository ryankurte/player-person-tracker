# Makefile for player c++ clients

#Specify your target and your cc source file name below
CC = g++
TARGET = main.o
SRC = project.cc

#Included files.

SOURCES = project.cc collision.cc matrix.cc gridrenderer.cc blobdetect.cc persondetect.cc
HEADERS = collision.h matrix.h gridrenderer.h blobdetect.h blob.h persondetect.h person.h
OBJECTS=$(SOURCES:.cc=.o)

LIBS = playerc++ allegro-5.0 allegro_main-5.0 allegro_primitives-5.0

#include flags for player libraries.
CPPFLAGS = `pkg-config --cflags $(LIBS)` -Wall -g
LDFLAGS = `pkg-config --libs $(LIBS)`
#File name for backup function.
OUTFILE=../robot_`date '+%Y%m%d'`.tgz

# Pick up the necessary options, directories and options for compiling and linking with player 
# Add a warning on all errors, and -g3 for debugging information 

#Build all.
all: $(TARGET) $(OBJECTS)

#Build project [old slow builder].
project2: $(SRC) $(SOURCES) $(HEADERS)
	$(CC) $(CPPFLAGS) $(SRC) $(SOURCES) $(LDFLAGS) -o $(TARGET)

#Build project [new incremental builder].	
$(TARGET): $(OBJECTS) 
	$(CC) $(CPPFLAGS) $(OBJECTS) $(LDFLAGS) -o $(TARGET)



#Build project without renderer.
norender: $(SRC) $(SOURCES) $(HEADERS)
	$(CC) `pkg-config --cflags playerc++` -Wall -g $(SRC) $(SOURCES) `pkg-config --libs playerc++` -o $(TARGET)_norenderer -DNO_RENDERER
	
#Objects.
.cc.o:
	$(CC) $(CPPFLAGS) $(LDFLAGS) -c $< -o $@

#Clean objects.
clean:
	rm -f $(TARGET) *.o *~

