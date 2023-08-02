#ifndef _BGE_TYPES_HPP_
#define _BGE_TYPES_HPP_

#define BGE_DECLARE_PTR(TYPE) \
	using Unique ## TYPE ## Ptr = std::unique_ptr<TYPE>; \
	using Strong ## TYPE ## Ptr = std::shared_ptr<TYPE>; \
	using Weak ## TYPE ## Ptr = std::weak_ptr<TYPE>; \

#define BGE_BITOP_ENUM(ENUM) \
inline constexpr auto operator|(const ENUM &lhs, const ENUM &rhs) \
{ \
	return static_cast<ENUM>(BGE::Utils::ToUnderlying(lhs) | BGE::Utils::ToUnderlying(rhs)); \
} \
inline constexpr auto operator&(const ENUM &lhs, const ENUM &rhs) \
{ \
	return static_cast<ENUM>(BGE::Utils::ToUnderlying(lhs) & BGE::Utils::ToUnderlying(rhs)); \
} \
inline constexpr auto operator^(const ENUM &lhs, const ENUM &rhs) \
{ \
	return static_cast<ENUM>(BGE::Utils::ToUnderlying(lhs) ^ BGE::Utils::ToUnderlying(rhs)); \
} \

namespace BGE
{
	class EngineApp;
	BGE_DECLARE_PTR(EngineApp);

	class BaseGameLogic;
	BGE_DECLARE_PTR(BaseGameLogic);

	// Container for storing localized strings (ID, text)
	using TextStringMap = std::map<std::wstring, std::wstring>;

	// Foward declare Actor & ActorComponent:
	class Actor;
	class ActorComponent;
	// Actor and actor component ID number types:
	using ActorID = std::uint32_t;
	using ActorComponentID = std::uint32_t;
	using ActorType = std::string;
	// Actor and actor component invalid ID number constants:
	inline constexpr ActorID kINVALID_ACTOR_ID = 0;
	inline constexpr ActorComponentID kINVALID_ACTOR_COMPONENT_ID = 0;
	// Actor and actor component pointer types:
	BGE_DECLARE_PTR(Actor);
	BGE_DECLARE_PTR(ActorComponent);
	
	// GameView types & constants:
	using GameViewID = std::uint32_t;
	inline constexpr GameViewID kINVALID_GAMEVIEW_ID = 0xFFFFFFFF;
} // End namespace (BGE)

#endif /* !_BGE_TYPES_HPP_ */