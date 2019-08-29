#version 130

in vec3 Position;
in vec3 Color;

uniform mat4 PVM;

out vec4 fColor;
out vec2 fTextCoord;

void main(void) {
  fTextCoord = 0.5 * (Position.xy + 1.0);
  gl_Position = PVM * vec4(Position, 1.0f);
  fColor = vec4(Color, 1.0);
}
