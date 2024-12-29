#ifndef _BGE_TEXTURES_HPP_
#define _BGE_TEXTURES_HPP_

namespace BGE
{
	class ITexture
	{
	public:
		virtual ~ITexture(void) = default;
		// Interface:
		virtual bool VCreate(void) = 0;
	};
} // End namespace (BGE)

#endif /* !_BGE_TEXTURES_HPP_ */
