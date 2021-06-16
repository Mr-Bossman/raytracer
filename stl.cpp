#include "stl.h"
#include "common.h"

struct STL_Triangle_Head
{
    uint8_t header[80];
    uint32_t num;
};
std::vector<STL_Triangle> parsestl(std::string path){
    STL_Triangle_Head tmp;
    std::vector<STL_Triangle> triangles; 
    std::ifstream input;
    input.open(path, std::ios::binary | std::ios::out | std::ios::in);
    input.seekg(0,std::ios_base::end);
    unsigned long int size = input.tellg();
    if(size < 84){
        return std::vector<STL_Triangle>{};
    }
    input.seekg(0,std::ios_base::beg);
    input.read( (char *)&tmp, 84 );

    if(size != (tmp.num * 50)+84){
        return std::vector<STL_Triangle>{};
    }
    triangles.resize(tmp.num);
    for(size_t i = 0;i < tmp.num;i++){
        input.read( (char *)&(triangles[i]), sizeof(STL_Triangle) );
    }
    return triangles;
}
