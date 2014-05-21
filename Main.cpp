#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "SLIC.h"

#define MatrixOfDouble2D std::vector<std::vector<double>>

using namespace std;
using namespace cv;

int main()
{
	/* Video source location. */
	const string videoLocation = "C:\\Insert_video_path_here.avi";

	/* Output window name. */
	const string windowName = "Captured video";

	/* Declare a container for the video and try to import the video from
	   a specific location. */
	VideoCapture capturedVideo(videoLocation);

	/* Check if the video was correctly imported into the program. */
	if (capturedVideo.isOpened() == false)
	{
		/* Send an error message and then close the program if there was
		   an error with the video capture. */
		cout << "\nSorry, there was an error with video capturing.\n";
		return -1;
	}

	/* Get video width and height. */
	const int videoWidth = (int)capturedVideo.get(CV_CAP_PROP_FRAME_WIDTH);
	const int videoHeight = (int)capturedVideo.get(CV_CAP_PROP_FRAME_HEIGHT);

	/* A container which will hold a video frame for
	   the necessary time for its elaboration. */
	Mat currentFrame;

	/* SLIC algorithm parameters. */
	int spatialDistanceWeight = 60;
	int superpixelNumber = 600;

	/* Round the sampling step to the nearest integer. */
	int stepSLIC = (int)(sqrt((videoHeight * videoWidth) / superpixelNumber) + 0.5);

	SLIC SLICVideoElaboration;

	/* Open a new window where to play the imported video. */
	namedWindow(windowName, CV_WINDOW_AUTOSIZE);

	/* A container which will hold cluster centres from a frame. */
	MatrixOfDouble2D previousCentres(0);

	/* Enter an infinite cycle to elaborate the video until its last frame. */
	while (true)
	{
		/* Take the next frame from the video. */
		capturedVideo >> currentFrame;

		/* If there are no more frames in the video, break the loop because
		   the video has reached its end or there was an error. */
		if (currentFrame.data == NULL)
			break;

		/* Convert the frame from RGB to LAB color space
		   before SLIC elaboration. */
		cvtColor(currentFrame, currentFrame, CV_BGR2Lab);

		/* Perform the SLIC algorithm operations. */
		previousCentres = SLICVideoElaboration.createSuperpixels(
			currentFrame, stepSLIC, spatialDistanceWeight, previousCentres);
		//SLICVideoElaboration.enforceConnectivity(currentFrame);

		/* Convert frame back to RGB. */
		cvtColor(currentFrame, currentFrame, CV_Lab2BGR);

		SLICVideoElaboration.colorSuperpixels(currentFrame);
		//SLICVideoElaboration.drawClusterContours(currentFrame, Vec3b(0, 0, 255));
		//SLICVideoElaboration.drawClusterCentres(currentFrame, CV_RGB(255, 0, 0));

		/* Show frame in the window. */
		imshow(windowName, currentFrame);

		/* End program on ESC press. */
		if (cvWaitKey(1) == 27)
			break;
	}
	/* Release resources after video processing. */
	destroyAllWindows();

	return 0;
}