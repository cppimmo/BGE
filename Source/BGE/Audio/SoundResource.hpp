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
	 * @brief Extra resource data for plaintext file contents.
	 */
	class SoundExtraData : public IResourceExtraData
	{
	public:
		struct SoundData
		{
			std::uint8_t channels;
			std::int32_t sampleRate;
			std::uint8_t bitsPerSample;
			std::vector<char> soundData;
		};
	protected:
		SoundData m_soundData;
	public:
		explicit SoundExtraData(const SoundData &kSoundData);
		SoundExtraData(const SoundExtraData &) = default;
		SoundExtraData(SoundExtraData &&) noexcept = default;
		SoundExtraData &operator=(const SoundExtraData &) = default;
		SoundExtraData &operator=(SoundExtraData &&) noexcept = default;
		virtual ~SoundExtraData(void) = default;
		// IResourceExtraData's interface:
		virtual std::string VGetExtraData(void) override;
		const SoundData &GetSoundData(void) const;
	};

	/**
	 * @brief Resource loader for .glsl shader source code files.
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
	 * @brief Resource loader for .glsl shader source code files.
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
