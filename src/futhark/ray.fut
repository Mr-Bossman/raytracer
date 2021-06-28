import "types"


entry Sphere (o: vec3) (r: f64) (mat:material): sphere = {o=o,r=r,mat=mat}
entry Ray (o: vec3) (d: vec3): ray = {o=o,d=d}
entry Vec (xs: f64) (ys: f64)(zs: f64) : vec3 = {x=xs,y=ys,z=zs}
entry Cam (c:ray)(fov: f64):cam = {c=c,fov=fov}
entry Albedo (a:f64)(D:f64)(at:f64)(ap:f64):albedo = {a=a,D=D,at=at,ap=ap}
entry Material (refractive_index:f64)(albedo:albedo)(diffuse_color:vec3)(specular_exponent:f64):material = {refractive_index=refractive_index,albedo=albedo,diffuse_color=diffuse_color,specular_exponent=specular_exponent}

entry VecA [cn](xs: [cn]f64) (ys: [cn]f64)(zs: [cn]f64) : [cn]vec3 = map3(\x y z -> {x=x,y=y,z=z}) xs ys zs
entry AlbedoA [cn] (a:[cn]f64)(D:[cn]f64)(at:[cn]f64)(ap:[cn]f64) : [cn]albedo = map4(\a D at ap-> {a=a,D=D,at=at,ap=ap}) a D at ap
entry MaterialA [cn](refractive_index:[cn]f64)(albedo:[cn]albedo)(diffuse_color:[cn]vec3)(specular_exponent:[cn]f64):[cn]material = map4(\refractive_index albedo diffuse_color specular_exponent -> {refractive_index=refractive_index,albedo=albedo,diffuse_color=diffuse_color,specular_exponent=specular_exponent}) refractive_index albedo diffuse_color specular_exponent
entry SphereA [cn](o: [cn]vec3) (r: [cn]f64) (mat:[cn]material) : [cn]sphere = map3(\o r mat -> {o=o,r=r,mat=mat}) o r mat

type state[cn] = {s:[cn]sphere,c:cam,h:u32,w:u32}
entry State [obj](s: [obj]sphere)(c:cam)(h: u32)(w: u32): state[obj] = {s=s,c=c,h=h,w=w}


let EPSILON:f64 = 0.0001
let bgC:vec3 = {x=0.01,y=0,z=0}

let u32color (c:vec3):u32 =
    let r = u32.f64 (c.x * 255)
    let g = u32.f64 (c.y * 255)
    let b = u32.f64 (c.z * 255)
    in (b + (g*256) + (r*65536))

type intersection 'shape =  #Yes f64 shape | #No

let ray_sphere_intersect (r:ray) (s:sphere):intersection sphere=
    let L = vec.(s.o - r.o)
    let tca = vec.dot r.d L
    let d2 =  (vec.quadrance L) - (tca*tca)
    in if (d2 > s.r*s.r) then #No else
    let thc = f64.sqrt((s.r*s.r) - d2)
    let t0 = tca - thc
    let t1 = tca + thc
    in if (t0 < EPSILON) then 
    if (t1 < EPSILON) then #No else #Yes t0 s
    else #Yes t0 s
let scene_intersect_check 'shape  (a: intersection shape) (b: intersection shape):intersection shape  = match a
  case #No -> b
  case #Yes dist_a _ -> match b
    case #No -> a
    case #Yes dist_b _ -> if dist_a < dist_b then a else b

let scene_intersect_sphere [obj](s:[obj]sphere)(r:ray) = 
    let closest = reduce(scene_intersect_check) #No (map(\sf -> ray_sphere_intersect r sf) s)
    in match closest
        case #No -> (false,vec.zero,vec.zero,s[0].mat)
        case #Yes t0 sp -> let h = vec.(r.o + vec.scale t0 r.d)
            let N = vec.normalise(vec.(h - sp.o))
            in (true,h,N,sp.mat)

let scene_intersect [obj](s:state[obj])(r:ray) = 
scene_intersect_sphere s.s r

let reflect(I:vec3,N:vec3) = vec.(I - vec.scale 2 vec.(N*vec.(I*N)))
--vec3 refract(const vec3 &I, const vec3 &N, const double eta_t, const double eta_i) { // Snell's law
--    double cosi = - std::max(-1., std::min(1., (I*N).doub()));
--    if (cosi<0) return refract(I, -N, eta_i, eta_t); // if the ray comes from the inside the object, swap the air and the media
--    double eta = eta_i / eta_t;
--    double k = 1 - eta*eta*(1 - cosi*cosi);
--    return k<0 ? vec3{1,0,0} : I*eta + N*(eta*cosi - std::sqrt(k)); // k<0 = total reflection, no ray to refract. I refract it anyways, this has no physical meaning
--}
let ray_cast [obj](s:state[obj]) (h) (w):u32 = 
    let x = ((f64.i64 w)+0.5)-((f64.u32 s.w)/2)
    let y = -((f64.i64 h)+0.5)+((f64.u32 s.h)/2)
    let z = -(f64.u32 s.h)/(2*f64.tan(s.c.fov/2))
    let d = vec.normalise (vec.rot_y  s.c.c.d.y  (vec.rot_x  s.c.c.d.x {x=x,y=y,z=z}))
    let r:ray = {o=s.c.c.o,d=d}
    let (hit,h,N,mat) = scene_intersect s r 
    in if(hit) then u32color mat.diffuse_color else u32color bgC

entry main [obj](h:i64) (w:i64) (s:state[obj]):[h][w]u32 = 
let s:state[obj] = s with h = u32.i64 h with w = u32.i64 w 
    in map(\y -> 
    map(\x -> 
    ray_cast s y x
    )(0..<w)
    )(0..<h)