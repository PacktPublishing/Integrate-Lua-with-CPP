LUA_PATH = ../../lua
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Werror
CPPFLAGS = -I${LUA_PATH}/src
LDFLAGS = -L${LUA_PATH}/src
EXECUTABLE = executable
ALL_O = main.o LuaExecutor.o LoggingLuaExecutorListener.o

all: clean lua project

lua:
	@cd ${LUA_PATH} && make

project: ${ALL_O}
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $(EXECUTABLE) ${ALL_O} -llua

clean:
	rm -f ${ALL_O} $(EXECUTABLE)
