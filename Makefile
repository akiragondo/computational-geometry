OBJS	= main.o DCELStructures.o DataStructures.o appDCEL.o ElementalStructures.o Common.o CompositeStructures.o shader.o
SOURCE	= main.cpp DataStructures/DCELStructures.cpp DataStructures/DataStructures.cpp Apps/appDCEL.cpp DataStructures/ElementalStructures.cpp Common.cpp DataStructures/CompositeStructures.cpp shader.cpp
HEADER	= Common.hpp DataStructures/Constants.h DataStructures/DCELStructures.hpp DataStructures/DataStructures.hpp Apps/appDCEL.hpp DataStructures/ElementalStructures.hpp include/cg.h include/cgImage.h include/cgPrimitives.h include/cgTypes.h DataStructures/CompositeStructures.hpp shader.h
OUT	= out
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = -L./lib -lGLEW -lglfw.3.3 -framework OpenGL
INC_DIR  = -I./ -I./DataStructures -I./Apps -I./include -I./Shaders


all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.cpp
	$(CC) $(FLAGS) $< $(INC_DIR) $(LFLAGS) main.cpp

DCELStructures.o: DataStructures/DCELStructures.cpp
	$(CC) $(FLAGS) $< $(INC_DIR) $(LFLAGS) DataStructures/DCELStructures.cpp

DataStructures.o: DataStructures/DataStructures.cpp
	$(CC) $(FLAGS) $< $(INC_DIR) $(LFLAGS) DataStructures/DataStructures.cpp

appDCEL.o: Apps/appDCEL.cpp
	$(CC) $(FLAGS) $< $(INC_DIR) $(LFLAGS) Apps/appDCEL.cpp

ElementalStructures.o: DataStructures/ElementalStructures.cpp
	$(CC) $(FLAGS) $< $(INC_DIR) $(LFLAGS) DataStructures/ElementalStructures.cpp

Common.o: Common.cpp
	$(CC) $(FLAGS) $< $(INC_DIR) $(LFLAGS) Common.cpp

CompositeStructures.o: DataStructures/CompositeStructures.cpp
	$(CC) $(FLAGS) $< $(INC_DIR) $(LFLAGS) DataStructures/CompositeStructures.cpp

shader.o: shader.cpp
	$(CC) $(FLAGS) $< $(INC_DIR) $(LFLAGS) shader.cpp


clean:
	rm -f $(OBJS) $(OUT)
