#version 330

in vec3 fragtc;
uniform samplerCube texture0;
uniform mat4 unimat40;

out vec4 fragColor;
void main(){
	fragColor = texture(texture0, fragtc);// * vec4(0.0, 0.0, 1.0, 1.0);
	fragColor.rgb = abs(fragtc);
//	fragColor.ba = vec2(0.0);
}
