CXX = g++
CPPFLAGS = -Wall -O2 -g

SOURCES =  Tables.cpp Identifier.cpp Assembler.cpp
OBJECTS = $(patsubst %.cpp, %.o, $(SOURCES))
EXECUTABLE = Assembler

SHARED_OBJ = ../shared/ProcessLogger.o
BOOST_LIBS = -lboost_thread -lboost_system -lboost_filesystem -lboost_regex -lboost_program_options
ARMA_LIBS = -larmadillo

INC = -I. -I../shared

.PHONY: all
all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CPPFLAGS) $(INC) $(SHARED_OBJ) $(BOOST_LIBS) $(PLAYER_LIBS) $(ARMA_LIBS) $^ -o $@

.cpp.o:
	$(CXX) $(CPPFLAGS) $(INC) $(PLAYER_LIBS) -c -g $< -o $@

.PHONY: print
print:
	@echo Sources: $(SOURCES)
	@echo Objects: $(OBJECTS)
	@echo Executable: $(EXECUTABLE)

.PHONY: clean
clean:
	-rm -f $(EXECUTABLE) *.o
