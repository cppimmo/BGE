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

bool BGE::WildcardMatch(std::string_view pattern, std::string_view str)
{
	return false;
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
	WideConverter wConverter;
	return wConverter.from_bytes(str); // UTF-8 to Wide
}

std::string BGE::WStringToString(const std::wstring &wstr)
{
	WideConverter wConverter;
	return wConverter.to_bytes(wstr); // Wide to UTF-8
}

bool IsNotBlank(int ch)
{
	return !std::isblank(ch);
}