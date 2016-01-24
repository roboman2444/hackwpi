#version 330

in vec2 fragpos;
in vec2 fragtc;
uniform sampler2D texture0;
uniform mat4 unimat40;

out vec4 fragColor;
void main(){
	fragColor = vec4(fragtc, 1, 0);
//	fragColor.rg = fragtc;
//	fragColor.ba = vec2(0.0);
}
