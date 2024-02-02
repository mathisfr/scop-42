#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal; 
layout (location = 2) in vec2 aTexCoord; 
layout (location = 3) in vec3 aColor; 

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float pointSize;

out vec2 TexCoord;
out vec3 Color;
out vec3 Normal;

void main()
{
    TexCoord = aTexCoord;
    Color = aColor;
    Normal = aNormal;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    gl_PointSize = pointSize * (10.0 / length(gl_Position.xyz));
}