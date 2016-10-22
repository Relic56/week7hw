#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <math.h>

using namespace std;
using namespace cv;

int main()
{
    vector<Mat> channels;
    Mat img = imread("rect.jpg");
    Mat hsv;
    cvtColor(img,hsv,CV_BGR2HSV);
    split(hsv,channels);
    Mat hue_orig = channels.at(0).clone();
    Mat low, high;
    Mat result;
    threshold(hue_orig,low,72,255,CV_THRESH_BINARY);
    threshold(hue_orig,high,95,255,CV_THRESH_BINARY_INV);
    result = low & high;
    Mat edge;
    Canny(result,edge,20,80);
    vector<vector<Point> > contours;
    findContours(edge,contours,CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE, Point(0,0) );
    for(int i = 0; i < contours.size(); i++)
    {
	vector<Point> out;
	approxPolyDP(contours[i],out,arcLength(Mat(contours.at(i)),true)*0.02,true);
	if(out.size()==4 && contourArea(out) > 1300)
	{
	    int maxX=0, minX = 2000;
	    int maxY = 0, minY = 2000;
	    for(int j = 0; j < out.size(); j++)
	    {
		if(out[j].x > maxX)
		{
		    maxX = out[j].x;
		}
		if(out[j].x < minX)
		{
		    minX = out[j].x;
		}
		if(out[j].y > maxY)
		{
		    maxY = out[j].y;
		}
		if(out[j].y < minY)
		{
		    minY = out[j].y;
		}
	    }
	    double width = maxX - minX;
	    cout<< "Dist: " << 48/width<< "m"<<endl;
	    cout<<"X Azimuth: "<<azimuth(maxX,minX,img.size().width,48/width)<<endl;
	    cout<<"Y Azimuth: "<<azimuth(maxY,minY,img.size().width,48/width)<<endl;
	}
    }
    waitKey(0);
    return 0;
}

int azimuth(double p1, double p2,double imgWidth,double dist)
{
    double angle;
    imgWidth = imgWidth/2;
    double average = p1 + p2;
    average = average/2;
    imgWidth = average - imgWidth;
    imgWidth = imgWidth/480;
    imgWidth = imgWidth * dist;
    angle = arcTan(dist,imgWidth);
    return angle;
}

int arcTan(double dist, double imgWidth)
{
    double frac, angle;
    frac = imgWidth/dist;
    angle = atan(frac) * 180/ 3.14159;
    return angle;
}
