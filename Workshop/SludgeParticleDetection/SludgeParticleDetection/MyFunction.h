#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
using namespace std;
using namespace cv;

char* window_name = "Sludge Particle Detection";
/// Global Variables
int DELAY_CAPTION = 1500;
int DELAY_BLUR = 100;
int MAX_KERNEL_LENGTH = 31;

int threshold_value = 0;
int threshold_type = 3;
int const max_value = 255;
int const max_type = 4;
int const max_BINARY_value = 255;

Mat Laplace(char * srcImage)
{
    Mat src, src_gray, dst;
	Mat abs_dst;

    int kernel_size = 3;
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;    
	
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
Mat CannyThreshold(int, void*, Mat src, Mat src_gray)
{
    int edgeThresh = 1;
	int lowThreshold = 50;
	int const max_lowThreshold = 100;
	int ratio = 3;
	int kernel_size = 3;
	Mat dst, detected_edges;
	/// Reduce noise with a kernel 3x3
    blur( src_gray, detected_edges, Size(3,3) );

    /// Canny detector
    Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );

    /// Using Canny's output as a mask, we display our result
    dst = Scalar::all(0);

    src.copyTo( dst, detected_edges);    

	return dst;
}
Mat thresh_callback(int, void*, Mat src_gray)
{
    int thresh = 100;
	int max_thresh = 255;
	RNG rng(12345);
	Mat canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    /// Detect edges using canny
    Canny( src_gray, canny_output, thresh, thresh*2, 3 );
    /// Find contours
    findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    /// Draw contours
    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
    for ( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
    }

	return drawing;
}
int DisplayCaption( const char* caption , Mat mat)
{
	Mat tmp = Mat::zeros( mat.size(), mat.type() );
	putText( tmp, caption, Point( mat.cols/4, mat.rows/2),
			FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255) );	
	
	imshow( window_name, tmp );
	int c = waitKey( DELAY_CAPTION );
	if( c >= 0 ) { return -1; }
	return 0;
}
int DisplayImg( int delay, Mat img)
{
	
	imshow(window_name, img);
	int c = waitKey ( delay );
	if( c >= 0 ) { return -1; }
	return 0;
}
Mat Transform7Steps(Mat src)
{
	/// Convert to grayscale
	Mat src_gray;
	cvtColor( src, src_gray, COLOR_RGB2GRAY );
	
	/// Apply Histogram Equalization
	Mat dst;
	equalizeHist(src_gray, dst);
	
	/// Apply erosion
	Mat dstErosed;
	int erosion_size = 3;
	int erosion_type = MORPH_RECT;
	Mat element = getStructuringElement( erosion_type, Size( 2*erosion_size + 1, 2*erosion_size+1 ), Point( erosion_size, erosion_size ) );
	erode( dst, dstErosed, element );

	/// Apply threshold
	Mat dstThreshold;
	threshold( dstErosed, dstThreshold, 80, max_BINARY_value, 1 );

	/// Invert color
	Mat dstInvert;
	bitwise_not (dst, dstInvert);

	/// Canny edge detection
	Mat dstCanny;
	dstCanny = CannyThreshold(0, 0, src, dstInvert);

	/// Find contours
	Mat dstContours;
	dstContours = thresh_callback( 0, 0, dstCanny);

	return dstContours;
}
void AutoDisplay(char* filename)
{
	namedWindow(window_name, WINDOW_NORMAL);
	Mat src, src_gray, dst;
	src = imread( filename, 1 );	


    if ( DisplayCaption( "Original Image", src) != 0 ) { return; }
    //dstSmooth = srcSmooth.clone();
    if ( DisplayImg( DELAY_CAPTION, src) != 0 ) { return; }


    if ( DisplayCaption( "Convert to grayscale", src) != 0 ) { return; }
    cvtColor( src, src_gray, COLOR_RGB2GRAY );
	if ( DisplayImg( DELAY_CAPTION, src_gray) != 0 ) { return; }


    if ( DisplayCaption("Apply Histogram Equalization", src_gray) != 0 ) { return; }
	equalizeHist(src_gray, dst);    
	if ( DisplayImg( DELAY_CAPTION, dst) != 0 ) { return; }


    if ( DisplayCaption( "Apply erosion", dst) != 0 ) { return; }
	Mat dstErosed;
	int erosion_size = 3;
	int erosion_type = MORPH_RECT;
	Mat element = getStructuringElement( erosion_type, Size( 2*erosion_size + 1, 2*erosion_size+1 ), Point( erosion_size, erosion_size ) );
	erode( dst, dstErosed, element );
	if ( DisplayImg( DELAY_CAPTION, dstErosed) != 0 ) { return; }
	
	
	if ( DisplayCaption( "Apply threshold", dstErosed) != 0 ) { return; }
	Mat dstThreshold;
	threshold( dstErosed, dstThreshold, 80, 255, 1 );
	if ( DisplayImg( DELAY_CAPTION, dstThreshold) != 0 ) { return; }

	
	if ( DisplayCaption( "Invert color", dstThreshold) != 0 ) { return; }
	Mat dstInvert;
	bitwise_not (dst, dstInvert);
	if ( DisplayImg( DELAY_CAPTION, dstInvert) != 0 ) { return; }

	
	if ( DisplayCaption( "Canny edge detection", dstInvert) != 0 ) { return; }
	Mat dstCanny;
	dstCanny = CannyThreshold(0, 0, src, dstInvert);
	if ( DisplayImg( DELAY_CAPTION, dstCanny) != 0 ) { return; }


    /// Wait until user press a key
    DisplayCaption( "End: Press a key!", dstCanny);

    waitKey(0);
}