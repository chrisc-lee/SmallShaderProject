#version 410
uniform sampler2D tex;
in vec2 fragUV;

out vec4 outCol;

// set alpha
uniform float alpha = 1.0;

void main()
{
	outCol = texture(tex, fragUV);
	outCol.a = min(outCol.a, alpha);
}