#include "jetspeed.h"
#include "../../Utilities/xorstr.h"

namespace Features
{
	void SetJetSpeed()
	{
		const auto pBorderInputNode = BorderInputNode::GetInstance();
		if (!pBorderInputNode) return;

		const auto GameContext = ClientGameContext::GetInstance();
		if (!GameContext) return;

		const auto Level = GameContext->m_pLevel;
		if (!Level) return;

		const auto GameWorld = Level->m_pGameWorld;
		if (!GameWorld) return;

		const auto PlayerManager = GameContext->m_pPlayerManager;
		if (!PlayerManager) return;

		const auto LocalPlayer = PlayerManager->m_pLocalPlayer;
		if (!LocalPlayer) return;

		const auto LocalSoldier = LocalPlayer->GetSoldier();
		if (!LocalSoldier) return;

		if (!LocalSoldier->IsAlive()) return;

		const auto LocalVehicle = LocalPlayer->GetVehicle();
		if (!LocalVehicle) return;

		const auto VehicleData = reinterpret_cast<VehicleEntityData*>(LocalVehicle->m_Data);
		if (!VehicleData) return;

		static const char* Jets[]
		{
			xorstr_("ID_P_VNAME_F35"),
			xorstr_("ID_P_VNAME_J20"),
			xorstr_("ID_P_VNAME_PAKFA"),
			xorstr_("ID_P_VNAME_A10"),
			xorstr_("ID_P_VNAME_SU39"),
			xorstr_("ID_P_VNAME_Q5")
		};

		static bool InJet = false;
		for (int i = 0; i < sizeof(Jets); i++)
		{
			if (VehicleData->m_NameSid == Jets[i])
			{
				InJet = true;
				break;
			}
		}

		if (InJet)
		{
			const auto pKeyboard = pBorderInputNode->m_pKeyboard;
			if (!pKeyboard) return;

			static bool LastFrameKeyPress = false;
			if (pKeyboard->m_pDevice->m_Buffer[InputDeviceKeys::IDK_ArrowUp] || pKeyboard->m_pDevice->m_Buffer[InputDeviceKeys::IDK_ArrowDown])
			{
				auto velVec = LocalSoldier->GetVelocity();
				auto velocity = sqrt(pow(velVec->x, 2) + pow(velVec->y, 2) + pow(velVec->z, 2)) * 3.6f;

				if (velocity > 315.0f)
				{
					pKeyboard->m_pDevice->m_Buffer[InputDeviceKeys::IDK_S] = 1;
					pKeyboard->m_pDevice->m_Buffer[InputDeviceKeys::IDK_LeftShift] = 0;
				}
				else if (velocity < 310.0f)
				{
					pKeyboard->m_pDevice->m_Buffer[InputDeviceKeys::IDK_S] = 0;
					pKeyboard->m_pDevice->m_Buffer[InputDeviceKeys::IDK_LeftShift] = 1;
				}

				LastFrameKeyPress = true;
			}
			else if (LastFrameKeyPress)
			{
				pKeyboard->m_pDevice->m_Buffer[InputDeviceKeys::IDK_S] = 0;
				pKeyboard->m_pDevice->m_Buffer[InputDeviceKeys::IDK_LeftShift] = 0;
				
				LastFrameKeyPress = false;
			}
		}
	}
}