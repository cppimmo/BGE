/*=============================================================================*
 * Timer.cpp - Timer utilities.
 *
 * Copyright (c) 2023, Brian Hoffpauir All rights reserved.
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
 *============================================================================*/
#include "Engine/EngineStd.hpp"
#include "Timer.hpp"

namespace ch = std::chrono;

BGE::Timer::Timer(bool isPaused)
	: m_isPaused(isPaused)
{
	Reset(); // Reset high res clocks
}

BGE::Timer::Milliseconds BGE::Timer::GetElapsedMillis(void) const
{
	return GetRawDuration().count();
}

BGE::Timer::Seconds BGE::Timer::GetElapsedSecs(void) const
{
	return MillisToSecs(GetElapsedMillis());
}

BGE::Timer::Minutes BGE::Timer::GetElapsedMins(void) const
{
	return SecsToMins(GetElapsedSecs());
}

BGE::Timer::Hours BGE::Timer::GetElapsedHrs(void) const
{
	return MinsToHrs(GetElapsedMins());
}

void BGE::Timer::Reset(void)
{
	m_start = ch::high_resolution_clock::now();
	m_end   = ch::high_resolution_clock::now();
}

void BGE::Timer::Start(void) noexcept
{
	if (!m_isPaused) // Do nothing if already unpaused
		return;
	m_isPaused = false;
	m_start = ch::high_resolution_clock::now();
}

void BGE::Timer::Stop(void) noexcept
{
	if (m_isPaused) // Do nothing if already paused
		return;
	m_isPaused = true;
	m_end = ch::high_resolution_clock::now();
}

bool BGE::Timer::IsPaused(void) const noexcept
{
	return m_isPaused;
}

BGE::Timer::Duration BGE::Timer::GetRawDuration(void) const noexcept
{
	return ((!m_isPaused) ? Duration(ch::high_resolution_clock::now() - m_start)
						  : Duration(m_end - m_start));
}