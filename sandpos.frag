#version 330
in sampler2D texture0;
in sampler2D texture1;
in sampler2D texture2;

in vec2 tc;
uniform int uniint0;

out vec4 fragColor;

void main(){
	vec3 vel;
	if(uniint0){
		vel = texture(texture2, tc);
	} else {
		vel = texture(texture1, tc);
	}
	fragColor.rgb = vel;
	fragColor.a = 0.0;
}
