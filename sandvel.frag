#version 330
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

in vec2 tc;
uniform bool uniint0;

out vec4 frag1;
out vec4 frag2;

void main(){
	vec3 vel;
	if(uniint0){
		frag1.xyz = texture(texture2, tc).xyz;
//		frag1.xyz = vec3(0.0);
		frag1.y += -0.01;
	} else {
		frag2.xyz = texture(texture1, tc).xyz;
//		frag2.xyz = vec3(0.0);
		frag2.y += -0.01;
	}
}
