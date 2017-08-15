#ifndef PRESENTATION_H
#define PRESENTATION_H

#include <FS.h>

class CTimeline;

struct SStrandEntry
{
	uint32_t	m_Offset;
	uint16_t	m_ByteCount;
};

typedef SStrandEntry* SIndex;

class CPresentation
{
    protected:
        File		m_File;
        float		m_BPM = 0;
		SIndex		m_Index = nullptr;

    public:
        CPresentation(const String& inPresentationPath);
        virtual ~CPresentation();

        CTimeline* CreateTimeline(short inStrandIndex);

    private:
		uint8_t ReadByte();
		short ReadShort();
		uint16_t ReadUShort();
		long ReadLong();
		uint32_t ReadULong();
        float ReadFloat();
		void ReadBytes(char* ioBytes, long inLength);
		void SkipString();
};

#endif // PRESENTATION_H
