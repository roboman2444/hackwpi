#version 330
in vec2 fragpos;
in vec3 worldpos;
flat in vec3 norm;
uniform sampler2D texture0;
uniform samplerCube texture1;
uniform mat4 unimat40;
uniform vec3 univec31;
out vec4 fragColor;
void main(){
	fragColor = vec4(0.0);
	vec3 viewnorm = normalize(worldpos - univec31);
	vec3 bounce = reflect(viewnorm, norm);
	fragColor = texture(texture1, bounce);

//	fragColor.r = texture(texture0, worldpos.rg).r;
	//fragColor.rg = worldpos.rg;
}
