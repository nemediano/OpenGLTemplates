#version 130

in vec4 fColor;
in vec2 fTextCoord;

out vec4 fragcolor;

uniform sampler2D textureMap;

void main(void) {
  vec4 textColor = texture(textureMap, fTextCoord);
  fragcolor = mix(fColor, textColor, 0.5);
}