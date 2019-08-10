CXX = g++

EXE = trackballSample
SOURCES = main.cpp
SOURCES += trackball.cpp
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))

CXXFLAGS += -Wall -std=c++11
LIBS = -lGLEW -lGL -lglfw -lm

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LIBS)

all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(OBJS)

clear:
	rm -f $(EXE) $(OBJS)