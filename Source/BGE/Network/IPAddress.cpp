#include "Engine/EngineStd.hpp"
#include "Network/IPAddress.hpp"

namespace BGE
{
#if 0
	IPAddress::IPAddress(std::string_view address, AddressType type)
		: m_type(type)
	{
		using enum AddressType;
		switch (type)
		{
		case kIPv4:
		{
			in_addr addr;
			if (inet_pton(AF_INET, address.data(), &addr) != 1)
			{
				BGE_ASSERT(false);
			}
			m_address = addr;
			break;
		}
		case kIPv6:
			in6_addr addr;
			if (inet_pton(AF_INET6, address.data(), &addr) != 1)
			{
				BGE_ASSERT(false);
			}
			m_address = addr;
			break;
		default:
			BGE_ASSERT(false);
			break;
		}
	}

	//auto IPAddress::operator<=>(const IPAddress &kOther) const
	//{
	//	// Compare type first
	//	if (auto cmp = m_type <=> kOther.m_type; cmp != 0)
	//	{
	//		return cmp;
	//	}
	//	// If types are equal, compare the address
	//	using enum AddressType;
	//	if (m_type == kIPv4)
	//	{
	//		const auto *pAddr1 = std::get_if<in_addr>(&m_address);
	//		const auto *pAddr2 = std::get_if<in_addr>(&kOther.m_address);
	//		return pAddr1->s_addr <=> pAddr2->s_addr;
	//	}
	//	else
	//	{
	//		const auto *pAddr1 = std::get_if<in6_addr>(&m_address);
	//		const auto *pAddr2 = std::get_if<in6_addr>(&kOther.m_address);
	//		return *pAddr1 <=> *pAddr2;
	//	}
	//}

	std::string IPAddress::VToString(void) const
	{
		using enum AddressType;
		std::array<char, INET6_ADDRSTRLEN> buffer{};
		if (m_type == kIPv4)
		{
			const auto *pAddress = std::get_if<in_addr>(&m_address);
			inet_ntop(AF_INET, pAddress, buffer.data(), buffer.size());
		}
		else
		{
			const auto *pAddress = std::get_if<in6_addr>(&m_address);
			inet_ntop(AF_INET6, pAddress, buffer.data(), buffer.size());
		}
		return std::string(buffer.begin(), buffer.end());
	}

	sockaddr_storage IPAddress::ToSockAddr(std::uint16_t port) const
	{
		using enum AddressType;

		sockaddr_storage storage{};
		if (m_type == kIPv4)
		{
			sockaddr_in *pAddress = reinterpret_cast<sockaddr_in*>(&storage);
			pAddress->sin_family = AF_INET;
			pAddress->sin_port = htons(port);
			//pAddress->sin_addr = std::get_if<in_addr>(&pAddress);
		}
		else
		{
			sockaddr_in6* pAddress6 = reinterpret_cast<sockaddr_in6*>(&storage);
			pAddress6->sin6_family = AF_INET6;
			pAddress6->sin6_port = htons(port);
			//pAddress6->sin6_addr = std::get_if<in6_addr>(&pAddress6);
		}
		return storage;
	}

	IPAddress::AddressType IPAddress::GetType(void) const
	{
		return m_type;
	}

	std::optional<std::string>  IPAddress::GetSubnetMask(int prefixLength) const
	{
		using enum AddressType;
		if (m_type == kIPv4)
		{
			in_addr mask{};
			mask.s_addr = htonl(~((1 << (32 - prefixLength)) - 1));
			std::array<char, INET_ADDRSTRLEN> buffer{};
			inet_ntop(AF_INET, &mask, buffer.data(), buffer.size());
			return std::string(buffer.begin(), buffer.end());
		}
		return std::nullopt;
	}

	bool IPAddress::IsLoopback(void) const
	{
		using enum AddressType;
		if (m_type == kIPv4)
		{
			const auto *pAddress = std::get_if<in_addr>(&m_address);
			return (ntohl(pAddress->s_addr) >> 24) == 127;
		}
		else
		{
			const auto *pAddress = std::get_if<in6_addr>(&m_address);
			return IN6_IS_ADDR_LOOPBACK(pAddress);
		}
	}

	bool IPAddress::IsMulticast(void) const
	{
		using enum AddressType;
		if (m_type == kIPv4)
		{
			const auto *pAddress = std::get_if<in_addr>(&m_address);
			std::uint32_t firstOctet = ntohl(pAddress->s_addr) >> 24;
			return (firstOctet >= 224 && firstOctet <= 239);
		}
		else
		{
			const auto *pAddress = std::get_if<in6_addr>(&m_address);
			return IN6_IS_ADDR_MULTICAST(pAddress);
		}
	}

	IPAddress IPAddress::Localhost(AddressType type)
	{
		using enum AddressType;
		if (type == kIPv4)
		{
			return IPAddress("127.0.0.1", kIPv4);
		}
		else
		{
			return IPAddress("::1", kIPv6);
		}
	}

	bool IPAddress::IsValidIPv4(std::string_view address)
	{
		in_addr addr;
		return inet_pton(AF_INET, address.data(), &addr) == 1;
	}

	bool IPAddress::IsValidIPv6(std::string_view address)
	{
		in6_addr addr;
		return inet_pton(AF_INET6, address.data(), &addr) == 1;
	}
#endif
} // End namespace (BGE)
