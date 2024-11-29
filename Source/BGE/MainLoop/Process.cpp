/*=============================================================================*
 * Process.cpp - .
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
#include "MainLoop/Process.hpp"

BGE::Process::Process(int priority)
    : m_ID(0), m_state(kSTATE_UNINITIALIZED), m_priority(priority)
{
}

BGE::Process::~Process(void)
{
    if (m_pChild)
    {
        m_pChild->VOnAbort();
    }
}

bool BGE::Process::VOnInit(void)
{
    m_state = kSTATE_RUNNING;
    return true;
}

void BGE::Process::VOnUpdate(float deltaTime)
{
}

void BGE::Process::VOnSuccess(void)
{
}

void BGE::Process::VOnFail(void)
{
}

void BGE::Process::VOnAbort(void)
{
}

void BGE::Process::Succeed(void)
{
    BGE_ASSERT(m_state == kSTATE_RUNNING || m_state == kSTATE_PAUSED);
    m_state = kSTATE_SUCCEEDED;
}

void BGE::Process::Fail(void)
{
    BGE_ASSERT(m_state == kSTATE_RUNNING || m_state == kSTATE_PAUSED);
    m_state = kSTATE_FAILED;
}

void BGE::Process::Pause(void)
{
    if (m_state == kSTATE_RUNNING)
    {
        m_state = kSTATE_PAUSED;
    }
    else
    {
        BGE_WARNING("Attempting to pause a process that isn't running");
    }
}

void BGE::Process::Resume(void)
{
    if (m_state == kSTATE_PAUSED)
    {
        m_state = kSTATE_RUNNING;
    }
    else
    {
        BGE_WARNING("Attempting to resume a process that isn't paused");
    }
}

BGE::ProcessID BGE::Process::GetID(void) const noexcept
{
    return m_ID;
}

BGE::Process::State BGE::Process::GetState(void) const noexcept
{
    return m_state;
}

int BGE::Process::GetPriority(void) const noexcept
{
    return m_priority;
}

bool BGE::Process::IsAlive(void) const noexcept
{
    return (m_state == kSTATE_RUNNING || m_state == kSTATE_PAUSED);
}

bool BGE::Process::IsDeceased(void) const noexcept
{
    return (m_state == kSTATE_SUCCEEDED || m_state == kSTATE_FAILED || m_state == kSTATE_ABORTED);
}

bool BGE::Process::IsRemoved(void) const noexcept
{
    return m_state == kSTATE_REMOVED;
}

bool BGE::Process::IsPaused(void) const noexcept
{
    return m_state == kSTATE_PAUSED;
}

void BGE::Process::SetID(ProcessID ID) noexcept
{
    m_ID = ID;
}

void BGE::Process::AttachChild(StrongProcessPtr pChild)
{
    if (m_pChild)
    {
        m_pChild->AttachChild(pChild);
    }
    else
    {
        m_pChild = pChild;
    }
}

BGE::StrongProcessPtr BGE::Process::RemoveChild(void)
{
    if (m_pChild)
    {
        // Prevent the children from being destroyed upon being cleared
        StrongProcessPtr pChild = m_pChild;
        m_pChild.reset();
        return pChild;
    }
    return StrongProcessPtr();
}

BGE::StrongProcessPtr BGE::Process::PeekChild(void)
{
    return m_pChild;
}

void BGE::Process::SetState(State state) noexcept
{
    m_state = state;
}
