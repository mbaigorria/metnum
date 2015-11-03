#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

void fprintframe(FILE* outputFile, int frame, int videoWidth, int videoHeight, vector<vector<int> >& video);

int main(int argc, char* argv[]) {

	if (argc != 5) {
		printf("Use: %s input.txt output.txt interpolationMethod framesToGenerate \n"
			   "Where interpolationMethod:\n"
			   "0: nearest neighbour      \n"
			   "1: lineal  \n"
			   "2: splines \n"
			   "3: other...\n"
			   "e.g. %s input.txt output.txt 0 5\n", argv[0], argv[0]);
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

	// video data
	vector<vector<int> > video(videoFrames, vector<int>(videoWidth*videoHeight));

	for (int i = 0; i < videoFrames; ++i) {
		for (int j = 0; j < videoWidth*videoHeight; ++j) {
			fscanf(inputFile, "%d,", &video[i][j]);
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
			break;
		case 2:
			printf("interpolationMethod: Splines\n");
			break;
		default:
			printf("Error: Invalid interpolation method\n");
			return 0;
	}

	fclose(inputFile);
	fclose(outputFile);

	return 0;
}


void fprintframe(FILE* outputFile, int frame, int videoWidth, int videoHeight, vector<vector<int> >& video) {
	for (int i = 0; i < videoHeight; ++i) {
		for (int j = 0; j < videoWidth -1; ++j) {
			fprintf(outputFile, "%d,", video[frame][i*videoWidth + j]);
		}
		fprintf(outputFile, "%d\n", video[frame][i*videoWidth + videoWidth -1]);
	}
}