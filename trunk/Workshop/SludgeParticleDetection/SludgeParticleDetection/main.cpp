#include "Threshold.h"
#include "Smoothing.h"

void main()
{
	/*AutoDisplay("lena.png");
	return;*/
	
	char* filename;
	filename = new char[100];
	bool bInput = true;	

	/// Load the source image
	if(bInput) {
		cout << "Enter image's filename (e.g. F2_P1N.bmp): ";
		cin >> filename;
		cout << filename;
	} else {
		filename = "F4_P1N.bmp";
	}

	namedWindow( "Original", WINDOW_NORMAL);
	namedWindow( "Result", WINDOW_NORMAL);

	if (src.empty())
		src = imread( filename, 1 );
	
	//Threshold(filename);	// Run threshold demo
	//Smoothing(filename);	// Run smoothing demo

	/// Apply Laplace
	//dst = Laplace(filename);

	/// Apply Sobel
	//dst = Sobel(filename);	

	/// Enhance contrast: alpha value [1.0-3.0], beta value [0-100]
	//src.convertTo(dst, -1, 1.2, 50);

	/// Convert to grayscale
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

    imshow( "Original", src);
	imshow( "Result", dstCanny);

	//DisplayCaption( "End: Press a key!", dst);
    waitKey(0);
}