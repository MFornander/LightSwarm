#include "presentation.h"
#include "timeline.h"
#include "strand.h"
#include "debug.h"
#include <FS.h>

namespace LightSwarm {


CPresentation::CPresentation()
{
	// Create an empty presentation that we can add strands to in the future
	m_Index = new SStrandEntry[MAX_PRESENTATION_STRANDS];
	for (uint8_t theStrand = 0; theStrand < m_PresentationStrandCount; theStrand++)
	{
		m_Index[theStrand].m_Offset = 0;
		m_Index[theStrand].m_ByteCount = 0;
		m_Index[theStrand].m_Sequence = nullptr;
	}
}


CPresentation::CPresentation(const String& inPresentationPath)
{
	m_File = SPIFFS.open(inPresentationPath, "r");
	if (m_File)
	{
		// Read the presentation header information: BPM, URI, StrandCount, StrandOffsets
		m_BPM = ReadFloat();
		SkipString(); //Skip the URI
		m_PresentationStrandCount = ReadByte();
		m_Index = new SStrandEntry[m_PresentationStrandCount];
		for (uint8_t theStrand = 0; theStrand < m_PresentationStrandCount; theStrand++)
		{
			m_Index[theStrand].m_Offset = ReadULong();
			m_Index[theStrand].m_ByteCount = ReadUShort();
			m_Index[theStrand].m_Sequence = nullptr;
		}
	}
	else
		ERR("Unable to open the file=%s", inPresentationPath.c_str());
}


CPresentation::~CPresentation()
{
	for (uint8_t theStrand = 0; theStrand < m_PresentationStrandCount; theStrand++)
		delete [] m_Index[theStrand].m_Sequence;

	delete [] m_Index;
	m_Index = nullptr;
}


void CPresentation::CreateSequence(uint8_t inEffect, char*& outSequence, uint32_t& outByteCount)
{
	outByteCount = sizeof(uint32_t)+sizeof(SEvent);
	outSequence = new char[outByteCount];
	SEvent* theEvent = reinterpret_cast<SEvent*>(outSequence + sizeof(uint32_t));

	reinterpret_cast<uint32_t*>(outSequence)[0] = 1; //EventCount
	theEvent->m_EffectID = inEffect;
	theEvent->m_Start=0;
	theEvent->m_ArgCount = 0;
	theEvent->m_Speed = 1.0f;
}


void CPresentation::AddStrand(char* inSequence, uint32_t inByteCount)
{
	if (m_PresentationStrandCount>=MAX_PRESENTATION_STRANDS)
		return;

	m_Index[m_PresentationStrandCount].m_ByteCount = inByteCount;
	m_Index[m_PresentationStrandCount].m_Sequence = inSequence;

	m_PresentationStrandCount++;
}


CTimeline* CPresentation::CreateTimeline(short inStrandIndex)
{
	// Wrap the stand index to the number of strands in this presentation
	short   theWrappedIndex = inStrandIndex % m_PresentationStrandCount;

	// If this is from a file and we don't have the data yet then jump to the
	// location of this strand's timeline and read in the events as a single
	// buffer.  Cache for later
	if (m_Index[theWrappedIndex].m_Sequence == nullptr && m_File)
	{
		m_File.seek(m_Index[theWrappedIndex].m_Offset, SeekSet);
		m_Index[theWrappedIndex].m_Sequence = new char[m_Index[theWrappedIndex].m_ByteCount];
		ReadBytes(m_Index[theWrappedIndex].m_Sequence, m_Index[theWrappedIndex].m_ByteCount);
	}

	return new CTimeline(m_Index[theWrappedIndex].m_Sequence, m_Index[theWrappedIndex].m_ByteCount);
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
}
