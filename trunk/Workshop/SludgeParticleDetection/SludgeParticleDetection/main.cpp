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
		cout << "Enter image's filename (e.g. F1_P3N.bmp): ";
		cin >> filename;
		cout << filename;
	} else {
		filename = "F1_P3N.bmp";
	}

	namedWindow("Original", WINDOW_NORMAL);
	namedWindow("Metal detection", WINDOW_NORMAL);
	namedWindow("Powder detection", WINDOW_NORMAL);

	if (src.empty())
		src = imread(filename, 1);
	
	//Threshold(filename);	// Run threshold demo
	//Smoothing(filename);	// Run smoothing demo

	/// Apply Laplace
	//dst = Laplace(filename);

	/// Apply Sobel
	//dst = Sobel(filename);	

	/// Enhance contrast: alpha value [1.0-3.0], beta value [0-100]
	//src.convertTo(dst, -1, 1.2, 50);

	/// Transform with 7 steps
	//Mat dst = Transform7Steps(src);


	/// Convert to grayscale
	Mat src_gray;
	cvtColor(src, src_gray, COLOR_RGB2GRAY);


	/// Apply threshold
	Mat dstThreshold;
	threshold( src_gray, dstThreshold, 80, max_BINARY_value, 1 );

	
	/// Apply morphology
	Mat dstErosed;
	Mat dstDilated;
	int erosion_size = 5;
	int erosion_type = MORPH_RECT;
	Mat element = getStructuringElement( erosion_type, Size( 2*erosion_size + 1, 2*erosion_size+1 ), Point( erosion_size, erosion_size ) );
	dilate(dstThreshold, dstDilated, element);
	erode(dstDilated, dstErosed, element);
	

	/// Apply the metal mask to original image
	Mat dstMetal;
	src.copyTo(dstMetal, dstErosed);


	/// Threshold
	Mat dstThreshold2;
	threshold(src_gray, dstThreshold2, 130, max_BINARY_value, 1 );


	/// Apply morphology
	Mat dstDilated2;
	Mat dstErosed2;	
	dilate(dstThreshold2, dstDilated2, element);
	erode(dstDilated2, dstErosed2, element);


	/// Subtract metal mask from powder mask
	Mat dstPowderMask = dstErosed2 - dstErosed;


	/// Apply the powder mask to original image
	Mat dstPowder;
	src.copyTo(dstPowder, dstPowderMask);

    imshow( "Original", src);
	imshow( "Metal detection", dstMetal);
	imshow( "Powder detection", dstPowder);
	

	//DisplayCaption( "End: Press a key!", dst);
    waitKey(0);
}