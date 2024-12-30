/*******************************************************************************
 * @file   CommandParser.hpp
 * @author Brian Hoffpauir
 * @date   12.29.2023
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
#ifndef _BGE_COMMANDPARSER_HPP_
#define _BGE_COMMANDPARSER_HPP_

#include <string>
#include <string_view>
#include <span>
#include <limits>
#include <unordered_map>
#include <optional>

namespace BGE
{
	class CommandParser; // Forward declare
	BGE_DECLARE_PTR(CommandParser);

	/**
	 * @brief A class to parse and handle command-line arguments.
	 */
	class CommandParser final
	{
	private: // Internal state
		std::span<std::string_view> m_arguments; //!< List of command-line arguments passed to the program.
		std::unordered_map<std::string, std::string> m_argumentMap; //!< Map of argument name to their corresponding values.
		std::string m_lastError; //!< Stores the last error message encountered during parsing or operations.

		// Bounds checking
		double m_min = std::numeric_limits<double>::lowest(); //!< Minimum allowable value for arguments.
		double m_max = std::numeric_limits<double>::max(); //!< Maxmimum allowable value for arguments.
		bool m_bMinSet = false; //!< Indicates whether a minimum bound has been set.
		bool m_bMaxSet = false; //!< Inidcates whether a maximum bound has been set.
	public:
		/**
		 * @brief Constructs a CommandParser with a given list of arguments.
		 * @param arguments The command line arguments to be parsed.
		 */
		explicit CommandParser(std::span<std::string_view> arguments);

		//! Default constructor.
		~CommandParser(void) = default;

		/**
		 * @brief Checks if an argument exists in the list.
		 * @param name The name of the argument to check for.
		 * @return true if the argument exists, false otherwise.
		 */
		bool Boolean(std::string_view name) const;

		/**
		 * @brief Extracts a boolean parameter from the arguments.
		 * @param name The name of the parameter to extract.
		 * @param bValue Reference to store the extracted boolean value.
		 * @return Optional containing a boolean value if successful, empty otherwise.
		 */
		std::optional<bool> Boolean(std::string_view name, bool &bValue) const;

		/**
		 * @brief Extracts an integer parameter from the arguments.
		 * @param name The name of the parameter to extract.
		 * @param value Reference to store the extracted integer value.
		 * @return Optional containing an integer value if successful, empty otherwise.
		 */
		std::optional<int> Integer(std::string_view name, int &value) const;

		/**
		 * @brief Extracts a floating-point parameter from the arguments.
		 * @param name The name of the parameter to extract.
		 * @param value Reference to store the extracted float value.
		 * @return Optional containing a float value if successful, empty otherwise.
		 */
		std::optional<float> Float(std::string_view name, float &value) const;

		/**
		 * @brief Extracts a double-precision floating-point parameter from the arguments.
		 * @param name The name of the parameter to extract.
		 * @param value Reference to store the extracted double value.
		 * @return Optional containing a double value if successful, empty otherwise.
		 */
		std::optional<double> Double(std::string_view name, double &value) const;

		/**
		 * @brief Extracts a string parameter from the arguments.
		 * @param name The name of the parameter to extract.
		 * @param value Reference to store the extracted string.
		 * @return Optional containing a string value if successful, empty otherwise.
		 */
		std::optional<std::string> String(std::string_view name, std::string &value) const;

		/**
		 * @brief Extracts a filename parameter from the arguments.
		 * @param name Reference to store the extracted filename.
		 * @return Optional containing the filename if successful, empty otherwise.
		 */
		std::optional<std::string> Filename(std::string &name) const;

		/**
		 * @brief Sets the minimum allowable value for arguments.
		 * @param value The minimum value to set.
		 * @return Reference to the current CommandParser instance.
		 */
		CommandParser &Min(double value);

		/**
		 * @brief Sets the maximum allowable value for arguments.
		 * @param value The maximum value to set.
		 * @return Reference to the current CommandParser instance.
		 */
		CommandParser &Max(double value);

		/**
		 * @brief Sets the strict minimum bound for arguments (exclusive).
		 * @param value The bound value to set.
		 * @return Reference to the current CommandParser instance.
		 */
		CommandParser &Inf(double value);

		/**
		 * @brief Sets the strict maximum bound for arguments (exclusive).
		 * @param value The bound value to set.
		 * @return Reference to the current CommandParser instance.
		 */
		CommandParser &Sup(double value);

		/**
		 * @brief Detects and counts unprocessed excess arguments.
		 * @return The number of unprocessed arguments.
		 */
		int ExcessArguments(void) const;

		/**
		 * @brief Retrieves the last error message encountered.
		 * @return String view of the last error message.
		 */
		std::string_view GetLastError(void) const;
	private:
		/**
		 * @brief Parses the given list of arguments and initializes the internal state.
		 * @param arguments The arguments to parse.
		 */
		void ParseArguments(std::span<std::string_view> arguments);
	};
} // End namespace (BGE)

#endif /* !_BGE_COMMANDPARSER_HPP_ */
