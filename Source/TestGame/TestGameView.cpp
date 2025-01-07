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
		/*if (pResourceHandle)
		{
			BGE_LOG("Resources", "Shaders/test_vert.glsl size: %d", pResourceHandle->GetSize());

			std::string shaderSource = pResourceHandle->GetExtraData()->VGetExtraData();
			//std::string buf(pResourceHandle->Buffer());
			BGE_LOG("Resources", "Shaders/test_vert.glsl: %s", shaderSource.c_str());
		}*/

		auto pShaderFactory = std::make_unique<BGE::GLShaderFactory>();

		auto pVSSourceHandle = resCache.GetHandle(BGE::Resource("Assets\\Shaders\\test_vert.glsl"));
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

		if (m_meshData.vertices.empty())
		{
			BGE_LOG("TestGame", "Oops!");
		}
		sizeof(glm::vec3) + sizeof(glm::vec4) + sizeof(glm::vec2);
		// Create VBO and upload data
		glCreateBuffers(1, &m_vbo);

		void *pFunc = SDL_GL_GetProcAddress("glCreateBuffers");
		if (pFunc != nullptr)
		{
			BGE_LOG("TestGame", "glCreateBuffers");
		}

		pFunc = SDL_GL_GetProcAddress("glIsBuffer");
		if (pFunc != nullptr)
		{
			BGE_LOG("TestGame", "glIsBuffer");
		}

		if (glIsBuffer(m_vbo) == GL_TRUE)
		{
			BGE_LOG("TestGame", "Is buffer!");
		}
		glNamedBufferStorage(m_vbo, m_meshData.vertices.size() * sizeof(BGE::Vertex), m_meshData.vertices.data(), 0);

		// Create EBO and upload data
		glCreateBuffers(1, &m_ebo);
		glNamedBufferStorage(m_ebo, m_meshData.indices.size() * sizeof(BGE::VertexIndex), m_meshData.indices.data(), 0);

		// Create VAO
		glCreateVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		//glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		// Enable and set up vertix attributes
		glVertexArrayVertexBuffer(m_vao, 0, m_vbo, 0, sizeof(BGE::Vertex));

		// Bind the EBO to the VAO
		glVertexArrayElementBuffer(m_vao, m_ebo);

		// Position attribute (layout location = 0)
		glEnableVertexArrayAttrib(m_vao, 0);
		glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(BGE::Vertex, position));
		glVertexArrayAttribBinding(m_vao, 0, 0);  // Attribute 0 uses binding index 0

		// Normal attribute (layout location = 1)
		glEnableVertexArrayAttrib(m_vao, 1);
		glVertexArrayAttribFormat(m_vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(BGE::Vertex, normal));
		glVertexArrayAttribBinding(m_vao, 1, 0);  // Attribute 1 uses binding index 0

		// Tangent attribute (layout location = 2)
		glEnableVertexArrayAttrib(m_vao, 2);
		glVertexArrayAttribFormat(m_vao, 2, 3, GL_FLOAT, GL_FALSE, offsetof(BGE::Vertex, tangent));
		glVertexArrayAttribBinding(m_vao, 2, 0);  // Attribute 2 uses binding index 0

		// Texcoord attribute (layout location = 3)
		glEnableVertexArrayAttrib(m_vao, 3);
		glVertexArrayAttribFormat(m_vao, 3, 2, GL_FLOAT, GL_FALSE, offsetof(BGE::Vertex, texcoord));
		glVertexArrayAttribBinding(m_vao, 3, 0);  // Attribute 3 uses binding index 0

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
