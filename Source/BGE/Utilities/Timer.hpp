/*=============================================================================*
 * Timer.hpp - Timer utilities.
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
#ifndef _BGE_TIMER_HPP_
#define _BGE_TIMER_HPP_

namespace BGE
{
	/**
	 * Simple timer class.
	 */
	class Timer
	{
	public:
		using UnderlyingType = float;
		using Milliseconds = UnderlyingType;
		using Seconds = UnderlyingType;
		using Minutes = UnderlyingType;
		using Hours = UnderlyingType;
	private:
		using Duration = std::chrono::duration<UnderlyingType, std::milli>;
	
		bool m_isPaused; // Pause state
		std::chrono::high_resolution_clock::time_point m_start;
		std::chrono::high_resolution_clock::time_point m_end;
	public:
		explicit Timer(bool isPaused = true);
		// TODO: Implement operator - and + to get difference between two timers.
		Milliseconds GetElapsedMillis(void) const; // Elapsed milliseconds
		Seconds GetElapsedSecs(void) const; // Seconds
		Minutes GetElapsedMins(void) const; // Minutes
		Hours GetElapsedHrs(void) const; // Hours
		void Reset(void); // Reset timer
		void Start(void) noexcept; // Unpause
		void Stop(void) noexcept; // Pause
		bool IsPaused(void) const noexcept; // Check for pause
		// Conversion helpers:
		static constexpr Seconds MillisToSecs(Milliseconds millis) noexcept;
		static constexpr Milliseconds SecsToMillis(Seconds secs) noexcept;
		static constexpr Minutes SecsToMins(Seconds secs) noexcept;
		static constexpr Seconds MinsToSecs(Minutes mins) noexcept;
		static constexpr Hours MinsToHrs(Minutes mins) noexcept;
		static constexpr Minutes HrsToMins(Hours hrs) noexcept;
	private:
		Duration GetRawDuration(void) const noexcept;
	};

	inline constexpr Timer::Seconds Timer::MillisToSecs(Milliseconds millis) noexcept
	{
		constexpr UnderlyingType kCONVERSION = 1000.0f;
		return millis / kCONVERSION;
	}

	inline constexpr Timer::Milliseconds Timer::SecsToMillis(Seconds secs) noexcept
	{
		constexpr UnderlyingType kCONVERSION = 1000.0f;
		return secs * kCONVERSION;
	}

	inline constexpr Timer::Minutes Timer::SecsToMins(Seconds secs) noexcept
	{
		constexpr UnderlyingType kCONVERSION = 0.016667f;
		return secs * kCONVERSION;
	}

	inline constexpr Timer::Seconds Timer::MinsToSecs(Minutes mins) noexcept
	{
		constexpr UnderlyingType kCONVERSION = 0.016667f;
		return mins / kCONVERSION;
	}

	inline constexpr Timer::Hours Timer::MinsToHrs(Minutes mins) noexcept
	{
		constexpr UnderlyingType kCONVERSION = 60.0f;
		return mins / kCONVERSION;
	}

	inline constexpr Timer::Minutes Timer::HrsToMins(Hours hrs) noexcept
	{
		constexpr UnderlyingType kCONVERSION = 60.0f;
		return hrs * kCONVERSION;
	}
} // End namespace (BGE)

#endif /* !_BGE_TIMER_HPP_ */