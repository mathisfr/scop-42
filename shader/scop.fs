#version 330 core
out vec4 FragColor;

uniform sampler2D texture0;
uniform float mixColor;

in vec2 TexCoord;
in vec3 Color;

void main()
{
    vec4 InterColor = vec4(Color, 1.0);
    vec4 textureColor = texture2D(texture0, TexCoord);
    FragColor = mix(textureColor, InterColor, mixColor);
}