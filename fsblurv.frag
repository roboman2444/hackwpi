#version 150

in vec2 fragposition;
in vec2 fragtexcoord;
uniform sampler2D texture0;
in vec2 blurTexCoords[14];

out vec4 frag1;

void main(){
//		frag1 = vec4(1.0, 1.0, 1.0, texture(texture0, fragtexcoord).a);
//		frag1 = texture(texture0, fragtexcoord);
//		frag1.b = 0.0;
//		frag1 = vec4(1.0, 0.5, 0.5, 1.0);
//		frag1 = vec4(fragposition, 1.0, 1.0);
//		frag1 = vec4(1.0, 0.0, 0.0, 1.0);


    frag1 = texture(texture0, blurTexCoords[ 0])*0.0044299121055113265;
    frag1 += texture(texture0, blurTexCoords[ 1])*0.00895781211794;
    frag1 += texture(texture0, blurTexCoords[ 2])*0.0215963866053;
    frag1 += texture(texture0, blurTexCoords[ 3])*0.0443683338718;
    frag1 += texture(texture0, blurTexCoords[ 4])*0.0776744219933;
    frag1 += texture(texture0, blurTexCoords[ 5])*0.115876621105;
    frag1 += texture(texture0, blurTexCoords[ 6])*0.147308056121;
    frag1 += texture(texture0, fragtexcoord         )*0.159576912161;
    frag1 += texture(texture0, blurTexCoords[ 7])*0.147308056121;
    frag1 += texture(texture0, blurTexCoords[ 8])*0.115876621105;
    frag1 += texture(texture0, blurTexCoords[ 9])*0.0776744219933;
    frag1 += texture(texture0, blurTexCoords[10])*0.0443683338718;
    frag1 += texture(texture0, blurTexCoords[11])*0.0215963866053;
    frag1 += texture(texture0, blurTexCoords[12])*0.00895781211794;
    frag1 += texture(texture0, blurTexCoords[13])*0.0044299121055113265;

}
