#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "eqsys.h"

// #define UNITARY_SPACING

using namespace std;

void fprintframe(FILE* outputFile, int frame, int videoWidth, int videoHeight, vector<vector<int> >& video);
void fprintlinearframe(FILE* outputFile, int startFrame, int currentFrame, int framesToGenerate,
						int videoWidth, int videoHeight, vector<vector<int> >& video);
void fprintframefromspline(FILE* outputFile, int frame, int currentNewFrame, int framesToGenerate,
							int videoWidth, int videoHeight, vector<vector<int> > video, vector<vector<double> > storage);
void naturalCubicSplineBuildA(int framesToGenerate, int videoFrames, Matrix<double>& A);
void naturalCubicSplineBuildB(int pixel, int framesToGenerate, int videoFrames, Matrix<double>& b, vector<vector<int> >& video);

int main(int argc, char* argv[]) {

	if (argc != 5) {
		printf("Use: %s input.txt output.txt interpolationMethod framesToGenerate \n"
			   "Where interpolationMethod:\n"
			   "0: nearest neighbour      \n"
			   "1: lineal  \n"
			   "2: splines \n"
			   "3: other...\n"
			   "e.g. %s input.txt output.txt 0 5 && python textfileToVideo.py && vlc fb.avi\n", argv[0], argv[0]);
		return 0;
	}

	// load input file
	FILE* inputFile = fopen(argv[1], "r");
	if (inputFile == NULL) {
		printf("Error: %s is not a valid input file.\n", argv[1]);
		return 0;
	}

	// video metadata
	int videoFrames, videoHeight, videoWidth, videoFrameRate;
	fscanf(inputFile, "%d", &videoFrames);
	fscanf(inputFile, "%d,%d", &videoHeight, &videoWidth);
	fscanf(inputFile, "%d", &videoFrameRate);
	printf("videoFrames: %d, videoHeight: %d, videoWidth: %d, videoFrameRate: %d\n",
		videoFrames, videoHeight, videoWidth, videoFrameRate);

	// video data (each inner vector has the pixels of a frame in order)
	vector<vector<int> > video(videoFrames, vector<int>(videoWidth*videoHeight));

	for (int frame = 0; frame < videoFrames; ++frame) {
		for (int i = 0; i < videoHeight; ++i) {
			for (int j = 0; j < videoWidth; ++j) {
				fscanf(inputFile, "%d,", &video[frame][i*videoWidth + j]);
			}
		}
	}

	// output file
	FILE* outputFile = fopen(argv[2], "w");
	if (outputFile == NULL) {
		printf("Error: Failed to create output file.\n");
		return 0;
	}

	// select method
	int interpolationMethod = atoi(argv[3]);
	int framesToGenerate = atoi(argv[4]);
	int totalFrames = videoFrames + framesToGenerate * (videoFrames - 1);

	switch (interpolationMethod) {
		case 0:
			printf("interpolationMethod: Nearest Neighbour\n");

			// write header
			fprintf(outputFile, "%d\n%d,%d\n%d\n", totalFrames, videoHeight, videoWidth, videoFrameRate);

			// generate frames
			for (int frame = 0; frame < videoFrames - 1; ++frame) {

				// print current frame
				fprintframe(outputFile, frame, videoWidth, videoHeight, video);

				// closer to first frame
				for (int j = 0; j < framesToGenerate/2; ++j) {
					fprintframe(outputFile, frame, videoWidth, videoHeight, video);
				}

				// closer to second frame
				for (int j = framesToGenerate/2; j < framesToGenerate; ++j) {
					fprintframe(outputFile, frame + 1, videoWidth, videoHeight, video);
				}

			}

			// print last frame
			fprintframe(outputFile, videoFrames-1, videoWidth, videoHeight, video);

			break;
		case 1:
			printf("interpolationMethod: Lineal\n");

			// write header
			fprintf(outputFile, "%d\n%d,%d\n%d\n", totalFrames, videoHeight, videoWidth, videoFrameRate);

			for (int frame = 0; frame < videoFrames - 1; ++frame) {

				// print current frame
				fprintframe(outputFile, frame, videoWidth, videoHeight, video);

				// generate frame, initial frame: frame 0 (current frame)
				for (int j = 1; j <= framesToGenerate; ++j) {
					fprintlinearframe(outputFile, frame, j, framesToGenerate, videoWidth, videoHeight, video);
				}

			}

			// print last frame
			fprintframe(outputFile, videoFrames-1, videoWidth, videoHeight, video);

			break;
		case 2: {
			printf("interpolationMethod: Splines\n");

			// save values of the polinomial coefficients for each pixel (c_j)
			vector<vector<double> > storage(videoWidth*videoHeight, vector<double>(videoFrames));
			
			Matrix<double> A(videoFrames, videoFrames, 0);
			naturalCubicSplineBuildA(framesToGenerate, videoFrames, A);

			// LU Factorization on matrix A. It doesn't depend on the pixel being processed. Only b changes. Ax = b
			// Reduces order from O(n^3) to O(n^2) per iteration.
			EquationSystemLU<double> e(A);

			Matrix<double> b(videoFrames);

			// fit a spline on every pixel
			for (int i = 0; i < videoWidth*videoHeight; ++i) {
				naturalCubicSplineBuildB(i, framesToGenerate, videoFrames, b, video);

				Matrix<double> result(e.solve(b));
				
				// store results
				for (int j = 0; j < videoFrames; ++j) {
					storage[i][j] = result(j);
				}

			}
			
			// write header
			fprintf(outputFile, "%d\n%d,%d\n%d\n", totalFrames, videoHeight, videoWidth, videoFrameRate);

			// generate frames
			for (int frame = 0; frame < videoFrames - 1; ++frame) {

				// print current frame
				fprintframe(outputFile, frame, videoWidth, videoHeight, video);

				// generate new frames
				for (int j = 1; j <= framesToGenerate; ++j) {
					fprintframefromspline(outputFile, frame, j, framesToGenerate, videoWidth, videoHeight, video, storage);
				}

			}

			// print last frame
			fprintframe(outputFile, videoFrames-1, videoWidth, videoHeight, video);

			break;
		}
		default:
			printf("Error: Invalid interpolation method\n");
			return 0;
	}

	fclose(inputFile);
	fclose(outputFile);

	return 0;
}

// new frames are counted from 1.
void fprintframefromspline(FILE* outputFile, int frame, int currentNewFrame, int framesToGenerate,
							int videoWidth, int videoHeight, vector<vector<int> > video, vector<vector<double> > storage) {

	#ifdef UNITARY_SPACING
		int h = framesToGenerate + 1;
	#else
		double h = 1.0 / (framesToGenerate + 1);
	#endif

	for (int pixel = 0; pixel < videoWidth*videoHeight; ++pixel) {
		double c_0 = storage[pixel][frame]; // !
		double c_1 = storage[pixel][frame+1];
		// frame >= 1
		int a_0 = video[frame]  [pixel]; // !
		int a_1 = video[frame+1][pixel];
		double b_0 = (1.0/h)*(a_1 - a_0) - (h/3.0)*(2*c_0 + c_1); // !
		double d_0 = (c_1 - c_0) / (3*h); // !

		// int dX = -currentNewFrame;
		double dX = -currentNewFrame / (framesToGenerate + 1);
		int res = a_0 + b_0*dX + c_0*pow(dX,2) + d_0*pow(dX,3);

		if (res < 0  ) res = 0;
		if (res > 255) res = 255;

		if ((pixel+1) % videoWidth == 0) {
			fprintf(outputFile, "%d\n",  res);
		} else {
			fprintf(outputFile, "%d,", res);
		}

	}
}

/* Possible improvements:
 * 1. Improve cache locality by saving one vector per pixel, right now a single pixel is in several vectors.
 * 3. A is sparse! Better representations!
 * Reminder: A is strictly diagonally dominant, LU factorization without pivoting is valid.
 */
void naturalCubicSplineBuildA(int framesToGenerate, int videoFrames, Matrix<double>& A) {
	A(0,0) = 1;
	A(videoFrames-1,videoFrames-1) = 1;
	
	/* h_i = x_{j+1} - x_{j}
	 * h is CONSTANT! (pixels are equidistant)
	 */
	#ifdef UNITARY_SPACING
		int h_0 = framesToGenerate + 1;
		int h_1 = framesToGenerate + 1;
		int h_2 = framesToGenerate + 1;
	#else
		double h_0 = 1.0 / (framesToGenerate + 1);
		double h_1 = 1.0 / (framesToGenerate + 1);
		double h_2 = 1.0 / (framesToGenerate + 1);
	#endif

	for (int i = 1; i < videoFrames - 1; ++i) {
		// h_i = x_{j+1} - x_{j}
		A(i, i-1) = h_0;
		A(i, i)   = 2*(h_0+h_1);
		A(i, i+1) = h_1;
	}
}

void naturalCubicSplineBuildB(int pixel, int framesToGenerate, int videoFrames, Matrix<double>& b, vector<vector<int> >& video) {

	#ifdef UNITARY_SPACING
		int h = framesToGenerate + 1;
	#else
		double h = 1.0 / (framesToGenerate + 1);
	#endif

	b(0) = 0;
	b(videoFrames-1) = 0;
	for (int i = 1; i < videoFrames -1; ++i) {
		// a_i = f(x_i)
		int a_0 = video[i-1][pixel];
		int a_1 = video[i]  [pixel];
		int a_2 = video[i+1][pixel];
		b(i) = (3.0/h) * (a_2 - a_1) - (3.0/h) * (a_1 - a_0);
	}
}

void fprintframe(FILE* outputFile, int frame, int videoWidth, int videoHeight, vector<vector<int> >& video) {
	for (int i = 0; i < videoHeight; ++i) {
		for (int j = 0; j < videoWidth -1; ++j) {
			fprintf(outputFile, "%d,", video[frame][i*videoWidth + j]);
		}
		fprintf(outputFile, "%d\n", video[frame][i*videoWidth + videoWidth -1]);
	}
}

void fprintlinearframe(FILE* outputFile, int startFrame, int currentFrame, int framesToGenerate,
						int videoWidth, int videoHeight, vector<vector<int> >& video) {
	for (int i = 0; i < videoHeight; ++i) {
		for (int j = 0; j < videoWidth -1; ++j) {
			int y_0 = video[startFrame  ][i*videoWidth + j];
			int y_1 = video[startFrame+1][i*videoWidth + j];
			double m = (y_1 - y_0) / (framesToGenerate + 1.0 - 0);
			double b = y_0 - m*0;
			fprintf(outputFile, "%d,", (int) (m*currentFrame + b));
		}
		int y_0 = video[startFrame  ][i*videoWidth + videoWidth -1];
		int y_1 = video[startFrame+1][i*videoWidth + videoWidth -1];
		double m = (y_1 - y_0) / (framesToGenerate + 1.0 - 0);
		double b = y_0 - m*0;
		fprintf(outputFile, "%d\n", (int) (m*currentFrame + b));
	}

}