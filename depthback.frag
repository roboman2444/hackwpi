#version 330
in vec2 fragpos;
in vec3 worldpos;
uniform sampler2D texture0;
uniform mat4 unimat40;
out vec4 fragColor;
void main(){
	fragColor = vec4(0.0);
	fragColor.r = texture(texture0, worldpos.rg).r;
	//fragColor.rg = worldpos.rg;
}
