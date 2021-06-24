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
    futhark_entry_Sphere(ctx, &sp,vec,s.radius,FmaterialC(ctx,s.material));
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

Falbedo FalbedoC(Fcontext ctx, vec4 al){
    Falbedo a;
    futhark_entry_Albedo(ctx,&a,al[0],al[1],al[2],al[2]);
    return a;
}
Fvec3 FcolorV3C(Fcontext ctx, color C){
    Fvec3 vec;
    futhark_entry_Vec(ctx, &vec,C.r,C.g,C.b);
    return vec;
}

Fmaterial FmaterialC(Fcontext ctx, Material al){
    Fmaterial mat;
    futhark_entry_Material(ctx,&mat,al.refractive_index,FalbedoC(ctx,al.albedo),FcolorV3C(ctx,al.diffuse_color),al.specular_exponent);
    return mat;
}