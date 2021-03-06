/*

//../data/stanford_bunny_02_rot.obj  -img_w 1280 -img_h 1024 -m SPHERE  -seg 5 -rows 5 -rad 1.3 -verbose
// ../data/stanford_bunny_02_rot.obj  -img_w 1280 -img_h 1024 -m POLY -sub 0  -rad 1.3 -verbose
// ../data/stanford_bunny_02_rot.obj -o debug_out -img_w 1280 -img_h 1024 -m POLY -sub 2 -level 2  -rad 1.3  -verbose
// ../data/stanford_bunny_02_rot.obj -o debug_out -img_w 1280 -img_h 1024 -m POSE -num 1000 -sub 4 -lim_near 1.0 -lim_far 3.0 -verbose

Rafael Radkowski
Iowa State University
rafael@iastate.edu
+1 (515) 294 7044
Jan 2019
All copyrights reserved
*/

#include <iostream>
#include <string>
#include <time.h>


// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>  // transformation
#include <glm/gtx/quaternion.hpp> // quaternions

// glfw includes
#include <GLFW/glfw3.h>

// local
#include "Window.h" // the windows
#include "OpenGLDefaults.h" // some open gl and glew defaults
#include "VertexBuffers.h"  // create vertex buffer object
#include "ShaderProgram.h"  // create a shader program
#include "CommonTypes.h"  // types that all projects use
#include "ModelCoordinateSystem.h"
#include "ModelRenderer.h"
#include "SphereCoordRenderer.h"
#include "PolyhedronViewRenderer.h"
#include "RandomPoseViewRenderer.h"
#include "BalancedPoseTree.h"
#include "Model3D.h"
#include "ArgParser.h"
#include "types.h"

using namespace cs557;



//------------------------------------------------------------
//
//	Some global variables to keep track of

// The handle to the window object
GLFWwindow *window = NULL;

// Transformation pipeline variables
glm::mat4 projectionMatrix; // Store the projection matrix
glm::mat4 viewMatrix;       // Store the view matrix



// Set up our green background color
GLfloat clear_color[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat clear_depth[] = {1.0f, 1.0f, 1.0f, 1.0f};


//------------------------------------------------------------
//
//	Models


arlab::CameraModel				cam_control = arlab::CameraModel::USER;
SphereCoordRenderer*				sphere_renderer = NULL;
PolyhedronViewRenderer*			poly_renderer = NULL;
BalancedPoseTree*				tree_renderer = NULL;
RandomPoseViewRenderer*			pose_renderer = NULL;
Model3D*							model_renderer = NULL;

using namespace std;
using namespace cs557;
using namespace arlab;

void InitWindow(void)
{

	//cout << _MSC_VER << endl;


	// Init the GLFW Window
    window = cs557::initWindow("Render to Image");

    // Initialize the GLEW apis
    cs557::initGlew();

}


void InitRenderer(Arguments& opt)
{
     //---------------------------------------------------------
    // Create models
	cam_control = opt.cam;
	if (opt.cam == SPHERE) {
		sphere_renderer = new SphereCoordRenderer(opt.windows_width, opt.window_height, opt.image_width, opt.image_height);
		sphere_renderer->setVerbose(opt.verbose); // set first to get all the output info
		sphere_renderer->setModel(opt.model_path_and_file);
		sphere_renderer->setOutputPath(opt.output_path);
		sphere_renderer->createSphereGeometry(opt.camera_distance, opt.segments, opt.rows);
		
	}
	else if (opt.cam == POLY)
	{
		poly_renderer = new PolyhedronViewRenderer(opt.windows_width, opt.window_height, opt.image_width, opt.image_height);
		poly_renderer->setVerbose(opt.verbose); // set first to get all the output info
		poly_renderer->setModel(opt.model_path_and_file);
		poly_renderer->setOutputPath(opt.output_path);
		poly_renderer->create(opt.camera_distance, opt.subdivisions);
		
	}
	else if (opt.cam == TREE)
	{
		tree_renderer = new BalancedPoseTree(opt.windows_width, opt.window_height, opt.image_width, opt.image_height);
		tree_renderer->setVerbose(opt.verbose); // set first to get all the output info
		tree_renderer->setModel(opt.model_path_and_file);
		tree_renderer->setOutputPath(opt.output_path);
		tree_renderer->create(opt.camera_distance, opt.bpt_levels);
	}
	else if (opt.cam == POSE)
	{
		pose_renderer = new RandomPoseViewRenderer(opt.windows_width, opt.window_height, opt.image_width, opt.image_height);
		pose_renderer->setVerbose(opt.verbose); // set first to get all the output info
		pose_renderer->setModel(opt.model_path_and_file);
		pose_renderer->setOutputPath(opt.output_path);
		pose_renderer->create(opt.num_images, opt.subdivisions);
	}
	else if (opt.cam == USER)
	{
		model_renderer = new Model3D();
		model_renderer->create(opt.model_path_and_file);
	}
}


void DrawLoop(void)
{
    // Enable depth test
    glEnable(GL_DEPTH_TEST); 
    glEnable(GL_BLEND); 
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

	 // Init the view matrix. 
	viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0, 1.3f), glm::vec3(0.0f, 0.0f, 00.f), glm::vec3(0.0f, 1.0f, 0.0f));
    cs557::InitControlsViewMatrix(viewMatrix);

	clock_t begin = clock();

    while (!glfwWindowShouldClose(window))
    {
        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR, 0, clear_color);
        glClearBufferfv(GL_DEPTH, 0, clear_depth);

        

		bool ret = false;
		switch (cam_control)
		{
		case USER:
			if (model_renderer != NULL)
				model_renderer->draw(cs557::GetCamera().getViewMatrix());
			break;
		case SPHERE:
			if(sphere_renderer != NULL)
				ret = sphere_renderer->draw_sequence();
			break;
		case	 POLY:
			if (poly_renderer != NULL)
				ret = poly_renderer->draw_sequence();
			break;
		case TREE:
			if (tree_renderer != NULL)
				ret = tree_renderer->draw_sequence();
			break;
		case POSE:
			if (pose_renderer != NULL)
				ret = pose_renderer->draw_sequence();
			break;
		}


        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();

		if (ret) {
			break;
		}
    }

	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC; 

	int num = 0;
	if(sphere_renderer != NULL)
		num = sphere_renderer->size();
	else if (poly_renderer != NULL)
		num = poly_renderer->size();
	else if (tree_renderer != NULL)
		num = tree_renderer->size();
	else if (pose_renderer != NULL)
		num = pose_renderer->size();

	cout << "\n[INFO] - Generated " << num << " images (time = " << elapsed_secs <<  "s)." << endl;
}






int main(int argc, char** argv) 
{	
	cout << "\n--------------------------------------" << endl;
	cout << "Dataset Renderer" << endl;
	cout << "Create RGB color maps, depth images (float), and normal maps (float) from a 3D model \n" << endl;
	cout << "Rafael Radkowski" << endl;
	cout << "Iowa State University" << endl;
	cout << "Rafael@iastate.edu" << endl;
	cout << "Jan 2019, All copyrights reserved." << endl;

	Arguments options = ArgParser::Parse(argc, argv);

	if (options.valid == false) return -1;

	// Init the output window
	InitWindow();

	// Init the image renderer 
	InitRenderer(options);

	// Start rendering
	DrawLoop();

	// The end
	if(sphere_renderer != NULL) delete sphere_renderer;
	if (poly_renderer != NULL) delete poly_renderer;
	if (tree_renderer != NULL) delete tree_renderer;
	if (model_renderer != NULL) delete model_renderer;
	if (pose_renderer != NULL) delete pose_renderer;

	return 1;
}