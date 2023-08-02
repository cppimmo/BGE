#ifndef _TB_TANKBATTLES_HPP_
#define _TB_TANKBATTLES_HPP_

#include <Engine/Engine.hpp>
#include <Engine/BaseGameLogic.hpp>

class TankBattlesApp final : public BGE::EngineApp
{
public:
	virtual BGE::BaseGameLogic *VCreateGameAndView(void) override;
	virtual std::string VGetGameTitle(void) override;
	virtual std::string VGetGameAppDirectory(void) override;
	virtual std::string VGetIcon(void) override;
	virtual int VGetRendererImpl(void) override;
protected:
	virtual void VRegisterGameEvents(void) override;
	virtual void VCreateNetworkEventForwarder(void) override;
	virtual void VDestroyNetworkEventForwarder(void) override;
};

#endif /* !_TB_TANKBATTLES_HPP_ */