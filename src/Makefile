TARGET = simulate
OBJS = main.o simulator.o event.o link.o server.o user.o core.o
HEADERS = simulator.h event.h link.h request.h server.h user.h core.h

all: $(TARGET)

$(TARGET): $(OBJS)
	g++ -o $@ $^

%.o: %.cpp $(HEADERS)
	g++ -o $@ -c $<

clean:
	rm -f *.o $(TARGET)