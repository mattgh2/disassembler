CXX := g++
CXXFLAGS := -g -ansi -pedantic -Wall  -Wextra -std=c++14

OBS := main.o memory.o hex.o rv32i_decode.o

all: rv32i

rv32i: $(OBS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBS) $(TARGET)
