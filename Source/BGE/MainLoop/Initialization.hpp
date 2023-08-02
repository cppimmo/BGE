/*******************************************************************************
 * @file   Initialization.hpp
 * @author Brian Hoffpauir
 * @date   01.08.2023
 * @brief  Initialization routines.
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
 ******************************************************************************/
#ifndef _BGE_INITIALIZATION_HPP_
#define _BGE_INITIALIZATION_HPP_

#undef GetCurrentDirectory

namespace BGE
{
	//! Retrieve cmdline args as a string container.
	[[nodiscard]] std::span<std::string_view> GetArguments(int numArgs, char *pArgs[]);
	//! Get runtime platform string.
	std::string_view GetPlatform(void);
	//! Ensure available disk space in MiB.
	bool IsDiskSpaceAvailable(int neededDiskSpaceMiB);
	//! Retrieve the # of logical CPU cores.
	int  ReadLogicalCPUCores(void);
	//! Retrieve the CPU speed in MHz.
	std::uint32_t ReadCPUSpeed(void);
	//! Ensure available system memory in MiB.
	bool IsMemoryAvailable(int neededRAM_MiB); // MiB
	//! Retrieve information about system power state.
	SDL_PowerState CheckPowerState(int &outSecondsLeft, int &outPercentage);
    //! Check if a debugger is present.
	bool IsDebuggerPresent(void);
	//! Ensure that only one instance of the game with GAMETITLE exists.
	bool IsOnlyInstance(std::string_view gameTitle);
	//! Retrieve the location of the game executable.
	std::optional<std::string> GetCurrentDirectory(void);
	//! Retrieve the location of the save game directory.
	std::optional<std::string> GetSaveGameDirectory(std::string_view orgName, std::string_view appName);
	//! Hide the application console (on Windows).
	void HideConsole(void);
} // End namespace (BGE)

#endif /* !_BGE_INITIALIZATION_HPP_ */
