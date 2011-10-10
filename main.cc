#include "src/utils.h"

int main(int argc, char *argv[]){

	if (argc == 1) {
		usage();
		exit(EXIT_SUCCESS);
	}

	/*
	 *
	 * SERIAL PORT INITIALIZATION
	 *
	 */

	int fd = 0;
	char serialport[256];
	int baudrate = B9600;  // default
	char buf[1];

	strcpy(serialport, argv[1]);

	fd = serialport_init(serialport, baudrate);
	if(fd == -1) return -1;
	printf("Serial Port %s was initialized succesfully", serialport);
	
	/*
	 *
	 * TRACKING BLUE OBJECT
	 *
	 */

	CvTracks tracks;

	cvNamedWindow("Tracking_Blue_Object", CV_WINDOW_AUTOSIZE);

	CvCapture *capture = cvCaptureFromCAM(0);
	cvGrabFrame(capture);
	IplImage *img = cvRetrieveFrame(capture);

	CvSize imgSize = cvGetSize(img);
	int limit_up = (imgSize.height/2)+50;
	int limit_down = (imgSize.height/2)-50;
	int limit_left= (imgSize.width/2)-50;
	int limit_right = (imgSize.width/2)+50;
	IplImage *frame = cvCreateImage(imgSize, img->depth, img->nChannels);

	IplConvKernel* morphKernel = cvCreateStructuringElementEx(5, 5, 1, 1, CV_SHAPE_RECT, NULL);

	//unsigned int frameNumber = 0;
	unsigned int blobNumber = 0;

	bool quit = false;
	while (!quit&&cvGrabFrame(capture))
	{
		IplImage *img = cvRetrieveFrame(capture);

		cvConvertScale(img, frame, 1, 0);

		IplImage *segmentated = cvCreateImage(imgSize, 8, 1);

		for (unsigned int j = 0; j < (unsigned int)imgSize.height; j++)
			for (unsigned int i = 0; i < (unsigned int)imgSize.width; i++)
			{
				CvScalar c = cvGet2D(frame, j, i);

				double b = ((double)c.val[0])/255.;
				double g = ((double)c.val[1])/255.;
				double r = ((double)c.val[2])/255.;
				unsigned char f = 255*((b>0.2+r)&&(b>0.2+g));

				cvSet2D(segmentated, j, i, CV_RGB(f, f, f));
			}

		cvMorphologyEx(segmentated, segmentated, NULL, morphKernel, CV_MOP_OPEN, 1);

		IplImage *labelImg = cvCreateImage(cvGetSize(frame), IPL_DEPTH_LABEL, 1);

		CvBlobs blobs;
		cvLabel(segmentated, labelImg, blobs);
		cvFilterByArea(blobs, 500, 1000000);
		cvFilterByLabel(blobs,cvGreaterBlob(blobs));
		cvRenderBlobs(labelImg, blobs, frame, frame, CV_BLOB_RENDER_BOUNDING_BOX);
		cvUpdateTracks(blobs, tracks, 200., 5);
		cvRenderTracks(tracks, frame, frame, CV_TRACK_RENDER_ID|CV_TRACK_RENDER_BOUNDING_BOX);

		cvShowImage("Tracking_Blue_Object", frame);

		for (CvBlobs::const_iterator it = blobs.begin(); it != blobs.end(); ++it)
		{
		//	cout << "Blob #" << it->second->label << ": Area=" << it->second->area << ", Centroid=(" << it->second->centroid.x << ", " << it->second->centroid.y << ")" << endl;
			if(it->second->centroid.x > limit_right){
				buf[0] = 49;
				int rc = serialport_write(fd, buf);
				if(rc==-1) return -1;
			}
			if(it->second->centroid.x < limit_left){
				buf[0] = 50;
				int rc = serialport_write(fd, buf);
				if(rc==-1) return -1;
			}
			if(it->second->centroid.y > limit_up){
				buf[0] = 51;
				int rc = serialport_write(fd, buf);
				if(rc==-1) return -1;
			}
			if(it->second->centroid.y < limit_down){
				buf[0] = 52;
				int rc = serialport_write(fd, buf);
				if(rc==-1) return -1;
			}
			usleep(5000);
		}

		cvReleaseImage(&labelImg);
		cvReleaseImage(&segmentated);

		char k = cvWaitKey(10) & 0xff;

		switch (k)
		{
			case 27:
			case 'q':
			case 'Q':
				quit = true;
				break;
			case 's':
			case 'S':
				for (CvBlobs::const_iterator it=blobs.begin(); it!=blobs.end(); ++it)
				{
					std::stringstream filename;
					filename << "redobject_blob_" << std::setw(5) << std::setfill('0') << blobNumber << ".png";
					cvSaveImageBlob(filename.str().c_str(), img, it->second);
					blobNumber++;
					std::cout << filename.str() << " saved!" << std::endl;
				}
				break;
		}

		cvReleaseBlobs(blobs);

	}

	cvReleaseStructuringElement(&morphKernel);
	cvReleaseImage(&frame);
	cvDestroyWindow("Tracking_Blue_Object");

	return 0;
}


