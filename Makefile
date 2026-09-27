CXX = usr/bin/clang
CXXFLAGS = -std=c++17 -Wall
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SRCS = src/main.cpp src/Units.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = app0
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
