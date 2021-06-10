#include <cmath>
#include <cassert>
#include <iostream>


#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__
template <size_t DIM> struct vec {
          double& operator[](const size_t i)       { assert(i<DIM); return data[i]; }
    const double& operator[](const size_t i) const { assert(i<DIM); return data[i]; }
    double data[DIM] = {};
};

template<size_t DIM> vec<DIM> operator*(const vec<DIM> &lhs, const double rhs) {
    vec<DIM> ret;
    for (size_t i=DIM; i--; ret[i]=lhs[i]*rhs);
    return ret;
}
template<size_t DIM> vec<DIM> operator*(const vec<DIM>& lhs, const vec<DIM>& rhs) {
    vec<DIM> ret;
    for (size_t i=DIM; i--; ret[i]+=lhs[i]*rhs[i]);
    return ret;
}
template<size_t DIM> double doub(const vec<DIM>& rhs) {
    double ret = 0;
    for (size_t i=DIM; i--; ret+=rhs[i]);
    return ret;
}

template<size_t DIM> vec<DIM> operator+(vec<DIM> lhs, const vec<DIM>& rhs) {
    for (size_t i=DIM; i--; lhs[i]+=rhs[i]);
    return lhs;
}

template<size_t DIM> vec<DIM> operator-(vec<DIM> lhs, const vec<DIM>& rhs) {
    for (size_t i=DIM; i--; lhs[i]-=rhs[i]);
    return lhs;
}

template<size_t DIM> vec<DIM> operator-(const vec<DIM> &lhs) {
    return lhs*(-1.f);
}

template <> struct vec<3> {
          double& operator[](const size_t i)       { assert(i<3); return i==0 ? x : (1==i ? y : z); }
    const double& operator[](const size_t i) const { assert(i<3); return i==0 ? x : (1==i ? y : z); }
    double norm() { return std::sqrt(x*x+y*y+z*z); }
    vec<3> & normalize(double l=1) { *this = (*this)*(l/norm()); return *this; }
    double x = 0, y = 0, z = 0;
};

typedef vec<3> vec3;
typedef vec<4> vec4;

inline vec3 cross(vec3 v1, vec3 v2) {
    return { v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x };
}

template <size_t DIM> std::ostream& operator<<(std::ostream& out, const vec<DIM>& v) {
    for (size_t i=0; i<DIM; i++)
        out << v[i] << " " ;
    return out ;
}

#endif //__GEOMETRY_H__

