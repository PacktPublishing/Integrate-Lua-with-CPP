LUA_PATH = ../lua
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Werror
CPPFLAGS = -I${LUA_PATH}/src
LDFLAGS = -L${LUA_PATH}/src
EXECUTABLE = executable
ALL_O = main.o LuaExecutor.o LoggingLuaExecutorListener.o Destinations.o
DESTINATIONS_O = Destinations.o DestinationsModule.o
DESTINATIONS_SO = destinations.so

all: clean lua project destinations

lua:
	@cd ${LUA_PATH} && make

project: ${ALL_O}
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) \
		-o $(EXECUTABLE) ${ALL_O} -llua

destinations: ${DESTINATIONS_O}
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -shared \
		-o $(DESTINATIONS_SO) ${DESTINATIONS_O} -llua

clean:
	rm -f ${ALL_O} $(EXECUTABLE) ${DESTINATIONS_O} ${DESTINATIONS_SO}
