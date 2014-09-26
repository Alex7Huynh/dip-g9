#include "Threshold.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
using namespace std;
using namespace cv;

// Laplace
Mat Laplace(char * srcImage);
// Sobel
Mat Sobel(char * srcImage);
// Smoothing
int display_caption( const char* caption );
int display_dst( int delay );

/// Global variables

void main()
{	
    Mat dst;
	char* filename;
	filename = new char[100];
	bool bInput = true;
	
	if(bInput) {
		cout << "Enter image's filename (e.g. P4N.bmp): ";
		cin >> filename;
		cout << filename;
	} else {
		filename = "P4N.bmp";
	}

	if (src.empty())
		src = imread( filename, 1 );

	//dst = Laplace(filename);	// apply Laplace
	//dst = Sobel(filename);	// apply Sobel
	
	//Threshold(filename);

	cvtColor( src, src_gray, COLOR_RGB2GRAY );
	threshold( src_gray, dst, 80, max_BINARY_value, 1 );

	
    imshow( "Sludge Particles Detection", dst );
    waitKey(0);
}

Mat Laplace(char * srcImage)
{
    Mat src, src_gray, dst;
	Mat abs_dst;

    int kernel_size = 3;
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;    
	const char* window_name = "Laplace Demo";
    /// Load an image
    //src = imread( "P4N.bmp" );
	src = imread( srcImage );

    if ( !src.data )
        return abs_dst;

    /// Remove noise by blurring with a Gaussian filter
    GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );

    /// Convert the image to grayscale
    cvtColor( src, src_gray, COLOR_RGB2GRAY );

    /// Create window
    //namedWindow( window_name, WINDOW_AUTOSIZE );

    /// Apply Laplace function
    Laplacian( src_gray, dst, ddepth, kernel_size, scale, delta, BORDER_DEFAULT );
	convertScaleAbs( dst, abs_dst );

	return abs_dst;
}
Mat Sobel(char * srcImage)
{
    Mat src, src_gray;
    Mat grad;
    const char* window_name = "Sobel Demo - Simple Edge Detector";
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;

    /// Load an image
    src = imread(srcImage);

    if ( !src.data )
    {        return grad;    }

    GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );

    /// Convert it to gray
    cvtColor( src, src_gray, COLOR_RGB2GRAY );

    /// Create window
    //namedWindow( window_name, WINDOW_AUTOSIZE );

    /// Generate grad_x and grad_y
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;

    /// Gradient X
    //Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
    Sobel( src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_x, abs_grad_x );

    /// Gradient Y
    //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
    Sobel( src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_y, abs_grad_y );

    /// Total Gradient (approximate)
    addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );

    return grad;
}