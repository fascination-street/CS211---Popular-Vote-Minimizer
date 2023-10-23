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
#include <ctype.h>

#include "MinPopVote.h"

bool setSettings(int argc, char** argv, int* year, bool* fastMode, bool* quietMode) {
    bool allValidFlags = true;
    *year = 0;
    *fastMode = false;
    *quietMode = false;

    //loop through all args, ignoring the run command
    for (int i = 1; i < argc; i++) {
        //check for flag, should hopefully ignore the number in year
       if (argv[i][0] == '-') {
           //switch statement depending on letter
           switch(argv[i][1]){
               case 'y': {
                   if (argv[i+1] == NULL || atoi(argv[i+1]) % 4 != 0) {
                       *year = 0;
                   }
                   else {
                       *year = atoi(argv[i+1]);
                   }
                   break;
               }
               case 'f': {
                   *fastMode = true;
                   break;
               }
               case 'q': {
                   *quietMode = true;
                   break;
               }
               default: {
                   //if you end up here you entered a bad flag
                   allValidFlags = false;
               }
           }
       }
    }
    
    return allValidFlags; 
}

void inFilename(char* filename, int year) {
    char yearS[5]; // year string
    //adding to the filename string in segments
    strcpy(filename, "data/"); 
    sprintf(yearS, "%d", year);
    strcat(filename, yearS);
    strcat(filename, ".csv\0");
    return;
}

void outFilename(char* filename, int year) {
    char yearS[5]; //year string
    //adding to the filename in segments
    strcpy(filename, "toWin/");
    sprintf(yearS, "%d", year);
    strcat(filename, yearS);
    strcat(filename, "_win.csv\0");
    return;
}

bool parseLine(char* line, State* myState) {
    //initial string format checker
    int commaCounter = 0;
    for (int i = 0; i < strlen(line); i++) {
        if (line[i] == ',') {
            commaCounter++;
        }
    }
    if (commaCounter != 3) {
        return false; //there should only be 3 commas in a valid format string
    } 
    
    //We shall now parse the line
    int j;
    for (int i = 0; i < 4; i++) {
        char* numberString = (char*)malloc(sizeof(char) * 15);
        //Parsing for the state name
        if (i == 0) {
            for (j = 0; line[j] != ','; j++) {
                //If there's a digit here, end the function; bad format
                if (isdigit(line[j])) {
                    free(numberString);
                    return false;
                }
                myState->name[j] = line[j];
            }
            myState->name[j] = '\0';
        }
        //Parsing for the postal code
        else if (i == 1) {
            for (j = 0; line[j] != ','; j++) {
                //End function if there's digits or more than 2 chars
                if (j > 2 || isdigit(line[j])) {
                    free(numberString);
                    return false;
                }
                myState->postalCode[j] = line[j];
            }
            myState->postalCode[j] = '\0';
        }
        //Parsing for the electoral votes
        else if (i == 2) {
            for (j = 0; line[j] != ','; j++) {
                //End the function if there's any non-digits
                if (!isdigit(line[j])) {
                    free(numberString);
                    return false;
                }
                numberString[j] = line[j];
            }
            numberString[j] = '\0';
            myState->electoralVotes = (int)atoi(numberString);
        }
        //Parsing for the popular votes
        else if (i == 3) {
            for (j = 0; j < strlen(line); j++) {
                //End the loop at a newline
                if (line[j] == '\n') {
                    break;
                }
                //Terminate function if there's any non-digits
                else if (!isdigit(line[j])) {
                    free(numberString);
                    return false;
                }
            }
            strcpy(numberString, line);
            numberString[strlen(line)] = '\0';
            myState->popularVotes = (int)atoi(numberString);
        }
        
        line = &line[j+1];
        free(numberString);
    }
    return true;
}

bool readElectionData(char* filename, State* allStates, int* nStates) {
    //Initializing file
    FILE* electionFile = NULL;
    electionFile = fopen(filename, "r");

    //Declaring useful variables
    int numLines = 0; //will count number of lines in the file (useful because each file has an extra line)
    char stateLine[50]; //Will save the line in this to pass through to parseLine()
    *nStates = 0; //initialize to zero to be safe

    //If we can't find a file, terminate function
    if (electionFile == NULL) {
        printf("No such file \"%s\"\n", filename);
        return false;
    }
    
    //Loop for each line
    do {
        numLines++;
        if (*nStates == 51) {
            //each file has an extra blank line that gives me segfaults, this is my way of accounting for it
            break;
        }
        fgets(stateLine, 50, electionFile);
        parseLine(stateLine, &(allStates[*nStates])); 
        (*nStates)++;
    }
    while(!feof(electionFile));

    //I get an extra state for some reason, getting rid of potential access to it here
    //Since we're dealing with an array on the stack it shouldn't cause any memory leaks
    if (*nStates < 51) {
        (*nStates)--;
    }
    fclose(electionFile);
    return true; 
}

int totalEVs(State* states, int szStates) {
    int total = 0;
    for (int i = 0; i < szStates; i++) {
        total += states[i].electoralVotes;
    }
    return total; 
}

int totalPVs(State* states, int szStates) {
    int total = 0;
    for (int i = 0; i < szStates; i++) {
        total += states[i].popularVotes;
    }
    return total; 
}

MinInfo minPopVoteAtLeast(State* states, int szStates, int start, int EVs) {
    //EVs = the number of electoral votes required to win
    //start = 0, will be incremented throughout the recursion
    //szStates = the number of states in the array
    MinInfo res;
    res.subsetPVs = 0;
    res.szSomeStates = 0;
    res.sufficientEVs = false;

    //if we've gotten enough electoral votes, set flag to true and end recursion
    if (EVs <= 0) {
        res.sufficientEVs = true;
        return res;
    }

    //End recursion; if you get here, this is a failing subset and won't be considered.
    if (start == szStates) {
        return res;
    }
    
    //Skip the state if its electoral votes surpass what's left (indicative of a big population)
    if (states[start].electoralVotes > EVs + 1) {
        return minPopVoteAtLeast(states, szStates, start + 1, EVs);
    }

    //res INCLUDES the current state, resExclude excludes it
    MinInfo resExclude = minPopVoteAtLeast(states, szStates, start + 1, EVs);
    res = minPopVoteAtLeast(states, szStates, start + 1, EVs - states[start].electoralVotes);
        res.subsetPVs += (states[start].popularVotes/2 + 1);
        res.someStates[res.szSomeStates] = states[start];
        res.szSomeStates++;
        
    if (res.sufficientEVs && (res.subsetPVs <= resExclude.subsetPVs || resExclude.subsetPVs == 0)) {
        //Will return res if:
        //  res has sufficient EVs AND
        //  res's subsetPVs is less than or equal than resExclude's OR resExclude's subsetPVs equal zero
        return res;
    }
    else {
        //Will return resExclude if:
        //  res does not have sufficient evs AND
        //  res's subsetPVs are greater than resExclude's AND resExclude's subsetPVs do NOT equal zero
        return resExclude;
    }
}

MinInfo minPopVoteToWin(State* states, int szStates) {
    int totEVs = totalEVs(states,szStates); 
    int reqEVs = totEVs/2 + 1; // required EVs to win election
    return minPopVoteAtLeast(states, szStates, 0, reqEVs);
}

MinInfo minPopVoteAtLeastFast(State* states, int szStates, int start, int EVs, MinInfo** memo) {
    //EVs = the number of electoral votes required to win
    //start = 0, will be incremented throughout the recursion
    //szStates = the number of states in the array
    MinInfo res;
    res.subsetPVs = 0;
    res.szSomeStates = 0;
    res.sufficientEVs = false;

    //if we've gotten enough electoral votes, set flag to true and end recursion
    if (EVs <= 0) {
        res.sufficientEVs = true;
        return res;
    }

    //End recursion; if you get here, this is a failing subset and won't be considered.
    if (start == szStates) {
        return res;
    }

    //If this has already been saved, then we don't need to keep going.
    if (memo[start][EVs].subsetPVs != -1) {
        return memo[start][EVs];
    }
    
    //Skip the state if its electoral votes surpass what's left (indicative of a big population)
    if (states[start].electoralVotes > EVs + 1) {
        return minPopVoteAtLeastFast(states, szStates, start + 1, EVs, memo);
    }

    //res INCLUDES the current state, resExclude excludes it
    MinInfo resExclude = minPopVoteAtLeastFast(states, szStates, start + 1, EVs, memo);
    res = minPopVoteAtLeastFast(states, szStates, start + 1, EVs - states[start].electoralVotes, memo);
        res.subsetPVs += (states[start].popularVotes/2 + 1);
        res.someStates[res.szSomeStates] = states[start];
        res.szSomeStates++;
    
        
    if (res.sufficientEVs && (res.subsetPVs <= resExclude.subsetPVs || resExclude.subsetPVs == 0)) {
        //Will return res if:
        //  res has sufficient EVs AND
        //  res's subsetPVs is less than or equal than resExclude's OR resExclude's subsetPVs equal zero
        memo[start][EVs] = res;
        return res;
    }
    else {
        //Will return resExclude if:
        //  res does not have sufficient evs AND
        //  res's subsetPVs are greater than resExclude's AND resExclude's subsetPVs do NOT equal zero
        memo[start][EVs] = resExclude;
        return resExclude;
    }
}

MinInfo minPopVoteToWinFast(State* states, int szStates) {
    int totEVs = totalEVs(states,szStates);
    int reqEVs = totEVs/2 + 1; // required EVs to win election

    MinInfo** memo = (MinInfo**)malloc((szStates+1)*sizeof(MinInfo*));
    for (int i = 0; i < szStates+1; ++i) {
        memo[i] = (MinInfo*)malloc((reqEVs+1)*sizeof(MinInfo));
        for (int j = 0; j < reqEVs+1; ++j) {
            memo[i][j].subsetPVs = -1;
        }
    }

    MinInfo res = minPopVoteAtLeastFast(states, szStates, 0, reqEVs, memo);
    
    for (int i = 0; i < szStates + 1; i++) {
        free(memo[i]);
    }
    free(memo);
    
    return res;
}

bool writeSubsetData(char* filenameW, int totEVs, int totPVs, int wonEVs, MinInfo toWin) {
    //-----------------------------------------------------   
    // TODO: Task 9 - write the writeSubsetData() function
    //-----------------------------------------------------
    FILE* saveFile = NULL;

    saveFile = fopen(filenameW, "w");

    if (saveFile == NULL) {
        printf("Error attempting to open \"%s\"\n", filenameW);
        return false;
    }

    //put the first line on the file
    char line[80];
    sprintf(line, "%d,%d,%d,%d", totEVs, totPVs, wonEVs, toWin.subsetPVs);
    fprintf(saveFile, "%s\n", line);

    for (int i = toWin.szSomeStates - 1; i >= 0 ; i--) {
        sprintf(line, "%s,%s,%d,%d", toWin.someStates[i].name, toWin.someStates[i].postalCode, toWin.someStates[i].electoralVotes, toWin.someStates[i].popularVotes / 2 + 1);
        fprintf(saveFile, "%s\n", line);
    }

    fclose(saveFile);

    return true; //modify or replace this
}
