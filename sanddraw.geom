#version 330
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;
in VS_OUT {
	flat vec4 pos;
} gs_in[];

uniform sampler2D texture0;
uniform vec2 univec20;
out vec2 tc;
void main(){
	vec4 mypos;
	vec4 texc;
	mypos = gs_in[0].pos;
	texc = vec4(-0.01 * univec20.x, -0.01 * univec20.y, 0.0, 0.0);
	tc = vec2(-1.0, -1.0);
        gl_Position = mypos + texc;
        EmitVertex(); //bottom left

        texc = vec4(0.01 * univec20.x, -0.01 * univec20.y, 0.0, 0.0);
	tc = vec2(1.0, -1.0);
        gl_Position = mypos + texc;
        EmitVertex(); //bottom right



        texc = vec4(-0.01 * univec20.x, 0.01 * univec20.y, 0.0, 0.0);
	tc = vec2(-1.0, 1.0);
        gl_Position = mypos + texc;
        EmitVertex();// top left


	texc = vec4(0.01 * univec20.x, 0.01 * univec20.y, 0.0, 0.0);
	tc = vec2(1.0, 1.0);
        gl_Position = mypos + texc;
        EmitVertex(); //top right
        EndPrimitive();
}
