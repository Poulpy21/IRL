#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>

#define _USE_MATH_DEFINES 
#define ONE_DEG_IN_RAD 0.0174532925

#include "log.hpp"
#include "window.hpp"
#include "polygon.hpp"
#include "shader.hpp"
#include "utils.hpp"
#include "image.hpp"
#include "data.hpp"
#include "camera.hpp"
#include "LocalizedUSImage.hpp"
#include "image.hpp"

#define USE_ROW_MAJOR GL_TRUE

using namespace std;

int g_gl_width;
int g_gl_height;
bool resize = false;

void glfw_error_callback (int error, const char* description) {
	clog << "\nGLFW Error\t" << error << " : " << "\n\t" << description << "\n"; 
}


void glfw_window_size_callback (GLFWwindow* window, int width, int height) {
  g_gl_width = width;
  g_gl_height = height;
  resize = true;
}

double _update_fps_counter (GLFWwindow* window) {
	static double previous_seconds = glfwGetTime ();
	static int frame_count;
	double current_seconds = glfwGetTime ();
	double elapsed_seconds = current_seconds - previous_seconds;
	if (elapsed_seconds > 0.25) {
		previous_seconds = current_seconds;
		double fps = (double)frame_count / elapsed_seconds;
		char tmp[128];
		sprintf (tmp, "opengl @ fps: %.2f", fps);
		glfwSetWindowTitle (window, tmp);
		frame_count = 0;
	}
	frame_count++;

	return elapsed_seconds;
}

int main( int argc, const char* argv[] )
{
	initLogs();
	
	LocalizedUSImage::initialize();
	LocalizedUSImage img("data/imagesUS/IQ[data #0 (RF Grid).mhd");
	Mat m(img.getHeight(), img.getWidth(), CV_32F, img.getImageData());
	cout << img;
	Image::displayImage(m);
	
	log_console.info("END OF MAIN PROGRAMM");

	return 0;
	//Data data;
	//data.loadData("data/data.txt");
	//data.printData();

	//Image img;	
	//img.loadImageFolder("img/");
	//img.computeGradientVectorFlow();
	

	glfwSetErrorCallback (glfw_error_callback);

	clog << "\nStarting GLFW version " << glfwGetVersionString();

	if (!glfwInit()) {
		clog <<  "ERROR: could not start GLFW3\n";
		return 1;
	} 

	//glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 4);
	//glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint (GLFW_SAMPLES, 4);

	GLFWmonitor* mon = glfwGetPrimaryMonitor ();
	const GLFWvidmode* vmode = glfwGetVideoMode (mon);

	Window *wd = new Window(vmode->width, vmode->height ,"test");
	g_gl_width = vmode->width;
	g_gl_height = vmode->height;
	resize = true;

	glfwSetWindowSizeCallback(wd->getWindow(), glfw_window_size_callback);
	
	//-- camera --
	float camSpeed = 1.0f;
	float camYawSpeed = 1.0f;

	vec3 initialPos = {0.0f,0.0f,0.0f};

	float initialRotationData[3] = {0.0f, 0.0f, 0.0f};

	Rotation initialRotation(INTRINSIC_ROTATION, EULER_ORIENTATION, ORDER_ZYZ, initialRotationData);
	
	Camera camera(0.1f, 100.0f, 67.0f * ONE_DEG_IN_RAD, (float) g_gl_width / (float) g_gl_height, initialPos, initialRotation, ALL_AXES); 
	// -----------

	log_gl_params();

	float points[] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,

		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	};

	float colours[] = {
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f
	};

	Polygon *triangle = new Polygon(points, colours, 36*3);

	unsigned int vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray (vao);
	glBindBuffer (GL_ARRAY_BUFFER, triangle->getVertexBufferObject());
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*) NULL);

	glBindBuffer (GL_ARRAY_BUFFER, triangle->getColorBufferObject());
	glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*) NULL);
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	Shader *vs = new Shader("shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
	Shader *fs = new Shader("shaders/fragment_shader.glsl", GL_FRAGMENT_SHADER);
	
	//GL_LINK_STATUS && glGetProgramInfoLog
	unsigned int shader_programme = glCreateProgram ();
	glAttachShader (shader_programme, fs->getShader());
	glAttachShader (shader_programme, vs->getShader());
	glLinkProgram (shader_programme);

	int camera_view_mat_location = glGetUniformLocation (shader_programme, "camera_view");
	int camera_proj_mat_location = glGetUniformLocation (shader_programme, "camera_proj");
	int model_mat_location = glGetUniformLocation (shader_programme, "model_matrix");

	cout << "\n" << camera_view_mat_location << "\t" << camera_proj_mat_location << "\t" << model_mat_location << "\n";

	assert(camera_view_mat_location != -1);
	assert(camera_proj_mat_location != -1);
	assert(model_mat_location != -1);

	glUseProgram (shader_programme);
	glUniformMatrix4fv (camera_view_mat_location, 1, USE_ROW_MAJOR, camera.getViewMatrix());

	glUseProgram (shader_programme);
	glUniformMatrix4fv (camera_proj_mat_location, 1, USE_ROW_MAJOR, camera.getProjectionMatrix());

	float model_matrix[16] = { 1.0f, 0.0f, 0.0f, -0.5f,
				0.0f, 1.0f, 0.0f, -0.5f,
				0.0, 0.0f, 1.0f, -0.5f,
				0.0f, 0.0f, 0.0f, 1.0f};
	glUniformMatrix4fv (model_mat_location, 1, USE_ROW_MAJOR, model_matrix);
	
	double elapsed_seconds;
	while (!glfwWindowShouldClose(wd->getWindow())) {
		elapsed_seconds = _update_fps_counter (wd->getWindow());

		// wipe the drawing surface clear
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glViewport (0, 0, g_gl_width, g_gl_height);

		glUseProgram (shader_programme);
		
		glBindVertexArray (vao);

		// draw points 0-3 from the currently bound VAO with current in-use shader
		glDrawArrays (GL_TRIANGLES, 0, 36*3);
		
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers (wd->getWindow());

		// update other events like input handling 
		glfwPollEvents ();
	
		if (GLFW_PRESS == glfwGetKey(wd->getWindow(), GLFW_KEY_ENTER)) {
			glfwSetWindowShouldClose (wd->getWindow(), 1);
		}

		// control keys
		bool cam_moved = false;
		if (glfwGetKey (wd->getWindow(), GLFW_KEY_X)) {
			cout << "\nEllapsed seconds" << elapsed_seconds;
			camera.rotate(camYawSpeed * elapsed_seconds, 0,0);
			cam_moved = true;
		}
		if (glfwGetKey (wd->getWindow(), GLFW_KEY_Y)) {
			camera.rotate(0, camYawSpeed * elapsed_seconds, 0);
			cam_moved = true;
		}
		if (glfwGetKey (wd->getWindow(), GLFW_KEY_A)) {
			camera.rotate(0, 0, camYawSpeed * elapsed_seconds);
			cam_moved = true;
		}
		
		if (glfwGetKey (wd->getWindow(), GLFW_KEY_LEFT)) {
			camera.translate(-camSpeed * elapsed_seconds, 0, 0);
			cam_moved = true;
		}
		if (glfwGetKey (wd->getWindow(), GLFW_KEY_RIGHT)) {
			camera.translate(+camSpeed * elapsed_seconds, 0, 0);
			cam_moved = true;
		}
		if (glfwGetKey (wd->getWindow(), GLFW_KEY_UP)) {
			camera.translate(0, +camSpeed * elapsed_seconds, 0);
			cam_moved = true;
		}
		if (glfwGetKey (wd->getWindow(), GLFW_KEY_DOWN)) {
			camera.translate(0, -camSpeed * elapsed_seconds, 0);
			cam_moved = true;
		}
		if (glfwGetKey (wd->getWindow(), GLFW_KEY_PAGE_UP)) {
			camera.translate(0, 0, +camSpeed * elapsed_seconds);
			cam_moved = true;
		}
		if (glfwGetKey (wd->getWindow(), GLFW_KEY_PAGE_DOWN)) {
			camera.translate(0, 0, -camSpeed * elapsed_seconds);
			cam_moved = true;
		}

		// update view matrix
		if (cam_moved) {
			glUniformMatrix4fv (camera_view_mat_location, 1, USE_ROW_MAJOR, camera.getViewMatrix());
			//cout << "\n\n " << printMat4(camera.getViewMatrix());
		}
		
		if(resize) {
			camera.setAspectRatio((float) g_gl_width / (float) g_gl_height);
			glUniformMatrix4fv (camera_proj_mat_location, 1, USE_ROW_MAJOR, camera.getProjectionMatrix());
			resize = false;
		}

	}

	return 0;
}

