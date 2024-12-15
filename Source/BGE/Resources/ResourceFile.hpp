#ifndef _BGE_RESOURCEFILE_HPP_
#define _BGE_RESOURCEFILE_HPP_

namespace BGE
{
	class Resource; // Forward declare
	class IResourceFile; // Forward declare
	BGE_DECLARE_PTR(IResourceFile);

	class IResourceFile
	{
	public:
		virtual ~IResourceFile(void) = default;

		virtual bool VOpen(void) = 0;
		virtual std::size_t VGetRawResourceSize(const Resource &kResource) = 0;
		virtual std::size_t VGetRawResource(const Resource &kResource, char *pBuffer) = 0;
		virtual std::size_t VGetNumResources(void) const = 0;
		virtual const std::string &VGetResourceName(std::size_t num) const = 0;
		virtual bool VIsUsingDevelopmentDirectories(void) const = 0;
	};
} // End namespace (BGE)

#endif /* !_BGE_RESOURCEFILE_HPP_ */
