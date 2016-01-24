#version 330
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

in vec2 tc;
uniform bool uniint0;

out vec4 fragColor;

void main(){
	vec3 vel = vec3(0.0);
	if(uniint0){
		vel = texture(texture2, tc).rgb;
	} else {
		vel = texture(texture1, tc).rgb;
	}
	fragColor.rgb = vel;
	fragColor.a = 0.0;
}
