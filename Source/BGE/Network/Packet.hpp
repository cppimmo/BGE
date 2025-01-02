#ifndef _BGE_PACKET_HPP_
#define _BGE_PACKET_HPP_

#include <cstddef>
#include <string_view>

namespace BGE
{
	class IPacket; // Forward declare
	BGE_DECLARE_PTR(IPacket);

	class IPacket
	{
	public:
		virtual ~IPacket(void) = default;
		// Interface:
		virtual constexpr std::string_view VGetType(void) const noexcept = 0;
		virtual const char *const VGetData(void) const = 0;
		virtual std::size_t VGetSize(void) const noexcept = 0;
	};
} // End namespace (BGE)

#endif /* !_BGE_PACKET_HPP_ */
