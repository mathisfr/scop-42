#version 330 core
out vec4 FragColor;

uniform sampler2D texture0;
uniform float mixColor;

uniform vec3 lightColor;
uniform float ambientStrength;

in vec2 TexCoord;
in vec3 Color;
in vec3 Normal;

void main()
{
    vec3 ambient = ambientStrength * lightColor;
    vec4 InterColor = vec4(Color, 1.0);
    vec4 textureColor = texture2D(texture0, TexCoord) * vec4(ambient, 1.0);
    FragColor = mix(textureColor, InterColor, mixColor);
}