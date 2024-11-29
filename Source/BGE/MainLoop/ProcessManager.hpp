/*=============================================================================*
 * ProcessManager.hpp - .
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
 *============================================================================*/
#ifndef _BGE_PROCESSMANAGER_HPP_
#define _BGE_PROCESSMANAGER_HPP_

#include "MainLoop/Process.hpp"

namespace BGE
{
    class Process; // Forware declare

    /**
     * @brief Base class for representing processes.
     */
    class ProcessManager final : public INonCopyable, public INonMovable, public IStringable
    {
        using ProcessList = std::list<StrongProcessPtr>;
        ProcessList m_processList;
        ProcessID m_nextID = 1; //!< ID counter for assigning new process IDs
    public:
        ~ProcessManager(void);
        // IStringable's interface:
        virtual std::string VToString(void) const override;
        // Interface:
        std::uint32_t UpdateProcesses(unsigned long deltaMS);
        WeakProcessPtr AttachProcess(StrongProcessPtr pProcess, int priority = Process::kDEFAULT_PRIORITY);
        void AbortAllProcesses(bool bImmediate);
        // Accessors:
        std::size_t GetProcessCount(void) const;
        StrongProcessPtr GetProcessByID(ProcessID ID) const;
    private:
        void ClearAllProcesses(void); // Should only be called by the destructor.
    };
} // End namespace (BGE)

#endif /* !_BGE_PROCESSMANAGER_HPP_ */
