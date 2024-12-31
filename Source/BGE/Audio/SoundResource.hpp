#ifndef _BGE_SOUNDRESOURCE_HPP_
#define _BGE_SOUNDRESOURCE_HPP_

#include "Resources/ResourceLoader.hpp"

namespace BGE
{
	inline constexpr std::array<std::string_view, 2> kSOUND_EXTENSIONS =
	{
		".wav", ".ogg"
	};

	class SoundExtraData; // Forward declare
	BGE_DECLARE_PTR(SoundExtraData);
	class WAVResourceLoader; // Forward declare
	BGE_DECLARE_PTR(WAVResourceLoader);
	class OGGResourceLoader; // Forward declare
	BGE_DECLARE_PTR(OGGResourceLoader);

	/**
	 * @brief Enumeration for identifiying the sound format.
	 */
	enum struct SoundType
	{
		kWAV,
		kOGG
	};

	/**
	 * @brief Data used for setting up sounds.
	 */
	struct SoundData
	{
		std::uint8_t channels;
		std::int32_t sampleRate;
		std::uint8_t bitsPerSample;
		std::vector<char> soundData;
	};

	/**
	 * @brief Extra resource data for sound file contents.
	 */
	class SoundResourceExtraData : public IResourceExtraData
	{
	protected:
		SoundType m_soundType; //!< The type of the sound.
		SoundData m_soundData; //!< The sound data.
	public:
		SoundResourceExtraData(SoundType type, const SoundData &kSoundData);
		SoundResourceExtraData(const SoundResourceExtraData &) = default;
		SoundResourceExtraData(SoundResourceExtraData &&) noexcept = default;
		SoundResourceExtraData &operator=(const SoundResourceExtraData &) = default;
		SoundResourceExtraData &operator=(SoundResourceExtraData &&) noexcept = default;
		virtual ~SoundResourceExtraData(void) override = default;
		// IResourceExtraData's interface:
		virtual std::string VGetExtraData(void) override;
		SoundType GetSoundType(void) const;
		const SoundData &GetSoundData(void) const;
	};

	/**
	 * @brief Resource loader for .wav sound files.
	 *
	 * @see https://en.wikipedia.org/wiki/WAV
	 */
	class WAVResourceLoader final : public IResourceLoader
	{
		std::string m_pattern = "*.wav";
	public:
		WAVResourceLoader(void) = default;
		// IResourceLoader's interface:
		virtual const std::string &VGetPattern(void) const override;
		virtual bool VUseRawFile(void) const override;
		virtual bool VDiscardRawBufferAfterLoad(void) override;
		virtual bool VAddNullZero(void) override;
		virtual std::size_t VGetLoadedResourceSize(char *pRawBuffer, std::size_t rawSize) override;
		virtual bool VLoadResource(char *pRawBuffer, std::size_t size, StrongResourceHandlePtr pResourceHandle) override;
	};

	/**
	 * @brief Resource loader for .ogg sound files.
	 *
	 * @see https://en.wikipedia.org/wiki/Vorbis
	 */
	class OGGResourceLoader final : public IResourceLoader
	{
		std::string m_pattern = "*.ogg";
	public:
		OGGResourceLoader(void) = default;
		// IResourceLoader's interface:
		virtual const std::string &VGetPattern(void) const override;
		virtual bool VUseRawFile(void) const override;
		virtual bool VDiscardRawBufferAfterLoad(void) override;
		virtual bool VAddNullZero(void) override;
		virtual std::size_t VGetLoadedResourceSize(char *pRawBuffer, std::size_t rawSize) override;
		virtual bool VLoadResource(char *pRawBuffer, std::size_t size, StrongResourceHandlePtr pResourceHandle) override;
	};
} // End namespace (BGE)

#endif /* !_BGE_SOUNDRESOURCE_HPP_ */
