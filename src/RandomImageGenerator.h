#pragma once
/*
class RandomImageGenerator

This class reads filenames of a specific type from a folder and 
returns a list with all filenames.

Rafael Radkowski
Iowa State University
rafael@iastate.edu
Jan 25, 2019
MIT license
*/


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <time.h>

// opencv
#include <opencv2/opencv.hpp>

// local
#include "ReadImages.h" // read a set of random images from a file
#include "ImageLogReader.h" // load the log file 

using namespace std;


class RandomImageGenerator
{
public:

    /*
    Constructor
    @param image_height, image_width - the output image size in pixels. 
    */
    RandomImageGenerator(int image_height, int image_widht);
    ~RandomImageGenerator();

    /*
    Set the path to the folder containing the background images.
    @param image_path - vector of strings with the path to all files.
    @param type - the image type to be read.
    */
    void setImagePath(vector <string> image_paths, string type = "jpg");

     /*
    Set the path to the folder containing the rendered images along with the pose. 
    @param image_path - strings with the path to all files.
    @param type - the image type to be read.
    */
    void setRenderPath(string render_path, string type = "jpg");

    /*
    Set the output path and output type. 
    @param path - string containing the path to all output imags
    @param type - the output image type. 
    */
    void setOutputPath(string path);

    /*
    Start processing
    @param num_images - integer with the number of images to generate. 
    @return - number of stored images
    */
    int process(int num_images);


private:

    /*
    Adapt the loaded image to geometric constraints. 
    */
    cv::Mat adaptImage(cv::Mat& image);

    cv::Mat adaptRendering(cv::Mat& image, int& x, int& y, int& width, int& height);

    cv::Mat combineImages(cv::Mat image1, cv::Mat image2, int threshold);

	bool writeHeader(void);


	bool writeData(int id, cv::Mat& image, ImageLogReader::ImageLog& data, cv::Rect& roi);

    //----------------------------------------
    // members

    vector<string>  image_filenames;
    vector<ImageLogReader::ImageLog>		rendered_files;

    vector<string>  _image_path;
    string          _image_type;


    string          _render_path;
    string          _render_type;

	string			_output_path;
	string			_output_file_name;

    int _image_height; 
    int _image_widht;

    int _rendering_height; 
    int _rendering_widht;
};