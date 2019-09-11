CXX = g++

EXE = template
SOURCES = main.cpp common.cpp callbacks.cpp menu.cpp
SOURCES += ui/trackball.cpp
SOURCES += imgui/imgui_impl_glfw.cpp imgui/imgui_impl_opengl3.cpp
SOURCES += imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_widgets.cpp
SOURCES += image/texture.cpp image/proceduraltextures.cpp image/screengrabber.cpp
SOURCES += math/mathhelpers.cpp
SOURCES += ogl/oglprogram.cpp ogl/oglhelpers.cpp
SOURCES += light/materialphong.cpp
SOURCES += mesh/mesh.cpp mesh/model.cpp mesh/proceduralmeshes.cpp

OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))

CXXFLAGS += -Wall -std=c++11
LIBS = -lGLEW -lGL -lglfw -lfreeimage -lassimp -lm

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LIBS)

%.o:ui/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LIBS)

%.o:imgui/%.cpp
	$(CXX) $(CXXFLAGS) -DIMGUI_IMPL_OPENGL_LOADER_GLEW -c -o $@ $< $(LIBS)

%.o:image/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LIBS)

%.o:light/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LIBS)

%.o:math/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LIBS)

%.o:ogl/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LIBS)

%.o:mesh/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LIBS)

all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(OBJS)

clear:
	rm -f $(EXE) $(OBJS)