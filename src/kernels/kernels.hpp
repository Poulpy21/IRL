
#ifndef _KERNELS_H_
#define _KERNELS_H_

#include "cuda.h"
#include "cuda_runtime.h"

namespace kernel {

void castKernel(const int nImages, const int imgWidth, const int imgHeight, float *float_data, unsigned char *char_data);

void VNNKernel(
		int nImages, int imgWidth, int imgHeight, 
		float deltaGrid, float deltaX, float deltaY,
		float xMin, float yMin, float zMin,
		unsigned int voxelGridWidth, unsigned int voxelGridHeight, unsigned int voxelGridLength,
		float **offsets_d,
		float **rotations_d,
		unsigned char *char_image_data, unsigned char *voxel_data, unsigned char *hit_counter, 
		cudaStream_t stream);

enum NormalType { NORMAL_PER_QUAD, NORMAL_PER_VERTEX};
enum ColorType { COLOR_PER_QUAD, COLOR_PER_VERTEX};

unsigned int computeQuads(float **h_quads, float **h_normals, float **h_colors, 
		unsigned char* d_voxel_grid,
		const unsigned int gridWidth, const unsigned int gridHeight, const unsigned int gridLength,
		const float cube_w, const float cube_h, const float cube_d,
		const unsigned char threshold, 
		const NormalType nt=NORMAL_PER_QUAD, const ColorType ct=COLOR_PER_QUAD);
}

#endif



