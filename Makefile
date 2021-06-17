CFLAGS = -c -g -O3 -fno-stack-protector
LDFLAGS = -lm -std=c++11 -g
# executable
EXEC = main
# fichiers sources
SRC = main.cpp demand.cpp corr.cpp

# tous les fichiers .cpp deviennent des .o
OBJ = $(SRC:.cpp=.o)

all : $(EXEC)

$(EXEC) : $(OBJ)
	g++ $(LDFLAGS) -o $@ $^

%.o : %.cpp
	g++ $(CFLAGS) -o $@ $<

clean:
	rm -rf *.o
	rm -rf $(EXEC)
