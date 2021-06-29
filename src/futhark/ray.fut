import "types"


entry Sphere (o: vec3) (r: f64) (mat:material): sphere = {o=o,r=r,mat=mat}
entry Ray (o: vec3) (d: vec3): ray = {o=o,d=d}
entry Light (o: vec3) (c: vec3): light = {o=o,c=c}
entry Vec (xs: f64) (ys: f64)(zs: f64) : vec3 = {x=xs,y=ys,z=zs}
entry Cam (c:ray)(fov: f64):cam = {c=c,fov=fov}
entry Albedo (a:f64)(D:f64)(at:f64)(ap:f64):albedo = {a=a,D=D,at=at,ap=ap}
entry Material (refractive_index:f64)(albedo:albedo)(diffuse_color:vec3)(specular_exponent:f64):material = {refractive_index=refractive_index,albedo=albedo,diffuse_color=diffuse_color,specular_exponent=specular_exponent}
entry Triangle (a: vec3) (b: vec3) (c: vec3) (d: vec3)(mat:material): triangle = {a=a,b=b,c=c,n=d,mat=mat}

entry VecA [cn](xs: [cn]f64) (ys: [cn]f64)(zs: [cn]f64) : [cn]vec3 = map3(\x y z -> {x=x,y=y,z=z}) xs ys zs
entry AlbedoA [cn] (a:[cn]f64)(D:[cn]f64)(at:[cn]f64)(ap:[cn]f64) : [cn]albedo = map4(\a D at ap-> {a=a,D=D,at=at,ap=ap}) a D at ap
entry MaterialA [cn](refractive_index:[cn]f64)(albedo:[cn]albedo)(diffuse_color:[cn]vec3)(specular_exponent:[cn]f64):[cn]material = map4(\refractive_index albedo diffuse_color specular_exponent -> {refractive_index=refractive_index,albedo=albedo,diffuse_color=diffuse_color,specular_exponent=specular_exponent}) refractive_index albedo diffuse_color specular_exponent
entry SphereA [cn](o: [cn]vec3) (r: [cn]f64) (mat:[cn]material) : [cn]sphere = map3(\o r mat -> {o=o,r=r,mat=mat}) o r mat
entry TriangleA [cn](a: [cn]vec3) (b: [cn]vec3) (c: [cn]vec3) (d: [cn]vec3)(mat:[cn]material) : [cn]triangle = map5(\a b c d mat-> {a=a,b=b,c=c,n=d,mat=mat}) a b c d mat
entry LightA [cn](o: [cn]vec3) (c: [cn]vec3): [cn]light = map2(\o c -> {o=o,c=c}) o c

type state[cn][la][tr] = {l:[la]light,s:[cn]sphere,t: [tr]triangle,c:cam,h:u32,w:u32}
entry State [obj][la][tr](l:[la]light)(s: [obj]sphere)(t: [tr]triangle)(c:cam)(h: u32)(w: u32): state[obj][la][tr] = {l=l,s=s,t=t,c=c,h=h,w=w}

let EPSILON:f64 = 0.001
let bgC:vec3 = {x=0.2,y=0.7,z=0.8}
let Dmat:material = {refractive_index=1,albedo={a=1,D=0.6,at=0.1,ap=0},diffuse_color=bgC,specular_exponent=50}

let u32color (c:vec3):u32 =
    let m = f64.max c.x (f64.max c.y c.z)
    let nc  = if m > 1 then vec.scale (1/m) c else c
    let r = u32.f64 (nc.x * 255)
    let g = u32.f64 (nc.y * 255)
    let b = u32.f64 (nc.z * 255)
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

let ray_triangle_intersect(r:ray) (t:triangle):intersection triangle =
    let e1 = vec.(t.b - t.a)
    let e2 = vec.(t.c - t.a)
    let h = vec.cross r.d e2
    let a = vec.dot e1 h
    in if ((f64.abs a) < EPSILON) then #No else 
        let f = 1.0/a
        let s = vec.(r.o - t.a)
        let u = f * (vec.dot s h)
        in if (u < 0.0) || (u > 1.0) then #No else
            let q = vec.cross s e1
            let v = f * (vec.dot r.d q)
            in if (v < 0.0) || (u+v > 1.0) then #No else
                let t0 = f * (vec.dot e2 q)
                in if t0 > EPSILON then #Yes t0 t
                else #No

let scene_intersect_check 'shape  (a: intersection shape) (b: intersection shape):intersection shape  = match a
  case #No -> b
  case #Yes dist_a _ -> match b
    case #No -> a
    case #Yes dist_b _ -> if dist_a < dist_b then a else b

let scene_intersect_triangle [tr](t:[tr]triangle)(r:ray) = 
    let closest = reduce(scene_intersect_check) #No (map(\tk -> ray_triangle_intersect r tk) t)
    in match closest
        case #No -> (false,vec.zero,vec.zero,0,Dmat)
        case #Yes t0 t -> let h = vec.(r.o + vec.scale t0 r.d)
            let e1 = vec.(t.b - t.a)
            let e2 = vec.(t.c - t.a)
            let N = vec.normalise (vec.cross e1 e2)
            -- let N = t.n
            in (true,h,N,t0,t.mat)

let scene_intersect_sphere [obj](s:[obj]sphere)(r:ray) = 
    let closest = reduce(scene_intersect_check) #No (map(\sf -> ray_sphere_intersect r sf) s)
    in match closest
        case #No -> (false,vec.zero,vec.zero,0,Dmat)
        case #Yes t0 sp -> 
            let h = vec.(r.o + vec.scale t0 r.d)
            let N = vec.normalise(vec.(h - sp.o))
            in (true,h,N,t0,sp.mat)

let scene_intersect [obj][la][tr](s:state[obj][la][tr])(r:ray) = 
let (st,sh,sN,t0a,smat) = (scene_intersect_sphere s.s r) 
let (tt,th,tN,t0b,tmat)  = (scene_intersect_triangle s.t r)
in if st then 
    if (tt && (t0b < t0a)) then 
        (tt,th,tN,tmat)
    else (st,sh,sN,smat) 
else (tt,th,tN,tmat)


let scene_intersect_flood_light (l:light)(p:vec3) =  vec.normalise(vec.(l.o - p)) 

let reflect(I:vec3)(N:vec3) = vec.(I - vec.scale 2 vec.(N*vec.(I*N)))


let refract(I:vec3)(N:vec3)(t:f64)(i:f64):vec3 =
    let cosai = (-1)*(vec.dot I N)
    let (cosi,eta) = if(cosai < 0) then 
        let tmp = (-1)*(vec.dot (vec.scale (-1) I) N)
        in (f64.max (-1) (f64.min 1 tmp ),t/i) 
    else 
        (f64.max (-1) (f64.min 1 cosai ),i/t)
    let k = 1 - ((eta*eta)*(1-(cosi*cosi)))
    in if (k < 0) then {x=1,y=0,z=0} else 
        let les = (eta*cosi) - (f64.sqrt(k))
        in vec.((vec.scale eta I) + (vec.scale les N))

let scene_intersect_light_sub [obj][la][tr](s:state[obj][la][tr])(p:vec3)(N:vec3)(l:light) = 
    let dir  = scene_intersect_flood_light l p 
    let (hit,h,_,mat) = scene_intersect s {o=p,d=dir}
    let a = vec.norm(vec.(h-p))
    let b = vec.norm(vec.(l.o-p))
    in if (hit && (a < b)) then (vec.zero,vec.zero) else 
        let dt = f64.max 0 (vec.dot dir N)
        let diffuse = vec.scale dt l.c
        let base = f64.max 0 (vec.dot (vec.scale (-1) (reflect (vec.scale (-1) dir) N)) dir)
        let spec = vec.scale (base**mat.specular_exponent) l.c
        in (diffuse,spec) 


let vec_twotup_add (a:vec3,b:vec3)(c:vec3,d:vec3):(vec3,vec3) = (vec.(a+c),vec.(b+d))


let scene_intersect_light [obj][la][tr](s:state[obj][la][tr])(p:vec3)(N:vec3) =
    reduce (vec_twotup_add) (vec.zero,vec.zero) (map(\lp -> scene_intersect_light_sub s p N lp) s.l)


let cast_ray_once [obj][la][tr](s:state[obj][la][tr])(r:ray):(vec3,vec3,vec3,vec3,material) = 
    let (hit,h,N,mat) = scene_intersect s r 
    in if(hit) then 
        let  (diffuse,spec)  = scene_intersect_light s h N
        let diff = vec.scale mat.albedo.a vec.(mat.diffuse_color * diffuse)
        let sp = vec.scale mat.albedo.D spec
        let refr = vec.normalise(refract r.d N mat.refractive_index 1)
        let refl = vec.normalise(reflect r.d N)
        in (vec.(diff+sp),h,refl,refr,mat)
    else (bgC,vec.zero,vec.zero,vec.zero,mat)



let ray_cast [obj][la][tr](s:state[obj][la][tr]) (h) (w):u32 = 
    let x = ((f64.i64 w)+0.5)-((f64.u32 s.w)/2)
    let y = -((f64.i64 h)+0.5)+((f64.u32 s.h)/2)
    let z = -(f64.u32 s.h)/(2*f64.tan(s.c.fov/2))
    let d = vec.normalise (vec.rot_y  s.c.c.d.y  (vec.rot_x  s.c.c.d.x {x=x,y=y,z=z}))
    let r:ray = {o=s.c.c.o,d=d}
    let bounces:i64 = 2

    --jenk
    -- do diffuse light first
    let (sc,sh,sfl,sfr,m) = cast_ray_once s r
    let (diffuse,(_,_,_,_)) = 
        loop (c ,(ha,fla,_,mat)) = (sc,(sh,sfl,sfr,m) ) for _ in (0...bounces) do
            (let (cp,h,fl,fr,ma)  = cast_ray_once s {o=ha,d=fla} let alb = ma with albedo.at = (mat.albedo.at*ma.albedo.at) in (vec.(c + (vec.scale mat.albedo.at cp)),(h,fl,fr,alb)))

    --do secular light second
    let (c,(_,_,_,_)) = 
        loop (c ,(ha,_,fra,mat)) = (diffuse,(sh,sfl,sfr,m) ) for _ in (0...bounces) do
            (let (cp,h,fl,fr,ma)  = cast_ray_once s {o=ha,d=fra} let alb = ma with albedo.ap = (mat.albedo.ap*ma.albedo.ap) in (vec.(c + (vec.scale mat.albedo.ap cp)),(h,fl,fr,alb)))
    --jenk
    in u32color c


entry main [obj][la][tr](h:i64) (w:i64) (s:state[obj][la][tr]):[h][w]u32 = 
let s:state[obj][la][tr] = s with h = u32.i64 h with w = u32.i64 w 
    in map(\y -> 
    map(\x -> 
    ray_cast s y x
    )(0..<w)
    )(0..<h)