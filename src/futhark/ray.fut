import "types"


entry Sphere (o: vec3) (r: f64): sphere = {o=o,r=r}
entry Ray (o: vec3) (d: vec3): ray = {o=o,d=d}
entry Vec (xs: f64) (ys: f64)(zs: f64) : vec3 = {x=xs,y=ys,z=zs}
entry Cam (c:ray)(fov: f64):cam = {c=c,fov=fov}

type state = {s:sphere,c:cam,h:u32,w:u32}
entry State (s: sphere)(c:cam)(h: u32)(w: u32): state = {s=s,c=c,h=h,w=w}
let EPSILON:f64 = 0.0001








let ray_sphere_intersect (r:ray) (s:sphere) =
    let L = vec.(s.o - r.o)
    let tca = vec.dot r.d L
    let d2 =  (vec.quadrance L) - (tca*tca)
    in if (d2 > s.r*s.r) then (0:f64,false) else
    let thc = f64.sqrt((s.r*s.r) - d2)
    let t0 = tca - thc
    let t1 = tca + thc
    in if (t0 < EPSILON) then 
    if (t1 < EPSILON) then (0,false) else (t1,true)
    else (t0,true)










let ray_cast (s:state) (h) (w):u32 = 
let x = ((f64.i64 w)+0.5)-((f64.u32 s.w)/2)
let y = -((f64.i64 h)+0.5)+((f64.u32 s.h)/2)
let z = -(f64.u32 s.h)/(2*f64.tan(s.c.fov/2))
let d = vec.normalise (vec.rot_y  s.c.c.d.y  (vec.rot_x  s.c.c.d.x {x=x,y=y,z=z}))
let r:ray = {o=s.c.c.o,d=d}
let (t0,hit) = ray_sphere_intersect r s.s 
in if(hit)then 16777215 else 0

entry main (h:i64) (w:i64) (s:state):[h][w]u32 = 
let s:state = s with h = u32.i64 h with w = u32.i64 w 
    in map(\y -> 
    map(\x -> 
    ray_cast s y x
    )(0..<w)
    )(0..<h)