#version 330

in vec3 fragtc;
uniform samplerCube texture0;
uniform mat4 unimat40;

out vec4 fragColor;
void main(){
//	fragColor = texture(texture0, fragtc);// * vec4(0.0, 0.0, 1.0, 1.0);
	fragColor = textureLod(texture0, fragtc, 2.5);// * vec4(0.0, 0.0, 1.0, 1.0);
	fragColor += textureLod(texture0, fragtc, 3.5);// * vec4(0.0, 0.0, 1.0, 1.0);
	fragColor += textureLod(texture0, fragtc, 4.5);// * vec4(0.0, 0.0, 1.0, 1.0);
	fragColor += textureLod(texture0, fragtc, 5.5);// * vec4(0.0, 0.0, 1.0, 1.0);
	fragColor += textureLod(texture0, fragtc, 6.5);// * vec4(0.0, 0.0, 1.0, 1.0);
	fragColor *= 0.2;
//	fragColor.rgb = abs(fragtc);
//	fragColor.ba = vec2(0.0);
}
