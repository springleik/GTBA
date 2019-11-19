//
//  WAVEchunk.cpp
//  gtba
//
//  Created by Mark Williamsen on 10/25/13.
//  Copyright (c) 2013 Williamsonic. All rights reserved.
//

#include "WAVEchunk.h"

chunkWAVEchunk::chunkWAVEchunk(void)  {memset(this, 0, sizeof(chunkWAVEchunk));}
riffWAVEchunk::riffWAVEchunk(void)    {memset(this, 0, sizeof(riffWAVEchunk));}
formatWAVEchunk::formatWAVEchunk(void){memset(this, 0, sizeof(formatWAVEchunk));}
dataWAVEchunk::dataWAVEchunk(void)    {memset(this, 0, sizeof(dataWAVEchunk));}

chunkWAVEchunk::chunkWAVEchunk(istream &i)
{
    memset(this, 0, sizeof(chunkWAVEchunk));
    i.read((char *)this, sizeof(chunkWAVEchunk));
}

riffWAVEchunk::riffWAVEchunk(istream &i)
{
    memset(this, 0, sizeof(riffWAVEchunk));
    i.read((char *)this, sizeof(riffWAVEchunk));
}

formatWAVEchunk::formatWAVEchunk(istream &i)
{
    memset(this, 0, sizeof(formatWAVEchunk));
    i.read((char *)this, sizeof(formatWAVEchunk));
}

dataWAVEchunk::dataWAVEchunk(istream &i)
{
    memset(this, 0, sizeof(dataWAVEchunk));
    i.read((char *)this, sizeof(dataWAVEchunk));
}

// set data members of RIFF chunk
riffWAVEchunk::riffWAVEchunk(int theSize)
{
    // populate fields
	memcpy(chunkID, "RIFF", 4);	// not a null-terminated string
	chunkSize = theSize + 36;
	memcpy(format, "WAVE", 4);	// not a null-terminated string
}

// set data members of fmt chunk
formatWAVEchunk::formatWAVEchunk(int theRate)
{
	// assume always 16 bit samples, 2 channel stereo
	memcpy(chunkID, "fmt ", 4);	// not a null-terminated string
	chunkSize = 16 ;	// fixed size = 16 for PCM
	fmtCode = 1;		// code = 1 for PCM
	numChan = 2;		// number of audio channels
	sampRate = theRate;                     // sample rate per second
	byteRate = numChan * sampRate * 2;		// byte rate per second
	blockAlign = numChan * 2;				// byte count per sample
	bitsSamp = 16;		// bit count per sample
}

// set data members of data chunk
dataWAVEchunk::dataWAVEchunk(int theSize)
{
    // populate fields
	memcpy(chunkID, "data", 4);	// not a null-terminated string
	chunkSize = theSize;
}

void chunkWAVEchunk::showDetails(ostream &o)
{
    char id[] = "ABCD";
    memcpy(id, chunkID, 4);
    o << "chunkID," << id << endl;
    o << "chunkSize," << chunkSize << endl;
}

void riffWAVEchunk::showDetails(ostream &o)
{
    chunkWAVEchunk::showDetails(o);
    char fmt[] = "ABCD";
    memcpy(fmt, format, 4);
    o << "format," << fmt << endl;
}

void formatWAVEchunk::showDetails(ostream &o)
{
    chunkWAVEchunk::showDetails(o);
    o << "fmtCode," << fmtCode << endl;
	o << "numChan," << numChan << endl;
	o << "sampRate," << sampRate << endl;
	o << "byteRate," << byteRate << endl;
	o << "blockAlign," << blockAlign << endl;
	o << "bitsSamp," << bitsSamp << endl;
}

void dataWAVEchunk::showDetails(ostream &o)
{
    chunkWAVEchunk::showDetails(o);
}



