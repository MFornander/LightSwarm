#ifndef PRESENTATION_H
#define PRESENTATION_H

#include <FS.h>

namespace LightSwarm {


class CTimeline;

struct SStrandEntry
{
	uint32_t	m_Offset;
	uint16_t	m_ByteCount;
	char*		m_Sequence;
};

typedef SStrandEntry* SIndex;

#define MAX_PRESENTATION_STRANDS 24


class CPresentation final
{
protected:
	File		m_File;
	float		m_BPM = 0;
	short		m_PresentationStrandCount = 0;
	SIndex		m_Index = nullptr;

public:
	CPresentation();
	CPresentation(const String& inPresentationPath);
	~CPresentation();

	static void	CreateSequence(uint8_t inEffect, char*& outSequence, uint32_t& outByteCount);
	void 		AddStrand(char* inSequence, uint32_t inByteCount);

	CTimeline*	CreateTimeline(short inStrandIndex);

private:
	uint8_t		ReadByte();
	short		ReadShort();
	uint16_t	ReadUShort();
	long		ReadLong();
	uint32_t	ReadULong();
	float		ReadFloat();
	void		ReadBytes(char* ioBytes, long inLength);
	void		SkipString();
};
}

#endif // PRESENTATION_H
