#include "wur-common-phy.h"
#include "ns3/log.h"
#include "wur-common-ppdu.h"
#include "wur-common-channel.h"
#include "wur-common-net-device.h"
#include "ns3/simulator.h"
namespace ns3 {
NS_LOG_COMPONENT_DEFINE("WurCommonPhy");
Ptr<WurCommonChannel> WurCommonPhy::GetChannel() const { return m_channel; }
Ptr<MobilityModel> WurCommonPhy::GetMobility() const { return m_mobility; }
Ptr<WurCommonNetDevice> WurCommonPhy::GetDevice() const { return m_netdevice; }
void WurCommonPhy::StartReceivePreamble(Ptr<WurCommonPpdu> ppdu,
					double rxPowerDbm) {
	NS_LOG_FUNCTION(this << rxPowerDbm);
	if (ppdu->IsTruncatedTx()) {
		NS_LOG_DEBUG(
		    "Packet reception stopped because transmitter has been "
		    "switched off");
		return;
	}
	switch (GetState()) {
		case WurCommonPhyState::RX:
			NS_LOG_DEBUG("Drop packet because already in Rx (power="
				     << rxPowerDbm << "dBm)");
			NotifyRxDrop(ppdu, "Already receiving packet");
			break;
		case WurCommonPhyState::TX:
			NS_LOG_DEBUG("Drop packet because already in Tx (power="
				     << rxPowerDbm << "dBm)");
			NotifyRxDrop(ppdu, "Already in Tx");
			break;
		case WurCommonPhyState::IDLE:
			NS_LOG_INFO("start rx");
			m_state = WurCommonPhyState::RX;
			SetRxPacket(ppdu);
			Simulator::Schedule(m_preambleDuration, &WurCommonPhy::StartRx, this, ppdu, rxPowerDbm);
			break;
		case WurCommonPhyState::OFF:
			NS_LOG_DEBUG("Drop packet because in sleep mode");
			NotifyRxDrop(ppdu, "Sleeping");
			break;
		default:
			NS_FATAL_ERROR("Invalid WurCommonPhy state.");
			break;
	}
}

void WurCommonPhy::TurnOn() {
	if(m_state == WurCommonPhyState::OFF) {
		m_state = WurCommonPhyState::IDLE;
	}
}

void WurCommonPhy::TurnOff() {
	if(m_state != WurCommonPhyState::OFF) {
		m_state = WurCommonPhyState::OFF;
		if(m_rxPacket != nullptr)
			m_rxPacket->SetTruncatedRx();
		if(m_txPacket != nullptr)
			m_txPacket->SetTruncatedTx();
	}
		
}
	
}  // namespace ns3
