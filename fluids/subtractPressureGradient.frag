#version 330

in vec2 pos;
uniform sampler2D texture0; // pressure 0
uniform sampler2D texture1; // velocity 0
layout(location=1) out vec4 velocity1;

void main(){

	float px = 1.0 / 640;
	float py = 1.0 / 480;
	
    float x0 = texture(texture0, pos-vec2(px, 0)).r;
    float x1 = texture(texture0, pos+vec2(px, 0)).r;
    float y0 = texture(texture0, pos-vec2(0, py)).r;
    float y1 = texture(texture0, pos+vec2(0, py)).r;
    vec2 v = texture(texture1, pos).xy;
    velocity1 = vec4((v-(vec2(x1, y1)-vec2(x0, y0))*0.5), 0.0, 1.0);
}