
import "lib/github.com/athas/vector/vspace"

module vec = mk_vspace_3d f64
type vec3 = vec.vector

type ray = {o:vec3,d:vec3}

type sphere = {o:vec3,r:f64}


type triangle = (vec3,vec3,vec3,vec3)
entry Triangle (a: vec3) (b: vec3) (c: vec3) (d: vec3): triangle = (a,b,c,d) 