#include "voxelRenderer.hpp"
#include <cmath>
#include <GL/glut.h>
#include <iostream>

using namespace std;

	VoxelRenderer::VoxelRenderer( 
			unsigned int width, unsigned int height, unsigned int length, 
			unsigned char *data,
			float cube_w, float cube_h, float cube_d, 
			bool drawGrid, unsigned char threshold) 
:	width(width), height(height), length(length), data(data),
	cube_w(cube_w), cube_h(cube_h), cube_d(cube_d), 
	drawGrid(drawGrid), threshold(threshold)
{

	//for (int x = 0; x < size; x++) {
		//if(x % 10 != 0) 
			//continue;
		//for (int y = 0; y < size; y++) {
			//for (int z = 0; z < size; z++) {
				//tree.set(x,y,z, (x*x*y*z)%255);
			//}
		//}
	//}

	//for (int z = 0; z < size; z++ ) {
	//for (int y = 0; y < size; y++ ) {
	//for (int x = 0; x < size; x++ ) {
	//tree.set(x,y,z, 255);
	//}
	//}
	//}
}
		
unsigned char VoxelRenderer::getData(unsigned int x, unsigned int y, unsigned int z) {
	return data[z*width*height + y*width + x];
}
void VoxelRenderer::draw() {

	//draw grid
	if(drawGrid) {
		drawWireFrame();
	}

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
	drawSurface();
}

bool inline VoxelRenderer::isVisible(unsigned char voxel) {
	return (voxel > threshold);
}

void VoxelRenderer::drawNaive() {
	unsigned char voxel;

	//draw voxels
	for (unsigned int z = 0; z < length; z++ ) {
		for (unsigned int y = 0; y < height; y++ ) {
			for (unsigned int x = 0; x < width; x++ ) {
				voxel = getData(x, y, z);
				drawVoxel(voxel, x, y, z);
			}
		}
	}
}

void VoxelRenderer::drawSurface() {

	unsigned char left, right, up, down, front, back, current;

	for (unsigned int z = 0; z < length; z++ ) {
		for (unsigned int y = 0; y < height; y++ ) {
			for (unsigned int x = 0; x < width; x++ ) {
				right = (x == width - 1 ? 0 : getData(x+1,y,z));
				left = (x == 0 ? 0 : getData(x-1,y,z));

				up = (y == height - 1 ? 0 : getData(x,y+1,z));
				down = (y == 0 ? 0 : getData(x,y-1,z));

				front = (z == length - 1 ? 0 : getData(x,y,z+1));
				back = (z == 0 ? 0 : getData(x,y,z-1));

				current = getData(x,y,z);	

				drawVoxel(current, right, left, up, down, front, back, x,y,z);
			}
		}
	}
}


void inline VoxelRenderer::drawVoxel(
		unsigned char current, 
		unsigned char right, unsigned char left, 
		unsigned char up, unsigned char down, 
		unsigned char front, unsigned char back,
		int x, int y, int z) {

	if(!isVisible(current))	
		return;

	glTranslatef(x*cube_w, y*cube_h, z*cube_d);
	glColor3f(current/255.0f, current/255.0f, current/255.0f);

	if(!isVisible(up))
		drawQuad(UP);

	if(!isVisible(down))
		drawQuad(DOWN);

	if(!isVisible(right))
		drawQuad(RIGHT);

	if(!isVisible(left))
		drawQuad(LEFT);

	if(!isVisible(front))
		drawQuad(FRONT);

	if(!isVisible(back))
		drawQuad(BACK);

	glTranslatef(-x*cube_w, -y*cube_h, -z*cube_d);

}

void inline VoxelRenderer::drawQuad(Side side) {

	glBegin(GL_QUADS);

	switch(side) 
	{
		case UP: 
			{
				//glColor3f(0.0f, 1.0f, 0.0f);
				glNormal3f(0.0f, 1.0f, 0.0f);
				glVertex3f(0.0f, cube_h, 0.0f);
				glVertex3f(0.0f, cube_h, cube_d);
				glVertex3f(cube_w, cube_h, cube_d);
				glVertex3f(cube_w, cube_h, 0.0f);
				break;
			}

		case DOWN: 
			{
				//glColor3f(0.0f, 0.5f, 0.0f);
				glNormal3f(0.0f, -1.0f, 0.0f);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(0.0f, 0.0f, cube_d);
				glVertex3f(cube_w, 0.0f, cube_d);
				glVertex3f(cube_w, 0.0f, 0.0f);
				break;
			}

		case RIGHT: 
			{
				//glColor3f(1.0f, 0.0f, 0.0f);
				glNormal3f(1.0f, 0.0f, 0.0f);
				glVertex3f(cube_w, 0.0f, 0.0f);
				glVertex3f(cube_w, cube_h, 0.0f);
				glVertex3f(cube_w, cube_h, cube_d);
				glVertex3f(cube_w, 0.0f, cube_d);
				break;
			}
		case LEFT: 
			{
				//glColor3f(0.5f, 0.0f, 0.0f);
				glNormal3f(-1.0f, 0.0f, 0.0f);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(0.0f, cube_h, 0.0f);
				glVertex3f(0.0f, cube_h, cube_d);
				glVertex3f(0.0f, 0.0f, cube_d);
				break;
			}
		case FRONT: 
			{
				//glColor3f(0.0f, 0.0f, 1.0f);
				glNormal3f(0.0f, 0.0f, 1.0f);
				glVertex3f(0.0f, 0.0f, cube_d);
				glVertex3f(cube_w, 0.0f, cube_d);
				glVertex3f(cube_w, cube_h, cube_d);
				glVertex3f(0.0f, cube_h, cube_d);
				break;
			}
		case BACK: 
			{
				//glColor3f(0.0f, 0.0f, 0.5f);
				glNormal3f(0.0f, 0.0f, -1.0f);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(cube_w, 0.0f, 0.0f);
				glVertex3f(cube_w, cube_h, 0.0f);
				glVertex3f(0.0f, cube_h, 0.0f);
				break;
			}
	}

	glEnd();
}

void VoxelRenderer::drawVoxel(unsigned char voxel, const int x, const int y, const int z) {

	if(!isVisible(voxel)) 
		return;

	float dw, dh, dd;
	dw = 0.5f * cube_w;
	dh = 0.5f * cube_h;
	dd = 0.5f * cube_d;

	glTranslatef(x*cube_w + dw, y*cube_h + dh, z*cube_d + dd);
	glColor3f(voxel/255.0f, voxel/255.0f, voxel/255.0f);

	for (float dx = -dw; dx <= dw; dx+=cube_w) {
		glNormal3f(dx < 0.0f ? -1.0f : 1.0f, 0.0f, 0.0f);
		glBegin(GL_QUADS);
		glVertex3f(dx, dh, dd);
		glVertex3f(dx, -dh, dd);
		glVertex3f(dx, -dh, -dd);
		glVertex3f(dx, dh, -dd);
		glEnd();
	}
	for (float dy = -dh; dy <= dh; dy+=cube_h) {
		glNormal3f(0.0f, dy < 0.0f ? -1.0f : 1.0f, 0.0f);
		glBegin(GL_QUADS);
		glVertex3f(dw, dy, dd);
		glVertex3f(dw, dy, -dd);
		glVertex3f(-dw, dy, -dd);
		glVertex3f(-dw, dy, dd);
		glEnd();
	}
	for (float dz = -dd; dz <= dd; dz+=cube_d) {
		glNormal3f(0.0f, 0.0f, dz < 0.0f ? -1.0f : 1.0f);
		glBegin(GL_QUADS);
		glVertex3f(dw, dh, dz);
		glVertex3f(-dw, dh, dz);
		glVertex3f(-dw, -dh, dz);
		glVertex3f(dw, -dh, dz);
		glEnd();
	}

	glTranslatef(-x*cube_w - dw, -y*cube_h - dh, -z*cube_d - dd);
}

void VoxelRenderer::drawWireFrame() {

	GLfloat *v1,*v2;
	v1 = new GLfloat[3];
	v2 = new GLfloat[3];

	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glColor3f(1.0f, 1.0f, 1.0f);

for (unsigned int z = 0; z <= length; z++) {

		for (unsigned int y = 0; y <= height; y++) {

			v1[0] = 0; v1[1] = y*cube_h; v1[2] = z*cube_d;
			v2[0] = width*cube_w; v2[1] = y*cube_h; v2[2] = z*cube_d;

			glBegin(GL_LINE_STRIP);
			glVertex3fv(v1);
			glVertex3fv(v2);
			glEnd();
		}

		for (unsigned int x = 0; x <= width; x++) {
			v1[0] = x*cube_w; v1[1] = 0; v1[2] = z*cube_d;
			v2[0] = x*cube_w; v2[1] = height*cube_h; v2[2] = z*cube_d;

			glBegin(GL_LINE_STRIP);
			glVertex3fv(v1);
			glVertex3fv(v2);
			glEnd();
		}
	}

	for (unsigned int x = 0; x <= width; x++) {
		for (unsigned int y = 0; y <= height; y++) {
			v1[0] = x*cube_w; v1[1] = y*cube_h; v1[2] = 0;
			v2[0] = x*cube_w; v2[1] = y*cube_h; v2[2] = length*cube_d;

			glBegin(GL_LINE_STRIP);
			glVertex3fv(v1);
			glVertex3fv(v2);
			glEnd();
		}
	}



}


