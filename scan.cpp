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
	int tr1 =50;
	int tr2 =240;
	int aperture=3;
	if(argc >= 2) {
		device += atoi(argv[1]);
	}


	VideoCapture cap;
	cap.open(device);
	if(!cap.isOpened()) {
		printf("Erreur cam %d\n",device);
		return -1;
	}
	printf("OPENED\n");
	namedWindow("edges",1);
	createTrackbar("seuil", "edges", &arg1, 255);
	createTrackbar("tr1", "edges", &tr1, 1000);
	createTrackbar("tr2", "edges", &tr2, 1000);

	for(;;) {
		Mat frame;
		cap >> frame;
		Mat grayscl;
		Mat bin;
		cvtColor(frame, grayscl, CV_RGB2GRAY);
		threshold(grayscl, bin, arg1, 255, THRESH_TOZERO);
		Canny(bin,frame,tr1,tr2,aperture,false);
		threshold(frame, bin, 1,255, THRESH_BINARY);
		Mat shrink;
		resize(bin, shrink, Size(), 0.1, 0.1, CV_INTER_AREA);
		imshow("edges", bin);
		//printf("%dx%d",bin.size[0],bin.size[1]);
		int k = waitKey(30);
		if(k > 0) {
			nbEllipses(shrink);
			resize(shrink, bin, Size(), 10, 10, CV_INTER_AREA);
			imshow("edges", bin);
			//printf("%dx%d",bin.size[0],bin.size[1]);
			waitKey(-1);
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
				blancs.push_front(Point(i,j));
			}
		}
	}
	img.create(img.size(), img.type());
	pListIterator pt1 = blancs.begin();
	pListIterator pt2 = blancs.begin();
	pListIterator pt3;
	pListIterator end = blancs.end();
	printf("%d\n", (int)blancs.size());
	std::vector<int> acc = std::vector<int>(max(taille.width,taille.height));
	//int i = 1;
	while(pt1 != end) {
		while(pt2 != end) {
			if(pt1 != pt2) {
				//printf("%d over %d\n", i, (int)blancs.size());
				int x = (pt1->x + pt2->x)/2;
				int y = (pt1->y + pt2->y)/2;
				double a = (pt2->x - x)*(pt2->x - x);
				a += (pt2->y - y)*(pt2->y - y);
				if (a<2500  && a>100) {
				        for(unsigned int i=0;i<acc.size();i++) {
						acc[i] = 0;
					}	
					//double alpha = atan2(pt2->y - pt1->y, pt2->x - pt1->x);
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
								if(b < a){
									acc[(int)round(sqrt(b))] += 1;
								}
							}
						}
						++pt3;
					}
					int vmax = 0;
				        for(int i=0;i<(int)ceil(sqrt(a));i++) {
						if(acc[i]>vmax) {
							vmax = acc[i];
						}
					}
					if(vmax >= 30) {
						printf("Ellipse %d\n", vmax);
						img.at<double>((int)x,(int)y) = 1;
						nb++;
					}
				}
			}
			++pt2;
		}
		++pt1;
		//i++;
		pt2 = pt1;
	}
	printf("%d\n",nb);
	printf("%d\n",(int)blancs.size());
	return nb;
}
