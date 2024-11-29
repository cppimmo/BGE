/*******************************************************************************
 * @file   Console.hpp
 * @author Brian Hoffpauir
 * @date   07.09.2024
 * @brief  Debug console.
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
#ifndef _BGE_CONSOLE_HPP_
#define _BGE_CONSOLE_HPP_

namespace BGE
{
	// TODO: Use struct or std::function
	// Commands should have a name and function that accepts arguments and returns an execution status result.
	struct ConsoleCommand
	{
	};
	/**
	 * Engine debug console for running registered commands.
	 */
	class DebugConsole
	{
		bool m_bEnabled; //!< Is the widget currently visible?
		// TODO: Add container to hold registered commands.
	public:
		// TODO: Implement ImGuiable interface to setup immediate mode GUI.
		// TODO: Add member functions for registering commands.
	private:
	};
} // End namespace (BGE)

#endif /* !_BGE_CONSOLE_HPP_ */
