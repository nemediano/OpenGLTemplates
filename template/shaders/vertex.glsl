#version 130
in vec3 posAttr;
in vec3 normalAttr;
in vec2 textCoordAttr;

uniform mat4 PVM;
uniform mat4 NormalMat;

out vec3 fNormal;
out vec2 fTextCoord;

void main(void) {
  gl_Position = PVM * vec4(posAttr, 1.0);
  // The lighting calculations will be in view space.
  // But this shader is independent, it just matters how the
  // input NormalMat is recieved
  fNormal = vec3(NormalMat * vec4(normalAttr, 0.0));
  fTextCoord = textCoordAttr;
}
