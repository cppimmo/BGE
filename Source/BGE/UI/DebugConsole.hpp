/*******************************************************************************
 * @file   DebugConsole.hpp
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
#ifndef _BGE_DEBUGCONSOLE_HPP_
#define _BGE_DEBUGCONSOLE_HPP_

namespace BGE
{
	class DebugConsole; // Forward declare
	BGE_DECLARE_PTR(DebugConsole);

	/**
	 * @brief Engine debug console for running registered commands.
	 */
	class DebugConsole final : public INonCopyable, public INonMovable, public IImGuiable
	{
	public:
		//! .
		using CommandCallback = std::function<std::string(const std::vector<std::string> &)>;
		//! .
		struct Command
		{
			std::string name; //!< Command name
			CommandCallback callback; //!< Command callback
		};
	private:
		static constexpr std::string_view kPROMPT = ">> ";

		bool m_bEnabled = false; //!< Is the widget currently visible (off by default)?
		bool m_bAutoCompleteEnabled = true; //!< Auto completion for registered commands
		bool m_bAutoScroll = true; //!< Auto scroll the output log
		bool m_bAutoReturn = true; //!< Auto prepend "return " to entered commands.
		std::vector<Command> m_commands; //!< Registered commands
		std::vector<std::string> m_outputLog; //!< Console output log
		std::vector<std::string> m_commandHistory; //!< History of commands
		std::int64_t m_historyIndex = -1; //!< Current position in history
		std::int64_t m_selectedSuggestion = 0;
		std::vector<std::string> m_autoCompleteSuggestions;
		std::string m_inputBuffer; // Input buffer
	public:
		DebugConsole(void);

		void RegisterCommand(std::string_view name, CommandCallback callback);
		void ExecuteCommand(std::string_view input);
		void AddToOutputLog(std::string_view message);
		void FlushOutputLog(void);
		// IImGuiable's interface:
		virtual void VImGuiRoutine(void) override;
		virtual constexpr std::string_view VImGuiWidgetName(void) const noexcept override
		{
			return "Debug Console";
		}
		// Accessors:
		bool IsEnabled(void) const noexcept;
		bool IsAutoCompleteEnabled(void) const noexcept;
		void SetEnabled(bool bEnabled) noexcept;
		void SetAutoCompleteEnabled(bool bEnabled) noexcept;
	private:
		void RenderInputBox(void);
		void RenderOutputLog(void);
		void RenderAutoComplete(void);
		void SubmitCommand(std::string_view input);
		void ExecuteLua(std::string_view code);
		std::vector<std::string> PopulateSuggestions(std::string_view input);
	};
} // End namespace (BGE)

#endif /* !_BGE_DEBUGCONSOLE_HPP_ */
