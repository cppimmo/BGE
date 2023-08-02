/*=============================================================================*
 * Screemshot.cpp - Screenshot capturing facilities.
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
 *============================================================================*/
#include "Engine/EngineStd.hpp"
#include "Screenshot.hpp"

static std::string GetScreenshotFilename(std::string_view saveGameDir);

void BGE::TakeScreenshot(std::string_view saveGameDir)
{
	SDL_Window *pWindow = BGUTGetWindowPtr();
	if (!pWindow)
	{
		BGE_ERROR("TakeScreenshot Failure: Window pointer could not be retrieved.");
		return;
	}

	int width = 0, height = 0;
	SDL_GetWindowSize(pWindow, &width, &height);

	Uint32 rMask, gMask, bMask;
	const Uint32 aMask = 0x000000;
#if SDL_BTYEORDER == SDL_BIG_ENDIAN
	rMask = 0xFF0000;
	gMask = 0x00FF00;
	bMask = 0x0000FF;
#else
	rMask = 0x0000FF;
	gMask = 0x00FF00;
	bMask = 0xFF0000;
#endif
	constexpr unsigned int kColorBytes = 4;
	constexpr unsigned int kBitsPerPixel = 32;
	
	auto createSurface = [&](void) -> auto
	{
		return  SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, kBitsPerPixel,
									 rMask, gMask, bMask, aMask);
	};

	SDL_Surface *pTemp = createSurface();
	SDL_Surface *pImage = createSurface();
	if (!pTemp || !pImage)
	{
		BGE_ERROR("TakeScreenshot Failure: Could not create surface (%s).", SDL_GetError());
		SDL_free(pTemp); SDL_free(pImage);
		return;
	}
	// Read the entire screen:
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pImage->pixels);
	for (std::size_t index = 0; index < height; ++index)
	{
		const auto kWidthPerColorBytes = (kColorBytes * width);
		std::memcpy(static_cast<char *>(pTemp->pixels) + kWidthPerColorBytes * index,
					static_cast<char *>(pImage->pixels) + kWidthPerColorBytes * (height - index),
					kWidthPerColorBytes);
	}
	
	std::memcpy(pImage->pixels, pTemp->pixels, width * height * kColorBytes);
	// Write image to file on disk (do not return on failure, so surfaces can be freed):
	BGE_ERROR_IF(SDL_SaveBMP(pTemp, GetScreenshotFilename(saveGameDir).c_str()) < 0,
				 "TakeScreenshot Failure: Could not save file (%s).", SDL_GetError());
	// Free surfaces:
	SDL_free(pTemp); SDL_free(pImage);
}

std::string GetScreenshotFilename(std::string_view saveGameDir)
{
	using BGE::Utils::GetSystemTimeString;

	constexpr bool kUSE_UNDERSCORES = true;
	const auto kTimeString = GetSystemTimeString(kUSE_UNDERSCORES);
	if (!kTimeString)
		return std::string();
	// TODO: Save screenshots to Screenshots/ directory in save game location.
	// TODO: Create Screenshots/ path with std::filesystem & verify that it exists.
	return std::string(saveGameDir) + "/Screenshots/snap_" + *kTimeString + ".bmp";
}