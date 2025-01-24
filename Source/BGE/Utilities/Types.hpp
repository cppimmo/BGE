/*******************************************************************************
 * @file   Types.hpp
 * @author Brian Hoffpauir
 * @date   12.08.2024
 * @brief  .
 *
 * Copyright (c) 2024, Brian Hoffpauir All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/
#ifndef _BGE_TYPES_HPP_
#define _BGE_TYPES_HPP_

#define BGE_DECLARE_PTR(TYPE) \
	using Unique ## TYPE ## Ptr = std::unique_ptr<TYPE>; \
	using Strong ## TYPE ## Ptr = std::shared_ptr<TYPE>; \
	using Weak ## TYPE ## Ptr = std::weak_ptr<TYPE>;

#define BGE_DECLARE_MULTITEMPLATED_PTR(CLASS_NAME) \
    template <typename... Types> \
    using Unique ## CLASS_NAME ## Ptr = std::unique_ptr<CLASS_NAME<Types...>>; \
    template <typename... Types> \
    using Strong ## CLASS_NAME ## Ptr = std::shared_ptr<CLASS_NAME<Types...>>; \
    template <typename... Types> \
    using Weak ## CLASS_NAME ## Ptr = std::weak_ptr<CLASS_NAME<Types...>>;

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
}

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

	//! Alias for representing a point in time.
	using Timestamp = std::chrono::time_point<std::chrono::system_clock>;
} // End namespace (BGE)

#endif /* !_BGE_TYPES_HPP_ */
