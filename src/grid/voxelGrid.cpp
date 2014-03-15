
#include "voxelGrid.hpp"
#include <cmath>

VoxelGrid::VoxelGrid(unsigned char *gridData,
		const float gridRealWidth, 
		const float gridRealHeight, 
		const float gridRealLength, 
		const float deltaGrid) :
	gridData(gridData),
	gridWidth(ceil(gridRealWidth/deltaGrid)),
	gridHeight(ceil(gridRealHeight/deltaGrid)),
	gridLength(ceil(gridRealLength/deltaGrid)),
	deltaGrid(deltaGrid){
	}

VoxelGrid::VoxelGrid(unsigned char *gridData,
		const unsigned int gridWidth, 
		const unsigned int gridHeight, 
		const unsigned int gridLength, 
		const float deltaGrid) :
	gridData(gridData),
	gridWidth(gridWidth),
	gridHeight(gridHeight),
	gridLength(gridLength),
	deltaGrid(deltaGrid){
	}
	
VoxelGrid::~VoxelGrid () {
}

unsigned int VoxelGrid::width() const {
	return this->gridWidth;
}
unsigned int VoxelGrid::height() const {
	return this->gridHeight;
}
unsigned int VoxelGrid::length() const {
	return this->gridLength;
}
unsigned int VoxelGrid::voxelSize() const {
	return this->deltaGrid;
}
unsigned int VoxelGrid::dataSize() const {
	return this->gridWidth * this->gridHeight * this->gridLength;
}

