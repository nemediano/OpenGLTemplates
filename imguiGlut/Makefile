CXX = g++

EXE = glutImgui
SOURCES = glutImgui.cpp
SOURCES += imgui/imgui_impl_glut.cpp imgui/imgui_impl_opengl3.cpp
SOURCES += imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_widgets.cpp
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))

CXXFLAGS += -Wall -std=c++11
LIBS = -lGLEW -lGL -lglut -lm

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LIBS)

%.o:imgui/%.cpp
	$(CXX) $(CXXFLAGS) -DIMGUI_IMPL_OPENGL_LOADER_GLEW -c -o $@ $< $(LIBS)


all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(OBJS)

clear:
	rm -f $(EXE) $(OBJS)