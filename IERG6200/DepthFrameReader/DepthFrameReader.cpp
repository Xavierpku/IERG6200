#include <iostream>
#include <fstream>
#include "PicoZense_api.h"
using namespace std;

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
	status = PsSetPulseCount(deviceIndex, pulseCount);
	for (;;)
	{
		count++;
		if(count%1800==0)
		{
			status = PsSetPulseCount(deviceIndex, pulseCount);
			pulseCount+=50;
		}
		//Get Depth Frame
		PsReadNextFrame(deviceIndex);
		PsGetFrame(deviceIndex, PsDepthFrame, &depthFrame);

		//Image Process: print the center pixel value of the depth image
		if (depthFrame.pFrameData != NULL)
		{
			//Read the depthFrameData in uint_16
			PsDepthPixel * DepthFrameData = (PsDepthPixel *)depthFrame.pFrameData;

			PsDepthPixel depthValue = DepthFrameData[depthFrame.height / 2 * depthFrame.width
				+ depthFrame.width /2];
			//cout << "The center depth value of depth image: " << depthValue << " mm" << endl;
			//PsDepthPixel depthValue2 = DepthFrameData[depthFrame.height/2*depthFrame.width + depthFrame.width/2+1];	
			if(count%2==1){
			cout<<depthValue<<endl;
			fout<<depthValue<<endl;
			}
		}
		else
			cout << "Depth Image is invalid " << endl;
		if(count == 18000)
			break;
	}

	PsCloseDevice(deviceIndex);

	PsShutdown();

	return 0;
}
