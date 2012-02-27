/*
 * Code written by Vinz (GeckoGeek.fr)
 */
 
#include <stdio.h>
#include "highgui.h"
#include "cv.h"
 
int main() {
 
	// Touche clavier
	char key;
	// Image
	 IplImage *image;
	// Capture vidéo
	CvCapture *capture;
 
	// Ouvrir le flux vidéo
	//capture = cvCreateFileCapture("/path/to/your/video/test.avi"); // chemin pour un fichier
	capture = cvCreateCameraCapture(CV_CAP_ANY);
 
	// Vérifier si l'ouverture du flux est ok
	if (!capture) {
 
	   printf("Ouverture du flux vidéo impossible !\n");
	   return 1;
 
	}
 
	// Définition de la fenêtre
	cvNamedWindow("GeckoGeek Window", CV_WINDOW_AUTOSIZE);
 
	// Boucle tant que l'utilisateur n'appuie pas sur la touche q (ou Q)
	while(key != 'q' && key != 'Q') {
 
	   // On récupère une image
	   image = cvQueryFrame(capture);
 
	   // On affiche l'image dans une fenêtre
	   cvShowImage( "GeckoGeek Window", image);
 
	   // On attend 10ms
	   key = cvWaitKey(10);
 
	}
 
	cvReleaseCapture(&capture);
	cvDestroyWindow("GeckoGeek Window");
 
	return 0;
 
}
