#version 330 core
out vec4 FragColor;

uniform sampler2D texture0;
uniform float mixColor;
uniform vec3 lightPos;
uniform vec3 lightColor;

in vec2 TexCoord;
in vec3 Color;
in vec3 Normal;

in vec3 FragPos;

void main()
{
    vec4 InterColor = vec4(Color, 1.0);
    vec4 textureColor = texture2D(texture0, TexCoord);
    FragColor = mix(textureColor, InterColor, mixColor);
}