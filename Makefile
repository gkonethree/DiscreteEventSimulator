TARGET = simulate
DIR = src
SRC= $(wildcard $(DIR)/*.cpp)
HEADERS = $(wildcard $(DIR)/*.h)
OBJS = $(patsubst %.cpp, %.o, $(SRC)) 
FLAGS = -O0 -fno-elide-constructors -g



all: $(TARGET)
	

$(TARGET): $(OBJS)
	g++ $(FLAGS) -o $@ $^

%.o: %.cpp $(HEADERS)
	g++ $(FLAGS) -o $@ -c $<

clean:
	rm -f -v $(DIR)/*.o $(TARGET)