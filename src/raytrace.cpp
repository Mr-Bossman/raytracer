#include "raytrace.h"
bool ray_sphere_intersect(const vec3 &orig, const vec3 &dir, const Sphere &s, double &t0) {
    vec3 L = s.center - orig;
    double tca = (L*dir).doub();
    double d2 =  (L*L).doub() - tca*tca;
    if (d2 > s.radius*s.radius) return false;
    double thc = sqrtf(s.radius*s.radius - d2);
    t0       = tca - thc;
    double t1 = tca + thc;
    if (t0 < EPSILON) t0 = t1;  // offset the original point to avoid occlusion by the object itself
    if (t0 < EPSILON) return false;
    return true;
}

bool ray_triangle_intersect(const vec3 &orig, const vec3 &dir, const Triangle &t, double &t0) {
    vec3 edge1, edge2, h, s, q;
    double a,f,u,v;
    edge1 = t.b - t.a;
    edge2 = t.c - t.a;
    h = dir.cross(edge2);
    a = edge1.dot(h);
    if (a > -EPSILON && a < EPSILON)
        return false;
    f = 1.0/a;
    s = orig - t.a;
    u = f * s.dot(h);
    if (u < 0.0 || u > 1.0)
        return false;
    q = s.cross(edge1);
    v = f * dir.dot(q);
    if (v < 0.0 || u + v > 1.0)
        return false;
    t0 = f * edge2.dot(q);
    if (t0 > EPSILON)
        return true;
    return false;
}

vec3 reflect(const vec3 &I, const vec3 &N) {
    return I - N*2.f*(I*N);
}

vec3 refract(const vec3 &I, const vec3 &N, const double eta_t, const double eta_i) { // Snell's law
    double cosi = - std::max(-1., std::min(1., (I*N).doub()));
    if (cosi<0) return refract(I, -N, eta_i, eta_t); // if the ray comes from the inside the object, swap the air and the media
    double eta = eta_i / eta_t;
    double k = 1 - eta*eta*(1 - cosi*cosi);
    return k<0 ? vec3{1,0,0} : I*eta + N*(eta*cosi - std::sqrt(k)); // k<0 = total reflection, no ray to refract. I refract it anyways, this has no physical meaning
}

bool scene_intersect(const vec3 &orig, const vec3 &dir, const Objects &obj, vec3 &hit, vec3 &N, Material &material) {
    double dist = std::numeric_limits<double>::max();
    for (const Sphere &s : obj.sphere) {
        double dist_i;
        if (ray_sphere_intersect(orig, dir, s, dist_i) && dist_i < dist) {
            dist = dist_i;
            hit = orig + dir*dist_i;
            N = (hit - s.center).normalize();
            material = s.material;
        }
    }
    for (const Triangle &t : obj.triangle) {
        double dist_i;
        if (ray_triangle_intersect(orig, dir, t, dist_i) && dist_i < dist) {
            dist = dist_i;
            hit = orig + dir*dist_i;
            material = t.material;

            // vvv from mark
            vec3 edge1 = t.b - t.a;
            vec3 edge2 = t.c - t.a;
            N = edge1.cross(edge2).normalize();
            // ^^^ from mark
        }
    }
    //do other shapes
    #if PLANE
    if (std::abs(dir.y)>EPSILON) { // avoid division by zero
        double d = -(orig.y+4)/dir.y; // the checkerboard plane has equation y = -4
        vec3 pt = orig + dir*d;
        if (d>EPSILON&& fabs(pt.x)<80 && fabs(pt.z)<80 && d < dist) {
            dist = d;
            hit = pt;
            N = vec3{0,1,0};
            material.diffuse_color = (int(0.5*hit.x+RENDER_DIST) + int(0.5*hit.z+RENDER_DIST)) & 1 ? color{.3, .3, .3} : color{.3, .2, .1};
        }
    }
    #endif
    return dist <RENDER_DIST;
}
//:(hit:bool,hitP:vec3,Norm:vec3,mat:material)
color cast_ray(const vec3 &orig, const vec3 &dir, const Objects &obj, const Lights &lights, size_t depth) {
    vec3 point, N;
    Material material;

    if (depth>REC_DEPTH || !scene_intersect(orig, dir, obj, point, N, material))
        return color BACKGROUND_COLOR; // background color

    vec3 reflect_dir = reflect(dir, N).normalize();
    vec3 refract_dir = refract(dir, N, material.refractive_index).normalize();
    color reflect_color = cast_ray(point, reflect_dir, obj, lights, depth + 1);
    color refract_color = cast_ray(point, refract_dir, obj, lights, depth + 1);

    color diffuse_light_intensity = {0,0,0}, specular_light_intensity = {0,0,0};
    for (const Light light : lights.light) {
        vec3 light_dir      = (light.position - point).normalize();

        vec3 shadow_pt, trashnrm;
        Material trashmat;
        if (scene_intersect(point, light_dir, obj, shadow_pt, trashnrm, trashmat) &&
                (shadow_pt-point).norm() < (light.position-point).norm()) // checking if the point lies in the shadow of the light
            continue;

        diffuse_light_intensity = diffuse_light_intensity  + (light.intensity * std::max(0.,  (light_dir*N).doub()));
        specular_light_intensity = specular_light_intensity + (light.intensity * std::pow(std::max(0., (-reflect(-light_dir, N)*dir).doub()), material.specular_exponent));
    }
    //do spot light
    return (material.diffuse_color * diffuse_light_intensity) * material.albedo[0] + (specular_light_intensity * material.albedo[1]) + reflect_color*material.albedo[2] + refract_color*material.albedo[3];
}