#include "ArgParser.h"


using namespace arlab;


namespace ArgParserTypes{

	Arguments opt;
	int error_count = 0;
}

using namespace ArgParserTypes;


/*
Parse the arguments
@param argc - number of arguments
@param argv - the argument line
@return struct with arguments
*/
Arguments ArgParser::Parse(int& argc, char** argv)
{
	//cout << argc << endl;
	//cout << argv[0] << endl;

	if (argc < 2) {
		Help();
		return opt;
	}

	opt.valid = true;

	// extract the path
	Path(argv[0]);

	// get the model
	opt.model_path_and_file = argv[1];

	int pos = 2;
	while(pos < argc)
	{
		string c_arg(argv[pos]); 

		if (c_arg.compare("-o") == 0) { // output path
			if (argc >= pos+1) opt.output_path = string(argv[pos+1]);
			else ParamError(c_arg);
		}
		else if(c_arg.compare("-img_w") == 0){ // image width 
			if (argc >= pos) opt.image_width = atoi( string(argv[pos+1]).c_str() );
			else ParamError(c_arg);
		}
		else if(c_arg.compare("-img_h") == 0){ // image width 
			if (argc >= pos) opt.image_height =atoi(  string(argv[pos+1]).c_str() );
			else ParamError(c_arg);
		}
		else if(c_arg.compare("-wnd_w") == 0){ // window width 
			if (argc >= pos) opt.windows_width = atoi( string(argv[pos+1]).c_str() );
			else ParamError(c_arg);
		}
		else if(c_arg.compare("-wnd_h") == 0){ // window height  
			if (argc >= pos) opt.window_height =atoi(  string(argv[pos+1]).c_str() );
			else ParamError(c_arg);
		}
		else if(c_arg.compare("-m") == 0){ // method
			if (argc >= pos) opt.cam = CameraModelEnum(string(argv[pos+1]));
			else ParamError(c_arg);
		}
		else if(c_arg.compare("-sub") == 0){ // number of subdivisions for the polyheder
			if (argc >= pos) opt.subdivisions = atoi(  string(argv[pos+1]).c_str() );
			else ParamError(c_arg);
		}
		else if(c_arg.compare("-seg") == 0){ // sphere segments   
			if (argc >= pos) opt.segments =atoi(  string(argv[pos+1]).c_str() );
			else ParamError(c_arg);
		}
		else if(c_arg.compare("-rows") == 0){ // sphere rows   
			if (argc >= pos) opt.rows =atoi(  string(argv[pos+1]).c_str() );
			else ParamError(c_arg);
		}
		else if(c_arg.compare("-rad") == 0){ // sphere rows   
			if (argc >= pos) opt.camera_distance = atof(  string(argv[pos+1]).c_str() );
			else ParamError(c_arg);
		}
		else if(c_arg.compare("-intr") == 0){ // intrinsic file
			if (argc >= pos) opt.intrincic_params_file = string(argv[pos + 1]).c_str();
			else ParamError(c_arg);
		}
		else if(c_arg.compare("-help") == 0 || c_arg.compare("-h") == 0){ // help
			Help();
		}
		else if(c_arg.compare("-verbose") == 0 ){ // help
			opt.verbose = true;
		}
	

		pos++;
	}


	if (opt.verbose)
		Display();

	return opt;
}


/*
Display help
*/
//static
void ArgParser::Help(void)
{
	cout << "[ERROR] - Missing parameters\nUsage:" << endl;
	cout << "DatasetRenderer [3d model path and filename] " << endl;
	cout << "Optional parameters:" << endl;
	cout << "\t-intr [param] - path and filename for the intrinsic camera parameters." << endl;
	cout << "\t-o [param] - set the output path" << endl;
	cout << "\t-img_w [param] \t- set the widht of the output image in pixels (integer)." << endl;
	cout << "\t-img_h [param] \t- set the height of the output image in pixels (integer)." << endl;
	cout << "\t-wnd_w [param] \t- set the widht of the application window in pixels (integer)." << endl;
	cout << "\t-wnd_h [param] \t- set the height of the application window in pixels (integer)." << endl;
	cout << "\t-m [param] \t- set the camera path models. Can be SPHERE, POLY." << endl;
	cout << "\t-seg [param] \t- for the camera path SPHERE model, set the number of segments (integer)." << endl;
	cout << "\t-rows [param] \t-for the camera path SPHERE model, set the number of rows (integer)" << endl;
	cout << "\t-dist [param] \t-for the camera path SPHERE model, set the sphere radius (float)" << endl;
	cout << "\t-sub [param] \t-for the camera path POLY model, set the number of subdivisions for the polyheder (int)" << endl;
	cout << "\t-verbose [param] \t- displays additional information." << endl;
	cout << "\t-help [param] \t- displays this help menu" << endl;

	cout << "\nExample: DatasetRenderer ../data/stanford_bunny_02_rot.obj -m POLY -img_w 1280 -img_h 1024 -wnd_w 1280 - wnd_h 1024 -o output -sub=1 -rad 1.3\n" << endl;
}


/*
Display all arguments
*/
//static 
void ArgParser::Display(void)
{
	std::cout << "\nParameters:" << endl;
	std::cout << "Model:\t" << opt.model_path_and_file << endl;
	std::cout << "Intrinsic param file:\t" << opt.intrincic_params_file << endl;
	std::cout << "Output path:\t" << opt.output_path << endl;
	//std::cout << "Path:\t" << opt.current_path << endl;
	std::cout << "Camera path: " << CameraModelString(opt.cam) << endl;
	if (opt.cam == SPHERE) {
		std::cout << "Sphere segments: " << opt.segments << endl;
		std::cout << "Sphere rows: " << opt.rows << endl;
		std::cout << "Sphere radius: " << opt.camera_distance << endl;
	}
	if (opt.cam == POLY) {
		std::cout << "Polyheder subdivisions: " << opt.subdivisions << endl;
		std::cout << "Polyheder radius: " << opt.camera_distance << endl;
	}
	std::cout << "Image width:\t" << opt.image_width << endl;
	std::cout << "Image height:\t" << opt.image_height << endl;
	std::cout << "Wnd width:\t" << opt.windows_width << endl;
	std::cout << "Wnd height:\t" << opt.window_height << endl;


}


/*
Extract the current path
*/
//static 
void ArgParser::Path(char* path)
{
	string str(path);

	int idx = str.find_last_of("\\");
	if(idx == -1){ 
		idx = str.find_last_of("/");
	}
	opt.current_path = str.substr(0, idx+1);
}

/*
Extract the current path
*/
//static 
void ArgParser::ParamError(string option)
{
	cout << "[ERROR] - Parameter for option " << option << " is missing or invalid." << endl;
	error_count++;
}