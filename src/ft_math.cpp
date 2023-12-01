#include "ft_math.hpp"
using namespace ftmath;

//  class vec4
//  ----------
vec4::vec4(){
    _x = 0.0f;
    _y = 0.0f;
    _z = 0.0f;
    _w = 0.0f;
};
vec4::vec4(vec3 v3, float w){
    _x = v3._x;
    _y = v3._y;
    _z = v3._z;
    _w = w;  
}
vec4::vec4(float x, float y, float z, float w){
    _x = x;
    _y = y;
    _z = z;
    _w = w;
}
vec4 vec4::operator+(vec4 &other){
    return vec4(other._x + _x, other._y + _y, other._z + _z, other._w + _w);
};
vec4& vec4::operator=(const vec4 &other){
    _x = other._x;
    _y = other._y;
    _z = other._z;
    _w = other._w;
    return *this;
};

//  class vec3
//  ----------
vec3::vec3(){
    _x = 0.0f;
    _y = 0.0f;
    _z = 0.0f;
};
vec3::vec3(float x, float y, float z){
    _x = x;
    _y = y;
    _z = z;
}
vec3 vec3::operator+(vec3 &other){
    return vec3(other._x + _x, other._y + _y, other._z + _z);
};
vec3& vec3::operator=(const vec3 &other){
    _x = other._x;
    _y = other._y;
    _z = other._z;
    return *this;
};

//  class vec2
//  ----------
vec2::vec2(){
    _x = 0.0f;
    _y = 0.0f;
};
vec2::vec2(float x, float y){
    _x = x;
    _y = y;
}
vec2 vec2::operator+(vec2 &other){
    return vec2(other._x + _x, other._y + _y);
};
vec2& vec2::operator=(const vec2 &other){
    _x = other._x;
    _y = other._y;
    return *this;
};

//  class m4x4
//  ----------
m4x4::m4x4(){
    _m0 = vec4();
    _m1 = vec4();
    _m2 = vec4();
    _m3 = vec4();
};
m4x4::m4x4(float identity){
    vec4 m0(identity, 0.0f, 0.0f, 0.0f);
    vec4 m1(0.0f, identity, 0.0f, 0.0f);
    vec4 m2(0.0f, 0.0f, identity, 0.0f);
    vec4 m3(0.0f, 0.0f, 0.0f, identity);
    _m0 = m0;
    _m1 = m1;
    _m2 = m2;
    _m3 = m3;
}
m4x4::m4x4(vec4 m0, vec4 m1, vec4 m2, vec4 m3){
    _m0 = m0;
    _m1 = m1;
    _m2 = m2;
    _m3 = m3;
}

float* m4x4::toglsl(){
    glsl[0] = _m0._x; glsl[1] = _m0._y; glsl[2] = _m0._z; glsl[3] = _m0._w;
    glsl[4] = _m1._x; glsl[5] = _m1._y; glsl[6] = _m1._z; glsl[7] = _m1._w;
    glsl[8] = _m2._x; glsl[9] = _m2._y; glsl[10] = _m2._z; glsl[11] = _m2._w;
    glsl[12] = _m3._x; glsl[13] = _m3._y; glsl[14] = _m3._z; glsl[15] = _m3._w;
    return glsl;
}
m4x4& m4x4::operator=(const m4x4 &other){
    _m0 = other._m0;
    _m1 = other._m1;
    _m2 = other._m2;
    _m3 = other._m3;
    return *this;
}
m4x4 m4x4::operator*(m4x4 &other){
    m4x4 retm4x4;

    // nb1 horizontal line of matrix
    retm4x4._m0._x = _m0._x * other._m0._x + _m1._x * other._m0._y + _m2._x * other._m0._z + _m3._x * other._m0._w;
    retm4x4._m1._x = _m0._x * other._m1._x + _m1._x * other._m1._y + _m2._x * other._m1._z + _m3._x * other._m1._w;
    retm4x4._m2._x = _m0._x * other._m2._x + _m1._x * other._m2._y + _m2._x * other._m2._z + _m3._x * other._m2._w;
    retm4x4._m3._x = _m0._x * other._m3._x + _m1._x * other._m3._y + _m2._x * other._m3._z + _m3._x * other._m3._w;

    // nb2 horizontal line of matrix
    retm4x4._m0._y = _m0._y * other._m0._x + _m1._y * other._m0._y + _m2._y * other._m0._z + _m3._y * other._m0._w;
    retm4x4._m1._y = _m0._y * other._m1._x + _m1._y * other._m1._y + _m2._y * other._m1._z + _m3._y * other._m1._w;
    retm4x4._m2._y = _m0._y * other._m2._x + _m1._y * other._m2._y + _m2._y * other._m2._z + _m3._y * other._m2._w;
    retm4x4._m3._y = _m0._y * other._m3._x + _m1._y * other._m3._y + _m2._y * other._m3._z + _m3._y * other._m3._w;

    // nb3 horizontal line of matrix
    retm4x4._m0._z = _m0._z * other._m0._x + _m1._z * other._m0._y + _m2._z * other._m0._z + _m3._z * other._m0._w;
    retm4x4._m1._z = _m0._z * other._m1._x + _m1._z * other._m1._y + _m2._z * other._m1._z + _m3._z * other._m1._w;
    retm4x4._m2._z = _m0._z * other._m2._x + _m1._z * other._m2._y + _m2._z * other._m2._z + _m3._z * other._m2._w;
    retm4x4._m3._z = _m0._z * other._m3._x + _m1._z * other._m3._y + _m2._z * other._m3._z + _m3._z * other._m3._w;

    // nb4 horizontal line of matrix
    retm4x4._m0._w = _m0._w * other._m0._x + _m1._w * other._m0._y + _m2._w * other._m0._z + _m3._w * other._m0._w;
    retm4x4._m1._w = _m0._w * other._m1._x + _m1._w * other._m1._y + _m2._w * other._m1._z + _m3._w * other._m1._w;
    retm4x4._m2._w = _m0._w * other._m2._x + _m1._w * other._m2._y + _m2._w * other._m2._z + _m3._w * other._m2._w;
    retm4x4._m3._w = _m0._w * other._m3._x + _m1._w * other._m3._y + _m2._w * other._m3._z + _m3._w * other._m3._w;

    return retm4x4;
}

vec4 m4x4::operator*(vec4 &other){
    vec4 v;
    v._x = _m0._x * other._x + _m1._x * other._y + _m2._x * other._z + _m3._x * other._w;
    v._y = _m0._y * other._x + _m1._y * other._y + _m2._y * other._z + _m3._y * other._w;
    v._z = _m0._z * other._x + _m1._z * other._y + _m2._z * other._z + _m3._z * other._w;
    v._w = _m0._w * other._x + _m1._w * other._y + _m2._w * other._z + _m3._w * other._w;
    return v;
}

//  Stream for ftmath class
//  -----------------------
std::ostream& operator<<(std::ostream& os, const ftmath::vec4& v){
    os << '[' << v._x << ", " << v._y << ", " << v._z << ", " << v._w << "]";
    return os;
};
std::ostream& operator<<(std::ostream& os, const vec3& v){
    os << '[' << v._x << ", " << v._y << ", " << v._z << "]";
    return os;
};
std::ostream& operator<<(std::ostream& os, const ftmath::m4x4& m){
    // nb1 horizontal line of matrix
    os << '[' << m._m0._x << ", " << m._m1._x << ", " << m._m2._x << ", " << m._m3._x << "]\n";
    // nb2 horizontal line of matrix
    os << '[' << m._m0._y << ", " << m._m1._y << ", " << m._m2._y << ", " << m._m3._y << "]\n";
    // nb3 horizontal line of matrix
    os << '[' << m._m0._z << ", " << m._m1._z << ", " << m._m2._z << ", " << m._m3._z << "]\n";
    // nb4 horizontal line of matrix
    os << '[' << m._m0._w << ", " << m._m1._w << ", " << m._m2._w << ", " << m._m3._w << "]\n";
    return os;
}

//  Function Translate, scale and rotate
//  ------------------------------------
//  Erreur de calcule translatem4 !
//  ------------------------------------
m4x4 ftmath::translatem4(m4x4 m, vec3 v){   
    m4x4 retm(1.0f);
    retm._m3._x = v._x;
    retm._m3._y = v._y;
    retm._m3._z = v._z;
    return m * retm;
}
//  ------------------------------------
//  Erreur de calcule scalem4 !
//  ------------------------------------
m4x4 ftmath:: scalem4(m4x4 m, vec3 v){
    m4x4 retm(1.0f);
    retm._m0._x *= v._x;
    retm._m1._y *= v._y;
    retm._m2._z *= v._z;
    return m * retm;  
}
m4x4 ftmath::rotatexm4(m4x4 m, float angle){
    m4x4 retm(1.0f);
    float radian = angle * (M_PI / 180);
    float c = cos(radian);
    float s = sin(radian);

    vec4 tmpm1( c * retm._m1._x - s * retm._m2._x, 
                c * retm._m1._y - s * retm._m2._y,
                c * retm._m1._z - s * retm._m2._z,
                c * retm._m1._w - s * retm._m2._w);

    vec4 tmpm2( s * retm._m1._x + c * retm._m2._x,
                s * retm._m1._y + c * retm._m2._y,
                s * retm._m1._z + c * retm._m2._z,
                s * retm._m1._w + c * retm._m2._w);

    retm._m1 = tmpm1;
    retm._m2 = tmpm2;
    return m * retm;
};
m4x4 ftmath::rotateym4(m4x4 m, float angle){
    m4x4 retm(1.0f);
    float radian = angle * (M_PI / 180);
    float c = cos(radian);
    float s = sin(radian);

    vec4 tmpm0( c * retm._m0._x + s * retm._m2._x, 
                c * retm._m0._y + s * retm._m2._y,
                c * retm._m0._z + s * retm._m2._z,
                c * retm._m0._w + s * retm._m2._w);

    vec4 tmpm2( -s * retm._m0._x + c * retm._m2._x,
                -s * retm._m0._y + c * retm._m2._y,
                -s * retm._m0._z + c * retm._m2._z,
                -s * retm._m0._w + c * retm._m2._w);

    retm._m0 = tmpm0;
    retm._m2 = tmpm2;
    return m * retm;
};
m4x4 ftmath::rotatezm4(m4x4 m, float angle){
    m4x4 retm(1.0f);
    float radian = angle * (M_PI / 180);
    float c = cos(radian);
    float s = sin(radian);

    vec4 tmpm0( c * retm._m0._x - s * retm._m1._x, 
                c * retm._m0._y - s * retm._m1._y,
                c * retm._m0._z - s * retm._m1._z,
                c * retm._m0._w - s * retm._m1._w);

    vec4 tmpm1( s * retm._m0._x + c * retm._m1._x,
                s * retm._m0._y + c * retm._m1._y,
                s * retm._m0._z + c * retm._m1._z,
                s * retm._m0._w + c * retm._m1._w);

    retm._m0 = tmpm0;
    retm._m1 = tmpm1;
    return m * retm;
};

//  Projection matrix
//  -----------------
m4x4 ftmath::persp(float fov, float ratio, float near, float far){
    const float zRange = near - far;
    const float tanHalfFOV = tanf((fov / 2.0f) * (180.0f/(float)M_PI));
    m4x4 matrix;
    matrix._m0._x = 1.0f / (tanHalfFOV * ratio);
    matrix._m1._y = 1.0f / tanHalfFOV;
    matrix._m2._z = (-near - far) / zRange;
    matrix._m2._w = 1.0f;
    matrix._m3._z = (2.0f * (far * near)) / zRange;
    return matrix;
}