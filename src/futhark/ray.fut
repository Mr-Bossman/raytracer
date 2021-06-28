import "types"


entry Sphere (o: vec3) (r: f64) (mat:material): sphere = {o=o,r=r,mat=mat}
entry Ray (o: vec3) (d: vec3): ray = {o=o,d=d}
entry Light (o: vec3) (c: vec3): light = {o=o,c=c}
entry Vec (xs: f64) (ys: f64)(zs: f64) : vec3 = {x=xs,y=ys,z=zs}
entry Cam (c:ray)(fov: f64):cam = {c=c,fov=fov}
entry Albedo (a:f64)(D:f64)(at:f64)(ap:f64):albedo = {a=a,D=D,at=at,ap=ap}
entry Material (refractive_index:f64)(albedo:albedo)(diffuse_color:vec3)(specular_exponent:f64):material = {refractive_index=refractive_index,albedo=albedo,diffuse_color=diffuse_color,specular_exponent=specular_exponent}

entry VecA [cn](xs: [cn]f64) (ys: [cn]f64)(zs: [cn]f64) : [cn]vec3 = map3(\x y z -> {x=x,y=y,z=z}) xs ys zs
entry AlbedoA [cn] (a:[cn]f64)(D:[cn]f64)(at:[cn]f64)(ap:[cn]f64) : [cn]albedo = map4(\a D at ap-> {a=a,D=D,at=at,ap=ap}) a D at ap
entry MaterialA [cn](refractive_index:[cn]f64)(albedo:[cn]albedo)(diffuse_color:[cn]vec3)(specular_exponent:[cn]f64):[cn]material = map4(\refractive_index albedo diffuse_color specular_exponent -> {refractive_index=refractive_index,albedo=albedo,diffuse_color=diffuse_color,specular_exponent=specular_exponent}) refractive_index albedo diffuse_color specular_exponent
entry SphereA [cn](o: [cn]vec3) (r: [cn]f64) (mat:[cn]material) : [cn]sphere = map3(\o r mat -> {o=o,r=r,mat=mat}) o r mat
entry LightA [cn](o: [cn]vec3) (c: [cn]vec3): [cn]light = map2(\o c -> {o=o,c=c}) o c
type state[cn][la] = {l:[la]light,s:[cn]sphere,c:cam,h:u32,w:u32}
entry State [obj][la](l:[la]light)(s: [obj]sphere)(c:cam)(h: u32)(w: u32): state[obj][la] = {l=l,s=s,c=c,h=h,w=w}


let EPSILON:f64 = 0.0001
let bgC:vec3 = {x=0.2,y=0.7,z=0.8}

let u32color (c:vec3):u32 =
    let r = u32.f64 (c.x * 255)
    let g = u32.f64 (c.y * 255)
    let b = u32.f64 (c.z * 255)
    in (r + (g*256) + (b*65536))

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

let scene_intersect [obj][la](s:state[obj][la])(r:ray) = 
scene_intersect_sphere s.s r

let scene_intersect_flood_light (l:light)(p:vec3) = 
    let d = vec.normalise(vec.(l.o - p)) 
    in (d,l.c)

let reflect(I:vec3)(N:vec3) = vec.(I - vec.scale 2 vec.(N*vec.(I*N)))


let refract(I:vec3)(N:vec3)(t:f64)(i:f64):vec3 =
    let cosi = vec.dot I N --need to constran to -1 and 1
    --if(cosai < 0) then let cosi = vec.dot -I N
    let eta = i/t
    let k = 1 - ((eta*eta)*(1-(cosi*cosi)))
    in if (k < 0) then {x=1,y=0,z=0} else 
        let les = (eta*cosi) - (f64.sqrt(k))
        in vec.((vec.scale eta I) + (vec.scale les N))

let scene_intersect_light_sub [obj][la](s:state[obj][la])(p:vec3)(l:light)(mate:material) = 
    let (dir,c)  = scene_intersect_flood_light l p 
    let (hit,h,N,mat) = scene_intersect s {o=p,d=dir}
    let a = vec.norm(vec.(h-p))
    let b = vec.norm(vec.(l.o-p))
    in if (hit && (a < b)) then (vec.zero,vec.zero) else 
        let dt = f64.max 0 (vec.dot dir N)
        let diffuse = vec.scale dt l.c
        let base = f64.max 0 (vec.dot (vec.scale (-1) (reflect (vec.scale (-1) dir) N)) dir)
        let spec = vec.scale (base**mate.specular_exponent) l.c
        --let spec = (light.intensity * std::pow(std::max(0., (-reflect(-light_dir, N)*dir).doub()), material.specular_exponent));
        in (diffuse,spec) 


let vec_twotup_add (a:vec3,b:vec3)(c:vec3,d:vec3):(vec3,vec3) = (vec.(a+c),vec.(b+d))


let scene_intersect_light [la][obj](s:state[obj][la])(p:vec3)(mat:material) =
    reduce (vec_twotup_add) (vec.zero,vec.zero) (map(\lp -> scene_intersect_light_sub s p lp mat) s.l)



let ray_cast [obj][la](s:state[obj][la]) (h) (w):u32 = 
    let x = ((f64.i64 w)+0.5)-((f64.u32 s.w)/2)
    let y = -((f64.i64 h)+0.5)+((f64.u32 s.h)/2)
    let z = -(f64.u32 s.h)/(2*f64.tan(s.c.fov/2))
    let d = vec.normalise (vec.rot_y  s.c.c.d.y  (vec.rot_x  s.c.c.d.x {x=x,y=y,z=z}))
    let r:ray = {o=s.c.c.o,d=d}
    let (hit,h,N,mat) = scene_intersect s r 
    in if(hit) then 
    let  (diffuse,spec)  = scene_intersect_light s h mat
    let diff = vec.scale mat.albedo.a vec.(mat.diffuse_color  * diffuse)
    let sp = vec.scale mat.albedo.D spec
    let refr = 0.1
    let refl = 0.1
    in u32color vec.(diff + sp) --+ refl + refr
    else u32color bgC

entry main [obj][la](h:i64) (w:i64) (s:state[obj][la]):[h][w]u32 = 
let s:state[obj][la] = s with h = u32.i64 h with w = u32.i64 w 
    in map(\y -> 
    map(\x -> 
    ray_cast s y x
    )(0..<w)
    )(0..<h)