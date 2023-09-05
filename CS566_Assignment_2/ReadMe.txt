========================================================================
    CONSOLE APPLICATION : CS566_Assignment_2 Project Overview
========================================================================

AppWizard has created this CS566_Assignment_2 application for you.

This file contains a summary of what you will find in each of the files that
make up your CS566_Assignment_2 application.


CS566_Assignment_2.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

CS566_Assignment_2.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

CS566_Assignment_2.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named CS566_Assignment_2.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////

Code Overview:

The code is taking the text file as input to detect whether it is YES or NO.
There are functions for the below operations:
    1. calculateMeanForDCShift - Calculates the DC shift from the silence file.
    2. ignoreMiceCharacteristics - Ignores the first 150 samples containing the noise mainly
    3. findNormalizationValue - The function is used to find the maximum amplitude which will be used during the energy and ZCR calculations.
    5. detectInput - The energy and ZCR would be read from the result (intermediate) file and detects the YES or NO.
    6. calculateEnergyAndZCR - The function is used to calculate the Energy and ZCR considering the dcshift, normalization and ignoring the first noise samples. The values are stored in result file that will be used in the detection process.
    7. _tmain - The main function is the starting point of the program.

Core logic:
    - When the speech is started - Detection using the energy values greater than 100 continuously for atleast 7 frames then it considers the speech is started.
    - The average ZCR values during the speech is being calculated.
    - For Yes, as it is having high noise at the end side, the average ZCR would be more than that of NO.
    - By looking at multiple samples and with the observation, I concluded that the average ZCR is not crossing 13 for NO.
    - So if the average ZCR is less than 7 then it is NO, otherwise it is YES.

/////////////////////////////////////////////////////////////////////////////

Steps to execute the code.
    1. Just run the code, it is having all the input files.
    2. It will prompt for the input file name. Enter the files from the list below for YES and NO.
        - Input1_Yes.txt
        - Input1_No.txt

        There are 6 different samples for Yes and No. So to check with different samples, replace the number 1 with any number between 1 to 6.

    3. The output would be printed with YES or NO.


