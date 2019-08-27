#version 130

in vec3 Position;
in vec3 Color;

uniform mat4 PVM;

out vec4 vColor;

void main(void) {
  gl_Position = PVM * vec4(Position, 1.0f);
  vColor = vec4(Color, 1.0);
}
