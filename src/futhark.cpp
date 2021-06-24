#include "futhark.h"
Fvec3 Fvec3C(Fcontext ctx, vec3 pos){
    Fvec3 vec;
    futhark_entry_Vec(ctx, &vec,pos.x,pos.y,pos.z);
    return vec;
}

Fray FrayC(Fcontext ctx, Ray ray){
    Fvec3 veca,vecb;
    Fray r;
    futhark_entry_Vec(ctx, &veca,ray.o.x,ray.o.y,ray.o.z);
    futhark_entry_Vec(ctx, &vecb,ray.d.x,ray.d.y,ray.d.z);
    futhark_entry_Ray(ctx, &r,veca,vecb);
    return r;
}

Fsphere FsphereC(Fcontext ctx, Sphere s){
    Fvec3 vec;
    Fsphere sp;
    futhark_entry_Vec(ctx, &vec,s.center.x,s.center.y,s.center.z);
    futhark_entry_Sphere(ctx, &sp,vec,s.radius);
    return sp;
}

Fcam FcamC(Fcontext ctx, Cam c){
    Fvec3 veca,vecb;
    Fray r;
    Fcam cm;
    futhark_entry_Vec(ctx, &veca,c.pos.x,c.pos.y,c.pos.z);
    futhark_entry_Vec(ctx, &vecb,c.dir.x,c.dir.y,c.dir.z);
    futhark_entry_Ray(ctx, &r,veca,vecb);
    futhark_entry_Cam(ctx, &cm,r,c.fov);
    return cm;
}