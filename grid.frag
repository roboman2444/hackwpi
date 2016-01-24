#version 330

in vec3 worldpos;
in vec2 fragtc;
uniform sampler2D texture0;
uniform samplerCube texture1;
uniform mat4 unimat40;
uniform vec3 univec31;

out vec4 fragColor;
void main(){
	/* //deriv method
	vec3 tang1 = normalize(vec3(dFdx(worldpos.x), dFdx(worldpos.y), dFdx(worldpos.z)));
	vec3 tang2 = normalize(vec3(dFdy(worldpos.x), dFdy(worldpos.y), dFdy(worldpos.z)));
	vec3 crosh = cross(tang1, tang2);
	*/
	//average from texture method
	vec3 pos[8];
	int i;
	for(i = 0; i < 8; i++){
		vec2 trigs = vec2(cos(i*0.45), sin(i*0.45));
		pos[i]= vec3(worldpos.xy + trigs * 0.1, texture(texture0, fragtc + trigs * 0.01).r);
		
	}
	vec3 cavg = vec3(0.0);
	vec3 tang1;
	vec3 tang2;
	tang1 = normalize(worldpos - pos[0]);
	tang2 = normalize(worldpos - pos[1]);
	cavg += cross(tang1, tang2);
	tang1 = normalize(worldpos - pos[1]);
	tang2 = normalize(worldpos - pos[2]);
	cavg += cross(tang1, tang2);
	tang1 = normalize(worldpos - pos[2]);
	tang2 = normalize(worldpos - pos[3]);
	cavg += cross(tang1, tang2);
	tang1 = normalize(worldpos - pos[3]);
	tang2 = normalize(worldpos - pos[4]);
	cavg += cross(tang1, tang2);
	tang1 = normalize(worldpos - pos[4]);
	tang2 = normalize(worldpos - pos[5]);
	cavg += cross(tang1, tang2);
	tang1 = normalize(worldpos - pos[5]);
	tang2 = normalize(worldpos - pos[6]);
	cavg += cross(tang1, tang2);
	tang1 = normalize(worldpos - pos[6]);
	tang2 = normalize(worldpos - pos[7]);
	cavg += cross(tang1, tang2);

	tang1 = normalize(worldpos - pos[7]);
	tang2 = normalize(worldpos - pos[0]);
	cavg += cross(tang1, tang2);



	vec3 crosh = cavg /8.0;

	vec3 viewnorm = normalize(worldpos - univec31);
	float fressy = dot(viewnorm, crosh);
	fressy *= fressy;
	vec3 bounce = reflect(viewnorm, crosh);
	fragColor.rgb = textureLod(texture1, bounce, 3.5).rgb * fressy;
	fragColor.a = fressy;
	
//	fragColor = vec4(crosh, 1.0);
//	fragColor = vec4(fragtc, 1, 0);
//	fragColor.rg = fragtc;
//	fragColor.ba = vec2(0.0);
}
