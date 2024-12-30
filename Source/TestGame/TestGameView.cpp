#include "TestGameStd.hpp"
#include "TestGameView.hpp"

#include "Audio/SoundResource.hpp"

#include <al.h>
#include <alc.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_clip_space.hpp>

namespace TestGame
{
	bool TestGameView::VInit(void)
	{
		m_pShaderProgram = std::make_unique<BGE::ShaderProgram>();
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

		auto pVSSourceHandle = resCache.GetHandle(BGE::Resource("Assets\\Shaders\\test_vert.glsl"));
		BGE::StrongIShaderPtr pVertexShader = std::make_shared<BGE::VertexShader>();
		pVertexShader->VCreate();
		pVertexShader->VCompile(pVSSourceHandle);

		auto pFSSourceHandle = resCache.GetHandle(BGE::Resource("Assets\\Shaders\\test_frag.glsl"));
		BGE::StrongIShaderPtr pFragmentShader = std::make_shared<BGE::FragmentShader>();
		pFragmentShader->VCreate();
		pFragmentShader->VCompile(pFSSourceHandle);

		m_pShaderProgram->VAttachShader(pVertexShader);
		m_pShaderProgram->VAttachShader(pFragmentShader);
		m_pShaderProgram->VLink();
		// Individual shaders can be destroyed now
		pVertexShader->VDestroy();
		pFragmentShader->VDestroy();

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

		/*ALCdevice *pDevice = alcOpenDevice(nullptr);
		if (!pDevice)
		{
			BGE_ERROR("Error creating ALCdevice!");
			return false;
		}

		ALCcontext *pContext = alcCreateContext(pDevice, nullptr);
		if (!pContext)
		{
			BGE_ERROR("Error creating ALCcontext!");
			return false;
		}

		ALCboolean bContextMadeCurrent = false;
		bContextMadeCurrent = alcMakeContextCurrent(pContext);
		if (bContextMadeCurrent != ALC_TRUE)
		{
			BGE_ERROR("Couldn't set current  ALCcontext!");
			return false;
		}

		auto pSoundHandle = resCache.GetHandle(BGE::Resource("Assets\\Sounds\\coin1.wav"));
		auto pSoundExtraData = std::dynamic_pointer_cast<BGE::SoundExtraData>(pSoundHandle->GetExtraData());
		const BGE::SoundExtraData::SoundData &kSoundData = pSoundExtraData->GetSoundData();

		ALuint buffer;
		alGenBuffers(1, &buffer);

		ALenum format;
		bool bSingleChannel = kSoundData.channels == 1;
		bool bDualChannel = kSoundData.channels == 2;
		if (bSingleChannel && kSoundData.bitsPerSample == 8)
		{
			format = AL_FORMAT_MONO8;
		}
		else if (bSingleChannel && kSoundData.bitsPerSample == 16)
		{
			format = AL_FORMAT_MONO16;
		}
		else if (bDualChannel && kSoundData.bitsPerSample == 8)
		{
			format = AL_FORMAT_STEREO8;
		}
		else if (bDualChannel && kSoundData.bitsPerSample == 16)
		{
			format = AL_FORMAT_STEREO16;
		}
		else
		{
			BGE_ERROR("Unsupported channels/bitrate!");
			return false;
		}

		alBufferData(buffer, format, kSoundData.soundData.data(), kSoundData.soundData.size(), kSoundData.sampleRate);

		ALuint source;
		alGenSources(1, &source);
		alSourcef(source, AL_PITCH, 1);
		alSourcef(source, AL_GAIN, 1.0f);
		alSource3f(source, AL_POSITION, 0, 0, 0);
		alSource3f(source, AL_VELOCITY, 0, 0, 0);
		alSourcei(source, AL_LOOPING, AL_FALSE);
		alSourcei(source, AL_BUFFER, buffer);

		alSourcePlay(source);

		ALint state = AL_PLAYING;
		while (state == AL_PLAYING)
		{
			alGetSourcei(source, AL_SOURCE_STATE, &state);
		}

		alDeleteSources(1, &source);
		alDeleteBuffers(1, &buffer);

		alcMakeContextCurrent(nullptr);
		alcDestroyContext(pContext);

		ALCboolean bClosed;
		bClosed = alcCloseDevice(pDevice);*/
		auto &audio = app.GetAudioSystem();
		auto pBuffer = audio.VCreateBuffer();
		auto pSoundHandle = resCache.GetHandle(BGE::Resource("Assets\\Sounds\\coin1.wav"));
		if (!pBuffer->VLoadFromResource(pSoundHandle))
		{
			return false;
		}

		m_pSource = audio.VCreateSource();
		m_pSource->VAttachBuffer(pBuffer);

		auto pListener = audio.VCreateListener();
		pListener->VSetPosition(glm::vec3(0, 0, 0));
		pListener->VSetVelocity(glm::vec3(0, 0, 0));
		audio.VSetListener(pListener);
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

		//static bool c_bInitialized = false;
		//if (!c_bInitialized)
		//{
		//	m_pSource->VSetVolume(1.0f);
		//	m_pSource->VPlay();
		//	c_bInitialized = true;
		//}
		m_pSource->VSetLooping(true);
		m_pSource->VPlay();

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
