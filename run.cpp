#define RUNLIBDLL  
#include "run.h"
#include "head.h"



OCRecognizer::OCRecognizer()
{
    isStop = false;
}



void OCRecognizer::draw_projection(vector<int>& pos, int mode)
{
	vector<int>::iterator max = max_element(pos.begin(), pos.end()); //求最大值
	if (mode == H_PROJECT)
	{
		int height = pos.size();
		int width = *max;
		Mat project = Mat::zeros(height, width, CV_8UC1);
		for (int i = 0; i < project.rows; i++)
		{
			for (int j = 0; j < pos[i]; j++)
			{
				project.at<uchar>(i, j) = 255;
			}
		}
		//imshow("horizational projection", project);

	}
	else if (mode == V_PROJECT)
	{
		int height = *max;
		int width = pos.size();
		Mat project = Mat::zeros(height, width, CV_8UC1);
		for (int i = 0; i < project.cols; i++)
		{
			for (int j = project.rows - 1; j >= project.rows - pos[i]; j--)
			{
				//std::cout << "j:" << j << "i:" << i << std::endl;
				project.at<uchar>(j, i) = 255;
			}
		}
		//imshow("vertical projection", project);
	}
}

int OCRecognizer:: GetTextProjection(Mat &src, vector<int>& pos, int mode)
{
	if (mode == V_PROJECT)
	{
		for (int i = 0; i < src.rows; i++)
		{
			uchar* p = src.ptr<uchar>(i);
			for (int j = 0; j < src.cols; j++)
			{
				if (p[j] == 0)
				{
					pos[j]++;
				}
			}
		}
		//draw_projection(pos, V_PROJECT);
	}
	else if (mode == H_PROJECT)
	{
		for (int i = 0; i < src.cols; i++)
		{

			for (int j = 0; j < src.rows; j++)
			{
				if (src.at<uchar>(j, i) == 0)
				{
					pos[j]++;
				}
			}
		}
		//draw_projection(pos, H_PROJECT);
	}

	return 0;
}

int OCRecognizer:: GetPeekRange(vector<int> &vertical_pos, vector<char_range_t> &peek_range, int min_thresh = 2, int min_range = 10, int max_range = 60)
{
	int begin = 0;
	int end = 0;
	for (int i = 0; i < vertical_pos.size(); i++)
	{

		if (vertical_pos[i] > min_thresh && begin == 0)
		{
			begin = i;
		}
		else if (vertical_pos[i] > min_thresh && begin != 0)
		{
			continue;
		}
		else if (vertical_pos[i] < min_thresh && begin != 0)
		{
			end = i;
			if (end - begin >= min_range && end - begin <= max_range)
			{
				char_range_t tmp;
				tmp.begin = begin;
				tmp.end = end;
				peek_range.push_back(tmp);
			}
			begin = 0;
			end = 0;
		}
		else if (vertical_pos[i] < min_thresh || begin == 0)
		{
			continue;
		}
		else
		{
			//printf("raise error!\n");
		}
	}

	return 0;
}

int OCRecognizer:: CutChar(int ii, Mat &img, const vector<char_range_t>& v_peek_range, vector<Mat>& chars_set)
{
	int count = 0;
	vector<Mat> chars_set1;
	Mat show_img = img.clone();
	Mat show_img1(img.rows, img.cols, CV_8UC3, Scalar::all(255));
	cvtColor(show_img, show_img, CV_GRAY2BGR);
	int num = v_peek_range.size();
	for (int i = num - 1; i >= 0; i--)
	{
		int char_gap = v_peek_range[i].end - v_peek_range[i].begin;
		int x = v_peek_range[i].begin - 2>0 ? v_peek_range[i].begin - 2 : 0;
		int width = x + char_gap + 5 <= img.cols ? char_gap + 5 : img.cols - x;
		Rect r(x, 0, width, img.rows);
		rectangle(show_img, r, Scalar(255, 0, 0), 1);
		Mat single_char = img(r).clone();
		chars_set1.push_back(single_char);
		count++;
		if (ii == 0 && count == 12)
			break;
		else if (ii == 1 && count == 8)
			break;
	}
	vector<Mat>::reverse_iterator riter;
	for (riter = chars_set1.rbegin(); riter != chars_set1.rend(); riter++)
		chars_set.push_back(*riter);
	/*
	if (ii == 0 && count == 12)
		//imshow("line1", show_img);
	else if (ii == 0 && count != 12)
	{
		//imshow("line1", show_img1);
		chars_set.clear();
	}
	else if (ii == 1 && count == 8)
		//imshow("line2", show_img);
	else
	{
		imshow("line2", show_img1);
		if (chars_set.size()>12)
			chars_set.erase(chars_set.begin() + 12, chars_set.end());
		else
			chars_set.clear();
	}
	*/
	return 0;
}

Mat OCRecognizer:: cut_one_line(const Mat& src, int begin, int end)
{
	Mat line = src(Rect(0, begin, src.cols, end - begin)).clone();
	return line;
}

vector<Mat> OCRecognizer::CutSingleLine(Mat& img)
{
	vector<int> horizion_pos(img.rows, 0);
	vector<char_range_t> h_peek_range;
	GetTextProjection(img, horizion_pos, H_PROJECT);
	GetPeekRange(horizion_pos, h_peek_range, 20, 20, 120);

	vector<Mat> lines_set;
	for (int i = 0; i < (int)h_peek_range.size(); i++)
	{
		Mat line = cut_one_line(img, h_peek_range[i].begin, h_peek_range[i].end);
		lines_set.push_back(line);
	}
	return lines_set;
}

vector<Mat> OCRecognizer::CutSingleChar(vector<Mat> lines_set)
{
	vector<Mat> chars_set;
	for (int i = 0; i < (int)lines_set.size(); i++)
	{
		Mat line = lines_set[i];
		vector<int> vertical_pos(line.cols, 0);
		vector<char_range_t> v_peek_range;
		GetTextProjection(line, vertical_pos, V_PROJECT);
		if (i == 0)
			GetPeekRange(vertical_pos, v_peek_range, 4, 10, 60);
		else
			GetPeekRange(vertical_pos, v_peek_range, 4, 20, 60);
		CutChar(i, line, v_peek_range, chars_set);
	}

	return chars_set;
}
void OCRecognizer::draw_border(Mat image, int up, int bottom, int left, int right, int middle, int ll)
{
	line(image, Point(left, up), Point(right, up), Scalar(0, 255, 0), 4, 4);
	line(image, Point(left, up), Point(left, bottom), Scalar(0, 255, 0), 4, 4);
	line(image, Point(right, bottom), Point(left, bottom), Scalar(0, 255, 0), 4, 4);
	line(image, Point(right, bottom), Point(right, up), Scalar(0, 255, 0), 4, 4);
}

void OCRecognizer::message(string sum1, string sum2, Mat image, bool flag){


}

void OCRecognizer::falsemessage(int a){

}
void OCRecognizer::run() {
	cout << "Built with OpenCV " << CV_VERSION << endl;
	Mat image;
	int cc = 1280;
	int rr = 960;
	int roi_r = 240;
	int roi_c = 720;
	//int roi_r=120; //480
	//int roi_c=360; //640
	int up = (rr - roi_r) / 2;
	int bottom = up + roi_r;
	int left = (cc - roi_c) / 2;
	int right = left + roi_c;
	int middle = (up + bottom) / 2;

	Caffe::set_mode(Caffe::CPU);
    //Net<float> lenet("D:/Qt/Projects/FAPIAO/models/lenet.prototxt", caffe::TEST);//参数1
    //lenet.CopyTrainedLayersFrom("D:/Qt/Projects/FAPIAO/models/lenet_iter_10000.caffemodel");//参数2
    Net<float> lenet(this->modelpath0, caffe::TEST);//参数1
    lenet.CopyTrainedLayersFrom(this->modelpath1);//参数2
	Blob<float> *input_ptr = lenet.input_blobs()[0];
	input_ptr->Reshape(1, 1, 28, 28);
	Blob<float> *output_ptr = lenet.output_blobs()[0];
	output_ptr->Reshape(1, 10, 1, 1);

	VideoCapture capture;
	capture.open(0);
	capture.set(3, cc);
	capture.set(4, rr);
	if (capture.isOpened())
	{
		cout << "Capture is opened" << endl;
		for (int i = 0; i < 30; i++)capture >> image;
        for (;!isStop;){
			capture >> image;
			if (image.empty())
				break;
			draw_border(image, up, bottom, left, right, middle, 15);
			//imshow("window", image);
			//waitKey;
			int key = waitKey(10);
			int truecount = 0;
			string tmp1 = "";
			string tmp2 = "";//存储上一次识别结果
			bool resflag;
            for (;!isStop;)
			{
				capture >> image;
				if (image.empty())
					break;
				draw_border(image, up, bottom, left, right, middle, 15);
				//imshow("window", image);
				//waitKey;
				Mat roi, roi_copy, roi_gray, roi_binary;
				roi = image(Range(up, bottom), Range(left, right));
				roi_copy = roi.clone();
				for (int y = 0; y < image.rows; y++)
					for (int x = 0; x < image.cols; x++)
						for (int c = 0; c < 3; c++)
							image.at<Vec3b>(y, x)[c] = saturate_cast<uchar>((0.4)*(image.at<Vec3b>(y, x)[c]));
				Mat mask(roi.rows, roi.cols, roi.depth(), Scalar(1));
				roi_copy.copyTo(roi, mask);

				Mat hsv(roi.rows, roi.cols, CV_8UC3);
				cvtColor(roi_copy, hsv, COLOR_BGR2HLS_FULL);
				int light = 150;
				bool flag = 0;
				resflag = false;
				for (int y = hsv.rows - 1; y >= 0; y--)
				{
					for (int x = hsv.cols - 1; x >= 0; x--)
						if ((int)hsv.at<Vec3b>(y, x)[1] > 150)
						{
							light = (int)hsv.at<Vec3b>(y, x)[1];
							flag = 1;
							break;
						}
					if (flag == 1)break;
				}
				float scale = 150. / light;
				//cout<<scale<<endl;
				for (int y = hsv.rows; y < hsv.rows; y++)
					for (int x = 0; x < hsv.cols; x++)
						hsv.at<Vec3b>(y, x)[1] = saturate_cast<uchar>(hsv.at<Vec3b>(y, x)[1] * scale);
				cvtColor(hsv, roi_copy, COLOR_HLS2BGR_FULL);

				medianBlur(roi_copy, roi_copy, 5);
				//imshow("zz",roi_copy);
				GaussianBlur(roi_copy, roi_copy, Size(5, 5), 0);
				Mat r_img(roi.rows, roi.cols, CV_8UC1);
				for (int y = 0; y < roi.rows; y++)
					for (int x = 0; x < roi.cols; x++)
						r_img.at<uchar>(y, x) = saturate_cast<uchar>(roi_copy.at<Vec3b>(y, x)[2] > 100 ? 255 : roi_copy.at<Vec3b>(y, x)[2]);
				//imshow("rg",r_img);
				equalizeHist(r_img, r_img);
				//imshow("equalize",r_img);
				Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
				morphologyEx(r_img, r_img, MORPH_OPEN, element);
				morphologyEx(r_img, r_img, MORPH_CLOSE, element);
				threshold(r_img, roi_binary, 200, 255.0, CV_THRESH_BINARY);
				vector<Mat> lines_set = CutSingleLine(roi_binary);
				vector<Mat> chars_set;
				if (lines_set.size() == 2)
				{
					chars_set = CutSingleChar(lines_set);
				}
				else
				{
					Mat show_img1(75, roi_binary.cols, CV_8UC3, Scalar::all(255));
				//	imshow("line1", show_img1);
				//	imshow("line2", show_img1);
				}
				int key = waitKey(10);
				int n = chars_set.size();
				string sum1 = "";
				string sum2 = "";

				//识别有结果
				if (n == 20){
					long int sum[20];
					for (int i = 0; i < 20; i++)
					{
						float ary[28 * 28];
						Mat tmp = chars_set[i].clone();
						int tmp_width = (tmp.rows + 10 - tmp.cols) / 2;
						if (tmp_width < 0) tmp_width = 0;
						copyMakeBorder(tmp, tmp, 5, 5, tmp_width, tmp_width, BORDER_CONSTANT, 255);
						resize(tmp, tmp, Size(28, 28), 0, 0, INTER_LANCZOS4);
						for (int i = 0; i < 28 * 28; i++)
							ary[i] = static_cast<float>((255 - tmp.data[i]) ^ 0xFF)*0.00390625;
						input_ptr->set_cpu_data(ary);
						lenet.Forward();
						const float* begin = output_ptr->cpu_data();
						int index = 0;
						for (int j = 1; j < 20; j++){
							if (begin[index] < begin[j])
								index = j;
						}
						sum[i] = index;
					}
					for (int i = 0; i < 12; i++){
						sum1.append(to_string(sum[i]));
					}
					for (int i = 12; i < 20; i++){
						sum2.append(to_string(sum[i]));
					}
				}

				if (sum1 == tmp1 && sum2 == tmp2 && sum1 != ""){
					truecount++;
				}
				else{
					truecount = 0;
				}
				if (truecount > 9){
					resflag = true;
				}
				tmp1 = sum1;
				tmp2 = sum2;
				message(sum1, sum2, image, resflag);
			}
		}
	}
	else
	{
		cout << "No capture" << endl;
		falsemessage(1);//
		image = Mat::zeros(480, 640, CV_8UC1);
		//imshow("window", image);
		//waitKey(0);
	}
}

void OCRecognizer::set_modelpath0(char* modelpath0){
    strcpy(this->modelpath0, modelpath0);
}

void OCRecognizer::set_modelpath1(char* modelpath1){
    strcpy(this->modelpath1, modelpath1);
}

void OCRecognizer::stop(){
    isStop = true;
}


OCRecognizer::~OCRecognizer()
{

}

