#if !defined( MYFUNCTION_H )
#define MYFUNCTION_H
#endif

#include <iostream>
#include <vector>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"

using namespace std;
using namespace cv;



Mat srcSmooth;Mat dstSmooth;
//char window_name[] = "Smoothing Demo";

/// Function headers
int display_caption( const char* caption );
int display_dst( int delay );

int Smoothing(char * srcImage)
{
    namedWindow( window_name, WINDOW_AUTOSIZE );

    /// Load the source image
    srcSmooth = imread( srcImage, 1 );

    if ( display_caption( "Original Image" ) != 0 ) {        return 0;    }

    dstSmooth = srcSmooth.clone();
    if ( display_dst( DELAY_CAPTION ) != 0 ) {        return 0;    }


    /// Applying Homogeneous blur
    if ( display_caption( "Homogeneous Blur" ) != 0 ) {        return 0;    }

    for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
    {        blur( srcSmooth, dstSmooth, Size( i, i ), Point(-1,-1) );
        if ( display_dst( DELAY_BLUR ) != 0 ) {            return 0;        }    }


    /// Applying Gaussian blur
    if ( display_caption( "Gaussian Blur" ) != 0 ) {        return 0;    }

    for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
    {        GaussianBlur( srcSmooth, dstSmooth, Size( i, i ), 0, 0 );
        if ( display_dst( DELAY_BLUR ) != 0 ) {            return 0;        }    }


    /// Applying Median blur
    if ( display_caption( "Median Blur" ) != 0 ) {        return 0;    }

    for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
    {        medianBlur ( srcSmooth, dstSmooth, i );
        if ( display_dst( DELAY_BLUR ) != 0 ) {            return 0;        }    }


    /// Applying Bilateral Filter
    if ( display_caption( "Bilateral Blur" ) != 0 ) {        return 0;    }

    for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
    {        bilateralFilter ( srcSmooth, dstSmooth, i, i*2, i/2 );
        if ( display_dst( DELAY_BLUR ) != 0 ) {            return 0;        }    }

    /// Wait until user press a key
    display_caption( "End: Press a key!" );

    waitKey(0);

    return 0;
}

int display_caption( const char* caption )
{
    dstSmooth = Mat::zeros( srcSmooth.size(), srcSmooth.type() );
    putText( dstSmooth, caption,
             Point( srcSmooth.cols/4, srcSmooth.rows/2),
             FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255) );

    imshow( window_name, dstSmooth );
	int c = waitKey( DELAY_CAPTION );
    if ( c >= 0 ) {        return -1;    }
    return 0;
}

int display_dst( int delay )
{
    imshow( window_name, dstSmooth );
    int c = waitKey ( delay );
    if ( c >= 0 ) {        return -1;    }
	while(waitKey(0));
    return 0;
}