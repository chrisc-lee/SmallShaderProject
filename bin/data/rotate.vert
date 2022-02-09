#version 410

layout (location = 0) in vec3 pos;
layout (location = 3) in vec2 uv;

out vec2 fragUV;

void main()
{
 float radians = 1.0f;
 vec3 rotatedPos = pos;
 rotatedPos.x = (cos(radians)*pos.x)-(sin(radians)*pos.y);
 rotatedPos.y = (cos(radians)*pos.y)+(sin(radians)*pos.x);
 gl_Position = vec4( rotatedPos, 1.0);
 fragUV = vec2(uv.x, 1.0-uv.y);
}
