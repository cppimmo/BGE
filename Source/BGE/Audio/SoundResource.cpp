#include "Engine/EngineStd.hpp"
#include "Audio/SoundResource.hpp"

namespace
{
	std::int32_t ConvertToInt(char *pBuffer, std::size_t length)
	{
		std::int32_t result = 0;
		if (std::endian::native == std::endian::little)
		{
			std::memcpy(&result, pBuffer, length);
		}
		else
		{
			for (std::size_t i = 0; i < length; ++i)
				reinterpret_cast<char *>(&result)[3 - i] = pBuffer[i];
		}
		return result;
	}
} // End namespace

namespace BGE
{
	SoundExtraData::SoundExtraData(const SoundData &kSoundData)
		: m_soundData(kSoundData)
	{
	}

	std::string SoundExtraData::VGetExtraData(void)
	{
		return "";
	}

	const SoundExtraData::SoundData &SoundExtraData::GetSoundData(void) const
	{
		return m_soundData;
	}

	const std::string &WAVResourceLoader::VGetPattern(void) const
	{
		return m_pattern;
	}

	bool WAVResourceLoader::VUseRawFile(void) const
	{
		return false;
	}

	bool WAVResourceLoader::VDiscardRawBufferAfterLoad(void)
	{
		return true;
	}

	bool WAVResourceLoader::VAddNullZero(void)
	{
		return true;
	}

	std::size_t WAVResourceLoader::VGetLoadedResourceSize(char *pRawBuffer, std::size_t rawSize)
	{
		return rawSize;
	}

	bool WAVResourceLoader::VLoadResource(char *pRawBuffer, std::size_t size, StrongResourceHandlePtr pResourceHandle)
	{
		if (!pRawBuffer || size <= 0)
		{
			return false;
		}

		// Create the SoundData structure
		SoundExtraData::SoundData soundData;
		char *pCursor = pRawBuffer;

		// Ensure the buffer starts with "RIFF"
		if (std::strncmp(pCursor, "RIFF", 4) != 0)
		{
			BGE_ERROR("Buffer does not start with RIFF header!");
			return false;
		}
		pCursor += 4;

		// Skip file size (next 4 bytes)
		pCursor += 4;

		// Ensure the file format is "WAVE"
		if (std::strncmp(pCursor, "WAVE", 4) != 0)
		{
			BGE_ERROR("Buffer does not contain WAVE tag!");
			return false;
		}
		pCursor += 4;

		// Locate "fmt " chunk
		if (std::strncmp(pCursor, "fmt ", 4) != 0)
		{
			BGE_ERROR("Missing 'fmt ' chunk!");
			return false;
		}
		pCursor += 4;

		// Ensure fmt chunk size is 16 (PCM format)
		std::int32_t fmtChunkSize = ConvertToInt(pCursor, 4);
		pCursor += 4;
		if (fmtChunkSize != 16)
		{
			BGE_ERROR("Unexpected fmt chunk size (expected 16)!");
			return false;
		}

		// Read audio format (2 bytes, should be 1 for PCM)
		std::int16_t audioFormat = ConvertToInt(pCursor, 2);
		pCursor += 2;
		if (audioFormat != 1)
		{
			BGE_ERROR("Unsupported audio format (expected PCM)!");
			return false;
		}

		// Read the number of channels (2 bytes)
		soundData.channels = static_cast<std::uint8_t>(ConvertToInt(pCursor, 2));
		pCursor += 2;

		// Read sample rate (4 bytes)
		soundData.sampleRate = ConvertToInt(pCursor, 4);
		pCursor += 4;

		// Skip byte rate & block align (6 bytes total)
		pCursor += 6;

		// Read bits per sample (2 bytes)
		soundData.bitsPerSample = static_cast<std::uint8_t>(ConvertToInt(pCursor, 2));
		pCursor += 2;

		// Locate the "data" chunk
		while (std::strncmp(pCursor, "data", 4) != 0)
		{
			pCursor += 4;
			std::int32_t chunkSize = ConvertToInt(pCursor, 4);
			pCursor += 4 + chunkSize; // Skip over this chunk's data
			if (pCursor >= pRawBuffer + size)
			{
				BGE_ERROR("Could not find 'data' chunk!");
				return false;
			}
		}
		pCursor += 4;

		// Read data size (4 bytes)
		std::int32_t dataSize = ConvertToInt(pCursor, 4);
		pCursor += 4;

		// Ensure we have enough buffer left for the data
		if (pCursor + dataSize > pRawBuffer + size)
		{
			BGE_ERROR("Data chunk size exceeds buffer size!");
			return false;
		}

		// Read the sound data
		soundData.soundData = std::vector<char>(pCursor, pCursor + dataSize);

		// Store SoundData in the resource handle
		auto pSoundExtraData = std::make_shared<SoundExtraData>(soundData);
		pResourceHandle->SetExtraData(pSoundExtraData);
		return true;
	}

	const std::string &OGGResourceLoader::VGetPattern(void) const
	{
		return m_pattern;
	}

	bool OGGResourceLoader::VUseRawFile(void) const
	{
		return false;
	}

	bool OGGResourceLoader::VDiscardRawBufferAfterLoad(void)
	{
		return true;
	}

	bool OGGResourceLoader::VAddNullZero(void)
	{
		return true;
	}

	std::size_t OGGResourceLoader::VGetLoadedResourceSize(char *pRawBuffer, std::size_t rawSize)
	{
		return rawSize;
	}

	bool OGGResourceLoader::VLoadResource(char *pRawBuffer, std::size_t size, StrongResourceHandlePtr pResourceHandle)
	{
		if (size <= 0)
			return false;

		return true;
	}
} // End namespace (BGE)
