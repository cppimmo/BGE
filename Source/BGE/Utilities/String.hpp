/*=============================================================================*
 * String.hpp : Defines some useful string utility functions
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
#ifndef _BGE_STRING_HPP_
#define _BGE_STRING_HPP_

namespace BGE
{
	// Alias for std::wstring_convert (between UTF-8 and Wide)
	using WideConverter = std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>>;

	bool WildcardMatch(std::string_view pattern, std::string_view str); // Supports * & ? patterns
	std::string TrimLeft(std::string str); // Trim whitespace to the left of string
	std::string TrimRight(std::string str); // Trim whitespace to the right of string
	std::string TrimString(std::string str); // Trim whitespace to the left and right of string
	std::wstring StringToWString(const std::string &str); // Convert string to wide string
	std::string WStringToString(const std::wstring &wstr); // Convert wide string to string
} // End namespace (BGE)

#endif /* !_BGE_STRING_HPP_ */