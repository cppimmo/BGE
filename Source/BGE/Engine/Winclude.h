/*=============================================================================*
 * Winclude.hpp - Windows API header.
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
#ifndef _BGE_WINCLUDE_H_
#define _BGE_WINCLUDE_H_

#include "Engine/EngineStd.hpp"
 /*
  * Use this file when you need access to the Win32 API.
  * It excludes unnecessary parts of the API in order to decrease build time.
  * If we find trouble with some Win32 API features not being available then try
  * uncommenting a related preprocessor statement and rebuilding.
  *
  * From Windows.h:
  * If defined, the following flags inhibit definition
  * of the indicated items.
  *
  *  NOGDICAPMASKS     - CC_*, LC_*, PC_*, CP_*, TC_*, RC_
  *  NOVIRTUALKEYCODES - VK_*
  *  NOWINMESSAGES     - WM_*, EM_*, LB_*, CB_*
  *  NOWINSTYLES       - WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
  *  NOSYSMETRICS      - SM_*
  *  NOMENUS           - MF_*
  *  NOICONS           - IDI_*
  *  NOKEYSTATES       - MK_*
  *  NOSYSCOMMANDS     - SC_*
  *  NORASTEROPS       - Binary and Tertiary raster ops
  *  NOSHOWWINDOW      - SW_*
  *  OEMRESOURCE       - OEM Resource values
  *  NOATOM            - Atom Manager routines
  *  NOCLIPBOARD       - Clipboard routines
  *  NOCOLOR           - Screen colors
  *  NOCTLMGR          - Control and Dialog routines
  *  NODRAWTEXT        - DrawText() and DT_*
  *  NOGDI             - All GDI defines and routines
  *  NOKERNEL          - All KERNEL defines and routines
  *  NOUSER            - All USER defines and routines
  *  NONLS             - All NLS defines and routines
  *  NOMB              - MB_* and MessageBox()
  *  NOMEMMGR          - GMEM_*, LMEM_*, GHND, LHND, associated routines
  *  NOMETAFILE        - typedef METAFILEPICT
  *  NOMINMAX          - Macros min(a,b) and max(a,b)
  *  NOMSG             - typedef MSG and associated routines
  *  NOOPENFILE        - OpenFile(), OemToAnsi, AnsiToOem, and OF_*
  *  NOSCROLL          - SB_* and scrolling routines
  *  NOSERVICE         - All Service Controller routines, SERVICE_ equates, etc.
  *  NOSOUND           - Sound driver routines
  *  NOTEXTMETRIC      - typedef TEXTMETRIC and associated routines
  *  NOWH              - SetWindowsHook and WH_*
  *  NOWINOFFSETS      - GWL_*, GCL_*, associated routines
  *  NOCOMM            - COMM driver routines
  *  NOKANJI           - Kanji support stuff.
  *  NOHELP            - Help engine interface.
  *  NOPROFILER        - Profiler interface.
  *  NODEFERWINDOWPOS  - DeferWindowPos routines
  *  NOMCX             - Modem Configuration Extensions
  */
#ifdef BGE_PLATFORM_WIN

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#include <Windows.h>

#endif /* BGE_PLATFORM_WIN */
#endif /* !_BGE_WINCLUDE_H_ */