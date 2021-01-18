//
//  main.cpp
//  gtba
//
//  Originated by Mark Williamsen on 9/28/13.
//  Copyright (c) 2013 Williamsonic. All rights reserved.
//

#include <cassert>
#include <cstring>

#include "header.h"

// data members
const int sampleRate = 44100;   // samples per second
double leftAmpl = 0.5;          // left channel gain factor relative to unity
double rightAmpl = 0.5;         // right channel gain factor relative to unity
int averaging = 1;              // number of bursts to average over
int numBursts = 201;            // number of bursts
int numCycles = 1;              // number of cycles per burst
int delay = 22050;              // samples to start of first burst
fileType theFileType = typeAIFF;              // file format type
int burstInterval = 22050;      // total samples per burst
int burstMin = 100;             // minimum number of samples within any burst
double startFreq = 100.;        // frequency of first tone burst
double stopFreq = 10000.;       // frequency of last tone burst
burstType theBurstType = harmonicSine;  // waveform type
int duration = 100;             // actual sample count within burst
double freqIncrement = 1.0;     // constant factor relating adjacent frequencies
double nominalFreq = 100.;      // nominal tone burst frequency
double actualFreq = 100.;       // actual tone burst frequency
int burstCount = 0;             // index to count bursts
int cycleCount = 0;             // index to count cycles
double factor = 100.;           // frequency in sample-based units
bool verbose = true;            // verbose output mode
bool pilot = true;              // pilot tone enabled
bool phase = true;              // output in phase

int
main (int argc, const char *argv[])
{
  // check some sizes
  assert(12 == sizeof(formatAIFFchunk));
  assert(26 == sizeof(commonAIFFchunk));
  assert(16 == sizeof(soundAIFFchunk));
  assert(12 == sizeof(riffWAVEchunk));
  assert(24 == sizeof(formatWAVEchunk));
  assert(8 == sizeof(dataWAVEchunk));

  // process command line args
  cout
      << "Gated Toneburst Analysis Program from <www.williamsonic.com>," __DATE__
      << '.' << endl;
  if (1 == argc)
    {
      cout << "Usage: ./gtba [-arg1 val1 -arg2 val2 ...]" << endl;
    }

  const char *s = NULL;

  // assign, test for null, then increment
  while ((s = *++argv))
    {
      // consume leading plus or minus sign
      char c = '\0';
      if (strchr ("+-/", *s))
	{
	  c = *s++;
	}
      else
	continue;

      // command selector
      if (!strncmp ("aiff", s, 3))
	{
	  theFileType = typeAIFF;
	}
      else if (!strncmp ("amplitude", s, 3))
	{
	  setAmplitude (*++argv);
	}
      else if (!strncmp ("average", s, 3))
	{
	  setAveraging (*++argv);
	}
      else if (!strncmp ("cos1", s, 4))
	{
	  theBurstType = simpleCosine;
	}
      else if (!strncmp ("cos2", s, 4))
	{
	  theBurstType = harmonicCosine;
	}
      else if (!strncmp ("count", s, 3))
	{
	  setNumBursts (*++argv);
	}
      else if (!strncmp ("cycles", s, 3))
	{
	  setNumCycles (*++argv);
	}
      else if (!strncmp ("delay", s, 3))
	{
	  setDelay (*++argv);
	}
      else if (!strncmp ("help", s, 3))
	{
	  setHelp ();
	}
      else if (!strncmp ("interval", s, 3))
	{
	  setBurstInt (*++argv);
	}
      else if (!strncmp ("left", s, 3))
	{
	  setLeftAmpl (*++argv);
	}
      else if (!strncmp ("minimum", s, 3))
	{
	  setBurstMin (*++argv);
	}
      else if (!strncmp ("phase", s, 3))
	{
	  phase = ('+' == c);
	}
      else if (!strncmp ("pilot", s, 3))
	{
	  pilot = ('+' == c);
	}
      else if (!strncmp ("read", s, 3))
	{
	  readInputFile (*++argv);
	}
      else if (!strncmp ("right", s, 3))
	{
	  setRightAmpl (*++argv);
	}
      else if (!strncmp ("show", s, 3))
	{
	  reset ();
	  showDetails (cout);
	}
      else if (!strncmp ("sin1", s, 4))
	{
	  theBurstType = simpleSine;
	}
      else if (!strncmp ("sin2", s, 4))
	{
	  theBurstType = harmonicSine;
	}
      else if (!strncmp ("start", s, 3))
	{
	  setStartFreq (*++argv);
	}
      else if (!strncmp ("stop", s, 3))
	{
	  setStopFreq (*++argv);
	}
      else if (!strncmp ("verbose", s, 3))
	{
	  verbose = ('+' == c);
	}
      else if (!strncmp ("wave", s, 3))
	{
	  theFileType = typeWAVE;
	}
      else if (!strncmp ("write", s, 3))
	{
	  writeOutputFile (*++argv);
	}
      else
	{
	  reportError (s);
	}
    }
  return 0;
}

// set methods
void
setStartFreq (const char *s)
{
  if (s && *s)
    startFreq = stopFreq = atof (s);
}
void
setStopFreq (const char *s)
{
  if (s && *s)
    stopFreq = atof (s);
}
void
setAveraging (const char *s)
{
  if (s && *s)
    averaging = atoi (s);
}
void
setBurstInt (const char *s)
{
  if (s && *s)
    burstInterval = atoi (s);
}
void
setNumBursts (const char *s)
{
  if (s && *s)
    numBursts = atoi (s);
}
void
setNumCycles (const char *s)
{
  if (s && *s)
    numCycles = atoi (s);
}
void
setAmplitude (const char *s)
{
  if (s && *s)
    leftAmpl = rightAmpl = atof (s);
}
void
setLeftAmpl (const char *s)
{
  if (s && *s)
    leftAmpl = atof (s);
}
void
setRightAmpl (const char *s)
{
  if (s && *s)
    rightAmpl = atof (s);
}
void
setBurstMin (const char *s)
{
  if (s && *s)
    burstMin = atoi (s);
}
void
setDelay (const char *s)
{
  if (s && *s)
    delay = atoi (s);
}
void
reportError (const char *s)
{
  if (s && *s)
    cout << "Unknown command: " << s << endl;
}
void
setHelp (void)
{
  cout
      << "Arguments are minus sign '-' with (at least) the first four characters from keywords listed below."
	  "\nSeparate keywords from values with spaces, enclose values that contain spaces in double quotes."
	  "\n -aiff      (aiff file format)"
	  "\n -amplitude gain factor"
	  "\n -average   bursts to average"
	  "\n -cos1      (simple cosine)"
	  "\n -cos2      (harmonic cosine)"
	  "\n -count     number of bursts"
	  "\n -cycles    minimum cycles per burst"
	  "\n -delay     samples to start of first burst"
	  "\n -help      (this list)"
	  "\n -interval  total samples per interval"
	  "\n -left      left channel gain"
	  "\n -minimum   minimum samples per burst"
	  "\n -phase     (+/- phase on output)"
	  "\n -pilot     (+/- pilot tone)"
	  "\n -read      name of input file"
	  "\n -right     right channel gain"
	  "\n -show      (measurement setup)"
	  "\n -sin1      (simple sine"
	  "\n -sin2      (harmonic sine)"
	  "\n -start     first frequency (Hz)"
	  "\n -stop      last frequency (Hz)"
	  "\n -verbose   (+/- verbose output)"
	  "\n -wave      (wave file format)"
	  "\n -write     name of output file" << endl;
}

string
getText (burstType b)
{
  switch (b)
    {
    case simpleSine:
      return "simpleSine";
    case simpleCosine:
      return "simpleCosine";
    case harmonicSine:
      return "harmonicSine";
    case harmonicCosine:
      return "harmonicCosine";
    default:
      return "Unknown burst type";
    }
}

string
getText (fileType f)
{
  switch (f)
    {
    case typeAIFF:
      return "typeAIFF";
    case typeWAVE:
      return "typeWAVE";
    default:
      return "Unknown file type";
    }
}

void
showDetails (ostream &o)
{
  o << "startFreq," << startFreq << endl;
  o << "stopFreq," << stopFreq << endl;
  o << "averaging," << averaging << endl;
  o << "burstInterval," << burstInterval << endl;
  o << "numBursts," << numBursts << endl;
  o << "numCycles," << numCycles << endl;
  o << "leftAmpl," << leftAmpl << endl;
  o << "rightAmpl," << rightAmpl << endl;
  o << "theBurstType," << getText (theBurstType) << endl;
  o << "theFileType," << getText (theFileType) << endl;
  o << "burstMin," << burstMin << endl;
  o << "delay," << delay << endl;
  o << "sampleRate," << sampleRate << endl;
  o << "duration," << duration << endl;
  o << "freqIncrement," << freqIncrement << endl;
  o << "nominalFreq," << nominalFreq << endl;
  o << "actualFreq," << actualFreq << endl;
  o << "burstCount," << burstCount << endl;
  o << "factor," << factor << endl;
  o << boolalpha;
  o << "phase," << phase << endl;
  o << "pilot," << pilot << endl;
  o << "verbose," << verbose << endl;
}

void
readInputFile (const char *s)
{
  if (!s || !*s)
    {
      cout << "Required file name missing." << endl;
      return;
    }

  // must be opened in binary mode
  ifstream inFile (s, ios::in | ios::binary);
  if (!inFile)
    {
      cout << "Failed to open input file: " << s << endl;
      return;
    }
  cout << "Reading input file," << s << endl;

  theFileType = unknownFile;
  if (readAIFFheader (inFile))
    {
      theFileType = typeAIFF;
      readInputData (inFile);
      return;
    }

  inFile.clear ();
  inFile.seekg (0, inFile.beg);
  if (readWAVEheader (inFile))
    {
      theFileType = typeWAVE;
      readInputData (inFile);
      return;
    }

  // TODO consider handling AIFC file type
  cout << "Unexpected input file type!" << endl;
}

bool
readAIFFheader (ifstream &inFile)
{
  // read AIFF header from disk
  formatAIFFchunk theFormat (inFile);
  if (strncmp (theFormat.chunkID, "FORM", 4))
    {
      return false;
    }
  if (strncmp (theFormat.formType, "AIFF", 4))
    {
      return false;
    }
  cout << "AIFF file detected." << endl;

  // obtain a list of chunks and file positions
  map<string, long int> theMap;
  theMap["FORM"] = 0;
  while (inFile)
    {
      long thePos = inFile.tellg ();
      chunkAIFFchunk theChunk (inFile);
      if (!inFile)
	{
	  break;
	}
      char s[] = "ABCD";
      memcpy (s, theChunk.chunkID, 4);
      theMap[s] = thePos;
      inFile.seekg (theChunk.chunkSize, inFile.cur);
    };

  // show list contents on console
  cout << "Chunk List:" << endl;
  for (auto i = theMap.begin (); i != theMap.end (); i++)
    {
      cout << i->first << ',' << i->second << endl;
    }

  // populate required common chunk
  // TODO find() may crash if chunk is missing (4 pl.)
  long chunkPos = 0;
  chunkPos = theMap.find ("COMM")->second;
  if (!chunkPos)
    {
      cout << "Required COMM chunk not found!" << endl;
      return false;
    }
  inFile.clear ();
  inFile.seekg (chunkPos, inFile.beg);
  commonAIFFchunk theCommon (inFile);
  if (!inFile)
    {
      cout << "Failed to read COMM chunk." << endl;
      return false;
    }

  // populate required sound chunk
  chunkPos = 0;
  chunkPos = theMap.find ("SSND")->second;
  if (!chunkPos)
    {
      cout << "Required SSND chunk not found!" << endl;
      return false;
    }
  inFile.seekg (chunkPos, inFile.beg);
  soundAIFFchunk theSound (inFile);
  if (!inFile)
    {
      cout << "Failed to read SSND chunk." << endl;
      return false;
    }

  // show results on console
  if (verbose)
    {
      theFormat.showDetails (cout);
      theCommon.showDetails (cout);
      theSound.showDetails (cout);
    }

  // leave file pointer on start of sound data
  return true;
}

bool
readWAVEheader (ifstream &inFile)
{
  // read WAVE header from disk
  riffWAVEchunk theRiff (inFile);
  if (strncmp (theRiff.chunkID, "RIFF", 4))
    {
      return false;
    }
  if (strncmp (theRiff.format, "WAVE", 4))
    {
      return false;
    }
  cout << "WAVE file detected." << endl;

  // obtain a list of chunks and file positions
  map<string, long int> theMap;
  theMap["RIFF"] = 0;
  while (inFile)
    {
      long thePos = inFile.tellg ();
      chunkWAVEchunk theChunk (inFile);
      if (!inFile)
	{
	  break;
	}
      char s[] = "ABCD";
      memcpy (s, theChunk.chunkID, 4);
      theMap[s] = thePos;
      inFile.seekg (theChunk.chunkSize, inFile.cur);
    };

  // show list contents on console
  cout << "Chunk List:" << endl;
  for (auto i = theMap.begin (); i != theMap.end (); i++)
    {
      cout << i->first << ',' << i->second << endl;
    }

  // populate required common chunk
  // TODO find() may crash if chunk is missing (4 pl.)
  long chunkPos = 0;
  chunkPos = theMap.find ("fmt ")->second;
  if (!chunkPos)
    {
      cout << "Required fmt chunk not found!" << endl;
      return false;
    }
  inFile.clear ();
  inFile.seekg (chunkPos, inFile.beg);
  formatWAVEchunk theFormat (inFile);
  if (!inFile)
    {
      cout << "Failed to read fmt chunk." << endl;
      return false;
    }

  // populate required sound chunk
  chunkPos = 0;
  chunkPos = theMap.find ("data")->second;
  if (!chunkPos)
    {
      cout << "Required data chunk not found!" << endl;
      return false;
    }
  inFile.seekg (chunkPos, inFile.beg);
  dataWAVEchunk theData (inFile);
  if (!inFile)
    {
      cout << "Failed to read data chunk." << endl;
      return false;
    }

  // show results on console
  if (verbose)
    {
      theRiff.showDetails (cout);
      theFormat.showDetails (cout);
      theData.showDetails (cout);
    }

  // leave file pointer on start of sound data
  return true;
}

// tone burst iterator
void
next (void)
{
  // only needed for freq sweep case
  nominalFreq *= freqIncrement;
  calc ();
  burstCount--;
  return;
}

// test to see if we are done
bool
good (void)
{
  return (burstCount > 0);
}

void
reset (void)
{
  // recycle toneBurst object
  burstCount = numBursts;
  cycleCount = numCycles;
  nominalFreq = startFreq;
  freqIncrement = pow (stopFreq / startFreq, 1.0 / (numBursts - 1));
  calc ();
}

// update data members for a particular frequency
void
calc (void)
{
  // find least number of full cycles whose duration exceeds burst minimum
  while ((sampleRate / nominalFreq) * cycleCount < burstMin)
    {
      cycleCount++;
    }

  // round off (truncate) duration to exact samples
  duration = int ((sampleRate / nominalFreq) * cycleCount);

  // find frequency that exactly fills the duration
  actualFreq = 1.0 * sampleRate * cycleCount / duration;

  // calculate common factor for sine and cosine
  factor = 2.0 * M_PI * actualFreq / sampleRate;
}

// show burst parameters on console
void
showBurst (ostream &o)
{
  o << cycleCount << ',' << duration << ',' << nominalFreq << ',' << actualFreq;
}

void
readInputData (ifstream &inFile)
{
  // check for pilot tone
  findPilot (inFile);

  // show column headings here
  cout << "numCyc,duration,nomFreq,actFreq,"
      "abs1,abs2,dB1,dB2,dBDiff,"
      "phase1,phase2,phaseDiff,bkg1,bkg2" << endl;

  // iterate over tone bursts while reading from disk
  for (reset (); good (); next ())
    {
      // check input file before reading
      if (inFile.eof ())
	{
	  cerr << "Failed to read tone bursts from disk." << endl;
	  return;
	}
      showBurst (cout);
      readInputBurst (inFile);
    }
  cout << "Next file position," << inFile.tellg () << endl;
}

// read tone burst from disk, matched filter technique
// looks only for the exact frequency being measured
void
readInputBurst (ifstream &inFile)
{
  int i = 0, j = 0;		// local loop indices
  short a = 0, b = 0;
  complex<double> sum1 (0, 0);          // channel 1 response
  complex<double> sum2 (0, 0);          // channel 2 response
  complex<double> sum3 (0, 0);          // channel 1 background
  complex<double> sum4 (0, 0);          // channel 2 background
  complex<double> cfactor (0, factor); // complex frequency in rad/sec
  complex<double> shift (0, -1.0);     // 90 deg. phase shift

  // iterate over averaging, burst interval
  for (i = 0; i < averaging; i++)
    for (j = 0; j < burstInterval; j++)
      {
	// read data from both channels
	inFile.read ((char*) &a, 2);
	inFile.read ((char*) &b, 2);

	// swap bytes for AIFF files
	if (typeAIFF == theFileType)
	  {
	    byteSwap (a);
	    byteSwap (b);
	  }

	// analyze burst response
	// this is a single frequency discrete Fourier transform
	// phase angle is referred to start of burst
	if (j < duration)
	  {
	    complex<double> ccoeff = exp (cfactor * double (j)) * shift;
	    sum1 += double (a) * ccoeff;
	    sum2 += double (b) * ccoeff;
	  }

	// analyze background level, near end of burst burstInterval
	if ((j < (burstInterval - duration))
	    && (j >= (burstInterval - (2 * duration))))
	  {
	    complex<double> ccoeff = exp (cfactor * double (j)) * shift;
	    sum3 += double (a) * ccoeff;
	    sum4 += double (b) * ccoeff;
	  }
      }

  // factor out sample count and averaging, normalize to +0 dB
  sum1 /= (duration * averaging * leftAmpl * 32767.0 / 2.0);
  sum2 /= (duration * averaging * rightAmpl * 32767.0 / 2.0);
  sum3 /= (duration * averaging * leftAmpl * 32767.0 / 2.0);
  sum4 /= (duration * averaging * rightAmpl * 32767.0 / 2.0);

  // report results to console
  // 0.0 dB reference level when analyzing original generated file
  cout << ',' << abs (sum1)          // magnitude channel 1
      << ',' << abs (sum2)               // magnitude channel 2
      << ',' << 20.0 * log10 (abs (sum1))   // dB channel 1
      << ',' << 20.0 * log10 (abs (sum2))   // dB channel 2
      << ',' << 20.0 * log10 (abs (sum1) / abs (sum2))	// dB difference
      << ',' << arg (sum1)               // phase channel 1
      << ',' << arg (sum2)               // phase channel 2
      << ',' << arg (sum1) - arg (sum2)		// phase difference
      << ',' << 20.0 * log10 (abs (sum3))		// dB background 1
      << ',' << 20.0 * log10 (abs (sum4))		// dB background 2
      << endl;
  return;
}

void
writeOutputFile (const char *s)
{
  if (!s || !*s)
    {
      cout << "Required file name missing." << endl;
      return;
    }

  // don't overwrite existing file
  ifstream inFile (s);
  if (inFile)
    {
      inFile.close ();
      cout << "File already exists: " << s << endl;
      return;
    }

  // must be opened in binary mode
  ofstream outFile (s, ios::out | ios::binary);
  if (!outFile)
    {
      cout << "Failed to open output file: " << s << endl;
      return;
    }
  cout << "Writing output file," << s << endl;

  switch (theFileType)
    {
    case typeAIFF:
      writeAIFFfile (outFile);
      break;

    case typeWAVE:
      writeWAVEfile (outFile);
      break;

    default:
      cout << "Unexpected output file type!" << endl;
      break;
    }
}

// get byte count for sound data in wave file
int
getSize (void)
{
  // bytes/sample * num channels * (samples/burst * averaging * num bursts + delay)
  // always assumes 2 byte samples, 2 channel stereo, add one burst for pilot tone
  return (2 * 2
      * (burstInterval * (averaging * numBursts + (pilot ? 1 : 0)) + delay));
}

void
writeAIFFfile (ofstream &outFile)
{
  // set up AIFF header chunks
  int theSize = getSize ();
  formatAIFFchunk theFormat (theSize);
  commonAIFFchunk theCommon (theSize, sampleRate);
  soundAIFFchunk theSound (theSize);

  // write AIFF header to file
  outFile.write ((char*) &theFormat, sizeof(theFormat));
  outFile.write ((char*) &theCommon, sizeof(theCommon));
  outFile.write ((char*) &theSound, sizeof(theSound));

  // show column headings here
  cout << "numCyc,duration,nomFreq,actFreq" << endl;

  // write initial delay to disk
  int i = 0;
  const short a = 0;
  for (i = 0; i < delay; i++)
    {
      // no need to swap bytes, it's just zero
      outFile.write ((char*) &a, 2);
      outFile.write ((char*) &a, 2);
    }

  if (pilot)
    {
      writePilotTone (outFile);
    }

  // iterate over tone bursts while writing to disk
  for (reset (); good (); next ())
    {
      // check input file before reading
      if (!outFile)
	{
	  cerr << "Failed while writing WAVE data." << endl;
	  return;
	}
      showBurst (cout);
      cout << endl;

      // write AIFF data to file
      writeOutputData (outFile);
    }
}

void
writeWAVEfile (ofstream &outFile)
{
  // set up WAVE header chunks
  int theSize = getSize ();
  riffWAVEchunk theRiff (theSize);
  formatWAVEchunk theFormat (sampleRate);
  dataWAVEchunk theData (theSize);

  // write WAVE header to file
  outFile.write ((char*) &theRiff, sizeof(theRiff));
  outFile.write ((char*) &theFormat, sizeof(theFormat));
  outFile.write ((char*) &theData, sizeof(theData));

  // show column headings here
  cout << "numCyc,duration,nomFreq,actFreq" << endl;

  // write initial delay to disk
  int i = 0;
  const short a = 0;
  for (i = 0; i < delay; i++)
    {
      outFile.write ((char*) &a, 2);
      outFile.write ((char*) &a, 2);
    }

  if (pilot)
    {
      writePilotTone (outFile);
    }

  // iterate over tone bursts while writing to disk
  for (reset (); good (); next ())
    {
      // check input file before reading
      if (!outFile)
	{
	  cerr << "Failed while writing WAVE data." << endl;
	  return;
	}
      showBurst (cout);
      cout << endl;

      // write WAVE data to file
      writeOutputData (outFile);
    }
}

// write a burst to output stream
void
writeOutputData (ofstream &outfile)
{
  int i = 0, j = 0;	// local loop indices
  short a = 0, b = 0;
  double y = 0.0;

  // iterate over averaging, burst interval, number of channels
  for (i = 0; i < averaging; i++)
    for (j = 0; j < burstInterval; j++)
      {
	// calculate burst waveform
	if (j < duration)
	  {
	    // compute the selected waveform
	    switch (theBurstType)
	      {
	      case simpleSine:
		y = sin (factor * j);
		break;
	      case harmonicSine:
		y = sin (factor * j) - sin (2.0 * factor * j) / 2.0;
		break;
	      case simpleCosine:
		y = 1.0 - cos (factor * j);
		break;
	      case harmonicCosine:
		y = cos (2.0 * factor * j) - cos (factor * j);
		break;
	      default:
		cout << "Unexpected burst type." << endl;
		return;
	      }
	    // normalize to +0 dB amplitude, convert to short word
	    a = short (y * leftAmpl * 32767.0);
	    b = short (y * rightAmpl * 32767.0);
	    if (!phase)
	      {
		a = -a;
		b = -b;
	      }
	    if (typeAIFF == theFileType)
	      {
		byteSwap (a);
		byteSwap (b);
	      }
	  }
	else
	  {
	    a = b = 0;
	  }   // write silence between bursts

	// write left channel data, then right
	outfile.write ((char*) &a, 2);
	outfile.write ((char*) &b, 2);
      }
  return;
}

// write single cycle at 441 hz
void
writePilotTone (ofstream &outfile)
{
  int j = 0;    // local loop indices
  short a = 0, b = 0;
  double y = 0.0;
  double tone = 2.0 * M_PI / 100.0;

  // iterate over burst interval, number of channels
  for (j = 0; j < burstInterval; j++)
    {
      // calculate burst waveform
      if (j < 100)
	{
	  // compute the selected waveform
	  switch (theBurstType)
	    {
	    case simpleSine:
	      y = sin (tone * j);
	      break;
	    case harmonicSine:
	      y = sin (tone * j) - sin (2.0 * tone * j) / 2.0;
	      break;
	    case simpleCosine:
	      y = 1.0 - cos (tone * j);
	      break;
	    case harmonicCosine:
	      y = cos (2.0 * tone * j) - cos (tone * j);
	      break;
	    default:
	      cout << "Unexpected burst type." << endl;
	      return;
	    }

	  // normalize to +0 dB amplitude, convert to short word
	  a = short (y * leftAmpl * 32767.0);
	  b = short (y * rightAmpl * 32767.0);
	  if (!phase)
	    {
	      a = -a;
	      b = -b;
	    }
	  if (typeAIFF == theFileType)
	    {
	      byteSwap (a);
	      byteSwap (b);
	    }
	}
      else
	{
	  a = b = 0;
	}   // write silence after pilot tone

      // write left channel data, then right
      outfile.write ((char*) &a, 2);
      outfile.write ((char*) &b, 2);
    }
  return;
}

// true if signs of a and b differ
bool
signDiff (int a, int b)
{
  return (0 > (a ^ b));
}

// scan sound data to find 441 Hz pilot tone
void
findPilot (ifstream &inFile)
{
  // file pointer is at start of sound data on entry
  if (!pilot)
    {
      inFile.seekg (2 * 2 * delay, inFile.cur);
      inFile.seekg (2 * 2 * burstInterval, inFile.cur);
      return;
    }
  long thePos = inFile.tellg ();

  // allocate and clear sample FIFOs and axis crossing arrays
  short a[100], b[100];
  memset (a, 0, sizeof(a));
  memset (b, 0, sizeof(b));
  enum
  {
    I1, Q1, I2, Q2, NChan
  };
  int next[NChan], prev[NChan], minPos[NChan], maxPos[NChan];
  memset (minPos, 0, sizeof(minPos));
  memset (maxPos, 0, sizeof(maxPos));
  memset (next, 0, sizeof(next));
  memset (prev, 0, sizeof(prev));
  int minVal[NChan] =
    { -1, -1, -1, -1 }, maxVal[NChan] =
    { 1, 1, 1, 1 };

  // look at 10 seconds of data
  ofstream outFile ("outPut.csv");	// overwrite temp file each time through
  for (int i = 0; i < (10 * sampleRate); i++)
    {
      // slide fifo along input stream
      int j = 100 - 1;
      while (j--)
	{
	  a[j + 1] = a[j];
	  b[j + 1] = b[j];
	}

      // read data from both channels
      inFile.read ((char*) a, 2);
      inFile.read ((char*) b, 2);

      // swap bytes for AIFF files
      if (typeAIFF == theFileType)
	{
	  byteSwap (a[0]);
	  byteSwap (b[0]);
	}

      // keep previous sums, update next
      prev[I1] = next[I1];
      next[I1] = prev[I1] + a[0] - a[49] - a[50] + a[99];
      prev[Q1] = next[Q1];
      next[Q1] = prev[Q1] + a[0] - a[24] - a[25] + a[74] + a[75] - a[99];
      prev[I2] = next[I2];
      next[I2] = prev[I2] + b[0] - b[49] - b[50] + b[99];
      prev[Q2] = next[Q2];
      next[Q2] = prev[Q2] + b[0] - b[24] - b[25] + b[74] + b[75] - b[99];

      // write debug output
      bool first = true;
      if (outFile)
	for (int j = I1; j < NChan; j++)
	  {
	    if (first)
	      {
		first = false;
	      }
	    else
	      {
		outFile << ',';
	      }
	    outFile << next[j];
	  }
      outFile << endl;

      // look for axis crossings, keeping maximums & minimums
      if (signDiff (prev[I1], next[I1]))
	{
	  if (next[Q1] > maxVal[Q1])
	    {
	      maxVal[Q1] = next[Q1];
	      maxPos[Q1] = i;
	    }
	  if (next[Q1] < minVal[Q1])
	    {
	      minVal[Q1] = next[Q1];
	      minPos[Q1] = i;
	    }
	}
      if (signDiff (prev[Q1], next[Q1]))
	{
	  if (next[I1] > maxVal[I1])
	    {
	      maxVal[I1] = next[I1];
	      maxPos[I1] = i;
	    }
	  if (next[I1] < minVal[I1])
	    {
	      minVal[I1] = next[I1];
	      minPos[I1] = i;
	    }
	}
      if (signDiff (prev[I2], next[I2]))
	{
	  if (next[Q2] > maxVal[Q2])
	    {
	      maxVal[Q2] = next[Q2];
	      maxPos[Q2] = i;
	    }
	  if (next[Q2] < minVal[Q2])
	    {
	      minVal[Q2] = next[Q2];
	      minPos[Q2] = i;
	    }
	}
      if (signDiff (prev[Q2], next[Q2]))
	{
	  if (next[I2] > maxVal[I2])
	    {
	      maxVal[I2] = next[I2];
	      maxPos[I2] = i;
	    }
	  if (next[I2] < minVal[I2])
	    {
	      minVal[I2] = next[I2];
	      minPos[I2] = i;
	    }
	}
    }

  // log results
  if (verbose)
    {
      cout << "Chan,minVal,minPos,maxVal,maxPos" << endl;
      for (int j = I1; j < NChan; j++)
	{
	  cout << j << ',' << minVal[j] << ',' << minPos[j] << ',' << maxVal[j]
	      << ',' << maxPos[j] << endl;
	}
    }

  // find axis crossing with greatest amplitude
  int offset = 0;
  double maxAbs = 0;
  int maxChan = 0;
  for (int j = I1; j < NChan; j++)
    {
      // don't divide by zero
      if (!minVal[j] || !maxVal[j])
	{
	  continue;
	}

      // update maximum and position
      if (abs (1.0 * maxVal[j] / minVal[j]) > maxAbs)
	{
	  maxAbs = abs (1.0 * maxVal[j] / minVal[j]);
	  offset = maxPos[j];
	  maxChan = j;
	}
      else if (abs (1.0 * minVal[j] / maxVal[j]) > maxAbs)
	{
	  maxAbs = abs (1.0 * minVal[j] / maxVal[j]);
	  offset = minPos[j];
	  maxChan = j;
	}
    }

  // compute total delay including pilot tone
  delay = offset;
  if (verbose)
    {
      cout << "Pilot tone found at offset," << offset << ",in chan," << maxChan
	  << endl;
    }

  // restore file pointer to start of sound data
  // NOTE this assumes we didn't hit EOF!
  inFile.seekg (thePos, inFile.beg);
}
