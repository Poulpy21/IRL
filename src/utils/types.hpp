
#include <iostream>
#include <string>

#ifndef TYPES_H
#define TYPES_H

using namespace std;

struct vec3 {
	float x;
	float y;
	float z;
};

struct vec4 {
	float x;
	float y;
	float z;
	float t;
};

typedef float *mat3;
typedef float *mat4;

string printVec3(vec3 v);
string printVec4(vec4 v);

string printMat3(mat3 m);
string printMat4(mat4 m);

#endif /* end of include guard: TYPES_H */
