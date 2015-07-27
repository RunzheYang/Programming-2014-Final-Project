## You might want to add -O2 to CFLAGS after debug.
## Change BOOST_PATH to the location where boost is located

AI = player

TARGET = client
LIBS = -lpthread -lboost_system
CXX = g++
#CFLAGS = -std=c++11 -Wall -Wextra -g		## use this for debug
CFLAGS = -std=c++11 -Wall -Wextra -O2 -DNDEBUG

HEADERS = network.h player.h 
OBJECTS = ${AI}.o main.o

.PHONY: all clean
.PRECIOUS: $(TARGET) $(OBJECTS)

all: ${TARGET}

${TARGET}: ${OBJECTS}

${AI}.o: player.h ${AI}.cpp
	${CXX} ${CFLAGS} -c -o ${AI}.o ${AI}.cpp

main.o: network.h main.cpp
	${CXX} ${CFLAGS} -c -o main.o main.cpp


${TARGET}: ${OBJECTS}
	${CXX} ${OBJECTS} -Wall ${LIBS} -o ${TARGET} 

clean:
	-rm *.o
	-rm ${TARGET}
