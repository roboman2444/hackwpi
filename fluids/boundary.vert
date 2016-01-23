#version 330

in vec3 posattrib;
in vec2 offsetattrib;
out vec2 pos;
out vec2 OFFSET;

void main() {
	OFFSET = offsetattrib/vec2(640,480);
	pos = (vec2(posattrib) + vec2(1,1)) / 2.0;
	gl_Position = vec4(posattrib, 1.0);
}
