#ifndef _BGE_HUMANVIEW_HPP_
#define _BGE_HUMANVIEW_HPP_

namespace BGE
{
	class HumanView : public IGameView
	{
		friend class EngineApp;
	protected:
		GameViewID m_viewID;
		ActorID m_actorID;

		//UniqueProcessManager m_pProcessManager;

	public:
		HumanView(void);
		virtual ~HumanView(void);
	};
} // End namespace (BGE)

#endif /* !_BGE_HUMANVIEW_HPP_ */