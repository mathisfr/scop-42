#include "ft_light.hpp"

void ft_light::colorCorrection(){
    if (this->color[0] > 1.0f) this->color[0] = 1.0f;
    if (this->color[1] > 1.0f) this->color[1] = 1.0f;
    if (this->color[2] > 1.0f) this->color[2] = 1.0f;
    if (this->color[0] < 0.0f) this->color[0] = 0.0f;
    if (this->color[1] < 0.0f) this->color[1] = 0.0f;
    if (this->color[2] < 0.0f) this->color[2] = 0.0f;
}

ft_light::ft_light(float r, float g, float b)
{
    this->color[0] = r;
    this->color[1] = g;
    this->color[2] = b;
    colorCorrection();
}


void ft_light::setAmbientStrength(float strength){
    this->ambientStrength = strength;
    if (this->ambientStrength > 1.0f) this->ambientStrength = 1.0f;
    if (this->ambientStrength < 0.0f) this->ambientStrength = 0.0f;
}
void ft_light::setColor(float r, float g, float b){
    this->color[0] = r;
    this->color[1] = g;
    this->color[2] = b;
    colorCorrection();
}

ft_light::~ft_light()
{
}