#include "Engine/EngineStd.hpp"
#include "Audio/SoundResource.hpp"

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

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

	struct OggVorbis_MemoryFile
	{
		unsigned char *pDataPtr = nullptr; //!< Pointer to the data in memory.
		std::size_t dataSize = 0; //!< Size of the data.
		std::size_t dataRead = 0; //!< Bytes read thus far.
	};

	std::size_t VorbisRead(void *pDataPtr, std::size_t byteSize, std::size_t sizeToRead, void *pDataSource)
	{
		OggVorbis_MemoryFile *pVorbisMemoryFile = reinterpret_cast<OggVorbis_MemoryFile *>(pDataSource);
		if (!pVorbisMemoryFile)
		{
			return 0;
		}

		std::size_t actualSizeToRead{}, spaceToEOF = (pVorbisMemoryFile->dataSize - pVorbisMemoryFile->dataRead);
		if ((sizeToRead * byteSize) < spaceToEOF)
		{
			actualSizeToRead = (sizeToRead * byteSize);
		}
		else
		{
			actualSizeToRead = spaceToEOF;
		}

		if (actualSizeToRead)
		{
			std::memcpy(pDataPtr, (reinterpret_cast<char *>(pVorbisMemoryFile->pDataPtr)
				+ pVorbisMemoryFile->dataRead), actualSizeToRead);
			pVorbisMemoryFile->dataRead += actualSizeToRead;
		}
		return actualSizeToRead;
	}

	int VorbisSeek(void *pDataSource, ogg_int64_t offset, int origin)
	{
		OggVorbis_MemoryFile *pVorbisMemoryFile = reinterpret_cast<OggVorbis_MemoryFile *>(pDataSource);
		if (!pVorbisMemoryFile)
		{
			return -1;
		}

		switch (origin)
		{
		case SEEK_SET:
		{
			ogg_int64_t actualOffset = ((pVorbisMemoryFile->dataSize >= offset)
				? offset : pVorbisMemoryFile->dataSize);
			pVorbisMemoryFile->dataRead = static_cast<std::size_t>(actualOffset);
			break;
		}
		case SEEK_CUR:
		{
			std::size_t spaceToEOF = (pVorbisMemoryFile->dataSize - pVorbisMemoryFile->dataRead);

			ogg_int64_t actualOffset = ((offset < spaceToEOF) ? offset : spaceToEOF);
			pVorbisMemoryFile->dataRead = static_cast<long>(actualOffset);
			break;
		}
		case SEEK_END:
			pVorbisMemoryFile->dataRead = (pVorbisMemoryFile->dataSize + 1);
			break;
		default:
			BGE_ASSERT(false && "Bad parameter for 'origin', requires same as fseek.");
			break;
		}

		return 0;
	}

	int VorbisClose(void *pDataSource)
	{
		// Do nothing - we assume someone else is managing the data source.
		return 0;
	}

	long VorbisTell(void *pDataSource)
	{
		OggVorbis_MemoryFile *pVorbisMemoryFile = reinterpret_cast<OggVorbis_MemoryFile *>(pDataSource);
		if (!pVorbisMemoryFile)
		{
			return -1l;
		}
		return static_cast<long>(pVorbisMemoryFile->dataRead);
	}
} // End namespace

namespace BGE
{
	SoundResourceExtraData::SoundResourceExtraData(SoundType type, const SoundData &kSoundData)
		: m_soundType(type), m_soundData(kSoundData)
	{
	}

	std::string SoundResourceExtraData::VGetExtraData(void)
	{
		return "";
	}

	SoundType SoundResourceExtraData::GetSoundType(void) const
	{
		return m_soundType;
	}

	const SoundData &SoundResourceExtraData::GetSoundData(void) const
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
		SoundData soundData;
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
		auto pExtraData = std::make_shared<SoundResourceExtraData>(SoundType::kWAV, soundData);
		pResourceHandle->SetExtraData(pExtraData);
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
		OggVorbis_File vorbisFile;
		ov_callbacks callbacks;
		callbacks.read_func = VorbisRead;
		callbacks.seek_func = VorbisSeek;
		callbacks.close_func = VorbisClose;
		callbacks.tell_func = VorbisTell;

		OggVorbis_MemoryFile vorbisMemoryFile;
		vorbisMemoryFile.dataRead = 0;
		vorbisMemoryFile.dataSize = rawSize;
		vorbisMemoryFile.pDataPtr = reinterpret_cast<unsigned char *>(pRawBuffer);

		int result = ov_open_callbacks(&vorbisMemoryFile, &vorbisFile, nullptr, 0, callbacks);
		if (result < 0)
		{
			BGE_ERROR("Call to ov_open_callbacks failed");
			return false;
		}

		vorbis_info *pVorbisInfo = ov_info(&vorbisFile, -1);
		if (!pVorbisInfo)
		{
			ov_clear(&vorbisFile);
			BGE_ERROR("Call to ov_info failed");
			return false; // Failed to get audio information.
		}
		std::uint32_t size = 4096 * 16;
		std::uint32_t pos = 0;
		int sec = 0;
		int ret = 1;

		std::uint32_t bytes = static_cast<std::uint32_t>(ov_pcm_total(&vorbisFile, -1));
		bytes *= 2 * pVorbisInfo->channels;

		ov_clear(&vorbisFile);
		return bytes;
	}

	bool OGGResourceLoader::VLoadResource(char *pRawBuffer, std::size_t size, StrongResourceHandlePtr pResourceHandle)
	{
		if (size <= 0)
		{
			return false;
		}

		OggVorbis_File vorbisFile;
		ov_callbacks callbacks;
		callbacks.read_func = VorbisRead;
		callbacks.seek_func = VorbisSeek;
		callbacks.close_func = VorbisClose;
		callbacks.tell_func = VorbisTell;

		OggVorbis_MemoryFile vorbisMemoryFile;
		vorbisMemoryFile.dataRead = 0;
		vorbisMemoryFile.dataSize = size;
		vorbisMemoryFile.pDataPtr = reinterpret_cast<unsigned char *>(pRawBuffer);

		int result = ov_open_callbacks(&vorbisMemoryFile, &vorbisFile, nullptr, 0, callbacks);
		if (result < 0)
		{
			BGE_ERROR("Call to ov_open_callbacks failed");
			return false; // Failed to initialize OGG Vorbis stream.
		}

		// Retrieve audio info
		vorbis_info *pVorbisInfo = ov_info(&vorbisFile, -1);
		if (!pVorbisInfo)
		{
			ov_clear(&vorbisFile);
			BGE_ERROR("Call to ov_info failed");
			return false; // Failed to get audio information.
		}

		SoundData soundData;
		soundData.channels = static_cast<std::uint8_t>(pVorbisInfo->channels);
		soundData.sampleRate = static_cast<std::int32_t>(pVorbisInfo->rate);
		// OGG Vorbis is always 16-bit.
		constexpr std::uint8_t kOGG_BITS_PER_SAMPLE = 16;
		soundData.bitsPerSample = kOGG_BITS_PER_SAMPLE;

		// Determine total PCM size (bytes).
		long long pcmTotalSamples = ov_pcm_total(&vorbisFile, -1);
		if (pcmTotalSamples <= 0)
		{
			ov_clear(&vorbisFile);
			BGE_ERROR("Call to ov_pcm_total failed");
			return false;
		}

		std::size_t totalPCMSize = pcmTotalSamples * soundData.channels * (soundData.bitsPerSample / 8);
		soundData.soundData.reserve(totalPCMSize);

		// Read PCM data into soundData.soundData.
		char tempBuffer[4096];
		int bitstream = 0;
		long bytesRead{};

		while ((bytesRead = ov_read(&vorbisFile, tempBuffer, sizeof(tempBuffer), 0, 2, 1, &bitstream)) > 0)
		{
			soundData.soundData.insert(soundData.soundData.end(), tempBuffer, tempBuffer + bytesRead);
		}

		if (soundData.soundData.size() != totalPCMSize)
		{
			ov_clear(&vorbisFile);
			return false; // Size mismatch, corrupted data.
		}

		ov_clear(&vorbisFile);

		// Wrap SoundData in SoundResourceExtraData and associate with resource handle.
		auto pExtraData = std::make_shared<SoundResourceExtraData>(SoundType::kOGG, soundData);
		pResourceHandle->SetExtraData(pExtraData);

		return true;
	}
} // End namespace (BGE)
