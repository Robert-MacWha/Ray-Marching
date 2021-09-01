#version 430

in vec3 vertexPosition;

out vec3 colour;

void main() {

  gl_Position = vec4(vertexPosition, 1.0);
  colour = vec3((vertexPosition.xy + 0.5) / 2, 1);

}