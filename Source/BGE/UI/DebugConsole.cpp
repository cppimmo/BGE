/*******************************************************************************
 * @file   DebugConsole.cpp
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
#include "Engine/EngineStd.hpp"
#include "UI/DebugConsole.hpp"

#include "Engine/EngineApp.hpp"

namespace BGE
{
	DebugConsole::DebugConsole(void)
	{
		std::ostringstream startupMessage;
		startupMessage << kENGINE_NAME << ' ' << kVERSION.VToString() << " Console\n";
		AddToOutputLog(startupMessage.str());

		// Register default commands
		RegisterCommand("help", [](const std::vector<std::string>& args)
		{
			std::ostringstream oss;
			oss << "Available builtin commands:\n";
			oss << "  help : Display this help message.\n";
			oss << "  clear : Clear the console output.\n";
			oss << "Any other inputs are interpreted by the global Lua state.\n";
			return oss.str();
		});

		RegisterCommand("clear", [&](const std::vector<std::string>&)
		{
			FlushOutputLog();
			return "Console cleared.";
		});
	}

	void DebugConsole::RegisterCommand(std::string_view name, CommandCallback callback)
	{
		m_commands.push_back({std::string(name), callback});
	}

	void DebugConsole::ExecuteCommand(std::string_view input)
	{
		std::string inputStr(input);
		AddToOutputLog(">> " + inputStr);

		// Parse command and arguments
		std::istringstream iss(inputStr);
		std::vector<std::string> tokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};

		if (tokens.empty())
			return;

		std::string command = tokens[0];
		tokens.erase(tokens.begin());

		// Find & execute the command
		auto it = std::find_if(m_commands.begin(), m_commands.end(), [&](const Command &cmd) { return cmd.name == command; });

		if (it != m_commands.end())
		{
			AddToOutputLog(it->callback(tokens));
		}
		else
		{
			ExecuteLua(input);
		}

		m_commandHistory.push_back(inputStr); // Store in history
		m_historyIndex = -1; // Reset history index
	}

	void DebugConsole::AddToOutputLog(std::string_view message)
	{
		m_outputLog.push_back(std::string(message));
		if (m_outputLog.size() > 100) // Limit log size
			m_outputLog.erase(m_outputLog.begin());
	}

	void DebugConsole::FlushOutputLog(void)
	{
		m_outputLog.clear();
	}

	void DebugConsole::VImGuiRoutine(void)
	{
		if (!m_bEnabled)
		{
			return;
		}

		ImGui::Begin(VImGuiWidgetName().data(), &m_bEnabled);

		RenderOutputLog();
		RenderInputBox();

		ImGui::End();
	}

	bool DebugConsole::IsEnabled(void) const noexcept
	{
		return m_bEnabled;
	}

	bool DebugConsole::IsAutoCompleteEnabled(void) const noexcept
	{
		return m_bAutoCompleteEnabled;
	}

	void DebugConsole::SetEnabled(bool bEnabled) noexcept
	{
		m_bEnabled = bEnabled;
	}

	void DebugConsole::SetAutoCompleteEnabled(bool bEnabled) noexcept
	{
		m_bAutoCompleteEnabled = bEnabled;
	}

	void DebugConsole::RenderInputBox(void)
	{
		// Define a fixed-size intermediate buffer
		constexpr std::size_t kBUFFER_SIZE = 256;
		char tempBuffer[kBUFFER_SIZE] = { 0 };

		// Copy existing m_inputBuffer contents to the temporary buffer
		std::strncpy(tempBuffer, m_inputBuffer.c_str(), kBUFFER_SIZE - 1);

		if (ImGui::InputText("Input", tempBuffer, kBUFFER_SIZE, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			// Update m_inputBuffer with the contents of tempBuffer
			m_inputBuffer = tempBuffer;

			// Execute the entered command
			ExecuteCommand(m_inputBuffer);

			// Clear the intermediate buffer and m_inputBuffer for the next input
			tempBuffer[0] = '\0';
			m_inputBuffer.clear();

			// Clear auto-complete suggestions
			m_autoCompleteSuggestions.clear();
		}

		// Navigation through history
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_UpArrow)) && !m_commandHistory.empty())
		{
			if (m_historyIndex == -1)
				m_historyIndex = static_cast<int>(m_commandHistory.size()) - 1;
			else if (m_historyIndex > 0)
				--m_historyIndex;

			m_inputBuffer = m_commandHistory[m_historyIndex];
		}
		else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_DownArrow)) && m_historyIndex != -1)
		{
			if (++m_historyIndex >= static_cast<int>(m_commandHistory.size()))
			{
				m_historyIndex = -1;
				m_inputBuffer.clear();
			}
			else
			{
				m_inputBuffer = m_commandHistory[m_historyIndex];
			}
		}
		else if (ImGui::IsKeyPressed(ImGuiKey_Tab))
		{
			m_autoCompleteSuggestions = PopulateSuggestions(m_inputBuffer); // Populate based on input
			m_selectedSuggestion = 0; // Default selection
		}

		// Render auto-complete suggestions
		RenderAutoComplete();
	}

	void DebugConsole::RenderOutputLog(void)
	{
		if (ImGui::BeginChild("OutputLog", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true))
		{
			for (const auto &line : m_outputLog)
				ImGui::TextWrapped("%s", line.c_str());

			if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
				ImGui::SetScrollHereY(1.0f);

			ImGui::EndChild();
		}
	}

	void DebugConsole::RenderAutoComplete(void)
	{
		if (!m_bAutoCompleteEnabled || m_autoCompleteSuggestions.empty())
		{
			return;
		}

		ImGui::SetNextWindowSize(ImVec2(300, std::min(200.0f, static_cast<float>(m_autoCompleteSuggestions.size() * 20))));
		ImGui::Begin("AutoComplete", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

		for (std::size_t i = 0; i < m_autoCompleteSuggestions.size(); ++i)
		{
			const std::string &suggestion = m_autoCompleteSuggestions[i];
			bool isSelected = (i == m_selectedSuggestion);

			// Highlight the currently selected suggestion
			if (isSelected)
				ImGui::Selectable(suggestion.c_str(), isSelected, ImGuiSelectableFlags_DontClosePopups);

			else
				ImGui::Text(suggestion.c_str());

			// Allow the user to select via arrow keys
			if (ImGui::IsKeyPressed(ImGuiKey_UpArrow) && m_selectedSuggestion > 0)
			{
				--m_selectedSuggestion;
			}
			else if (ImGui::IsKeyPressed(ImGuiKey_DownArrow) && m_selectedSuggestion < m_autoCompleteSuggestions.size() - 1)
			{
				++m_selectedSuggestion;
			}
			else if (ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_Tab))
			{
				m_inputBuffer = m_autoCompleteSuggestions[m_selectedSuggestion];
				m_autoCompleteSuggestions.clear();
				ImGui::End();
				return;
			}
		}
		ImGui::End();
	}

	void DebugConsole::ExecuteLua(std::string_view code)
	{
		auto &app = GetEngineApp();
		auto &lua = app.GetGameLogic().GetLuaScriptManager();
		// Attempt to run the code in the global Lua state
		try
		{
			lua.VExecuteString(code);
		}
		catch (const std::exception &ex)
		{
			AddToOutputLog(std::string("Lua Error: ") + ex.what());
		}
	}

	std::vector<std::string> DebugConsole::PopulateSuggestions(std::string_view input)
	{
		std::vector<std::string> suggestions;

		auto &app = GetEngineApp();
		auto &lua = app.GetGameLogic().GetLuaScriptManager();
		// Use to GetState() provide access to Lua state
		sol::state &luaState = lua.GetState();

		for (const auto &global : luaState.globals())
		{
			std::string key = global.first.as<std::string>();
			if (key.rfind(input, 0) == 0) // Check if `input` is a prefix of `key`
			{
				suggestions.push_back(key);
			}
		}
		return suggestions;
	}
} // End namespace (BGE)
