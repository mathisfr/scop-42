#ifndef FT_LIGHT_HPP
#define FT_LIGHT_HPP
#include "ft_math.hpp"
class ft_light
{
private:
    bool _lightError;
    void colorCorrection();
public:
    float color[3];
    float ambientStrength;
    ft_light(float r, float g, float b);
    ~ft_light();

    void setAmbientStrength(float strength);
    void setColor(float r, float g, float b);
};

#endif