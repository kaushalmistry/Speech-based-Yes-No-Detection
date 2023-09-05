// CS566_Assignment_2.cpp : Detect between the two words: YES or NO

#include "stdafx.h"
#include "stdio.h"
#include "conio.h"
#include "stdlib.h"

/**
 * @brief The function calculates the mean value of the silence file. it would be used to remove the DC Shift effect
 *
 * @param fileName - The file name from which the DC Shift would be calculatd
 * @return the mean value
 */
long double calculateMeanForDCShift(char *fileName)
{
    FILE *filePtr = fopen(fileName, "r");

    long double mean = 0;
    int count = 1;
    int i;

    fscanf_s(filePtr, "%d", &i);
    while (!feof(filePtr))
    {
        count++;
        mean += i;
        fscanf_s(filePtr, "%d", &i);
    }
    fclose(filePtr);

    return mean / count;
}

void ignoreMiceCharacteristics(FILE* fptr) {
    int count = 0;
    int i;
    while (count < 150) {
        fscanf(fptr, "%d", &i);
        count++;
    }
}

/**
 * @brief It would find the maximum value to normalize with
 *
 * @param fileName - the input file name
 * @return int max value in the amplitudes
 */
int findNormalizationValue(char *fileName)
{
    FILE *filePtr = fopen(fileName, "r");

    int maxAmp = INT_MIN;
    int i;

    fscanf_s(filePtr, "%d", &i);
    while (!feof(filePtr))
    {
        maxAmp = abs(i) > maxAmp ? abs(i) : maxAmp;
        fscanf_s(filePtr, "%d", &i);
    }
    fclose(filePtr);

    return maxAmp;
}

/**
 * @brief Detects the input that is YES or NO
 *
 * @param fileName - The name of the file having energy and ZCR
 */
void detectInput(char *fileName)
{
    // Read the text file
    FILE *filePtr = fopen(fileName, "r");

    if (filePtr == NULL)
    {
        printf("Error while opening the file to detect Yes or No");
        return;
    }

    int frameNo, zcr;
    long double energy;
    int sumZCR = 0, avgZCR;
    int startFrame = 0, endFrame = 0;
    int rangeFlag = 0;
    int energyUpCount = 0;

    while (!feof(filePtr))
    {
        fscanf(filePtr, "%d", &frameNo);
        fscanf(filePtr, "%Lf", &energy);
        fscanf(filePtr, "%d", &zcr);

        // putting the start and end pointer if the energy level goes beyond and below 100
        if (energy >= 100.0 && rangeFlag == 0)
        {
            rangeFlag = 1;
            startFrame = frameNo;
        }

        // Checking if the data is continuously increasing for min 7 frames 
        if (energy >= 100 && rangeFlag == 1) {
            energyUpCount++;
        }

        if (energy < 100.0 && energyUpCount < 7 && rangeFlag == 1) {
            energyUpCount = 0;
            rangeFlag = 0;
            sumZCR = 0;
        }
        else if (energy < 100.0 && rangeFlag == 1)
        {
            endFrame = frameNo;
            rangeFlag = 1;
            break;
        }

        if (rangeFlag)
        {
            sumZCR += zcr;
        }
    }

    fclose(filePtr);

    // Calculates the average ZCR for the speech part
    avgZCR = sumZCR / (endFrame - startFrame);

    // Distinguishing between yes and no
    if (avgZCR < 13)
    {
        printf("\nIt's a 'NO'");
    }
    else
    {
        printf("\nIt's a 'YES'");
    }
}

/**
 * @brief It is used to calculate the Energy and ZCR for the give file.
 * The two things are combined in one function to avoid reading the file multiple times
 * @param inputFileName - The input file name
 * @param outputFileName - The output file name where the energy and zcr would be stored
 * @param normValue - Normalization value
 * @param dcShift - The dcShift effect
 * @param frameSize the framesize (default would be 100)
 */
void calculateEnergyAndZCR(char *inputFileName, char *outputFileName, int normValue, long double dcShift = 0, int frameSize = 100)
{
    // The files for input and output

    FILE *inputFilePtr = fopen(inputFileName, "r");
    FILE *outputFilePtr = fopen(outputFileName, "w");

    if (inputFilePtr == NULL)
    {
        printf("Error in opening the input file.");
        return;
    }

    // Ignoring the first 100 samples that contains the noise
    ignoreMiceCharacteristics(inputFilePtr);

    // Initializing variables needed while execution
    int modifiedAmplitude, curAmp, steCount = 1;
    long double steSum = 0;

    int zcrCount = 0;
    int prevAmplitude = 0;
    int frameNo = 1;
    // printf("Frame No      Energy      ZCR\n");

    while (!feof(inputFilePtr))
    {

        fscanf(inputFilePtr, "%d", &curAmp);
        modifiedAmplitude = (curAmp - dcShift) * 5000 / normValue;

        if (steCount == frameSize)
        {
            // printf("%d          %.2f          %d\n", frameNo, steSum / frameSize, zcrCount);
            fprintf(outputFilePtr, "%d %.2Lf %d\n", frameNo, steSum / frameSize, zcrCount);
            steCount = 1;
            steSum = 0;
            zcrCount = 0;
            frameNo++;
        }
        steSum += (modifiedAmplitude * modifiedAmplitude);
        steCount++;

        if (prevAmplitude < 0 && modifiedAmplitude > 0 || prevAmplitude > 0 && modifiedAmplitude < 0)
        {
            zcrCount++;
        }

        prevAmplitude = modifiedAmplitude;
    }

    fclose(inputFilePtr);
    fclose(outputFilePtr);
}

/**
 * @brief The starting point of the program.
 *
 * @param argc - The number of command line argument
 * @param argv - The command line arguments
 * @return int
 */
int _tmain(int argc, _TCHAR *argv[])
{
    // The files used in the code
    char inputFileName[100];
    printf("Enter the file name: ");
    scanf("%s", &inputFileName);

    char *outputFileName = "resultFile.txt";

    // Find the normalization value
    int normalizationValue = findNormalizationValue(inputFileName);

    // Calculating the DC Shift from the initial silence
    long double dcShift = calculateMeanForDCShift("silence.txt");

    // Calculates the Energy and ZCR and stores in the output file
    calculateEnergyAndZCR(inputFileName, outputFileName, normalizationValue, dcShift);

    // Detects the input as YES or NO
    detectInput(outputFileName);

    getch();
    return 0;
}
