# 파일 이름: Makefile

CXX = clang++
CXXFLAGS = -std=c++17 -Wall
SRC_DIRS = application domain exception presentation
SRC = $(wildcard $(SRC_DIRS:%=%/*.cpp)) main.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = dvm_app

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)