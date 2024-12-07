/*******************************************************************************
 * @file   Localizer.cpp
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
#include "Engine/EngineStd.hpp"
#include "Resources/Localizer.hpp"

bool BGE::Localizer::LoadStrings(Language language)
{
	using namespace tinyxml2;

    std::lock_guard<std::mutex> lock(m_mutex);

    std::string languageFilePath = "Assets/Strings/";
    languageFilePath += LanguageToString(language).second; // Convert language enum to file path
    languageFilePath += ".xml";

    XMLDocument xmlDocument;
    if (xmlDocument.LoadFile(languageFilePath.c_str()) != XML_SUCCESS)
    {
        // Log error
        BGE_ERROR("Could not load language file: " + languageFilePath);
        return false;
    }

    auto *pRoot = xmlDocument.RootElement();
    if (!pRoot)
    {
        BGE_ERROR("Missing root element in: " + languageFilePath);
        return false;
    }

    TextStringMap localizedStrings;

    for (auto *pElem = pRoot->FirstChildElement(); pElem; pElem = pElem->NextSiblingElement())
    {
        const char *pKey = pElem->Attribute("sID");
        const char *pValue = pElem->Attribute("value");

        if (pKey && pValue)
        {
            auto wideKey = StringToWString(pKey);
            auto wideValue = StringToWString(pValue);
            localizedStrings[wideKey] = wideValue;
        }
    }

    m_localizedStrings[language] = std::move(localizedStrings);
    return true;
}

const std::wstring &BGE::Localizer::GetString(std::wstring_view sID) const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    // First, try the current language
    auto langIt = m_localizedStrings.find(m_currentLanguage);
    if (langIt != m_localizedStrings.cend())
    {
        auto &strings = langIt->second;
        auto it = strings.find(sID.data());
        if (it != strings.cend())
            return it->second;
    }

    // Fallback to the default language
    langIt = m_localizedStrings.find(kFALLBACK_LANGUAGE);
    if (langIt != m_localizedStrings.cend())
    {
        auto &strings = langIt->second;
        auto it = strings.find(sID.data());
        if (it != strings.cend())
            return it->second;
    }

    // Log missing string and return an empty string
    BGE_ERROR("Missing string ID: " + std::string(sID.begin(), sID.end()));
    static const std::wstring c_emptyString;
    return c_emptyString;
}

void BGE::Localizer::SetLanguage(Language language)
{
	std::lock_guard<std::mutex> lock(m_mutex);
    m_currentLanguage = language;
}

BGE::Localizer::Language BGE::Localizer::GetCurrentLanguage(void) const
{
	return m_currentLanguage;
}
