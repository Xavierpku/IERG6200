#include <iostream>
#include <fstream>
#include "PicoZense_api.h"
#include "Timer.h" 
#include <opencv2/core/core.hpp> 
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
        ofstream fout;

        fout.open("./record.txt");

        PsReturnStatus status;
        int32_t deviceIndex = 0;

        status = PsInitialize();
        if (status != PsReturnStatus::PsRetOK)
        {
                cout << "PsInitialize failed!" << endl;
                system("pause");
                return -1;
        }

        int32_t deviceCount = 0;
        status = PsGetDeviceCount(&deviceCount);
        if (deviceCount == 0)
        {
                cout << "Not find any camera!" << endl;
                system("pause");
                return -1;
        }

        status = PsOpenDevice(deviceIndex);
        if (status != PsReturnStatus::PsRetOK)
        {
                cout << "OpenDevice failed!" << endl;
                system("pause");
                return -1;
        }

        //Set output data mode to PsDepthAndRGB_30 to get Depth frame
        //The PsDepthAndIR_30, PsDepthAndIRAndRGB_30 are also available to output Depth frame
        PsSetDataMode(deviceIndex, PsDepthAndRGB_30);

        status = PsSetFilter(deviceIndex,PsSmoothingFilter,false);
        if(status != PsReturnStatus::PsRetOK)
        {
                cout<<"Set filter failted!"<<endl;
        }
        PsFrame depthFrame = { 0 };

	 //Frame Main Loop
        int count = 0;
        uint16_t pulseCount = 50;
	uint16_t pthreshold = 0;
        status = PsSetPulseCount(deviceIndex, pulseCount);
	clock_t start, end;
	double time_total=0.0;
	PsDepthRange pDepthRange;
	double loop_time_ms, loop_time_ms_total;
	Timer timer_total;
	Timer timer_;
	double time_interval = 333;
        for (;;)
        {
		timer_total.start();
		timer_.start();
//                count++;
                if(count%2==0)
//                       status = PsSetPulseCount(deviceIndex, 2);
			status = PsSetDepthRange(deviceIndex, PsNearRange);
		if(count%2==1)
//			status = PsSetPulseCount(deviceIndex,600); 
			status = PsSetDepthRange(deviceIndex, PsFarRange);
//		loop_time_ms = timer_.getTimeMilliSec();
                //Get Depth Frame
//                PsReadNextFrame(deviceIndex);
//                PsGetFrame(deviceIndex, PsDepthFrame, &depthFrame);

                //Image Process: print the center pixel value of the depth image
 /*               if (depthFrame.pFrameData != NULL)
                {
                        //Read the depthFrameData in uint_16
                        PsDepthPixel * DepthFrameData = (PsDepthPixel *)depthFrame.pFrameData;

                        PsDepthPixel depthValue = DepthFrameData[depthFrame.height / 2 * depthFrame.width
                                + depthFrame.width /2];
		}
*/
//		status = PsGetPulseCount(deviceIndex, &pulseCount);
//		cout<<pulseCount<<" ";
//		cout<<pDepthRange<<endl;


		loop_time_ms = timer_.getTimeMilliSec();
		cout<<loop_time_ms<<"      ";
		if(loop_time_ms<time_interval)
			char c = waitKey(time_interval-loop_time_ms);
		loop_time_ms_total = timer_total.getTimeMilliSec();
		printf(" | Total time taken: %f ms\n", loop_time_ms_total);
	}


        PsCloseDevice(deviceIndex);

        PsShutdown();

        return 0;
}

