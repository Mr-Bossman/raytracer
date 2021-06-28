
import "lib/github.com/athas/vector/vspace"

module vec = mk_vspace_3d f64
type vec3 = vec.vector


type light = {o:vec3,c:vec3}

type ray = {o:vec3,d:vec3}
type cam = {c:ray,fov:f64}
type albedo = {a:f64,D:f64,at:f64,ap:f64}
type material = {refractive_index:f64,albedo:albedo,diffuse_color:vec3,specular_exponent:f64}

type sphere = {o:vec3,r:f64,mat:material}


type triangle = {a:vec3,b:vec3,c:vec3,n:vec3,mat:material}
