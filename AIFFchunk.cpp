//
//  AIFFchunk.cpp
//  gtba
//
//  Created by Mark Williamsen on 10/25/13.
//  Copyright (c) 2013 Williamsonic. All rights reserved.
//

#include "AIFFchunk.h"

chunkAIFFchunk::chunkAIFFchunk(void)   {memset(this, 0, sizeof(chunkAIFFchunk));}
formatAIFFchunk::formatAIFFchunk(void) {memset(this, 0, sizeof(formatAIFFchunk));}
commonAIFFchunk::commonAIFFchunk(void) {memset(this, 0, sizeof(commonAIFFchunk));}
soundAIFFchunk::soundAIFFchunk(void)   {memset(this, 0, sizeof(soundAIFFchunk));}

chunkAIFFchunk::chunkAIFFchunk(istream &i)
{
    memset(this, 0, sizeof(chunkAIFFchunk));
    i.read((char *)this, sizeof(chunkAIFFchunk));
    byteSwap(chunkSize);
}

formatAIFFchunk::formatAIFFchunk(istream &i)
{
    memset(this, 0, sizeof(formatAIFFchunk));
    i.read((char *)this, sizeof(formatAIFFchunk));
    byteSwap(chunkSize);
}

commonAIFFchunk::commonAIFFchunk(istream &i)
{
    memset(this, 0, sizeof(commonAIFFchunk));
    i.read((char *)this, sizeof(commonAIFFchunk));
    byteSwap(chunkSize);
    byteSwap(channelCount);
    byteSwap(frameCount);
    byteSwap(sampleSize);
}

soundAIFFchunk::soundAIFFchunk(istream &i)
{
    memset(this, 0, sizeof(soundAIFFchunk));
    i.read((char *)this, sizeof(soundAIFFchunk));
    byteSwap(chunkSize);
    byteSwap(offset);
    byteSwap(blockSize);
}

formatAIFFchunk::formatAIFFchunk(int theSize)
{
    // populate fields
    memcpy(chunkID, "FORM", 4);   // not a null-terminated string
    chunkSize = theSize + 46;
    memcpy(formType, "AIFF", 4);  // not a null-terminated string
    
    // swap bytes as needed
    byteSwap(chunkSize);
}

commonAIFFchunk::commonAIFFchunk(int theSize, double theRate)
{
    // assume always 16 bit samples, 2 channel stereo
    memcpy(chunkID, "COMM", 4);   // not a null-terminated string
    chunkSize = 18;
    channelCount = 2;
    frameCount = theSize / 4;
    sampleSize = 16;
    dtox80(&theRate, &sampleRate);
    
    // swap bytes as needed
    byteSwap(chunkSize);
    byteSwap(channelCount);
    byteSwap(frameCount);
    byteSwap(sampleSize);
}

soundAIFFchunk::soundAIFFchunk(int theSize)
{
    // populate fields
    memcpy(chunkID, "SSND", 4); // not a null-terminated string
    chunkSize = theSize + 8;
    offset = 0;
    blockSize = 0;
    
    // swap bytes as needed
    byteSwap(chunkSize);
    byteSwap(offset);
    byteSwap(blockSize);
}

void chunkAIFFchunk::showDetails(ostream &o)
{
    // copy 4 character ID to an array
    char id[] = "ABCD";
    memcpy(id, chunkID, 4);
    o << "chunkID," << id << endl;
    o << "chunkSize," << chunkSize << endl;
}

void formatAIFFchunk::showDetails(ostream &o)
{
    chunkAIFFchunk::showDetails(o);
    char id[] = "ABCD";
    memcpy(id, formType, 4);
    o << "formType," << id << endl;
}

void commonAIFFchunk::showDetails(ostream &o)
{
    chunkAIFFchunk::showDetails(o);
    o << "channelCount," << channelCount << endl;
    o << "frameCount," << frameCount << endl;
    o << "sampleSize," << sampleSize << endl;
    o << "sampleRate," << x80tod(&sampleRate) << endl;
}

void soundAIFFchunk::showDetails(ostream &o)
{
    chunkAIFFchunk::showDetails(o);
    o << "offset," << offset << endl;
    o << "blockSize," << blockSize << endl;
}

