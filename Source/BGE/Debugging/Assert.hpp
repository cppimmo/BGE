#ifndef _BGE_DEBUGGING_ASSERT_HPP_
#define _BGE_DEBUGGING_ASSERT_HPP_

#include "Debugging/Logger.hpp" // Logger needed for runtime assertions

//! Function macro for static assertions.
#define BGE_STATIC_ASSERT(EXPR) static_assert(EXPR, "static assert failed:" #EXPR)

#if defined(BGE_CONFIG_DEBUG) || defined(BGE_CONFIG_PROFILE) // Debug mode

//! BGE's assertion macro (run errors messenger routine when EXPR is false).
#define BGE_ASSERT(EXPR) \
do \
{ \
	if (!(EXPR)) \
	{ \
		using namespace BGE::Logger; \
		static ErrorMessenger *pMessenger = new ErrorMessenger(true); \
		pMessenger->Show(#EXPR, ""); \
	} \
} \
while (0) \

#else // Release mode

#define BGE_ASSERT(EXPR, ...) do { ; } while (0)

#endif /* def BGE_CONFIG_DEBUG || BGE_CONFIG_PROFILE */

#endif /* !_BGE_DEBUGGING_ASSERT_HPP_ */
