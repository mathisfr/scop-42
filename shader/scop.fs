#version 330 core
out vec4 FragColor;

uniform sampler2D texture0;
uniform float mixColor;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform float lightStrength;
uniform float ambientStrength;

in vec2 TexCoord;
in vec3 Color;
in vec3 Normal;

in vec3 FragPos;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 ambient = ambientStrength * lightColor;
    vec3 lightResult = lightStrength * (ambient + diffuse);

    vec4 InterColor = vec4(Color, 1.0);
    vec4 textureColor = texture2D(texture0, TexCoord) * vec4(lightResult, 1.0);
    FragColor = mix(textureColor, InterColor, mixColor);
}