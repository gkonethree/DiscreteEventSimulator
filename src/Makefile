TARGET = simulate
OBJS = main.o simulator.o event.o link.o server.o user.o core.o thread.o
HEADERS = simulator.h event.h link.h request.h server.h user.h core.h thread.h
FLAGS = -O0 -fno-elide-constructors -g

all: $(TARGET)

$(TARGET): $(OBJS)
	g++ $(FLAGS) -o $@ $^

%.o: %.cpp $(HEADERS)
	g++ $(FLAGS) -o $@ -c $<

clean:
	rm -f *.o $(TARGET)