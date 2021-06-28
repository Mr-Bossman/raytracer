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

FlightArr FlightArrC(Fcontext ctx, std::vector<Light> l){
    std::vector<FdoubleA> jenk;
    double* x = new double[l.size()];
    double* y = new double[l.size()];
    double* z = new double[l.size()];
    double* r = new double[l.size()];
    double* g = new double[l.size()];
    double* b = new double[l.size()];
    for(size_t i = 0; i < l.size(); i++){
        x[i] = l[i].position.x;
        y[i] = l[i].position.y;
        z[i] = l[i].position.z;
        r[i] = l[i].intensity.r;
        g[i] = l[i].intensity.g;
        b[i] = l[i].intensity.b;
    }
    jenk.push_back(futhark_new_f64_1d(ctx,x,l.size()));
    jenk.push_back(futhark_new_f64_1d(ctx,y,l.size()));
    jenk.push_back(futhark_new_f64_1d(ctx,z,l.size()));
    jenk.push_back(futhark_new_f64_1d(ctx,r,l.size()));
    jenk.push_back(futhark_new_f64_1d(ctx,g,l.size()));
    jenk.push_back(futhark_new_f64_1d(ctx,b,l.size()));
    delete[] x;
    delete[] y;
    delete[] z;
    delete[] r;
    delete[] g;
    delete[] b;

    struct futhark_opaque_arr_vec3_1d *c;
    struct futhark_opaque_arr_vec3_1d *pos;
    futhark_entry_VecA(ctx,&c,jenk[3],jenk[4],jenk[5]);
    futhark_entry_VecA(ctx,&pos,jenk[0],jenk[1],jenk[2]);
    FlightArr ret;
    futhark_entry_LightA(ctx,&ret,pos,c);
    for(auto bruh:jenk){
        futhark_free_f64_1d(ctx,bruh);
    }
    futhark_free_opaque_arr_vec3_1d(ctx,c);
    futhark_free_opaque_arr_vec3_1d(ctx,pos);
    return ret;
}

Fsphere FsphereC(Fcontext ctx, Sphere s){
    Fvec3 vec;
    Fsphere sp;
    futhark_entry_Vec(ctx, &vec,s.center.x,s.center.y,s.center.z);
    futhark_entry_Sphere(ctx, &sp,vec,s.radius,FmaterialC(ctx,s.material));
    return sp;
}
//cursed code 1000
FsphereArr FsphereArrC(Fcontext ctx, std::vector<Sphere> s){
    std::vector<FdoubleA> jenk;
    double* x = new double[s.size()];
    double* y = new double[s.size()];
    double* z = new double[s.size()];
    double* r = new double[s.size()];
    double* ri = new double[s.size()];
    double* se = new double[s.size()];
    double* cr = new double[s.size()];
    double* cg = new double[s.size()];
    double* cb = new double[s.size()];
    double* a = new double[s.size()];
    double* D = new double[s.size()];
    double* at = new double[s.size()];
    double* ap = new double[s.size()];

    for(size_t i = 0; i < s.size(); i++){
        x[i] = s[i].center.x;
        y[i] = s[i].center.y;
        z[i] = s[i].center.z;
        r[i] = s[i].radius;
        ri[i] = s[i].material.refractive_index;
        se[i] = s[i].material.specular_exponent;
        cr[i] = s[i].material.diffuse_color.r;
        cg[i] = s[i].material.diffuse_color.g;
        cb[i] = s[i].material.diffuse_color.b;
        a[i] = s[i].material.albedo[0];
        D[i] = s[i].material.albedo[1];
        at[i] = s[i].material.albedo[2];
        ap[i] = s[i].material.albedo[3];
    }

    jenk.push_back(futhark_new_f64_1d(ctx,x,s.size()));
    jenk.push_back(futhark_new_f64_1d(ctx,y,s.size()));
    jenk.push_back(futhark_new_f64_1d(ctx,z,s.size()));
    jenk.push_back(futhark_new_f64_1d(ctx,r,s.size()));
    jenk.push_back(futhark_new_f64_1d(ctx,ri,s.size()));
    jenk.push_back(futhark_new_f64_1d(ctx,se,s.size()));
    jenk.push_back(futhark_new_f64_1d(ctx,cr,s.size()));
    jenk.push_back(futhark_new_f64_1d(ctx,cg,s.size()));
    jenk.push_back(futhark_new_f64_1d(ctx,cb,s.size()));
    jenk.push_back(futhark_new_f64_1d(ctx,a,s.size()));
    jenk.push_back(futhark_new_f64_1d(ctx,D,s.size()));
    jenk.push_back(futhark_new_f64_1d(ctx,at,s.size()));
    jenk.push_back(futhark_new_f64_1d(ctx,ap,s.size()));

    delete[] x;
    delete[] y;
    delete[] z;
    delete[] r;
    delete[] ri;
    delete[] se;
    delete[] cr;
    delete[] cg;
    delete[] cb;
    delete[] a;
    delete[] D;
    delete[] at;
    delete[] ap;



    struct futhark_opaque_arr_vec3_1d *c;
    struct futhark_opaque_arr_vec3_1d *pos;
    futhark_entry_VecA(ctx,&c,jenk[6],jenk[7],jenk[8]);
    futhark_entry_VecA(ctx,&pos,jenk[0],jenk[1],jenk[2]);
    struct futhark_opaque_arr_albedo_1d* al;
    futhark_entry_AlbedoA(ctx,&al,jenk[9],jenk[10],jenk[11],jenk[12]);
    struct futhark_opaque_arr_material_1d* mat;
    futhark_entry_MaterialA(ctx,&mat,jenk[4],al,c,jenk[5]);
    FsphereArr ret;
    futhark_entry_SphereA(ctx,&ret,pos,jenk[3],mat);


    futhark_free_opaque_arr_material_1d(ctx,mat);
    futhark_free_opaque_arr_albedo_1d(ctx,al);
    futhark_free_opaque_arr_vec3_1d(ctx,c);
    futhark_free_opaque_arr_vec3_1d(ctx,pos);
    for(auto bruh:jenk){
        futhark_free_f64_1d(ctx,bruh);
    }
    return ret;
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