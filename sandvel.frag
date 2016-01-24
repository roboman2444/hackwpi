#version 330
in sampler2D texture0;
in sampler2D texture1;
in sampler2D texture2;

in vec2 tc;
uniform int uniint0;

out vec4 frag1;
out vec4 frag2;

void main(){
	vec3 vel;
	if(uniint0){
		frag1 = texture(texture2, tc).rgb;
		frag1.y = -0.01;
	} else {
		frag2 = texture(texture1, tc).rgb;
		frag2.y = -0.01;
	}
}
