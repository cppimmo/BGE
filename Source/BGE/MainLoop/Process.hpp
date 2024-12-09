/*=============================================================================*
 * Process.hpp - .
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
#ifndef _BGE_PROCESS_HPP_
#define _BGE_PROCESS_HPP_

#include <cstddef>

namespace BGE
{
    class Process; // Forward declare
    BGE_DECLARE_PTR(Process);

    /**
     * @brief ID number for process instances.
     *
     * New process IDs are administered by the ProcessManager.
     */
    using ProcessID = std::uint64_t;

    /**
     * @brief Base class for representing processes.
     */
    class Process
    {
        friend class ProcessManager;
    public:
        static constexpr int kDEFAULT_PRIORITY = 0;
        //! Enumeration for representing the state of a process.
        enum State
        {
            kSTATE_UNINITIALIZED, /**< */
            kSTATE_REMOVED,       /**< */
            kSTATE_RUNNING,       /**< */
            kSTATE_PAUSED,        /**< */
            kSTATE_SUCCEEDED,     /**< */
            kSTATE_FAILED,        /**< */
            kSTATE_ABORTED        /**< */
        };
    private:
        ProcessID m_ID; //!< ID number of the process
        State m_state; //!< State of the process
        int m_priority; //!< Priority of the process (lower is higher priority)
        StrongProcessPtr m_pChild; //!< Pointer to child process (if any)
    public:
        explicit Process(int priority = 0);
        virtual ~Process(void);
    protected:
        // Process interface:
        //! Called on process creation.
        virtual bool VOnInit(void);
        //! Called in application main loop.
        virtual void VOnUpdate(float deltaTime) = 0;
        //! Called if the process ends successfully.
        virtual void VOnSuccess(void);
        //! Called if the process has failed.
        virtual void VOnFail(void);
        //! Called if the process was aborted.
        virtual void VOnAbort(void);
    public:
        //! End the process in a state of success.
        void Succeed(void);
        //! End the process in a state of failure.
        void Fail(void);
        //! Pause the process.
        void Pause(void);
        //! Resume the process.
        void Resume(void);
        // Accessors:
        ProcessID GetID(void) const noexcept;
        State GetState(void) const noexcept;
        int GetPriority(void) const noexcept;
        bool IsAlive(void) const noexcept;
        bool IsDeceased(void) const noexcept;
        bool IsRemoved(void) const noexcept;
        bool IsPaused(void) const noexcept;

        void SetID(ProcessID ID) noexcept;
        // Child process management member functions:
        //! Attach a child process to this process.
        void AttachChild(StrongProcessPtr pChild);
        //! Release ownership of the child process (releases ownership).
        StrongProcessPtr RemoveChild(void);
        //! Retrieve a pointer to the child process (doesn't release ownership).
        StrongProcessPtr PeekChild(void);
    private:
        //! Set the current state of this process.
        void SetState(State state) noexcept;
    };
} // End namespace (BGE)

#endif /* !_BGE_PROCESS_HPP_ */
