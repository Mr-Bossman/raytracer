#include "common.h"
#include "raytrace.h"
#include "geometry.h"
#include "objects.h"

bool ray_sphere_intersect(const vec3 &orig, const vec3 &dir, const Sphere &s, double &t0) {
    vec3 L = s.center - orig;
    double tca = (L*dir).doub();
    double d2 =  (L*L).doub() - tca*tca;
    if (d2 > s.radius*s.radius) return false;
    double thc = sqrtf(s.radius*s.radius - d2);
    t0       = tca - thc;
    double t1 = tca + thc;
    if (t0 < 1e-3) t0 = t1;  // offset the original point to avoid occlusion by the object itself
    if (t0 < 1e-3) return false;
    return true;
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

bool scene_intersect(const vec3 &orig, const vec3 &dir, const std::vector<Sphere> &spheres, vec3 &hit, vec3 &N, Material &material) {
    double spheres_dist = std::numeric_limits<double>::max();
    for (const Sphere &s : spheres) {
        double dist_i;
        if (ray_sphere_intersect(orig, dir, s, dist_i) && dist_i < spheres_dist) {
            spheres_dist = dist_i;
            hit = orig + dir*dist_i;
            N = (hit - s.center).normalize();
            material = s.material;
        }
    }

    double checkerboard_dist = std::numeric_limits<double>::max();
    if (std::abs(dir.y)>1e-3) { // avoid division by zero
        double d = -(orig.y+4)/dir.y; // the checkerboard plane has equation y = -4
        vec3 pt = orig + dir*d;
        if (d>1e-3 && fabs(pt.x)<10 && pt.z<-10 && pt.z>-30 && d<spheres_dist) {
            checkerboard_dist = d;
            hit = pt;
            N = vec3{0,1,0};
            material.diffuse_color = (int(0.5*hit.x+1000) + int(0.5*hit.z)) & 1 ? color{.3, .3, .3} : color{.3, .2, .1};
        }
    }
    return std::min(spheres_dist, checkerboard_dist)<1000;
}

color cast_ray(const vec3 &orig, const vec3 &dir, const std::vector<Sphere> &spheres, const std::vector<Light> &lights, size_t depth) {
    vec3 point, N;
    Material material;

    if (depth>4 || !scene_intersect(orig, dir, spheres, point, N, material))
        return color{0.2, 0.7, 0.8}; // background color

    vec3 reflect_dir = reflect(dir, N).normalize();
    vec3 refract_dir = refract(dir, N, material.refractive_index).normalize();
    color reflect_color = cast_ray(point, reflect_dir, spheres, lights, depth + 1);
    color refract_color = cast_ray(point, refract_dir, spheres, lights, depth + 1);

    color diffuse_light_intensity = {0,0,0}, specular_light_intensity = {0,0,0};
    for (const Light light : lights) {
        vec3 light_dir      = (light.position - point).normalize();

        vec3 shadow_pt, trashnrm;
        Material trashmat;
        if (scene_intersect(point, light_dir, spheres, shadow_pt, trashnrm, trashmat) &&
                (shadow_pt-point).norm() < (light.position-point).norm()) // checking if the point lies in the shadow of the light
            continue;

        diffuse_light_intensity = diffuse_light_intensity  + (light.intensity * std::max(0.,  (light_dir*N).doub()));
        specular_light_intensity = specular_light_intensity + (light.intensity * std::pow(std::max(0., (-reflect(-light_dir, N)*dir).doub()), material.specular_exponent));
    }
    return (material.diffuse_color * diffuse_light_intensity) * material.albedo[0] + (specular_light_intensity * material.albedo[1]) + reflect_color*material.albedo[2] + refract_color*material.albedo[3];
}