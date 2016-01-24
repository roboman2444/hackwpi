#version 330
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;

in vec2 tc;
uniform bool uniint0;

out vec4 frag1;

void main(){
	vec3 vel;
	vec3 pos = texture(texture0, tc).xyz;
	if(uniint0){
		vel = texture(texture2, tc).xyz;
	} else {
		vel = texture(texture1, tc).xyz;
	}
//	//grab 5 samples for heightmap
	vec3 samp[5];
	samp[0].xy = pos.xy;
	samp[1].xy = pos.xy + vec2(-0.01, 0.0);
	samp[2].xy = pos.xy + vec2(0.0, -0.01);
	samp[3].xy = pos.xy + vec2(0.01, 0.0);
	samp[4].xy = pos.xy + vec2(0.0, 0.01);

	int i;
	for(i = 0; i < 5; i++){
		samp[i].z = texture(texture3, samp[i].xy /10.0 + 0.5 ).r;
	}
	vec3 cavg = vec3(0.0);
	vec3 tang1;
	vec3 tang2;
	tang1 = samp[0] - samp[1];
	tang2 = samp[0] - samp[2];
	cavg += cross(tang1, tang2);
//	tang1 = normalize(samp[0] - samp[2]);
//	tang2 = normalize(samp[0] - samp[3]);
//	cavg += cross(tang1, tang2);
//	tang1 = normalize(samp[0] - samp[3]);
//	tang2 = normalize(samp[0] - samp[4]);
//	cavg += cross(tang1, tang2);
//	tang1 = normalize(samp[0] - samp[4]);
//	tang2 = normalize(samp[0] - samp[1]);
//	cavg += cross(tang1, tang2);
	cavg = normalize(cavg * vec3(1.0, 1.0, 0.1));


float delta = (samp[0].z - pos.z + 2.0);
    vec3 a2b = delta * cavg;
    vec3 u1 = vel;
    vec3 u2 = vec3(0.0);
    float ac1 = dot(u1, a2b);
    float ac2 = dot(u2, a2b);
    
    vec3 after = (u1 + 0.5 * (ac2 - ac1) * a2b) * 0.7;
	if(delta > 0) {
		vel += a2b*2 * vec3(0.2, 0.2, 0.1);
	}


	if(length(pos.xy) > 8.0){
		vel.xy += -pos.xy;
	}
 	
	vel.z -= 0.2;
	frag1.xyz = vel * vec3(0.5, 0.5, 0.5);
}
