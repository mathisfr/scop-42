#ifndef FT_LIGHT_HPP
#define FT_LIGHT_HPP
#include "ft_math.hpp"
class ft_light
{
private:
    float color[3];
    float pos[3];
    float lightStrength;
    float specularStrength;
    float ambientStrength;
    int specularShininess;
    bool _lightError;
    void colorCorrection();
    void posCorrection();
public:
    ft_light(float r, float g, float b);
    ~ft_light();

    void setAmbientStrength(float strength);
    void setLightStrength(float strength);
    void setSpecularStrength(float strength);
    void setColor(float r, float g, float b);
    void setPos(float x, float y, float z);
    void setSpecularShininess(int shininess);

    float &getAmbientStrength();
    float &getLightStrength();
    float &getSpecularStrength();
    float *getColor();
    float *getPos();
    int &getSpecularShininess();

    void reset();
};

#endif