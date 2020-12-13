#include <iostream>
#include <fstream>
#include "PicoZense_api.h"
#include "opencv2/opencv.hpp"

#define FRAME_HEIGHT 480
#define FRAME_WIDTH 640
#define slope 2000

using namespace std;
using namespace cv;

const string depthImageWindow = "Depth Image";
int main(int argc, char * argv[])
{
	PsReturnStatus status;
	int32_t deviceIndex = 0;

	//Initialize the pico device ------------------------------------------------------------
	status = PsInitialize();

	if(status != PsReturnStatus::PsRetOK)
	{
		cout<<"PsInitialize failed!"<<endl;
		system("pause");
		return -1;
	}

	int32_t deviceCount = 0;
	status = PsGetDeviceCount(&deviceCount);

	if(deviceCount == 0)
	{
		cout<<"Not find any camera!"<<endl;
		system("pause");
		return -1;
	}
	
	status = PsOpenDevice(deviceIndex);
	if(status != PsReturnStatus::PsRetOK)
	{
		cout<<"OpenDevice failed!"<<endl;
		system("pause");
		return -1;
	}

	
	PsSetDataMode(deviceIndex, PsDepthAndRGB_30);
	PsSetDepthRange(deviceIndex, PsFarRange);
	PsSetFilter(deviceIndex, PsSmoothingFilter, true);
	//end of initialization -----------------------------------------------------------------
	
	

	//Set the filter for smoothing
	/*
        status = PsSetFilter(deviceIndex,PsSmoothingFilter,false);
        if(status != PsReturnStatus::PsRetOK)
        {
                cout<<"Set filter failted!"<<endl;
        }
	*/


        PsFrame depthFrame = { 0 };

        //Frame Main Loop
	//Getting the depth map and deal with it
        int count = 0;
	Mat cache[5];
	for(int i=0;i<5;i++)
	{
		cache[i] = Mat(FRAME_HEIGHT, FRAME_WIDTH, CV_16UC1, Scalar::all(0));
	}
	//Mat cache = Mat(FRAME_HEIGHT, FRAME_WIDTH, CV_16UC1, Scalar::all(0));
	Mat current = Mat(FRAME_HEIGHT, FRAME_WIDTH, CV_16UC1, Scalar::all(0));
	Mat dispDepth = Mat(FRAME_HEIGHT, FRAME_WIDTH, CV_16UC1, Scalar::all(0));

	for (;;)
        {
                count++;
                //Get Depth Frame
                PsReadNextFrame(deviceIndex);
                PsGetFrame(deviceIndex, PsDepthFrame, &depthFrame);
		
                //Image Process: print the center pixel value of the depth image
                if (depthFrame.pFrameData != NULL)
                {
                        //Read the depthFrameData in uint_16
                        PsDepthPixel * DepthFrameData = (PsDepthPixel *)depthFrame.pFrameData;
			current = Mat(FRAME_HEIGHT, FRAME_WIDTH, CV_16UC1, DepthFrameData);
/*			dispDepth = (cache[0] + cache[1] + cache[2] + cache[3] + cache[4]  + current)/6;
			cout<<"cache[0]: "<<(int)cache[0].at<uchar>(200,200)<<endl;
			cout<<"cache[1]: "<<(int)cache[1].at<uchar>(200,200)<<endl;
			cout<<"cache[2]: "<<(int)cache[2].at<uchar>(200,200)<<endl;
			cout<<"cache[3]: "<<(int)cache[3].at<uchar>(200,200)<<endl;
			cout<<"cache[4]: "<<(int)cache[4].at<uchar>(200,200)<<endl;
			cout<<"current: "<<(int)current.at<uchar>(200,200)<<endl;
			cout<<"dispDepth: "<<(int)dispDepth.at<uchar>(200,200)<<endl;
		
			cout<<"cache[0]: "<<(double)sum(cache[0])[0]/(FRAME_WIDTH*FRAME_HEIGHT)<<endl;
                        cout<<"cache[1]: "<<(double)sum(cache[1])[0]/(FRAME_WIDTH*FRAME_HEIGHT)<<endl;
                        cout<<"cache[2]: "<<(double)sum(cache[2])[0]/(FRAME_WIDTH*FRAME_HEIGHT)<<endl;
                        cout<<"cache[3]: "<<(double)sum(cache[3])[0]/(FRAME_WIDTH*FRAME_HEIGHT)<<endl;
                        cout<<"cache[4]: "<<(double)sum(cache[4])[0]/(FRAME_WIDTH*FRAME_HEIGHT)<<endl;
                        cout<<"current: "<<(double)sum(current)[0]/(FRAME_WIDTH*FRAME_HEIGHT)<<endl;
                        cout<<"dispDepth: "<<(double)sum(dispDepth)[0]/(FRAME_WIDTH*FRAME_HEIGHT)<<endl;
	
			cout<<(double)sum(dispDepth)[0]/(FRAME_WIDTH*FRAME_HEIGHT);
			for(int i=0;i<4;i++)
			{
				cache[i] = cache[i+1];
			}
			cache[4] = current;
*/			dispDepth.convertTo(dispDepth, CV_8U, 255.0/slope);
			cout<<count<<endl;
			cv::imshow("show", dispDepth);
                }
		else
			cout<<"depth image is not valid"<<endl;

		unsigned char key = waitKey(1);
        }

        PsCloseDevice(deviceIndex);

        PsShutdown();

        return 0;

}

