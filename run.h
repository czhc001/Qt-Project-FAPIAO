#pragma once  
#ifdef RUNLIBDLL  
#define RUNAPI _declspec(dllexport)  
#else  
#define RUNAPI  _declspec(dllimport)  
#endif  
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/video.hpp"
#include <opencv2/opencv.hpp>
#include <caffe/caffe.hpp>
#include <string>


using namespace caffe;
using namespace std;
using namespace cv;

#define V_PROJECT 1
#define H_PROJECT 2

typedef struct
{
	int begin;
	int end;

}char_range_t;

class OCRecognizer
{
public:
	OCRecognizer();
	virtual ~OCRecognizer();
	string result[2];
	void draw_projection(vector<int>& pos, int mode);
	int GetTextProjection(Mat &src, vector<int>& pos, int mode);
	int GetPeekRange(vector<int> &vertical_pos, vector<char_range_t> &peek_range, int min_thresh, int min_range, int max_range);
	int CutChar(int ii, Mat &img, const vector<char_range_t>& v_peek_range, vector<Mat>& chars_set);
	Mat cut_one_line(const Mat& src, int begin, int end);
	vector<Mat> CutSingleLine(Mat& img);
	vector<Mat> CutSingleChar(vector<Mat> lines_set);
	void draw_border(Mat image, int up, int bottom, int left, int right, int middle, int ll);
	void run();
	void set_modelpath0(char* modelpath0);
	void set_modelpath1(char* modelpath1);
	char modelpath0[512];
	char modelpath1[512];
	bool isStop;
	void stop();
	virtual void message(string sum1, string sum2, Mat image, bool flag);
	virtual void falsemessage(int a);

};
