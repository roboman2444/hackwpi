#version 330

uniform sampler2D texture1; // velocity0
uniform sampler2D texture2; // pressure0
uniform sampler2D texture0; // Texture sampler

out vec4 Color;
in vec2 pos;

void main()
{
//for white color
	vec4 temp = texture(texture0, pos);
	Color = abs(vec4(vec3(temp.x + temp.y / 2.0),1.0));

	Color = vec4((texture(texture2, pos)).x,(texture(texture1, pos)*1.5+0.5).xy,1.0);

//For viewing divergence
//	float div = texture(texturesampler,pos).x;
//	Color = vec4(div,div,div,1.0);

//For normal viewing
	//Color = abs(texture(texture0,pos));
}