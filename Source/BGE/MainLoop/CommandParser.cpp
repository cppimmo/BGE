/*******************************************************************************
 * @file   CommandParser.cpp
 * @author Brian Hoffpauir
 * @date   12.29.2024
 * @brief  A class to parse and handle command-line arguments.
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
#include "Engine/EngineStd.hpp"
#include "MainLoop/CommandParser.hpp"

#include <charconv>

namespace BGE
{
	CommandParser::CommandParser(std::span<std::string_view> arguments)
		: m_arguments(arguments)
	{
		ParseArguments(m_arguments);
	}

	bool CommandParser::Boolean(std::string_view name) const
	{
		return m_argumentMap.find(std::string(name)) != m_argumentMap.end();
	}

	std::optional<bool> CommandParser::Boolean(std::string_view name, bool &bValue) const
	{
		auto it = m_argumentMap.find(std::string(name));
		if (it == m_argumentMap.end())
		{
			return std::nullopt;
		}

		std::string val = it->second;
		bValue = (val == "1" || val == "true");
		return bValue;
	}

	std::optional<int> CommandParser::Integer(std::string_view name, int &value) const
	{
		auto it = m_argumentMap.find(std::string(name));
		if (it == m_argumentMap.end())
		{
			return std::nullopt;
		}

		auto [ptr, ec] = std::from_chars(it->second.data(), it->second.data() + it->second.size(), value);
		if (ec == std::errc())
		{
			return value;
		}
		return std::nullopt;
	}

	std::optional<float> CommandParser::Float(std::string_view name, float &value) const
	{
		auto it = m_argumentMap.find(std::string(name));
		if (it == m_argumentMap.end())
		{
			return std::nullopt;
		}

		try
		{
			value = std::stof(it->second);
			return value;
		}
		catch (const std::invalid_argument &)
		{
			return std::nullopt;
		}
	}

	std::optional<double> CommandParser::Double(std::string_view name, double &value) const
	{
		auto it = m_argumentMap.find(std::string(name));
		if (it == m_argumentMap.end())
		{
			return std::nullopt;
		}

		try
		{
			value = std::stod(it->second);
			return value;
		}
		catch (const std::invalid_argument &)
		{
			return std::nullopt;
		}
	}

	std::optional<std::string> CommandParser::String(std::string_view name, std::string &value) const
	{
		auto it = m_argumentMap.find(std::string(name));
		if (it == m_argumentMap.end())
		{
			return std::nullopt;
		}

		value = it->second;
		return value;
	}

	std::optional<std::string> CommandParser::Filename(std::string &name) const
	{
		if (m_arguments.size() < 2)
		{
			return std::nullopt;
		}

		name = m_arguments.back();
		return name;
	}

	CommandParser &CommandParser::Min(double value)
	{
		m_min = value;
		m_bMinSet = true;
		return *this;
	}

	CommandParser &CommandParser::Max(double value)
	{
		m_max = value;
		m_bMaxSet = true;
		return *this;
	}

	CommandParser &CommandParser::Inf(double value)
	{
		// TODO: This needs a proper implementation.
		return *this;
	}

	CommandParser &CommandParser::Sup(double value)
	{
		// TODO: This needs a proper implementation.
		return *this;
	}

	int CommandParser::ExcessArguments(void) const
	{
		return static_cast<int>(m_arguments.size() - m_argumentMap.size() - 1);
	}

	std::string_view CommandParser::GetLastError(void) const
	{
		return m_lastError;
	}

	void CommandParser::ParseArguments(std::span<std::string_view> arguments)
	{
		for (std::size_t i = 1; i < arguments.size(); ++i)
		{
			const auto &kArg = arguments[i];
			if (kArg.starts_with("--"))
			{
				auto eqPos = kArg.find('=');
				if (eqPos != std::string_view::npos)
					m_argumentMap.emplace(kArg.substr(2, eqPos - 2), kArg.substr(eqPos + 1));
				else
					m_argumentMap.emplace(kArg.substr(2), "");
			}
		}
	}
} // End namespace (BGE)
