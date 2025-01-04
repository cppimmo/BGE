/*******************************************************************************
 * @file   GameView.hpp
 * @author Brian Hoffpauir
 * @date   12.09.2024
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
#ifndef _BGE_GAMEVIEW_HPP_
#define _BGE_GAMEVIEW_HPP_

#include <cstddef>

namespace BGE
{
	class IGameView; // Forware declare
	BGE_DECLARE_PTR(IGameView);

	using GameViewList = std::list<StrongIGameViewPtr>;
	using GameViewFilter = std::function<bool(const StrongIGameViewPtr &)>;

	enum struct GameViewType
	{
		Human,
		Remote,
		AI,
		Recorder,
		Other
	};

	using GameViewID = std::uint32_t;
	/**
	 * @brief .
	 */
	class IGameView
	{
	public:
		virtual ~IGameView(void) = default;

		virtual bool VInit(void) = 0;
		virtual void VOnRestore(void) = 0;
		virtual void VOnRender(float deltaTime, float elapsedTime) = 0;
		virtual void VOnLostDevice(void) = 0;
		virtual GameViewType VGetType(void) const = 0;
		virtual GameViewID VGetID(void) const = 0;
		virtual void VOnAttach(GameViewID viewID, ActorID actorID) = 0;
		virtual bool VOnHandleEvent(const SDL_Event &event) = 0;
		virtual void VOnUpdate(float deltaTime) = 0;
	public:
		static GameViewFilter CreateFilter(GameViewType type);
	};
} // End namespace (BGE)

#endif /* !_BGE_GAMEVIEW_HPP_ */
