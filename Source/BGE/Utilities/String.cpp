/*=============================================================================*
 * String.cpp : Defines some useful string utility functions
 *
 * Part of the BGEngine Project
 *
 * (c) Copyright 2022 Brian Hoffpauir
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser GPL v3
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 * http://www.gnu.org/licenses/lgpl-3.0.txt for more details.
 *
 * You should have received a copy of the GNU Lesser GPL v3
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *============================================================================*/
#include "Engine/EngineStd.hpp"
#include "String.hpp"

static bool IsNotBlank(int ch); // Helper function for std::isblank

#include <string_view>

bool BGE::WildcardMatch(std::string_view pattern, std::string_view str) {
	std::string_view::size_type i = 0;
	bool bStar = false;
new_segment:
	bStar = false;
	if (!pattern.empty() && pattern[0] == '*')
	{
		bStar = true;
		do
		{
			pattern.remove_prefix(1); // Skip '*' characters
		} while (!pattern.empty() && pattern[0] == '*');
	}
test_match:
	for (i = 0; i < pattern.size() && pattern[i] != '*'; i++)
	{
		if (i >= str.size() || (pattern[i] != str[i] && (pattern[i] != '?' || str[i] == '.')))
		{
			if (!bStar) return false;

			str.remove_prefix(1); // Consume one character of str
			goto test_match;
		}
	}

	if (i < pattern.size() && pattern[i] == '*')
	{
		// Advance both pattern and string
		str.remove_prefix(i);
		pattern.remove_prefix(i);
		goto new_segment;
	}

	// If we reached the end of the string
	if (i == str.size())
	{
		return true;
	}

	// Handle case where pattern ends with '*' or no more stars to handle
	if (i > 0 && pattern[i - 1] == '*')
	{
		return true;
	}

	// If no star, we have a mismatch
	if (!bStar)
	{
		return false;
	}
	// Retry with the next character of the string
	str.remove_prefix(1);
	goto test_match;
}

std::string BGE::SnakeCaseString(std::string_view str)
{
	std::string result = std::string(str);
	for (auto &ch : result)
	{
		if (std::isblank(ch))
			ch = '_';
		else
			ch = std::tolower(ch);
	}
	return result;
}
// These functions recieve the string by value to make it possbile to move the result.
std::string BGE::TrimLeft(std::string str)
{
	// Trim whitespace on left side of string
	str.erase(str.begin(), std::find_if(str.begin(), str.end(), ::IsNotBlank));
	return str;
}

std::string BGE::TrimRight(std::string str)
{
	// Trim whitespace on right side of string
	str.erase(std::find_if(str.rbegin(), str.rend(), ::IsNotBlank).base(), str.end());
	return str;
}

std::string BGE::TrimString(std::string str)
{
	// Trim whitespace on both sides of the string
	return TrimLeft(TrimRight(std::move(str)));
}
// https://stackoverflow.com/questions/4358870/convert-wstring-to-string-encoded-in-utf-8/12903901#12903901
std::wstring BGE::StringToWString(const std::string &str)
{
	WideConverter converter;
	return converter.from_bytes(str); // UTF-8 to Wide
}

std::string BGE::WStringToString(const std::wstring &wstr)
{
	WideConverter converter;
	return converter.to_bytes(wstr); // Wide to UTF-8
}

std::string BGE::StringToLower(std::string_view str)
{
	std::string result(str); // Create a modifiable copy of the input
	std::ranges::transform(result, result.begin(), [](unsigned char ch)
	{
		return std::tolower(ch);
	});
	return result;
}

std::string BGE::StringToUpper(std::string_view str)
{
	std::string result(str); // Create a modifiable copy of the input
	std::ranges::transform(result, result.begin(), [](unsigned char ch)
	{
		return std::toupper(ch);
	});
	return result;
}

bool IsNotBlank(int ch)
{
	return !std::isblank(ch);
}
