#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cv.h>
#include <highgui.h>
#include <list>


using namespace cv;

typedef std::list<Point> pList;
typedef std::list<Point>::iterator pListIterator;

int nbEllipses(Mat img);


int main(int argc, char* argv[]) {
	int device=CV_CAP_ANY;
	int arg1=230;
	int arg2=240;
	if(argc >= 2) {
		device += atoi(argv[1]);
	}

	if(argc >= 4) {
		arg1 = atoi(argv[2]);
		arg2 = atoi(argv[3]);
	}

	VideoCapture cap;
	cap.open(device);
	if(!cap.isOpened()) {
		printf("Erreur cam %d\n",device);
		return -1;
	}
	printf("OPENED\n");
	namedWindow("edges",1);
	for(;;) {
		Mat frame;
		cap >> frame;
		Mat grayscl;
		Mat bin;
		cvtColor(frame, grayscl, CV_RGB2GRAY);
		threshold(grayscl, bin, arg1, 255, THRESH_TOZERO);
		Canny(bin,frame,50,arg2,3,false);
		threshold(frame, bin, 1,255, THRESH_BINARY);
		imshow("edges", bin);
		//printf("%dx%d",bin.size[0],bin.size[1]);
		int k = waitKey(30);
		if(k == 32) {

			nbEllipses(bin);
		}
		if(k == 27) {
			imwrite("capture.jpg", frame);
			break;
		}
	
	}
	return 0;
}

int nbEllipses(Mat img) {
	int nb = 0;
	pList blancs;
	cv::Size2i taille = img.size();

	for( int i = 0; i < taille.width; i++) {
		for(int j = 0; j< taille.height; j++) {
			if(img.at<double>(i,j) != 0) {
				nb++;
				blancs.push_front(Point(i,j));
			}
		}
	}
	pListIterator pt1 = blancs.begin();
	pListIterator pt2 = blancs.begin();
	pListIterator pt3;
	pListIterator end = blancs.end();
	int * acc = new int[max(taille.width,taille.height)];
	while(pt1 != end) {
		printf("1 over %d\n", blancs.size());
		while(pt2 != end) {
			if(pt1 != pt2) {
				int x = (pt1->x + pt2->x)/2;
				int y = (pt1->y + pt2->y)/2;
				double a = (pt2->x - pt1->x)*(pt2->x - pt1->x);
				a += (pt2->y - pt1->y)*(pt2->y - pt1->y);
				a = a/4.0;
				if (a<2500  && a>100) {
					//printf("a=%d\n",(int)ceil(sqrt(a)));

					//int* acc = new int[(int)ceil(sqrt(a))];
				        for(int i=0;i<max(taille.width,taille.height);i++) {
						acc[i] = 0;
					}	
					double alpha = atan2(pt2->y - pt1->y, pt2->x - pt1->x);
					pt3 = blancs.begin();
					while(pt3 != end) {
						if(pt3 != pt1 && pt3 != pt2) {
							double d = (pt3->x - x)*(pt3->x - x);
							d += (pt3->y - y)*(pt3->y - y);
							if(d < a) {
								double f = (pt2->x - pt3->x)*(pt2->x - pt3->x);
								f += (pt2->y - pt3->y)*(pt2->y - pt3->y);
								double ct = (a + d - f)*(a + d - f)/(4.0*a*d);
						       		double b = (a * d * (1.0 - ct))/(a - d*ct);
								//printf("b=%d\n",(int)round(b));
								if(b < a){
									acc[(int)round(sqrt(b))] ++;
								}
							}
						}
						++pt3;
					}
					//int* max = std::max_element(acc,acc+(int)ceil(a));
					//if(*max > 100) {
					//	printf("Ellipse %d\n", *max);
					//}
					
					// TODO: verifier ce delete...
					//delete[] acc;
				}
			}
			++pt2;
		}
		++pt1;
		pt2 = pt1;
	}
	printf("%d\n",blancs.size());
	printf("%d\n",nb);
	return nb;
}
