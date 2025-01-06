#include "Engine/EngineStd.hpp"
#include "Graphics/AssimpResource.hpp"

#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>
#include <assimp/LogStream.hpp>
#include <assimp/Logger.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags

namespace
{
	/**
	 * @brief .
	 */
	class CustomLogStream final : public Assimp::LogStream
	{
	public:
		virtual void write(const char *message) override
		{
			BGE_LOG("Resources", "Assimp: %s", message);
		}
	};

	/**
	 * @brief .
	 */
	class CustomIOStream final : public Assimp::IOStream
	{
		friend class CustomIOSystem;
	protected:
		// Constructor protected for private usage by MyIOSystem
		CustomIOStream(void) { }
	public:
		~CustomIOStream(void) override { }
		// Assimp::IOStream's interface:
		virtual size_t Read(void* pvBuffer, size_t pSize, size_t pCount) override
		{
		}

		virtual size_t Write(const void* pvBuffer, size_t pSize, size_t pCount) override
		{
		}

		virtual aiReturn Seek(size_t pOffset, aiOrigin pOrigin) override
		{
		}

		virtual size_t Tell(void) const override
		{
		}

		virtual size_t FileSize(void) const override
		{
		}

		virtual void Flush (void) override
		{
		}
	};

	/**
	 * @brief .
	 */
	class CustomIOSystem final : public Assimp::IOSystem
	{
	public:
		CustomIOSystem(void)
		{
		}
		~CustomIOSystem(void) override
		{
		}

		// Check whether a specific file exists
		virtual bool Exists(const char *pFile) const override
		{
			const BGE::Resource kResource(pFile); // Create resource for the given file name

			// Attempt to retrieve a handle for the resource
			auto &app = BGE::GetEngineApp();
			auto pHandle = app.GetResourceCache().GetHandle(kResource);
			if (!pHandle)
			{
				return false; // File does NOT exist
			}
			return true; // File does exist
		}

		// Get the path delimiter character we'd like to see
		virtual char getOsSeparator(void) const override
		{
			// TODO: This may need to be changed.
			return '\\';
		}

		// ... and finally a method to open a custom stream
		virtual Assimp::IOStream* Open(const char *pFile, const char *pMode) override
		{
			BGE_ASSERT((std::strcmp(pMode, "rb") == 0) && "Assimp stream write not supported");

			if (std::strcmp(pMode, "rb") == 0)
			{
			}
			else if (std::strcmp(pMode, "r") == 0)
			{
			}
			else if (std::strcmp(pMode, "rt") == 0)
			{
			}
			else if (std::strcmp(pMode, "wb") == 0)
			{
			}
			else if (std::strcmp(pMode, "w") == 0)
			{
			}
			else if (std::strcmp(pMode, "wt") == 0)
			{
			}

			// TODO: Do some extra processing on the file name.
			const BGE::Resource kResource(pFile);
			auto &app = BGE::GetEngineApp();
			auto pHandle = app.GetResourceCache().GetHandle(kResource);
			if (!pHandle)
			{
				BGE_ERROR("");
				return nullptr;
			}

			//pHandle->Buffer();
			//pHandle->WritableBuffer();
			//pHandle->GetSize();
			//pHandle->GetType();

			return BGE_NEW CustomIOStream();
		}

		virtual void Close(Assimp::IOStream *pFile) override
		{
			BGE_SAFE_DELETE(pFile);
		}
	};
} // End namespace

namespace BGE
{
	std::string AssimpResourceLoader::VGetPattern(void) const
	{
		return "*.obj";
	}

	ResourceType AssimpResourceLoader::VGetType(void) const
	{
		return ResourceType::kAssimp;
	}

	bool AssimpResourceLoader::VUseRawFile(void) const
	{
		return false;
	}

	bool AssimpResourceLoader::VDiscardRawBufferAfterLoad(void)
	{
		return true;
	}

	bool AssimpResourceLoader::VAddNullZero(void)
	{
		return true;
	}

	std::size_t AssimpResourceLoader::VGetLoadedResourceSize(char *pRawBuffer, std::size_t rawSize)
	{
		return rawSize;
	}

	bool AssimpResourceLoader::VLoadResource(char *pRawBuffer, std::size_t size, StrongResourceHandlePtr pResourceHandle)
	{
		if (!pRawBuffer || size <= 0)
		{
			return false;
		}

		const auto kLogSeverity = Assimp::Logger::VERBOSE;
		// Create a logger instance
		Assimp::DefaultLogger::create("", kLogSeverity);
		CustomLogStream logStream;
		Assimp::DefaultLogger::get()->attachStream(&logStream, kLogSeverity);

		// Create the importer
		Assimp::Importer importer;
		CustomIOSystem ioSystem;
		importer.SetIOHandler(&ioSystem);

		const auto kFlags = aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType |
			aiProcess_FlipUVs |
			aiProcess_GenNormals;

		// Store resource name to use as hint
		const std::string kNameHint = pResourceHandle->GetName();

		const aiScene *pkScene = importer.ReadFileFromMemory(pRawBuffer, size, kFlags, kNameHint.c_str());
		if (!pkScene || pkScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pkScene->mRootNode)
		{
			BGE_ERROR("Assimp import failed: %s", importer.GetErrorString());
			return false;
		}

		// Log status of scene components
		BGE_LOG_IF(pkScene->HasAnimations(), "Resources", "Assimp scene has animations");
		BGE_LOG_IF(pkScene->HasCameras(), "Resources", "Assimp scene has cameras");
		BGE_LOG_IF(pkScene->HasLights(), "Resources", "Assimp scene has lights");
		BGE_LOG_IF(pkScene->HasMaterials(), "Resources", "Assimp scene has materials");
		BGE_LOG_IF(pkScene->HasMeshes(), "Resources", "Assimp scene has meshes");
		BGE_LOG_IF(pkScene->HasTextures(), "Resources", "Assimp scene has textures");
		BGE_LOG_IF(pkScene->hasSkeletons(), "Resources", "Assimp scene has skeletons");

		// Log count of each scene component
		BGE_LOG("Resources", "Assimp # of animations: %d", pkScene->mNumAnimations);
		BGE_LOG("Resources", "Assimp # of cameras: %d", pkScene->mNumCameras);
		BGE_LOG("Resources", "Assimp # of lights: %d", pkScene->mNumLights);
		BGE_LOG("Resources", "Assimp # of materials: %d", pkScene->mNumMaterials);
		BGE_LOG("Resources", "Assimp # of meshes: %d", pkScene->mNumMeshes);
		BGE_LOG("Resources", "Assimp # of textures: %d", pkScene->mNumTextures);
		BGE_LOG("Resources", "Assimp # of skeletons: %d", pkScene->mNumSkeletons);

		for (unsigned int i = 0; i < pkScene->mNumMeshes; ++i)
		{
			aiMesh *pMesh = pkScene->mMeshes[i];

			// Create mesh data

			// Extract vertices
			for (unsigned int v = 0; v < pMesh->mNumVertices; ++v)
			{
				if (pMesh->HasPositions())
				{
					// pMesh->mVertices[v]
				}
				if (pMesh->HasNormals())
				{
					// pMesh->mNormals[v]
				}
				if (pMesh->HasTextureCoords(0))
				{
					// pMesh->mTextureCoords[0][v]
				}
			}

			// Extract indices
			for (unsigned int f = 0; f < pMesh->mNumFaces; ++f)
			{
				aiFace face = pMesh->mFaces[f];
				for (unsigned int idx = 0; idx < face.mNumIndices; ++idx)
				{
					// Emplace indice face.mIndices[idx]
				}
			}

			// Extract material name
			if (pMesh->mMaterialIndex >= 0)
			{
				aiMaterial *pMaterial = pkScene->mMaterials[pMesh->mMaterialIndex];
				aiString name;
				pMaterial->Get(AI_MATKEY_NAME, name);

				// Set the mesh material data
			}

			// Store the mesh data
		}

		Assimp::DefaultLogger::kill(); // Release logger instance

		//auto pExtraData = std::make_shared<AssimpResourceExtraData>();
		//pResourceHandle->SetExtraData(pExtraData);
		return false; // Return false if any error occurs
	}
} // End namespace (BGE)
