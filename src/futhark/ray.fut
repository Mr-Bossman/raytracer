import "types"


entry Sphere (o: vec3) (r: f64): sphere = {o=o,r=r}
entry Ray (o: vec3) (d: vec3): ray = {o=o,d=d}
entry Vec (xs: f64) (ys: f64)(zs: f64) : vec3 = {x=xs,y=ys,z=zs}

type state = {s:sphere,h:u32,w:u32}
entry State (s: sphere)(h: u32)(w: u32): state = {s=s,h=h,w=w}

let ray_cast (s:state) (x) (y):u32 = u32.i64 (x*y)
entry main (h:i64) (w:i64) (s:state):[h][w]u32 = 
    let s:state = {s=s.s,h=(u32.i64 h),w=(u32.i64 w)}
    in map(\x -> 
    map(\y -> 
    ray_cast s x y
    )(0..<w)
    )(0..<h)