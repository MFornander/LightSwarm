#include "presentation.h"
#include "strand.h"
#include "debug.h"
#include <FS.h>

CPresentation::CPresentation(const String& inPresentationPath)
{
    m_File = SPIFFS.open(inPresentationPath, "r");
    if (m_File)
    {
        // Read the presentation header information: BPM, URI, StrandCount, StrandOffsets
        m_BPM = ReadFloat();
		SkipString(); //Skip the URI
		uint8_t theStrandCount = ReadByte();
		m_Index = new SStrandEntry[theStrandCount];
		for (uint8_t theStrand = 0; theStrand < theStrandCount; theStrand++)
		{
			m_Index[theStrand].m_Offset = ReadULong();
			m_Index[theStrand].m_ByteCount = ReadUShort();
		}
    }
    else
        ERR("Unable to open the file");
}

CPresentation::~CPresentation()
{
    delete [] m_Index;
    m_Index = nullptr;
}


CTimeline* CPresentation::CreateTimeline(short inStrandIndex)
{
	// Jump to the location of this strand's timeline and read in the events as a single buffer
    m_File.seek(m_Index[inStrandIndex].m_Offset, SeekSet);
	char* theSequence = new char[m_Index[inStrandIndex].m_ByteCount];
	ReadBytes(theSequence, m_Index[inStrandIndex].m_ByteCount);

	return new CTimeline(theSequence, m_Index[inStrandIndex].m_ByteCount);
}

// =============================================================================
// Private stream reading functions
// =============================================================================

uint8_t CPresentation::ReadByte()
{
	uint8_t   theByte;

	m_File.read(&theByte, 1);
	Serial.println(theByte);

	return theByte;
}

short CPresentation::ReadShort()
{
	short   theShort;

	m_File.read((uint8_t*)&theShort, 2);
	Serial.println(theShort);

	return theShort;
}

uint16_t CPresentation::ReadUShort()
{
	uint16_t   theUShort;

	m_File.read((uint8_t*)&theUShort, 2);
	Serial.println(theUShort);

	return theUShort;
}

long CPresentation::ReadLong()
{
	long   theLong;

	m_File.read((uint8_t*)&theLong, 4);
	Serial.println(theLong);

	return theLong;
}

uint32_t CPresentation::ReadULong()
{
	uint32_t   theULong;

	m_File.read((uint8_t*)&theULong, 4);
	Serial.println(theULong);

	return theULong;
}

float CPresentation::ReadFloat()
{
    float   theFloat;

	m_File.read((uint8_t*)&theFloat, 4);
	Serial.println(theFloat);

    return theFloat;
}

void CPresentation::ReadBytes(char* ioBytes, long inLength)
{
	m_File.read((uint8_t*)ioBytes, inLength);
}

void CPresentation::SkipString()
{
	char	theLength;
	char	theString[255];

	m_File.read((uint8_t*)&theLength, 1);
	m_File.read((uint8_t*)theString, theLength);
	theString[theLength] = '\0';
	Serial.println(theString);
}
