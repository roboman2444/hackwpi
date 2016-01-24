#version 330
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;
in VS_OUT {
	flat vec4 pos;
} gs_in[];

uniform sampler2D texture0;

void main(){
	vec4 mypos;
	mypos = gs_in[0].pos;

        gl_Position = mypos + vec4(-0.01, -0.01, 0.0, 0.0);
        EmitVertex(); //bottom left

        gl_Position = mypos + vec4(0.01, -0.01, 0.0, 0.0);
        EmitVertex(); //bottom right



        gl_Position = mypos + vec4(-0.01, 0.01, 0.0, 0.0);
        EmitVertex();// top left


        gl_Position = mypos + vec4(0.01, 0.01, 0.0, 0.0);
        EmitVertex(); //top right
        EndPrimitive();
}
