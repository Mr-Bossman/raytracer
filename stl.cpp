#include "stl.h"
#include "common.h"

struct STL_Triangle_Head
{
    uint8_t header[80];
    uint32_t num;
};
std::vector<STL_Triangle> parsestl(std::string path){
    printf(path.c_str());
    STL_Triangle_Head tmp;
    std::vector<STL_Triangle> triangles; 
    std::ifstream input;
    input.open(path, std::ios::binary | std::ios::out | std::ios::in);
    printf("%d",input.tellg());
    if(input.tellg() < 84){
        printf("bro");
        return std::vector<STL_Triangle>{};
    }
    input.read( (char *)&tmp, 84 );

    if(((unsigned long int)input.tellg()) != (tmp.num * sizeof(STL_Triangle)+84)){
        printf("broskia");
        return std::vector<STL_Triangle>{};
    }
    triangles.resize(tmp.num);
    for(size_t i = 0;i < tmp.num;i++){
        input.read( (char *)&(triangles[i]), sizeof(STL_Triangle) );
    }

    return triangles;
}
