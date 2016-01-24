#version 330

in vec3 worldpos;
in vec2 fragtc;
uniform sampler2D texture0;
uniform samplerCube texture1;
uniform mat4 unimat40;
uniform vec3 univec31;

out vec4 fragColor;
void main(){
	vec3 tang1 = normalize(vec3(dFdx(worldpos.x), dFdx(worldpos.y), dFdx(worldpos.z)));
	vec3 tang2 = normalize(vec3(dFdy(worldpos.x), dFdy(worldpos.y), dFdy(worldpos.z)));
	vec3 crosh = cross(tang1, tang2);

	vec3 viewnorm = normalize(worldpos - univec31);
	vec3 bounce = reflect(viewnorm, crosh);
	fragColor = textureLod(texture1, bounce, 3.5);	
	
//	fragColor = vec4(crosh, 1.0);
//	fragColor = vec4(fragtc, 1, 0);
//	fragColor.rg = fragtc;
//	fragColor.ba = vec2(0.0);
}
