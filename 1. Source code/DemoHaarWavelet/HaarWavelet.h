#pragma once

#include <opencv2\opencv.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>



class CHaarWavelet
{
private:
	// Variables for transform
	cv::Mat m_OriginalImage;
	cv::Mat m_ForwardImage;
	cv::Mat m_InvertImage;

	int m_Iteration;

	double w0;
	double w1;
	double s0;
	double s1;

	// Variables for denoising
	cv::Mat m_PaddedImage;
	cv::Mat m_DenoisedImage;
	cv::Mat m_NoisyImage;
	float m_Threshold;
	int m_Slider;


public:
	CHaarWavelet(void);
	~CHaarWavelet(void);

private:
	// Methods for Transforms
	double Scale(double inFromMin, double inFromMax, double inToMin, double inToMax, double inX);

	void FWT(double* inoutData, int inLength);
	void FWT(double** inoutData, int inWidth, int inHeight);

	void IWT(double* inoutData, int inLength);
	void IWT(double** inoutData, int inWidth, int inHeight);

	void ApplyHaarTransform(bool inIsForward);

	// Methods for denoising
	void HaarDecomposeArray(float *A, int width);
	void HaarDecomposeImage(float *A, int width, int height);
	void HaarTransposeArray(float *A, int width, int height);
	void HaarReconstructArray(float *A, int width);
	void HaarReconstructImage(float *A, int width, int height);
	static void OnTrackbar(int, void*);
	void CHaarWavelet::Denoise(const cv::Mat &input, cv::Mat &output, float threshold);

public:
	void RunHaarTransform(CString inFilePath, int inIterations);
	void Denoise(CString inFilePath);
};

