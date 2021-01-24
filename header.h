//
//  header.h
//  gtba
//
//  Originated by Mark Williamsen on 9/28/13.
//  Copyright (c) 2013 Williamsonic. All rights reserved.
//
// http://www.williamsonic.com/DipoleMic/
// http://muratnkonar.com/aiff/
//

#ifndef gtba_gtba_h
#define gtba_gtba_h

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <complex>

#include "WAVEchunk.h"
#include "AIFFchunk.h"

using namespace std;

// enumerations
enum burstType
{
  simpleSine, harmonicSine, simpleCosine, harmonicCosine, unknownBurst
};
enum fileType
{
  typeAIFF, typeWAVE, unknownFile
};

// method members
void
setAmplitude (const char*);
void
setLeftAmpl (const char*);
void
setRightAmpl (const char*);
void
setAveraging (const char*);
void
setNumBursts (const char*);
void
setNumCycles (const char*);
void
setDelay (const char*);
void
setHelp (void);
void
setBurstInt (const char*);
void
setBurstMin (const char*);
void
readInputFile (const char*);
void
setStartFreq (const char*);
void
setStopFreq (const char*);
void
writeOutputFile (const char*);
void
reportError (const char*);
void
writeAIFFfile (ofstream&);
void
writeWAVEfile (ofstream&);
void
writeOutputData (ofstream&);
void
writePilotTone (ofstream&);
bool
readAIFFheader (ifstream&);
bool
readWAVEheader (ifstream&);
void
readInputData (ifstream&);
void
readInputBurst (ifstream&);
void
findPilot (ifstream&);
void
reset (void);
void
next (void);
bool
good (void);
void
calc (void);
void
showDetails (ostream&);

#endif
