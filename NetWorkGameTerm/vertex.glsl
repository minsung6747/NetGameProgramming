#version 330 core
layout (location = 0) in vec3 vPos; 
layout (location = 1) in vec3 vColor;

out vec3 outColor;

uniform mat4 transform;
uniform mat4 projection;
uniform mat4 view;

void main()
{

   gl_Position =  projection * view * transform * vec4 (vPos, 1.0);
   outColor = vColor;

}