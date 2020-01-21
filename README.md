# C++ templates for learning OpenGL

A common obstacle to start learning **modern** [OpenGL](http://www.opengl.org/) is that requires several external libraries to start producing programs. I mean *modern* OpenGL, in the sense of programable pipeline (version 3.3 and above).

At the very least requires:
1. An OpenGL extension loader library.
2. A window manager library that support OpenGL context.
3. A graphics mathematics library.
And for an intermadiate level progammer also requires:
1. A mesh loader library.
2. An image reader/writer library.
3. A way to create user simple IU (Could be one of the previous libraries)

A novice graphic programmer, may struggle in the use of the libraries instead of focusing in the  actual OpenGL. I created several templates to help such users to have a minimal working OpenGL example. Moreover, in contrast to most of the examples in the internet and the books, I use C++ instead of C.

The current templates are:
* A minimal OpenGL [template](glutTriangle/) using: C++, GLEW, GLM and freeglut.
* A minimal OpenGL [template](glfwTriangle/) using: C++, GLEW, GLM and glfw.
* An OpenGL [template](imguiGlfw/) with an user menu (using [Dear imgui](http://github.com/ocornut/imgui)) using: C++, GLEW, GLM and glfw.
* An OpenGL [sample](trackballCamera/) of a trackball camera implementation using GLFW.
* An OpenGL [template](imguiGlut/) with an user menu (using [Dear imgui](http://github.com/ocornut/imgui)) using: C++, GLEW, GLM and freeglut.
* An intermediate OpenGL [template](template/) with all the previous features, plus a lot of extra helper clases for OpenGL.