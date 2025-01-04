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
	bool TestGameView::VInit(void)
	{
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

		BGE::ProjectionDesc projDesc;
		projDesc.fieldOfView = glm::pi<float>() / 4.0f;
		projDesc.aspectRatio = 1280.0f / 720.0f;
		projDesc.nearClip = 0.01f;
		projDesc.farClip = 1'000.0f;
		m_pCamera = std::make_unique<BGE::Camera>(BGE::CameraType::kPerspective, projDesc);
		m_pCamera->SetPosition(glm::vec3(0.0f, 0.0f, 100.0f));

		m_triTransform.SetPosition(glm::vec3(0.0f, 0.0f, -10.0f));

		//glm::fvec3 vertex(0.0f, 0.0f, 0.0f);
		static constexpr GLfloat vertices[3][3 + 3] =
		{
			{ -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f },
			{  0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f },
			{  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f }
		};

		glCreateBuffers(1, &m_vbo);
		glNamedBufferStorage(m_vbo, sizeof(vertices), vertices, 0);

		glCreateVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

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
		HumanView::VOnRender(deltaTime, elapsedTime);

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
		ImGui::End();

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void TestGameView::VRegisterDelegates(void)
	{
	}

	void TestGameView::VDeregisterDelegates(void)
	{
	}
} // End namespace (TestGame)
