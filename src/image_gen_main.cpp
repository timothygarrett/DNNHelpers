/*

-ipath ../data/imagenet -itype jpeg -rlog ./output/render_log.csv -img_w 512 -img_h 512 -o ./batch 

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

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>  // transformation
#include <glm/gtx/quaternion.hpp> // quaternions


// local
#include "RandomImageGenerator.h"
#include "Parser.h"

using namespace arlab;
using namespace std;

int main(int argc, char** argv)
{
	cout << "Image Generator\n" << endl;
	cout << "Generate images for cnn training" << endl;
	cout << "Rafael Radkowski" << endl;


	Parser::Arguments arg = Parser::Parse(argc, argv);

	if (!arg.valid) {
		return 1;
	}

	clock_t begin = clock();

	vector<string> path = { arg.background_images_path };
	RandomImageGenerator* generator = new RandomImageGenerator(arg.image_height, arg.image_width);
	generator->setImagePath(path, arg.background_images_type);
	generator->setRenderPath(arg.rendered_images_log_file);
	generator->setOutputPath(arg.output_path);
	int num = generator->process(arg.num_images);

	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC; 

	cout << "[INFO] - Generated " << num << " images (time = " << elapsed_secs <<  "s)." << endl;
	cout << "[INFO] - RGB format: CV_8UC3" << endl;
	cout << "[INFO] - Normal format: CV_16UC3" << endl;
	cout << "[DONE]" << endl;

	delete generator;


	return 1;
}
