#version 330
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;

in vec2 tc;
uniform bool uniint0;

out vec4 frag1;
//out vec4 frag2;

void main(){
	vec3 vel;
	vec3 pos = texture(texture0, tc).xyz;
	if(uniint0){
		vel = texture(texture2, tc).xyz;
	} else {
		vel = texture(texture1, tc).xyz;
	}
//	//grab 9 samples for heightmap
	vec3 samp[9];
	samp[0].xy = pos.xy;
	samp[1].xy = pos.xy + vec2(-0.1, 0.0);
	samp[2].xy = pos.xy + vec2(0.0, -0.1);
	samp[3].xy = pos.xy + vec2(0.1, 0.0);
	samp[4].xy = pos.xy + vec2(0.0, 0.1);

	samp[5].xy = pos.xy + vec2(-0.1, -0.1);
	samp[6].xy = pos.xy + vec2(0.1, -0.1);
	samp[7].xy = pos.xy + vec2(0.1, 0.1);
	samp[8].xy = pos.xy + vec2(-0.1, 0.1);

	int i;
	for(i = 0; i < 9; i++){
		samp[i].z = texture(texture3, samp[i].xy /10.0 + 0.5 ).r;
	}
	vec3 cavg = vec3(0.0);
	vec3 tang1;
	vec3 tang2;
	for(i = 1; i < 7; i++){
		tang1 = samp[0] - samp[i];
		tang2 = samp[0] - samp[i+1];
		cavg += cross(tang1, tang2);
	}
	tang1 = (samp[0] - samp[8]);
	tang2 = (samp[0] - samp[1]);
	cavg += cross(tang1, tang2);
	cavg = normalize(cavg);

//	vel.xy += cavg.xy * 0.2;



	if(abs(pos.x) > 5.0) vel.x = -pos.x;
	if(abs(pos.y) > 5.0) vel.y = -pos.y;
	if(uniint0) frag1.xyz = vel * vec3(0.9, 0.9, 0.0);
	else  frag1.xyz = vel * vec3(0.9, 0.9, 0.0);
}
