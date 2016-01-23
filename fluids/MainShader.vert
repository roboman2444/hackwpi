#version 330

in vec3 posattrib;
out vec2 pos;
out vec3 Color;

void main() {
	Color = vec3(1,0,0);
	pos = (vec2(posattrib) + vec2(1,1)) / 2.0;//vec2(position);//
	gl_Position = vec4(posattrib, 1.0);
}
