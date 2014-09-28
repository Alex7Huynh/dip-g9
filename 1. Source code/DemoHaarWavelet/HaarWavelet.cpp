#include "StdAfx.h"
#include "HaarWavelet.h"


template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

CHaarWavelet* st_HaarWavlet = NULL;

CHaarWavelet::CHaarWavelet(void)
{
	w0 = 0.5;
	w1 = -0.5;
	s0 = 0.5;
	s1 = 0.5;
	st_HaarWavlet = this;
}


CHaarWavelet::~CHaarWavelet(void)
{
}

double CHaarWavelet::Scale(double inFromMin, double inFromMax, double inToMin, double inToMax, double inX)
{
	if (inFromMax - inFromMin == 0) return 0;
	double value = (inToMax - inToMin) * (inX - inFromMin) / (inFromMax - inFromMin) + inToMin;

	if (value > inToMax)
	{
		value = inToMax;
	}
	if (value < inToMin)
	{
		value = inToMin;
	}

	return value;
}

void CHaarWavelet::FWT(double* inoutData, int inLength)
{
	if (!inoutData || inLength < 0)
		return;

	double* temp = new double[inLength];

	int h = inLength >> 1;
	for (int i = 0; i < h; i++)
	{
		int k = (i << 1);
		temp[i] = inoutData[k] * s0 + inoutData[k + 1] * s1;
		temp[i + h] = inoutData[k] * w0 + inoutData[k + 1] * w1;
	}

	for (int i = 0; i < inLength; ++i)
		inoutData[i] = temp[i];

	delete []temp;
}

void CHaarWavelet::FWT(double** inoutData, int inWidth, int inHeight)
{
	int rows = inWidth;
	int cols = inHeight;

	double* row = new double[cols];
	double* col = new double[rows];

	for (int k = 0; k < m_Iteration; k++)
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
				row[j] = inoutData[i][j];

			FWT(row, cols);

			for (int j = 0; j < cols; j++)
				inoutData[i][j] = row[j];
		}

		for (int j = 0; j < cols; j++)
		{
			for (int i = 0; i < rows; i++)
				col[i] = inoutData[i][j];

			FWT(col, rows);

			for (int i = 0; i < rows; i++)
				inoutData[i][j] = col[i];
		}
	}

	delete []row;
	delete []col;
}


void CHaarWavelet::IWT(double* inoutData, int inLength)
{
	if (!inoutData || inLength < 0)
		return;

	double* temp = new double[inLength];

	int h = inLength >> 1;
	for (int i = 0; i < h; i++)
	{
		int k = (i << 1);
		temp[k] = (inoutData[i] * s0 + inoutData[i + h] * w0) / w0;
		temp[k + 1] = (inoutData[i] * s1 + inoutData[i + h] * w1) / s0;
	}

	for (int i = 0; i < inLength; ++i)
		inoutData[i] = temp[i];

	delete []temp;
}

void CHaarWavelet::IWT(double** inoutData, int inWidth, int inHeight)
{
	int rows = inWidth;
	int cols = inHeight;

	double* row = new double[cols];
	double* col = new double[rows];

	for (int k = 0; k < m_Iteration; k++)
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
				row[j] = inoutData[i][j];

			IWT(row, cols);

			for (int j = 0; j < cols; j++)
				inoutData[i][j] = row[j];
		}

		for (int j = 0; j < cols; j++)
		{
			for (int i = 0; i < rows; i++)
				col[i] = inoutData[i][j];

			IWT(col, rows);

			for (int i = 0; i < rows; i++)
				inoutData[i][j] = col[i];
		}
	}

	delete []row;
	delete []col;
}

void CHaarWavelet::ApplyHaarTransform(bool inIsForward)
{
	cv::Mat bmp = inIsForward ? m_OriginalImage.clone() : m_ForwardImage.clone();

	int maxScale = (int)(log(MAX(bmp.rows, bmp.cols) * 1.0) / log(2.0));
	if (m_Iteration < 1 || m_Iteration > maxScale)
	{
		return;
	}

	double** Red = new double*[bmp.rows];
	double** Green = new double*[bmp.rows];
	double** Blue = new double*[bmp.rows];
	for (int i = 0; i < bmp.rows; ++i) {
		Red[i] = new double[bmp.cols];
		Green[i] = new double[bmp.cols];
		Blue[i] = new double[bmp.cols];
	}

	for (int j = 0; j < bmp.rows; ++j)
	{
		for (int i = 0; i < bmp.cols; ++i)
		{
			cv::Vec3b tmp;
			try {
				tmp = bmp.at<cv::Vec3b>(cv::Point(i, j));
			} catch(...) {
				MessageBox(NULL, "Something is not right! Please retry!", "Error", 0);
				return;
			}

			Red[j][i] = (double)Scale(0, 255, -1, 1, tmp[2]);
			Green[j][i] = (double)Scale(0, 255, -1, 1, tmp[1]);
			Blue[j][i] = (double)Scale(0, 255, -1, 1, tmp[0]);
		}
	}

	if (inIsForward)
	{
		FWT(Red, bmp.rows, bmp.cols);
		FWT(Green, bmp.rows, bmp.cols);
		FWT(Blue, bmp.rows, bmp.cols);
	}
	else
	{
		IWT(Red, bmp.rows, bmp.cols);
		IWT(Green, bmp.rows, bmp.cols);
		IWT(Blue, bmp.rows, bmp.cols);
	}

	for (int j = 0; j < bmp.rows; ++j)
	{
		for (int i = 0; i < bmp.cols; ++i)
		{
			cv::Vec3b& tmp = bmp.at<cv::Vec3b>(cv::Point(i, j));
			tmp[2] = (int)Scale(-1, 1, 0, 255, Red[j][i]);
			tmp[1] = (int)Scale(-1, 1, 0, 255, Green[j][i]);
			tmp[0] = (int)Scale(-1, 1, 0, 255, Blue[j][i]);
		}
	}

	// -------- RELEASE RGB resources --------
	for (int i = 0; i < bmp.rows; ++i) {
		delete []Red[i];
		delete []Green[i];
		delete []Blue[i];
	}

	delete []Red;
	delete []Green;
	delete []Blue;
	// -------------------------------- --------


	if (inIsForward) {
		m_ForwardImage = bmp;
	}

	if (inIsForward) {
		// Backup image to invert
		m_ForwardImage = bmp;

		// Show image and save to file
		cv::namedWindow("FWT", 1);
		cv::imshow("FWT", bmp);
		cv::imwrite("E:\\tan1.jpg", bmp);
	}
	else {
		// Backup image
		m_InvertImage = bmp;

		// Show image and save to file
		cv::namedWindow("IWT", 1);
		cv::imshow("IWT", bmp);
		cv::imwrite("E:\\tan2.png", bmp);
	}

	bmp.release();
}


void CHaarWavelet::RunHaarTransform(CString inFilePath, int inIterations)
{
	// Try to open image file
	m_OriginalImage = cv::imread(inFilePath.GetString());
	if (!m_OriginalImage.data) {
		MessageBox(NULL, "Can't read image file! Please recheck.", "Error", 0);
		return;
	}

	// Check iterations
	m_Iteration = inIterations;
	int maxScale = (int)(log(MAX(m_OriginalImage.rows, m_OriginalImage.cols) * 1.0) / log(2.0));
	if (m_Iteration < 1 || m_Iteration > maxScale)
	{
		char msg[1024] = { 0 };
		_snprintf_s(msg, _TRUNCATE, "With input image, the Iteration should be in range 1 to %d", maxScale);
		MessageBox(NULL, msg, "Error", 0);
		return;
	}

	// Forward
	ApplyHaarTransform(true);
	// Invert
	ApplyHaarTransform(false);

	// Write image to file
	CString outFileName= inFilePath.Mid(inFilePath.ReverseFind('\\')+1);
	CString outFolderPath = inFilePath;
	outFolderPath.Truncate(outFolderPath.ReverseFind('\\'));

	char outFilePath[256] = { 0 };
	_snprintf_s(outFilePath, _TRUNCATE, "%s\\Forward_%d_%s", outFolderPath.GetString(), m_Iteration, outFileName);
	cv::imwrite(outFilePath, m_ForwardImage);

	_snprintf_s(outFilePath, _TRUNCATE, "%s\\Invert_%d_%s", outFolderPath.GetString(), m_Iteration, outFileName);
	cv::imwrite(outFilePath, m_InvertImage);

	m_OriginalImage.release();
	m_ForwardImage.release();
	m_InvertImage.release();
}


void CHaarWavelet::HaarDecomposeArray(float *A, int width)
{
	const float inv_sqrt2 = 1/sqrt(2.0);

	float norm = 1.0f/sqrt(width * 1.0);

	for(int i=0; i < width; i++) {
		A[i] *= norm;
	}

	float *tmp = new float[width];

	while(width > 1) {
		width /= 2;

		for(int i=0; i < width; i++) {
			tmp[i] = (A[2*i] + A[2*i+1]) * inv_sqrt2;
			tmp[width + i] = (A[2*i] - A[2*i+1]) * inv_sqrt2;
		}

		memcpy(A, tmp, width*2*sizeof(float));
	}

	delete [] tmp;
}

void CHaarWavelet::HaarTransposeArray(float *A, int width, int height)
{
	float *B = new float[width * height];

	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x) {
			B[x * height + y] = A[y * width + x];
		}
	}

	memcpy(A, B, sizeof(float) * width * height);

	delete []B;
}

void CHaarWavelet::HaarDecomposeImage(float *A, int width, int height)
{
	for(int i = 0; i < height; ++i) {
		HaarDecomposeArray(&A[i * width], width);
	}

	HaarTransposeArray(A, width, height);

	for(int i = 0; i < width; ++i) {
		HaarDecomposeArray(&A[i * height], height);
	}

	HaarTransposeArray(A, height, width);
}

void CHaarWavelet::HaarReconstructArray(float *A, int width)
{
	const float inv_sqrt2 = 1 / sqrt(2.0);
	float inv_norm = sqrt(width * 1.0);

	float *tmp = new float[width];
	int k = 1;

	while(k < width) {
		for(int i = 0; i < k; ++i) {
			tmp[2 * i] = (A[i] + A[k + i]) * inv_sqrt2;
			tmp[2 * i + 1] = (A[i] - A[k + i]) * inv_sqrt2;
		}

		memcpy(A, tmp, sizeof(float) * (k * 2));

		k *= 2;
	}

	for(int i = 0; i < width; ++i) {
		A[i] *= inv_norm;
	}

	delete []tmp;
}

void CHaarWavelet::HaarReconstructImage(float *A, int width, int height)
{
	for(int i = 0; i < width; ++i) {
		HaarReconstructArray(&A[i * height], height);
	}

	HaarTransposeArray(A, height, width);

	for(int i = 0; i < height; ++i) {
		HaarReconstructArray(&A[i * width], width);
	}

	HaarTransposeArray(A, width, height);
}


void CHaarWavelet::Denoise(CString inFilePath)
{
	m_Threshold = 0;
	m_Slider = 0;

	if (m_OriginalImage.data) {
		m_OriginalImage.release();
	}

	// Try to open image file
	m_OriginalImage = cv::imread(inFilePath.GetString());
	if (!m_OriginalImage.data) {
		MessageBox(NULL, "Can't read image file! Please recheck.", "Error", 0);
		return;
	}

	int padded_width = pow(2, ceil(log((double)m_OriginalImage.cols)/log(2.0)));
	int padded_height = pow(2, ceil(log((double)m_OriginalImage.rows)/log(2.0)));

	// Add some noise to the image
	//g_noise = Mat(g_img.size(), CV_8UC3);
	//randn(g_noise, 0, 50);
	m_NoisyImage = m_OriginalImage;// + g_noise;
	cv::copyMakeBorder(m_NoisyImage, m_PaddedImage, 0, padded_height-m_OriginalImage.rows, 0, padded_width-m_OriginalImage.cols, cv::BORDER_CONSTANT);

	cv::namedWindow("Haar wavelet denoising", 1);
	cv::createTrackbar("Threshold", "Haar wavelet denoising", &m_Slider, 100, OnTrackbar);
	st_HaarWavlet->OnTrackbar(0, NULL);
	cv::waitKey(0);
}

void CHaarWavelet::OnTrackbar(int, void*)
{
	st_HaarWavlet->m_Threshold = 0.1 * st_HaarWavlet->m_Slider / 100.0;

	if(st_HaarWavlet->m_PaddedImage.channels() == 3) {
		cv::Mat bgr[3];
		cv::split(st_HaarWavlet->m_PaddedImage, bgr);

		st_HaarWavlet->Denoise(bgr[0].clone(), bgr[0], st_HaarWavlet->m_Threshold);
		st_HaarWavlet->Denoise(bgr[1].clone(), bgr[1], st_HaarWavlet->m_Threshold);
		st_HaarWavlet->Denoise(bgr[2].clone(), bgr[2], st_HaarWavlet->m_Threshold);

		merge(bgr, 3, st_HaarWavlet->m_DenoisedImage);
	}
	else {
		st_HaarWavlet->Denoise(st_HaarWavlet->m_PaddedImage, st_HaarWavlet->m_DenoisedImage, st_HaarWavlet->m_Threshold);
	}

	st_HaarWavlet->m_DenoisedImage = st_HaarWavlet->m_DenoisedImage(cv::Range(0, st_HaarWavlet->m_OriginalImage.rows), cv::Range(0, st_HaarWavlet->m_OriginalImage.cols));

	cv::imshow("Haar wavelet denoising", st_HaarWavlet->m_DenoisedImage);
}

void CHaarWavelet::Denoise(const cv::Mat &input, cv::Mat &output, float threshold)
{
	assert(input.data != output.data);

	input.convertTo(output, CV_32F);

	HaarDecomposeImage((float*)output.data, output.cols, output.rows);

	for(int y=0; y < output.rows; y++) {
		float *ptr = (float*)output.ptr(y);

		for(int x=0; x < output.cols; x++) {
			// signbit returns 1 for negative numbers, and 0 otherwise
			// Haar wavelet shrinkage using soft thresholding
			//ptr[x] = (signbit(ptr[x]) == 1 ? -1 : 1) * max(0.f, fabs(ptr[x]) - threshold);
			ptr[x] = (sgn(ptr[x]) < 0 ? -1 : 1) * MAX(0.f, fabs(ptr[x]) - threshold);
		}
	}

	HaarReconstructImage((float*)output.data, output.rows, output.cols);

	output.convertTo(output, CV_8U);
}