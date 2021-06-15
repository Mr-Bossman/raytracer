#include <cmath>
#include <cassert>
#include <iostream>

// need to seperate 
#ifndef __VECT_H__
#define __VECT_H__
template <size_t DIM> class vec {
    public:
          double& operator[](const size_t i)       { assert(i<DIM); return data[i]; }
    const double& operator[](const size_t i) const { assert(i<DIM); return data[i]; }
    double data[DIM] = {};
};
typedef vec<4> vec4;

class vec3 {
    public:
    union {
        vec<3> xyz;
        struct 
        {
            double x;
            double y;
            double z;
        };
    };
          double& operator[](const size_t i)       { assert(i<3); return xyz.data[i]; }
    const double& operator[](const size_t i) const { assert(i<3); return xyz.data[i]; }
    vec3(const double x = 0,const double y = 0,const double z = 0){
        xyz[0] = x;
        xyz[1] = y;
        xyz[2] = z;
    }


    vec3 operator-() const {
        return (*this)*(-1.0);
    }
    
    vec3 operator*=(const double rhs) {
        for (size_t i=3; i--; this->xyz[i]*=rhs);
        return *this;
    }
    vec3 operator*=(const vec3 rhs) {
        for (size_t i=3; i--; this->xyz[i]*=rhs[i]);
        return *this;
    }
    vec3 operator*(const double rhs) const {
        vec3 ret = *this;
        return ret *= rhs;
    }
    vec3 operator*(const vec3 rhs) const {
        vec3 ret = *this;
        return ret *= rhs;
    }


    vec3 operator/=(const double rhs) {
        for (size_t i=3; i--; this->xyz[i]/=rhs);
        return *this;
    }
    vec3 operator/=(const vec3 rhs) {
        for (size_t i=3; i--; this->xyz[i]/=rhs[i]);
        return *this;
    }
    vec3 operator/(const double rhs) const {
        vec3 ret = *this;
        return ret /= rhs;
    }
    vec3 operator/(const vec3 rhs) const {
        vec3 ret = *this;
        return ret /= rhs;
    }



    vec3 operator-=(const double rhs) {
        for (size_t i=3; i--; this->xyz[i]-=rhs);
        return *this;
    }
    vec3 operator-=(const vec3 rhs) {
        for (size_t i=3; i--; this->xyz[i]-=rhs[i]);
        return *this;
    }
    vec3 operator-(const double rhs) const {
        vec3 ret = *this;
        return ret -= rhs;
    }
    vec3 operator-(const vec3 rhs) const {
        vec3 ret = *this;
        return ret -= rhs;
    }



    vec3 operator+=(const double rhs) {
        for (size_t i=3; i--; this->xyz[i]+=rhs);
        return *this;
    }
    vec3 operator+=(const vec3 rhs) {
        for (size_t i=3; i--; this->xyz[i]+=rhs[i]);
        return *this;
    }
    vec3 operator+(const double rhs) const {
        vec3 ret = *this;
        return ret += rhs;
    }
    vec3 operator+(const vec3 rhs) const {
        vec3 ret = *this;
        return ret += rhs;
    }
    double doub() const {
        double ret = 0;
        for (size_t i=3; i--; ret+=(*this)[i]);
        return ret;
    }


    vec3 cross(const vec3  rhs) const {
        return vec3{ (*this)[1]*rhs[2] - (*this)[2]*rhs[1], (*this)[2]*rhs[0] - (*this)[0]*rhs[2] ,(*this)[0]*rhs[1]- (*this)[1]*rhs[0]};
    }

    double norm() const { return std::sqrt(((*this)* (*this)).doub()); }
    
    vec3& normalize(double l=1) { 
        *this = (*this)*(l/norm());
         return *this; 
    }
};




















class color {
    public:
    union {
        vec<4> xyz;
        struct{
            double r;
            double g;
            double b;
            double a;
        };
    };
          double& operator[](const size_t i)       { assert(i<3); return xyz.data[i]; }
    const double& operator[](const size_t i) const { assert(i<3); return xyz.data[i]; }
    color(const double r = 0,const double g = 0,const double b = 0,const double a = 1){
        xyz[0] = r;
        xyz[1] = g;
        xyz[2] = b;
        xyz[3] = a;
    }

    color operator-() {
        return (*this)*(-1.0);
    }

    
    color operator*=(const double rhs) {
        for (size_t i=3; i--; this->xyz[i]*=rhs);
        return *this;
    }
    color operator*=(const color rhs) {
        for (size_t i=3; i--; this->xyz[i]*=rhs[i]);
        return *this;
    }
    color operator*(const double rhs) const {
        color ret = *this;
        return ret *= rhs;
    }
    color operator*(const color rhs) const {
        color ret = *this;
        return ret *= rhs;
    }


    color operator/=(const double rhs) {
        for (size_t i=3; i--; this->xyz[i]/=rhs);
        return *this;
    }
    color operator/=(const color rhs) {
        for (size_t i=3; i--; this->xyz[i]/=rhs[i]);
        return *this;
    }
    color operator/(const double rhs) const {
        color ret = *this;
        return ret /= rhs;
    }
    color operator/(const color rhs) const {
        color ret = *this;
        return ret /= rhs;
    }



    color operator-=(const double rhs) {
        for (size_t i=3; i--; this->xyz[i]-=rhs);
        return *this;
    }
    color operator-=(const color rhs) {
        for (size_t i=3; i--; this->xyz[i]-=rhs[i]);
        return *this;
    }
    color operator-(const double rhs) const {
        color ret = *this;
        return ret -= rhs;
    }
    color operator-(const color rhs) const{
        color ret = *this;
        return ret -= rhs;
    }



    color operator+=(const double rhs) {
        for (size_t i=3; i--; this->xyz[i]+=rhs);
        return *this;
    }
    color operator+=(const color rhs) {
        for (size_t i=3; i--; this->xyz[i]+=rhs[i]);
        return *this;
    }
    color operator+(const double rhs) const {
        color ret = *this;
        return ret += rhs;
    }
    color operator+(const color rhs) const {
        color ret = *this;
        return ret += rhs;
    }


    color hue(){
        return color{0,0,0};
    }
    color saturation(){
        return color{0,0,0};

    }
    color value(){
        return color{0,0,0};

    }
    void rgb(char colors[3]){
        double max = std::max((*this)[0], std::max((*this)[1], (*this)[2]));
        if (max>1)  (*this) *= (1./max);
        colors[0] = 255*(*this)[0];
        colors[1] = 255*(*this)[1];
        colors[2] = 255*(*this)[2];
    }
    void rgba(char colors[4]){
        double max = std::max((*this)[0], std::max((*this)[1], (*this)[2]));
        if (max>1) (*this) *= (1./max);
        colors[0] = 255*(*this)[0];
        colors[1] = 255*(*this)[1];
        colors[2] = 255*(*this)[2];
        colors[3] = 255*(*this)[3];
    }
    double norm() const { return std::sqrt((*this)[0]*(*this)[0]+(*this)[1]*(*this)[1]+(*this)[2]*(*this)[2]); }
    
    color& normalize(double l=1) { 
        *this = (*this)*(l/norm());
         return *this; 
    }
};

#endif