#include "ft_light.hpp"

void ft_light::colorCorrection(){
    if (this->color[0] > 1.0f) this->color[0] = 1.0f;
    if (this->color[1] > 1.0f) this->color[1] = 1.0f;
    if (this->color[2] > 1.0f) this->color[2] = 1.0f;
    if (this->color[0] < 0.0f) this->color[0] = 0.0f;
    if (this->color[1] < 0.0f) this->color[1] = 0.0f;
    if (this->color[2] < 0.0f) this->color[2] = 0.0f;
}

void ft_light::posCorrection(){
    if (this->pos[0] > 50.0f) this->pos[0] = 50.0f;
    if (this->pos[0] < -50.0f) this->pos[0] = -50.0f;
    if (this->pos[1] > 50.0f) this->pos[1] = 50.0f;
    if (this->pos[1] < -50.0f) this->pos[1] = -50.0f;
    if (this->pos[2] > 50.0f) this->pos[2] = 50.0f;
    if (this->pos[2] < -50.0f) this->pos[2] = -50.0f;
}

void ft_light::reset(){
    this->color[0] = this->color[1] = this->color[2] = 1.0f;
    this->pos[0] = this->pos[2] = 1.0f;
    this->pos[1] = 0.325f;
    this->ambientStrength = 0.6f;
    this->lightStrength = 1.0f;
}

ft_light::ft_light(float r, float g, float b)
{
    this->color[0] = r;
    this->color[1] = g;
    this->color[2] = b;
    this->pos[0] = this->pos[2] = 1.0f;
    this->pos[1] = 0.325f;
    this->lightStrength = 1.0f;
    this->ambientStrength = 0.6f;
    colorCorrection();
}


void ft_light::setAmbientStrength(float strength){
    this->ambientStrength = strength;
    if (this->ambientStrength > 1.0f) this->ambientStrength = 1.0f;
    if (this->ambientStrength < 0.0f) this->ambientStrength = 0.0f;
}

void ft_light::setSpecularStrength(float strength){
    this->specularStrength = strength;
    if (this->specularStrength > 1.0f) this->specularStrength = 1.0f;
    if (this->specularStrength < 0.0f) this->specularStrength = 0.0f;
}

void ft_light::setLightStrength(float strength){
    this->lightStrength = strength;
    if (this->lightStrength > 1.0f) this->lightStrength = 1.0f;
    if (this->lightStrength < 0.0f) this->lightStrength = 0.0f;
}

void ft_light::setColor(float r, float g, float b){
    this->color[0] = r;
    this->color[1] = g;
    this->color[2] = b;
    colorCorrection();
}

void ft_light::setPos(float x, float y, float z){
    this->pos[0] = x;
    this->pos[1] = y;
    this->pos[2] = z;
    posCorrection();
}
void ft_light::setSpecularShininess(int shininess){
    this->specularShininess = shininess;
    if (this->specularStrength > 256) this->specularStrength = 256;
    if (this->specularStrength < 0) this->specularStrength = 0;
}

ft_light::~ft_light()
{
}

float &ft_light::getAmbientStrength(){
    return this->ambientStrength;
}
float &ft_light::getLightStrength(){
    return this->lightStrength;
}
float &ft_light::getSpecularStrength(){
    return this->specularStrength;
}
float* ft_light::getColor(){
    return this->color;
}
float* ft_light::getPos(){
    return this->pos;
}
int &ft_light::getSpecularShininess(){
    return this->specularShininess;
}