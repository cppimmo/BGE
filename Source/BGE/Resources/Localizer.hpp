/*******************************************************************************
 * @file   Localizer.hpp
 * @author Brian Hoffpauir
 * @date   12.07.2024
 * @brief  .
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
#ifndef _BGE_LOCALIZER_HPP_
#define _BGE_LOCALIZER_HPP_

namespace BGE
{
	class Localizer; // Forward declare
    BGE_DECLARE_PTR(Localizer);

	/**
	 * @brief .
	 */
	class Localizer : public INonCopyable, INonMovable
	{
	public:
		//! Enum for supported languages.
		enum struct Language
		{
			kEnglish,
			kSpanish,
			kFrench,
			kGerman
			// Add more...
		};
		//! Language two letter ISO-639 code and name.
		using LanguagePair = std::pair<std::string_view, std::string_view>;
	private:
		using TextStringMap = std::map<std::wstring, std::wstring>;
		//! Fallback to this language if a string is missing
		static constexpr Language kFALLBACK_LANGUAGE = Language::kEnglish;

		//! Store localized strings for all languages
		std::map<Language, TextStringMap> m_localizedStrings;
		Language m_currentLanguage = Language::kEnglish; //!< Current language
		mutable std::mutex m_mutex; //!< Mutex for thread-safe access
	public:
		Localizer(void) = default;

		//! Load strings for a specific language.
		bool LoadStrings(Language language);
		//! Unload strings for a specific language.
		bool UnloadString(Language language);
		//! Get a localized string by ID.
		const std::wstring &GetString(std::wstring_view sID) const;
		//! Change the current language.
		void SetLanguage(Language language);
		//! Get the current language.
		Language GetCurrentLanguage(void) const;
	private:
		// Convert enum to language string (e.g., "English")
		static constexpr LanguagePair LanguageToString(Language language);
	};

	inline constexpr Localizer::LanguagePair Localizer::LanguageToString(Language language)
	{
		switch (language)
		{
		case Language::kEnglish:
			return { "EN", "English" };
		case Language::kSpanish:
			return { "ES", "Spanish" };
		case Language::kFrench:
			return { "FR", "French" };
		case Language::kGerman:
			return { "DE", "German" };
		default:
			return { "Unknown", "Unknown" };
		}
	}
} // End namespace (BGE)

#endif /* !_BGE_LOCALIZER_HPP_ */
