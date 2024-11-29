/*=============================================================================*
 * ProcessManager.cpp - .
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
#include "Engine/EngineStd.hpp"
#include "MainLoop/ProcessManager.hpp"

BGE::ProcessManager::~ProcessManager(void)
{
    ClearAllProcesses();
}

std::string BGE::ProcessManager::VToString(void) const
{
    std::ostringstream oss;
    oss << "Process List:\n";
    for (const auto& pProcess : m_processList)
    {
        oss << "  ID: " << pProcess->GetID() << ", State: " << pProcess->GetState()
            << ", Priority: " << pProcess->GetPriority() << '\n';
    }
    return oss.str();
}

std::uint32_t BGE::ProcessManager::UpdateProcesses(unsigned long deltaMS)
{
    std::uint32_t successCount = 0, failCount = 0;

    auto iter = m_processList.begin();
    while (iter != m_processList.end())
    {
        // Retrieve the next process
        StrongProcessPtr pCurrProcess = *iter;

        auto iterCopy = iter;
        ++iter;

        if (pCurrProcess->GetState() == Process::kSTATE_UNINITIALIZED)
            (void)pCurrProcess->VOnInit();

        if (pCurrProcess->GetState() == Process::kSTATE_RUNNING)
            pCurrProcess->VOnUpdate(deltaMS);

        if (pCurrProcess->IsDeceased())
        {
            switch (pCurrProcess->GetState())
            {
            case Process::kSTATE_SUCCEEDED:
            {
                pCurrProcess->VOnSuccess();
                auto pChild = pCurrProcess->RemoveChild();
                if (pChild)
                    AttachProcess(pChild, pCurrProcess->GetPriority());
                else
                    ++successCount; // Only increment if entire chain succeeds
                break;
            }
            case Process::kSTATE_FAILED:
                pCurrProcess->VOnFail();
                ++failCount;
                break;
            case Process::kSTATE_ABORTED:
                pCurrProcess->VOnAbort();
                ++failCount;
                break;
            default:
                break;
            }
            // Remvoe the process & destroy it
            m_processList.erase(iterCopy);
        }
    }
    return ((successCount << 16) | failCount);
}

BGE::WeakProcessPtr BGE::ProcessManager::AttachProcess(StrongProcessPtr pProcess, int priority)
{
    BGE_ASSERT(pProcess);
    pProcess->m_ID = m_nextID++; // Set and increment the process ID
    pProcess->m_priority = priority; // Set the process priority

    // Insert the process into the list based on its priority
    auto iter = std::find_if(m_processList.begin(), m_processList.end(),
                             [priority](const StrongProcessPtr &pProcess)
                             {
                                return pProcess->GetPriority() > priority;
                             });

    // Insert the process into the list
    m_processList.insert(iter, pProcess);
    return WeakProcessPtr(pProcess);
}

void BGE::ProcessManager::AbortAllProcesses(bool bImmediate)
{
    for (auto iter = m_processList.begin(); iter != m_processList.end();)
    {
        StrongProcessPtr pProcess = *iter++;
        // Abort the process if it is alive
        if (pProcess->IsAlive())
        {
            pProcess->SetState(Process::kSTATE_ABORTED);
            // Should the process be aborted immediately?
            if (bImmediate)
            {
                pProcess->VOnAbort();
                m_processList.erase(std::prev(iter));
            }
        }
    }
}

std::size_t BGE::ProcessManager::GetProcessCount(void) const
{
    return m_processList.size();
}

BGE::StrongProcessPtr BGE::ProcessManager::GetProcessByID(ProcessID ID) const
{
    auto iter = std::find_if(m_processList.begin(), m_processList.end(),
                 [ID](const StrongProcessPtr &pProcess)
                 {
                     return pProcess->GetID() == ID;
                 });
    return (iter != m_processList.end()) ? *iter : nullptr;
}

void BGE::ProcessManager::ClearAllProcesses(void)
{
    m_processList.clear();
}
