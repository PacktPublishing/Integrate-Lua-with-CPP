LUA_PATH = ../lua
CXX = g++
CXXFLAGS = -Wall -Werror
CPPFLAGS = -I${LUA_PATH}/src
LDFLAGS = -L${LUA_PATH}/src
EXECUTABLE = executable

all: lua project

lua:
	@cd ${LUA_PATH} && make

project: main.cpp LuaExecutor.cc LuaExecutor.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $(EXECUTABLE) main.cpp LuaExecutor.cc -llua

clean:
	rm -f $(EXECUTABLE)
