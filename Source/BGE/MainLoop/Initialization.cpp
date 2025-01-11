/*******************************************************************************
 * @file   Initialization.hpp
 * @author Brian Hoffpauir
 * @date   01.08.2023
 * @brief  Initialization routines.
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
#include "Initialization.hpp"

#if BGE_PLATFORM_WIN
#include "Engine/Winclude.hpp"
#include <io.h>
#include <fcntl.h>
#elif BGE_PLATFORM_LINUX
#include <sys/file.h>
#include <cerrno>
#endif

#include <cmath>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

bool BGE::OpenConsole(std::string_view title)
{
#if BGE_PLATFORM_WIN
	int hConsole = 0;
	HANDLE stdHandle = nullptr;
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	ZeroMemory(&consoleInfo, sizeof(consoleInfo));

	std::FILE *pFile = nullptr;
	// Close prior references to console facilities so they can be reopened later
	if (_get_osfhandle(0) < 0)
	{
		_close(0);
	}

	pFile = freopen("//./NUL", "r", stdin);
	// auto freopen_sResult = freopen_s(&::pStreamStdin, "//./NUL", "r", stdin);
	setvbuf(stdin, NULL, _IONBF, 0u);
	if (_get_osfhandle(1) < 0)
	{
		_close(1);
	}

	pFile = freopen("//./NUL", "w", stdout);
	// freopen_sResult = freopen_s(&::pStreamStdout, "//./NUL", "w", stdout);
	setvbuf(stdout, NULL, _IONBF, 0u);
	if (_get_osfhandle(2) < 0)
	{
		_close(2);
	}

	pFile = freopen("//./NUL", "w", stderr);
	// freopen_sResult = freopen_s(&pStreamStderr, "//./NUL", "w", stderr);
	setvbuf(stderr, NULL, _IONBF, 0u);

	::FreeConsole();
	if (!::AllocConsole())
	{
		return false;
	}

	::SetConsoleTitleA(title.data());

	// Set the screen buffer to be big enough to scroll text
	::GetConsoleScreenBufferInfo(::GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo);
	consoleInfo.dwSize.Y = 1024;
	::SetConsoleScreenBufferSize(::GetStdHandle(STD_OUTPUT_HANDLE), consoleInfo.dwSize);
	// Redirect unbuffered STDIN to the console
	stdHandle = ::GetStdHandle(STD_INPUT_HANDLE);
	hConsole = _open_osfhandle(reinterpret_cast<intptr_t>(stdHandle), _O_TEXT);

	if (_dup2(hConsole, _fileno(stdin)) == -1)
	{
		return false;
	}

	::SetStdHandle(STD_INPUT_HANDLE, reinterpret_cast<HANDLE>(_get_osfhandle(_fileno(stdin))));
	_close(hConsole);

	// Redirect unbuffered STDOUT to the console
	stdHandle = ::GetStdHandle(STD_OUTPUT_HANDLE);
	hConsole = _open_osfhandle(reinterpret_cast<intptr_t>(stdHandle), _O_TEXT);

	if (_dup2(hConsole, _fileno(stdout)) == -1)
	{
		return false;
	}

	::SetStdHandle(STD_OUTPUT_HANDLE, reinterpret_cast<HANDLE>(_get_osfhandle(_fileno(stdout))));
	_close(hConsole);

	// Redirect unbuffered STDERR to the console
	stdHandle = ::GetStdHandle(STD_ERROR_HANDLE);
	hConsole = _open_osfhandle(reinterpret_cast<intptr_t>(stdHandle), _O_TEXT);

	if (_dup2(hConsole, _fileno(stderr)) == -1)
	{
		return false;
	}

	::SetStdHandle(STD_ERROR_HANDLE, reinterpret_cast<HANDLE>(_get_osfhandle(_fileno(stderr))));
	_close(hConsole);
	// Set the console's attributes
	::SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	::SetConsoleMode(::GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);
	::SetConsoleMode(::GetStdHandle(STD_INPUT_HANDLE), ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
	// Set _Crt Report Modes/Files
	// _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	// _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
	// _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	// _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
	// _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	// _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);

	// Sycronize with the C++ I/O facilities
	std::ios::sync_with_stdio();
#endif /* BGE_PLATFORM_WIN */
	return true;
}

std::span<std::string_view> BGE::GetArguments(int numArgs, char *pArgs[])
{
	// From C++ Weekly - Ep 361
	std::vector<std::string_view> argsVec(pArgs, std::next(pArgs, static_cast<std::ptrdiff_t>(numArgs)));
	return std::span(argsVec.data(), argsVec.size());
}

std::string_view BGE::GetPlatform(void)
{
	return SDL_GetPlatform();
}

bool BGE::IsDiskSpaceAvailable(int neededDiskSpaceMiB)
{
	std::error_code errorCode;
	const auto kCurrPath = fs::absolute(fs::current_path(errorCode), errorCode).root_path();
	const auto kSpaceInfo = fs::space(kCurrPath, errorCode);

	if (errorCode)
	{
		BGE_ERROR("CheckStorage Failure: %s", errorCode.message());
		return false;
	}

	// Convert to MiB
	const auto kFreeSpace = (kSpaceInfo.free * std::pow(10, -6));
	if (kFreeSpace < neededDiskSpaceMiB)
	{
		// Not enough space left on the current disk
		BGE_ERROR("CheckStorage Failure: Not enough physical storage remaining.");
		return false;
	}
	return true;
}

int BGE::ReadLogicalCPUCores(void)
{
	return SDL_GetCPUCount();
}
// Needs per-platform implementation:
std::uint32_t BGE::ReadCPUSpeed(void)
{
#if BGE_PLATFORM_WIN
	DWORD bufferSize = sizeof(DWORD);
	DWORD speedMHz = 0;
	DWORD type = REG_DWORD;
	HKEY hKey = nullptr;

	// Open the key where the proc speed is hidden:
	const auto kErrorCode = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
										 0, KEY_READ, &hKey);
	if (kErrorCode == ERROR_SUCCESS)
	{
		// Query the key:
		RegQueryValueEx(hKey, "~MHz", nullptr, &type, reinterpret_cast<LPBYTE>(&speedMHz), &bufferSize);
	}
	return speedMHz;
#elif BGE_PLATFORM_LINUX
	return 0;
#else
	return 0;
#endif
}

bool BGE::IsMemoryAvailable(int neededRAM_MiB)
{
	const auto kRAM = SDL_GetSystemRAM();
	
	if (kRAM < neededRAM_MiB)
	{
		BGE_ERROR("CheckMemory Failure: There is not enough memory on your system.");
		return false;
	}
	return true;
}

SDL_PowerState BGE::CheckPowerState(int &secondsLeft, int &percentage)
{
	return SDL_GetPowerInfo(&secondsLeft, &percentage);
}

bool BGE::IsDebuggerPresent(void)
{
#if BGE_PLATFORM_WIN
	return ::IsDebuggerPresent();
#else
	return false;
#endif
}
// See: https://stackoverflow.com/questions/5339200/how-to-create-a-single-instance-application-in-c-or-c
bool BGE::IsOnlyInstance(std::string_view gameTitle)
{
#if BGE_PLATFORM_WIN
	// Only one instance of the program can have the mutex referred to as gameTitle
	HANDLE hHandle = CreateMutex(NULL, TRUE, gameTitle.data());
	// Failure may indicate that the mutex already exists
	if (GetLastError() != ERROR_SUCCESS)
	{
		HWND hWnd = FindWindow(NULL, gameTitle.data());
		if (hWnd)
		{
			// Bring window to forefront and set focus
			ShowWindow(hWnd, SW_SHOWNORMAL);
			SetFocus(hWnd);
			SetForegroundWindow(hWnd);
			SetActiveWindow(hWnd);
			return false;
		}
	}
	return true;
#elif BGE_PLATFORM_LINUX
	std::string procFilename("/tmp/"); // Create process filename
	procFilename.append(gameTitle);
	procFilename.append(".pid");
	// Get the file descriptor
	const int pidFileDesc = open(procFilename.c_str(), O_CREAT | O_RDWR, 0666);
	if (pidFileDesc < 0)
	{
		return true;
	}

	const int flResult = flock(pidFileDesc, LOCK_EX | LOCK_NB);
	//if (flResult != 0) return true;
	// Indicates that another instance of the program is running
	if (errno == EWOULDBLOCK)
	{
		return false;
	}
	// Otherwise indicate that there is not another instance of the program
	return true;
#else
	return true;
#endif
}

std::optional<std::string> BGE::GetCurrentDirectory(void)
{
	std::error_code errCode;
	auto currPath = fs::current_path(errCode).string();

	if (errCode)
	{
		return std::nullopt;
	}
	return currPath;
}

std::optional<std::string> BGE::GetSaveGameDirectory(std::string_view orgName, std::string_view appName)
{
	auto *pPrefPath = SDL_GetPrefPath(orgName.data(), appName.data());

	if (!pPrefPath)
	{
		BGE_ERROR("GetSaveGameDirectory Failure: Unable to find suitable path.");
		return std::nullopt;
	}

	std::string result(pPrefPath);
	SDL_free(pPrefPath); // Free since we own the result
	return result;
}

void BGE::HideConsole(void)
{
	// Hide console window on windows in release configuration
#if BGE_PLATFORM_WIN
	HWND hConsole = GetConsoleWindow();
	if (!hConsole)
	{
		return;
	}
	
	ShowWindow(hConsole, SW_HIDE);
#endif
}

