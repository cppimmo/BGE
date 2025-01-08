#include "TestGameStd.hpp"
#include "TestGameView.hpp"

#include "Audio/SoundResource.hpp"
#include "Resources/JSONResource.hpp"

#include <al.h>
#include <alc.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_clip_space.hpp>

namespace TestGame
{
	void TestGameView::TestCreate(GLuint &vao, GLuint &vbo, GLuint &ebo, BGE::MeshData &meshData)
	{
		void *funcPtr = SDL_GL_GetProcAddress("glClearColor");
		if (!funcPtr)
		{
			std::cerr << "Failed to load glClearColor!" << std::endl;
		}
		else
		{
			std::cout << "Successfully loaded glClearColor." << std::endl;
		}

		funcPtr = SDL_GL_GetProcAddress("glCreateBuffers");
		if (!funcPtr)
		{
			std::cerr << "Failed to load glCreateBuffers!" << std::endl;
		}
		else
		{
			std::cout << "Successfully loaded glCreateBuffers." << std::endl;
		}

		// Create VBO and upload data
		glCreateBuffers(1, &vbo);
		BGE_LOG("TestGame", "Test 3");
		glNamedBufferStorage(vbo, meshData.vertices.size() * sizeof(BGE::Vertex), meshData.vertices.data(), 0);
		BGE_LOG("TestGame", "Test 4");
		// Create EBO and upload data
		glCreateBuffers(1, &ebo);
		glNamedBufferStorage(ebo, meshData.indices.size() * sizeof(BGE::VertexIndex), meshData.indices.data(), 0);

		// Create VAO
		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);
		//glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		// Enable and set up vertix attributes
		glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(BGE::Vertex));

		// Bind the EBO to the VAO
		glVertexArrayElementBuffer(vao, ebo);

		// Position attribute (layout location = 0)
		glEnableVertexArrayAttrib(vao, 0);
		glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(BGE::Vertex, position));
		glVertexArrayAttribBinding(vao, 0, 0);  // Attribute 0 uses binding index 0

		// Normal attribute (layout location = 1)
		glEnableVertexArrayAttrib(vao, 1);
		glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(BGE::Vertex, normal));
		glVertexArrayAttribBinding(vao, 1, 0);  // Attribute 1 uses binding index 0

		// Tangent attribute (layout location = 2)
		glEnableVertexArrayAttrib(vao, 2);
		glVertexArrayAttribFormat(vao, 2, 3, GL_FLOAT, GL_FALSE, offsetof(BGE::Vertex, tangent));
		glVertexArrayAttribBinding(vao, 2, 0);  // Attribute 2 uses binding index 0

		// Texcoord attribute (layout location = 3)
		glEnableVertexArrayAttrib(vao, 3);
		glVertexArrayAttribFormat(vao, 3, 2, GL_FLOAT, GL_FALSE, offsetof(BGE::Vertex, texcoord));
		glVertexArrayAttribBinding(vao, 3, 0);  // Attribute 3 uses binding index 0
	}

	TestGameView::TestGameView(void)
	{
	}

	bool TestGameView::VInit(void)
	{
		if (!HumanView::VInit()) // Call parent
		{
			return false;
		}

		BGE::ProjectionDesc projDesc;
		projDesc.fieldOfView = 75.0f;
		projDesc.aspectRatio = 1280.0f / 720.0f;
		projDesc.nearClip = 0.01f;
		projDesc.farClip = 1'000.0f;
		m_pCamera = std::make_shared<BGE::FirstPersonCamera>(projDesc);
		m_pCamera->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));

		m_triTransform.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

		m_pController = std::make_shared<TestController>(m_pCamera);
		AddGamepadHandler(m_pController);
		AddKeyboardHandler(m_pController);
		AddMouseHandler(m_pController);

		// NOTE: Most of this code is for testing purposes currently.

		m_pShaderProgram = std::make_unique<BGE::GLShaderProgram>();
		m_pShaderProgram->VCreate();

		auto &app = BGE::GetEngineApp();
		auto &resCache = app.GetResourceCache();
		//resCache.Preload(".glsl", [](int, bool &) {});

		auto pShaderFactory = std::make_unique<BGE::GLShaderFactory>();

		auto pVSSourceHandle = resCache.GetHandle(BGE::Resource("Assets\\Shaders\\test_vert.glsl"));
		if (!pVSSourceHandle)
		{
			BGE_ERROR("Couldn't locate vertex shader");
			return false;
		}
		BGE::StrongIShaderPtr pVertexShader = pShaderFactory->VCreateVertexShader();
		pVertexShader->VCreate();
		pVertexShader->VCompile(pVSSourceHandle);

		auto pFSSourceHandle = resCache.GetHandle(BGE::Resource("Assets\\Shaders\\test_frag.glsl"));
		BGE::StrongIShaderPtr pFragmentShader = pShaderFactory->VCreateFragmentShader();
		pFragmentShader->VCreate();
		pFragmentShader->VCompile(pFSSourceHandle);

		m_pShaderProgram->VAttachShader(pVertexShader);
		m_pShaderProgram->VAttachShader(pFragmentShader);
		m_pShaderProgram->VLink();
		// Individual shaders can be destroyed now
		pVertexShader->VDestroy();
		pFragmentShader->VDestroy();

		m_pSkyboxProgram = std::make_unique<BGE::GLShaderProgram>();
		m_pSkyboxProgram->VCreate();

		auto pSkyboxVSSourceHandle = resCache.GetHandle(BGE::Resource("Assets\\Shaders\\skybox_vert.glsl"));
		if (!pSkyboxVSSourceHandle)
		{
			BGE_ERROR("Couldn't locate vertex shader");
			return false;
		}
		BGE::StrongIShaderPtr pSkyboxVertexShader = pShaderFactory->VCreateVertexShader();
		pSkyboxVertexShader->VCreate();
		pSkyboxVertexShader->VCompile(pSkyboxVSSourceHandle);

		auto pSkyboxFSSourceHandle = resCache.GetHandle(BGE::Resource("Assets\\Shaders\\skybox_frag.glsl"));
		BGE::StrongIShaderPtr pSkyboxFragmentShader = pShaderFactory->VCreateFragmentShader();
		pSkyboxFragmentShader->VCreate();
		pSkyboxFragmentShader->VCompile(pSkyboxFSSourceHandle);

		m_pSkyboxProgram->VAttachShader(pSkyboxVertexShader);
		m_pSkyboxProgram->VAttachShader(pSkyboxFragmentShader);
		m_pSkyboxProgram->VLink();
		// Individual shaders can be destroyed now
		pSkyboxVertexShader->VDestroy();
		pSkyboxFragmentShader->VDestroy();

		//glm::fvec3 vertex(0.0f, 0.0f, 0.0f);
		/*static constexpr BGE::Vertex vertices[3] =
		{
			BGE::Vertex({ -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }),
			BGE::Vertex({  0.0f,  0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }),
			BGE::Vertex({  0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f })
		};*/
		BGE::GeometryGenerator generator;
		//generator.CreateBox(1.0f, 1.0f, 1.0f, m_meshData);
		generator.CreateSphere(1.0f, 16, 16, m_meshData);
		BGE_LOG("TestGame", "Test 1");
		TestCreate(m_vao, m_vbo, m_ebo, m_meshData);
		BGE_LOG("TestGame", "Test 2");
		generator.CreateBox(1.0f, 1.0f, 1.0f, m_skyboxMeshData);
		TestCreate(m_skyboxVao, m_skyboxVbo, m_skyboxEbo, m_skyboxMeshData);

		// Audio system test
		auto &audio = app.GetAudioSystem();
		auto pBuffer = audio.VCreateBuffer();
		auto pSoundHandle = resCache.GetHandle(BGE::Resource("Assets\\Music\\calmbgm.ogg"));
		if (!pBuffer->VLoadFromResource(pSoundHandle))
		{
			return false;
		}

		BGE_LOG("TestGame", "Buffer frequency (Hz): %d", pBuffer->VGetFrequency());
		BGE_LOG("TestGame", "Buffer bit depth: %d", pBuffer->VGetBitDepth());
		BGE_LOG("TestGame", "Buffer channels: %d", pBuffer->VGetChannels());
		BGE_LOG("TestGame", "Buffer size (MiB): %u", static_cast<decltype(BGE::kONE_MEGABYTE)>(pBuffer->VGetSize()) / BGE::kONE_MEGABYTE);

		m_pSource = audio.VCreateSource();
		m_pSource->VAttachBuffer(pBuffer);

		auto pListener = audio.VCreateListener();
		pListener->VSetPosition(glm::vec3(0, 0, 0));
		pListener->VSetVelocity(glm::vec3(0, 0, 0));
		audio.VSetListener(pListener);

		auto pJSON = resCache.GetHandle(BGE::Resource("Assets\\test.json"));
		if (pJSON)
		{
			auto pExtraData = std::dynamic_pointer_cast<BGE::JSONResourceExtraData>(pJSON->GetExtraData());

			const auto &json = pExtraData->GetJSON();
			for (const auto &element : json)
			{
				std::cout << element << '\n';
			}
		}

		return true;
	}

	void TestGameView::VOnRender(float deltaTime, float elapsedTime)
	{
		HumanView::VOnRender(deltaTime, elapsedTime); // Call parent

		int width, height;
		BGE::BGUTGetWindowSize(BGE::BGUTGetWindowPtr(), width, height);
		glm::mat4 projection = glm::perspective(1.0f, static_cast<float>(width) / static_cast<float>(height), 0.005f, 50.0f);
		//glm::mat4 view =

		m_pShaderProgram->VBind();

		m_pShaderProgram->VSetMat4("uModel", m_triTransform);
		m_pShaderProgram->VSetMat4("uView", m_pCamera->GetViewMatrix());
		m_pShaderProgram->VSetMat4("uProjection", m_pCamera->GetProjectionMatrix());

		//static bool c_bInitialized = false;
		//if (!c_bInitialized)
		//{
		//	m_pSource->VSetVolume(1.0f);
		//	m_pSource->VPlay();
		//	c_bInitialized = true;
		//}
		m_pSource->VSetLooping(true);
		m_pSource->VPlay();

		auto &app = BGE::GetEngineApp();
		auto position = m_pSource->VGetPosition();
		float radius = 10.0f;
		float angularSpeed = glm::pi<float>() / 4.0f;
		BGE::Timer::Seconds time = app.GetTimer().GetElapsedSecs();
		position.x = radius * std::cos(angularSpeed * time);
		position.y = 0.0f;
		position.z = radius * std::sin(angularSpeed * time);
		m_pSource->VSetPosition(position);

		ImGui::Begin("Test");
		ImGui::Text("Audio source progress: %1.2f", m_pSource->VGetProgress());
		ImGui::Text("Right axis: (%08.7f, %08.7f)", m_pController->GetRightAxis().x, m_pController->GetRightAxis().y);
		ImGui::End();

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// Bind the VAO
		glBindVertexArray(m_vao);
		// Issue the draw call
		//glDrawArrays(GL_TRIANGLES, 0, 24);
		glDrawElements(GL_TRIANGLES, m_meshData.indices.size(), GL_UNSIGNED_INT, nullptr);
	}

	void TestGameView::VOnUpdate(float deltaTime)
	{
		HumanView::VOnUpdate(deltaTime); // Call parent

		m_pController->OnUpdate(deltaTime);
	}

	void TestGameView::VRegisterDelegates(void)
	{
	}

	void TestGameView::VDeregisterDelegates(void)
	{
	}
} // End namespace (TestGame)
