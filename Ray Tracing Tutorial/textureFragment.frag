#version 430

in vec2 pass_textureCoords;

out vec4 out_color;

uniform sampler2D out_image;

void main() {

	out_color = texture(out_image, pass_textureCoords);

}