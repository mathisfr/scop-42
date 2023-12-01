#ifndef FT_MATH_HPP
#define FT_MATH_HPP
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
namespace ftmath{
    //  class vec2
    //  ----------
    class vec2{
        public:
            float _x;
            float _y;
            vec2(void);
            vec2(float x, float y);
            vec2 operator+(vec2 &other);
            vec2& operator=(const vec2 &other);
    };

    //  class vec3
    //  ----------
    class vec3{
        public:
            float _x;
            float _y;
            float _z;
            vec3(void);
            vec3(float x, float y, float z);
            vec3 operator+(vec3 &other);
            vec3& operator=(const vec3 &other);
    };

    //  class vec4
    //  ----------
    class vec4{
        public:
            float _x;
            float _y;
            float _z;
            float _w;
            vec4(void);
            vec4(vec3 v3, float w);
            vec4(float x, float y, float z, float w);
            vec4 operator+(vec4 &other);
            vec4& operator=(const vec4 &other);
    };

    //  class m4x4
    //  ----------
    class m4x4{
        public:
            vec4 _m0;
            vec4 _m1;
            vec4 _m2;
            vec4 _m3;
            float glsl[16];
            m4x4(void);
            m4x4(float identity);
            m4x4(vec4 m0, vec4 m1, vec4 m2, vec4 m3);
            m4x4 operator*(m4x4 &other);
            vec4 operator*(vec4 &other);
            m4x4& operator=(const m4x4 &other);
            float* toglsl(void);
    };

    //  Function Translate, scale and rotate
    //  ------------------------------------
    m4x4 translatem4(m4x4 m, vec3 v);
    m4x4 scalem4(m4x4 m, vec3 v);
    m4x4 rotatexm4(m4x4 m, float angle);
    m4x4 rotateym4(m4x4 m, float angle);
    m4x4 rotatezm4(m4x4 m, float angle);

    //  Projection matrix
    //  -----------------
    m4x4 persp(float fov, float ratio, float near, float far);
};

//  Stream for ftmath class
//  -----------------------
std::ostream& operator<<(std::ostream& os, const ftmath::vec4& v);
std::ostream& operator<<(std::ostream& os, const ftmath::vec3& v);
std::ostream& operator<<(std::ostream& os, const ftmath::m4x4& m);
#endif