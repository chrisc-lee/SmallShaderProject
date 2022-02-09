#version 410

layout (location = 0) in vec3 pos;
layout (location = 3) in vec2 uv;

uniform vec2 size;
uniform vec2 offset;
uniform mat4 transform; 
uniform bool invertX;

out vec2 fragUV;

void main()
{
 gl_Position = transform * vec4(pos, 1.0);
 
 // move verticies on screen
 // gl_Position = vec4(pos + translation, 1.0);

 // invert texture, multiply by size of a single sprite on 
 // the sheet and then offset by the size of the sprite to 
 // get next sprite on sheet and invertX if necessary
 // branching is not necessarily good for shader optimization but in this case this branch is uniform
 // and so it will run the same for all vertices in a single passthrough so it is not going to cause
 // a lot of performance issues  
 if (invertX){
	fragUV = vec2(1.0 - uv.x, 1.0-uv.y) * size + (offset*size);
 }
 else{
 	fragUV = vec2(uv.x, 1.0-uv.y) * size + (offset*size);
 }
 
}
