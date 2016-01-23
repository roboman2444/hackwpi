#version 150

in vec2 fragposition;
in vec2 fragtexcoord;
uniform sampler2D texture0;
uniform sampler2D texture1;

out vec4 frag1;

void main(){
//	frag1 = lensflare;
	frag1 = texture(texture0, fragtexcoord) + texture(texture1, fragtexcoord) * 0.2;
}
