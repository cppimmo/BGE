#ifndef _TG_TESTGAME_HPP_
#define _TG_TESTGAME_HPP_

#include <Engine/Engine.hpp>
#include <Engine/BaseGameLogic.hpp>

class TestGameApp final : public BGE::EngineApp
{
public:
	virtual BGE::UniqueBaseGameLogicPtr VCreateGameAndView(void) override;
	virtual std::string VGetGameTitle(void) override;
	virtual std::string VGetGameAppDirectory(void) override;
	virtual std::string VGetIcon(void) override;
	virtual int VGetRendererImpl(void) override;
protected:
	virtual void VRegisterGameEvents(void) override;
	virtual void VCreateNetworkEventForwarder(void) override;
	virtual void VDestroyNetworkEventForwarder(void) override;
};

class TestGameLogic final : public BGE::BaseGameLogic
{
public:
	TestGameLogic(void);
	virtual ~TestGameLogic(void);
	// BaseGameLogic's interface:
	virtual void VChangeState(BGE::BaseGameState state) override;
private:
};

#endif /* !_TG_TESTGAME_HPP_ */
