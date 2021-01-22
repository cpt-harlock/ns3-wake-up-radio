#include "wur-common-phy-dummy-impl.h"
#include "ns3/nstime.h"
#include "ns3/simulator.h"
#include "wur-common-phy.h"
#include "wur-common-ppdu.h"

namespace ns3 {
	void WurCommonPhyDummyImpl::NotifyRxBegin(Ptr<const WurCommonPpdu>) {
	}
	void WurCommonPhyDummyImpl::NotifyRxDrop(Ptr<const WurCommonPpdu>, std::string) {
	}
	void WurCommonPhyDummyImpl::NotifyRxEnd(Ptr<const WurCommonPpdu>) {}
	void WurCommonPhyDummyImpl::NotifyTxBegin(Ptr<const WurCommonPpdu>, double) {}
	void WurCommonPhyDummyImpl::NotifyTxDrop(Ptr<const WurCommonPpdu>) {}
	void WurCommonPhyDummyImpl::NotifyTxEnd(Ptr<const WurCommonPpdu>) {}
	void WurCommonPhyDummyImpl::StartRx(Ptr<WurCommonPpdu> ppdu, double rxPowerDbm) {
		//just schedule EndRx
		Time delay = Seconds((GetRxPacket()->GetPsdu()->GetPayload()->GetSerializedSize()) / m_dataRate );
		Simulator::Schedule(delay, &WurCommonPhyDummyImpl::EndRx, this, ppdu);
	}
	void WurCommonPhyDummyImpl::EndRx(Ptr<WurCommonPpdu> ppdu) {
		UnsetRxPacket();
		m_state = WurCommonPhyState::IDLE;
		m_rxOkCallback(ppdu->GetPsdu()->GetPayload());
	}
	void WurCommonPhyDummyImpl::StartTx(Ptr<WurCommonPsdu> psdu) {
		//assuming upper layer checked if idle
		Ptr<WurCommonPpdu> ppdu = Create<WurCommonPpdu>();
		ppdu->SetPsdu(psdu);
		SetTxPacket(ppdu);
		m_state = WurCommonPhyState::TX;
		Time delay = Seconds((GetTxPacket()->GetPsdu()->GetPayload()->GetSerializedSize()) / m_dataRate );
		Simulator::Schedule(delay + m_preambleDuration, &WurCommonPhyDummyImpl::EndTx, this, ppdu);

	}
	void WurCommonPhyDummyImpl::EndTx(Ptr<WurCommonPpdu> ppdu) {
		UnsetTxPacket();
		m_state = WurCommonPhyState::IDLE;
		m_txOkCallback(ppdu->GetPsdu()->GetPayload());
	}
		
}
