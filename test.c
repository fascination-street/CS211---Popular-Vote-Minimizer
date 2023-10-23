//-----------------------------------------
// Popular Vote Minimizer Application
// Author: Eric Webb
// Course: CS 211 - Programming Practicuum
// Term: Fall 2023
// Assignment: Project 04
//-----------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "MinPopVote.h"

bool test_totalEVs() {
    State aStates[10];
    int res;
    
    aStates[0].electoralVotes = 5;
    aStates[1].electoralVotes = 8;
    aStates[2].electoralVotes = 12;
    aStates[3].electoralVotes = 6;
    aStates[4].electoralVotes = 7;
    aStates[5].electoralVotes = 10;

    printf(" Checking totalEVs() for 5 States:\n");
    res = totalEVs(aStates,5);
    if (res != 38) {
        printf("  individual state EVs are 5, 8, 12, 6, 7\n");
        printf("  expected total EVs = 38, actual total EVs = %d\n",res);
        return 0;
    }

    aStates[0].electoralVotes = 5;
    aStates[1].electoralVotes = 8;
    aStates[2].electoralVotes = 12;
    aStates[3].electoralVotes = 6;
    aStates[4].electoralVotes = 7;
    aStates[5].electoralVotes = 15;
    aStates[6].electoralVotes = 12;
    aStates[7].electoralVotes = 8;
    aStates[8].electoralVotes = 14;
    aStates[9].electoralVotes = 13;
    
    printf(" Checking totalEVs() for 10 States:\n");
    res = totalEVs(aStates,10);
    if (res != 100) {
        printf("  expected total EVs = 100, actual total EVs = %d\n",res);
        return false;
    }

    return true;
}

bool test_totalPVs() {
    State testStates[10];

    testStates[0].popularVotes = 12314;
    testStates[1].popularVotes = 53459;
    testStates[2].popularVotes = 100230;
    testStates[3].popularVotes = 75482;
    testStates[4].popularVotes = 4859;

    int result = totalPVs(testStates, 5);

    printf(" Testing totalPVs() for 5 states:\n");
    if (result != 246344) {
        printf("Expected 246344, instead got %d.\n", result);
        return false;
    }

    testStates[0].popularVotes = 10001;
    testStates[1].popularVotes = 10002;
    testStates[2].popularVotes = 10003;
    testStates[3].popularVotes = 10004;
    testStates[4].popularVotes = 10005;
    testStates[5].popularVotes = 10006;
    testStates[6].popularVotes = 10007;
    testStates[7].popularVotes = 10008;
    testStates[8].popularVotes = 10009;
    testStates[9].popularVotes = 10010;

    result = totalPVs(testStates, 10);

    printf(" Testing totalPVs() for 10 states:\n");
    if (result != 100055) {
        printf("Expected 100055, instead got %d.\n", result);
        return false;
    }
    
    return true; 
}

bool test_setSettings() {
    char** argv = (char**)malloc(sizeof(char*) * 5);
    for (int i = 0; i < 5; i++) {
        argv[i] = (char*)malloc(sizeof(char) * 10);
    }
    int argc = 5;
    strcpy(argv[0], "./a.out");
    strcpy(argv[1], "-y");
    strcpy(argv[2], "2000");
    strcpy(argv[3], "-f");
    strcpy(argv[4], "-q");
    int year = 0;
    bool fastMode = false; 
    bool quietMode = false;

    printf(" Testing all valid flags w/ valid year...\n");

    if (setSettings(argc, argv, &year, &fastMode, &quietMode)) {
        printf("  All flags are valid!\n");
    }
    else {
        printf("  Flags not properly read.\n");
        return false;
    }

    if (year == 2000) {
        printf("  Year was read properly!\n");
    }
    else {
        printf("  Year should have been 2000, but instead it is %d.\n", year);
        return false;
    }

    if (fastMode) {
        printf("  Fast Mode was enabled!\n");
    }
    else {
        printf("  Fast Mode should have been enabled, but wasn't.\n");
    }

    if (quietMode) {
        printf("  Quiet Mode was enabled!\n");
    }
    else {
        printf("  Quiet Mode should have been enabled, but wasn't.\n");
    }

    argc = 1;
    strcpy(argv[0], "./a.out");
    strcpy(argv[1], "-y");
    strcpy(argv[2], "2000");
    strcpy(argv[3], "-f");
    strcpy(argv[4], "-q");
    year = 5420;
    fastMode = true; 
    quietMode = true;

    printf(" Testing empty command line...\n");

    if (setSettings(argc, argv, &year, &fastMode, &quietMode)) {
        printf("  Returned true; success!\n");
    }
    else {
        printf("  Returned false; failure.\n");
        return false;
    }

    if (year == 0) {
        printf("  Year set to default; success!\n");
    }
    else {
        printf("  Year should be 0, instead it is %d\n.", year);
        return false;
    }

    if (!quietMode && !fastMode) {
        printf("  Quiet and Fast Mode are OFF by default. Success!\n");
    }
    else {
        printf("  Either quiet or fast mode are ON. Failure.\n");
        return false;
    }

    argc = 4;
    strcpy(argv[0], "./a.out");
    strcpy(argv[1], "-y");
    strcpy(argv[2], "1433");
    strcpy(argv[3], "-h");
    year = 0;
    fastMode = true; 
    quietMode = true;

    printf(" Testing invalid flag and invalid year...\n");

    if (!setSettings(argc, argv, &year, &fastMode, &quietMode)) {
        printf("  setSettings successfully caught the invalid flag!\n");
    }
    else {
        printf("  setSettings did not return false.\n");
        return false;
    }

    if (year == 0) {
        printf("  setSettings caught the invalid year and set year to 0!\n");
    }
    else {
        printf("  setSettings did not catch the invalid year.\n");
        return false;
    }
    
    for (int i = 0; i < 5; i++) {
        free(argv[i]);
    }
    free(argv);
    return true;
}

bool test_inFilename() {
    char filename[50];
    int year = 1956;
    inFilename(filename, 1956);

    if (strcmp(filename, "data/1956.csv") != 0) {
        printf(" inFilename is not correctly reading the filename. It returned %s\n", filename);
        return false;
    }
    else {
        printf("  inFilename correctly read the file name!\n");
    }
    return true;
}

bool test_outFilename() {
    char filename[50];
    int year = 1956;
    outFilename(filename, 1956);

    if (strcmp(filename, "toWin/1956_win.csv") != 0) {
        printf(" outFilename is not correctly creating the filename. It returned %s\n", filename);
        return false;
    }
    else {
        printf(" outFilename correctly created the filename!\n");
    }
    return true;
}

bool test_parseLine() {
    State testState;
    
    bool allTestsPassed;

    //Testing case with no endline
    char line[] = "Illinois,IL,20,6033744";
    allTestsPassed = parseLine(line, &testState);
    printf("  Testing for line: Illinois,IL,20,6033744\n");

    printf("    STATE = %s\n", testState.name);
    printf("    POSTAL CODE = %s\n", testState.postalCode);
    printf("    ELECTORAL VOTES = %d\n", testState.electoralVotes);
    printf("    POPULAR VOTES = %d\n", testState.popularVotes);
    
    allTestsPassed ? printf("   Success!\n") : printf("   Failure.\n");

    //Testing case with endline
    char line2[] = "Kentucky,KY,8,1326646\n";
    allTestsPassed = parseLine(line2, &testState);
    printf("  Testing for line2: Kentucky,KY,8,1326646\\n\n");

    printf("    MY STATE = %s\n", testState.name);
    printf("    POSTAL CODE = %s\n", testState.postalCode);
    printf("    ELECTORAL VOTES = %d\n", testState.electoralVotes);
    printf("    POPULAR VOTES = %d\n", testState.popularVotes);

    allTestsPassed ? printf("   Success!\n") : printf("   Failure\n");
    
    //Testing bad line
    char line3[] = "Idaho,ID4,554119,";
    allTestsPassed = !parseLine(line3, &testState);
    printf("  Testing for line3: Idaho,ID4,554119,\n");

    allTestsPassed ? printf("   Success!\n") : printf(   "Failure\n");
    
    return allTestsPassed; 
}

bool test_readElectionData() {
    State* states1828 = (State*)malloc(sizeof(State) * 51);
    int numStates;
    readElectionData("data/1828.csv", states1828, &numStates);

    printf(" Testing for data/1828.csv\n");
    if (strcmp(states1828[15].name, "New York") != 0) {
        printf("  State 15 should be New York. Instead it is %s.\n", states1828[15].name);
        return false;
    }
    else {
        printf("  Function successfully returned New York for states1828[15].name.\n");
    }

    if (strcmp(states1828[11].postalCode, "MS") != 0) {
        printf("  Postal Code 11 should be MS. Instead it is %s.\n", states1828[11].postalCode);
        return false;
    }
    else {
        printf("  Function successfully returned MS for states1828[11].postalCode.\n");
    }

    if (states1828[22].electoralVotes != 7) {
        printf("  Vermont has 7 electoral votes. Your function said it has %d.\n", states1828[22].electoralVotes);
        return false;
    }
    else {
        printf("  Function successfully returned 7 for states1828[22].electoralVotes\n");
    }

    free(states1828);

    State states2020[51];
    
    printf(" Testing for data/2020.csv\n");
    readElectionData("data/2020.csv", states2020, &numStates);
    if (strcmp(states2020[39].name, "Rhode Island") != 0) {
        printf("  State 39 is Rhode Island. Function returned %s\n", states2020[39].name);
        return false;
    }
    else {
        printf("  Function successfully returned Rhode Island for states2020[39].name.\n");
    }

    if (strcmp(states2020[50].postalCode, "WY") != 0) {
        printf("  Postal Code 50 should be WY. Instead it is %s.\n", states2020[50].postalCode);
        return false;
    }
    else {
        printf("  Function successfully returned WY for states2020[50].postalCode.\n");
    }

    if (states2020[8].electoralVotes != 3) {
        printf("  Washington D.C. has 3 electoral votes. Your function said it has %d.\n", states2020[8].electoralVotes);
    }
    else {
        printf("  Function successfully returned 3 for states2020[8].electoralVotes.\n");
    }

    return true;
}

bool test_minPVsSlow() {
    //----------------------------------------------------------------- 
    // TODO: Task 7 - write your own test case for minPopVoteAtLeast();
    //                make sure to test all components
    //-----------------------------------------------------------------
    State slowStates[4];
    bool allTestsPassed = true;

    strcpy(slowStates[0].name, "StateA");
    strcpy(slowStates[0].postalCode, "AA");
    slowStates[0].electoralVotes = 5;
    slowStates[0].popularVotes = 60; //31

    strcpy(slowStates[1].name, "StateB");
    strcpy(slowStates[1].postalCode, "BB");
    slowStates[1].electoralVotes = 2;
    slowStates[1].popularVotes = 20; //11

    strcpy(slowStates[2].name, "StateC");
    strcpy(slowStates[2].postalCode, "CC");
    slowStates[2].electoralVotes = 8;
    slowStates[2].popularVotes = 70; //36
    
    strcpy(slowStates[3].name, "StateD");
    strcpy(slowStates[3].postalCode, "DD");
    slowStates[3].electoralVotes = 3;
    slowStates[3].popularVotes = 30; //16

    MinInfo slowInfo = minPopVoteToWin(slowStates, 4);

    printf("  Testing Case 1.\n");
    if (slowInfo.subsetPVs != 47) {
        printf("   The minimum amount of PVs to win is 47. Your function returned %d.\n", slowInfo.subsetPVs);
        allTestsPassed = false;
    }
    else {
        printf("   Function successfully returned 47 PVs!\n");
    }

    if (slowInfo.szSomeStates != 2) {
        printf("   The minimum amount of states needed to win is 2. Your function returned %d.\n", slowInfo.szSomeStates);
        allTestsPassed = false;
    }
    else {
        printf("   Function successfully returned 2 states!\n");
    }

    if (strcmp(slowInfo.someStates[0].name, "StateC") != 0 && strcmp(slowInfo.someStates[1].name, "StateB")) {
        printf("   The states that make the best subset are StateC and StateB. You returned %s and %s, and possibly more if szSomeStates is greater than two.\n", slowInfo.someStates[0].name, slowInfo.someStates[1].name);
        allTestsPassed = false;
    }
    else {
        printf("   You correctly had StateC and StateB in your subset!\n");
    }

    if (!slowInfo.sufficientEVs) {
        printf("   The function did not return true for sufficientEVs.\n");
        allTestsPassed = false;
    }
    else {
        printf("   sufficientEVs returned true!\n");
    }

    slowStates[0].electoralVotes = 5;
    slowStates[0].popularVotes = 65;

    slowStates[1].electoralVotes = 2;
    slowStates[1].popularVotes = 30;

    slowStates[2].electoralVotes = 8;
    slowStates[2].popularVotes = 70;
    
    slowStates[3].electoralVotes = 3;
    slowStates[3].popularVotes = 25;

    slowInfo = minPopVoteToWin(slowStates, 4);

    printf("  Testing Case 3.\n");

    if (slowInfo.subsetPVs != 49) {
        printf("   The minimum amount of PVs to win is 49. Your function returned %d.\n", slowInfo.subsetPVs);
        allTestsPassed = false;
    }
    else {
        printf("   Function successfully returned 49 PVs!\n");
    }

    if (slowInfo.szSomeStates != 2) {
        printf("   The minimum amount of states needed to win is 2. Your function returned %d.\n", slowInfo.szSomeStates);
        allTestsPassed = false;
    }
    else {
        printf("   Function successfully returned 2 states!\n");
    }

    if (strcmp(slowInfo.someStates[0].name, "StateD") != 0 && strcmp(slowInfo.someStates[1].name, "StateC")) {
        printf("   The states that make the best subset are StateD and StateC.\n    You returned %s and %s, and possibly more if szSomeStates is greater than two.\n", slowInfo.someStates[0].name, slowInfo.someStates[1].name);
        allTestsPassed = false;
    }
    else {
        printf("   You correctly had StateD and StateC in your subset!\n");
    }

    if (!slowInfo.sufficientEVs) {
        printf("   The function did not return true for sufficientEVs.\n");
        allTestsPassed = false;
    }
    else {
        printf("   sufficientEVs returned true!\n");
    }

    State slowStates2[3];
    MinInfo slowInfo2;

    strcpy(slowStates2[0].name, "StateA");
    strcpy(slowStates2[0].postalCode, "AA");
    slowStates2[0].electoralVotes = 5;
    slowStates2[0].popularVotes = 50;

    strcpy(slowStates2[1].name, "StateB");
    strcpy(slowStates2[1].postalCode, "BB");
    slowStates2[1].electoralVotes = 2;
    slowStates2[1].popularVotes = 20;

    strcpy(slowStates2[2].name, "StateC");
    strcpy(slowStates2[2].postalCode, "CC");
    slowStates2[2].electoralVotes = 8;
    slowStates2[2].popularVotes = 80;

    slowInfo2 = minPopVoteToWin(slowStates2, 3);

    printf("  Testing Case 5.\n");

    if (slowInfo2.subsetPVs != 41) {
        printf("   The minimum amount of PVs to win is 41. Your function returned %d.\n", slowInfo2.subsetPVs);
        allTestsPassed = false;
    }
    else {
        printf("   Function successfully returned 41 PVs!\n");
    }

    if (slowInfo2.szSomeStates != 1) {
        printf("   The minimum amount of states needed to win is 1. Your function returned %d.\n", slowInfo2.szSomeStates);
        allTestsPassed = false;
    }
    else {
        printf("   Function successfully returned 1 state!\n");
    }

    if (strcmp(slowInfo2.someStates[0].name, "StateC") != 0) {
        printf("   The states that make the best subset are StateC.\n    You returned %s, and possibly more if szSomeStates is greater than one.\n", slowInfo2.someStates[0].name);
        allTestsPassed = false;
    }
    else {
        printf("   You correctly had just StateC in your subset!\n");
    }

    if (!slowInfo2.sufficientEVs) {
        printf("   The function did not return true for sufficientEVs.\n");
        allTestsPassed = false;
    }
    else {
        printf("   sufficientEVs returned true!\n");
    }
    
    return allTestsPassed; //modify this
}

bool test_minPVsFast() {
    State fastStates[51];
    bool allTestsPassed = true;

    strcpy(fastStates[0].name, "StateA");
    strcpy(fastStates[0].postalCode, "AA");
    fastStates[0].electoralVotes = 5;
    fastStates[0].popularVotes = 60; //31

    strcpy(fastStates[1].name, "StateB");
    strcpy(fastStates[1].postalCode, "BB");
    fastStates[1].electoralVotes = 2;
    fastStates[1].popularVotes = 20; //11

    strcpy(fastStates[2].name, "StateC");
    strcpy(fastStates[2].postalCode, "CC");
    fastStates[2].electoralVotes = 8;
    fastStates[2].popularVotes = 70; //36
    
    strcpy(fastStates[3].name, "StateD");
    strcpy(fastStates[3].postalCode, "DD");
    fastStates[3].electoralVotes = 3;
    fastStates[3].popularVotes = 30; //16

    MinInfo fastInfo = minPopVoteToWinFast(fastStates, 4);

    printf("  Testing Small Case.\n");
    if (fastInfo.subsetPVs != 47) {
        printf("   The minimum amount of PVs to win is 47. Your function returned %d.\n", fastInfo.subsetPVs);
        allTestsPassed = false;
    }
    else {
        printf("   Function successfully returned 47 PVs!\n");
    }

    if (fastInfo.szSomeStates != 2) {
        printf("   The minimum amount of states needed to win is 2. Your function returned %d.\n", fastInfo.szSomeStates);
        allTestsPassed = false;
    }
    else {
        printf("   Function successfully returned 2 states!\n");
    }

    if (strcmp(fastInfo.someStates[0].name, "StateC") != 0 && strcmp(fastInfo.someStates[1].name, "StateB")) {
        printf("   The states that make the best subset are StateC and StateB. You returned %s and %s, and possibly more if szSomeStates is greater than two.\n", fastInfo.someStates[0].name, fastInfo.someStates[1].name);
        allTestsPassed = false;
    }
    else {
        printf("   You correctly had StateC and StateB in your subset!\n");
    }

    if (!fastInfo.sufficientEVs) {
        printf("   The function did not return true for sufficientEVs.\n");
        allTestsPassed = false;
    }
    else {
        printf("   sufficientEVs returned true!\n");
    }


    
    return allTestsPassed;
}

int main() {
    printf("Welcome to the Popular Vote Minimizer Testing Suite!\n\n");
    
    printf("Testing totalEVs()...\n"); 
    if (test_totalEVs()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing totalPVs()...\n"); 
    if (test_totalPVs()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing setSettings()...\n"); 
    if (test_setSettings()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }
    
    printf("Testing inFilename()...\n"); 
    if (test_inFilename()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing outFilename()...\n"); 
    if (test_outFilename()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing parseLine()...\n"); 
    if (test_parseLine()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing readElectionData()...\n"); 
    if (test_readElectionData()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }
    
    printf("Testing minPopVoteToWin()...\n"); 
    if (test_minPVsSlow()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing minPopVoteToWinFast()...\n"); 
    if (test_minPVsFast()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    return 0;
}
