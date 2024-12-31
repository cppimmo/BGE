/*******************************************************************************
 * @file   Resource.hpp
 * @author Brian Hoffpauir
 * @date   11.29.2024
 * @brief  Identifier and extra data for resources.
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
#ifndef _BGE_RESOURCE_HPP_
#define _BGE_RESOURCE_HPP_

#include <string>
#include <string_view>

namespace BGE
{
	class Resource; // Forward declare
	BGE_DECLARE_PTR(Resource);
	class IResourceExtraData; // Forward declare
	BGE_DECLARE_PTR(IResourceExtraData);

	/**
	 * @brief Represents a resource with an identifiable name.
	 *
	 * The `Resource` class serves as a lightweight representation of a named resource.
	 * It provides functionality to get and set the resource's name.
	 */
	class Resource final
	{
		std::string m_name; //!< The resource name.
	public:
		/**
		 * @brief Constructs a `Resource` with the given name.
		 *
		 * @param name The name of the resource as a `std::string_view`.
		 */
		Resource(std::string_view name) : m_name(name) { }

		// Accessors:
		/**
		 * @brief Retrieves the name of the resource.
		 *
		 * @return A constant reference to the resource's name as a `std::string`.
		 */
		const std::string &GetName(void) const { return m_name; }

		/**
		 * @brief Sets or updates the resource's name.
		 *
		 * @param name The new name to set for the resource as a `std::string_view`.
		 */
		void SetName(std::string_view name) { m_name = name; }
	};

	/**
	 * @brief Interface for attaching and managing extra data for a resource.
	 *
	 * The `IResourceExtraData` class is an interface designed to handle additional
	 * data associated with a resource.
	 *
	 * @see ResourceHandle
	 */
	class IResourceExtraData
	{
	public:
		/**
		 * @brief Default virtual destructor.
		 *
		 * Ensures proper cleanup of derived classes.
		 */
		virtual ~IResourceExtraData(void) = default;

		// Interface:
		/**
		 * @brief Retrieves the extra data associated with the resource.
		 *
		 * This method must be implemented by derived classes to return resource-specific
		 * extra data in string format. Some derived classes may provide different accesors which
		 * can be accessed by using `std::dynamic_pointer_cast` to cast to the appropriate subclass.
		 *
		 * @return A `std::string` containing the extra resource data.
		 */
		virtual std::string VGetExtraData(void) = 0;
	};

	/**
	 * @brief Extra resource data for plaintext file contents.
	 *
	 * This class is used to store and manage additional resource data specifically
	 * for plaintext files. The plaintext data is stored in a `std::string` member
	 * and can be accessed using the appropriate interface.
	 */
	class PlaintextResourceExtraData : public IResourceExtraData
	{
	protected:
		std::string m_plaintext; //!< The plaintext contents of the resource.
	public:
		/**
		 * @brief Constructs a new PlaintextResourceExtraData object with the given plaintext data.
		 *
		 * @param plaintext A `std::string_view` containing the plaintext contents to store.
		 */
		explicit PlaintextResourceExtraData(std::string_view plaintext);

		//! Default destructor.
		virtual ~PlaintextResourceExtraData(void) override = default;

		// IResourceExtraData's interface:
		/**
		 * @brief Retrieves the extra resource data as plaintext.
		 *
		 * This method returns the stored plaintext resource data, allowing consumers of the
		 * resource to retrieve its contents.
		 *
		 * @return A `std::string` containing the plaintext resource data.
		 */
		virtual std::string VGetExtraData(void) override;
	};
	BGE_DECLARE_PTR(PlaintextResourceExtraData);
} // End namespace (BGE)

#endif /* !_BGE_RESOURCE_HPP_ */
