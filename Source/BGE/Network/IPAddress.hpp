#ifndef _BGE_IPADDRESS_HPP_
#define _BGE_IPADDRESS_HPP_

#include <string>
#include <string_view>
#include <variant>
#include <compare>

#include <netinet/in.h>
#include <arpa/inet.h>

namespace BGE
{
	class IPAddress; // Forward declare
	BGE_DECLARE_PTR(IPAddress);

	/**
	 * @brief .
	 */
	class IPAddress : public IStringable
	{
	public:
		enum AddressType
		{
			kIPv4,
			kIPv6
		};
	private:
		AddressType m_type;
		std::variant<in_addr, in6_addr> m_address;
	public:
		IPAddress(std::string_view address, AddressType type);

		//auto operator<=>(const IPAddress &kOther) const;
		// IStringable's interface:
		std::string VToString(void) const override;

		sockaddr_storage ToSockAddr(std::uint16_t port) const;
		// Accessors:
		AddressType GetType(void) const;
		std::optional<std::string> GetSubnetMask(int prefixLength) const;
		bool IsLoopback(void) const;
		bool IsMulticast(void) const;
	public:
		static IPAddress Localhost(AddressType type);
		static bool IsValidIPv4(std::string_view address);
		static bool IsValidIPv6(std::string_view address);
	};
} // End namespace (BGE)

#endif /* !_BGE_IPADDRESS_HPP_ */
