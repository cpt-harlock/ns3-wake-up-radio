#include "wur-main-radio-net-device-phy.h"

#include <string>

#include "ns3/assert.h"
#include "ns3/double.h"
#include "ns3/log-macros-enabled.h"
#include "ns3/mobility-model.h"
#include "ns3/net-device.h"
#include "ns3/simulator.h"
#include "wur-common-mac.h"
#include "wur-main-radio-net-device-channel.h"
#include "wur-main-radio-net-device-phy-state-helper.h"
#include "wur-main-radio-net-device.h"
#include "wur-main-radio-ppdu.h"
namespace ns3 {
NS_LOG_COMPONENT_DEFINE("WurMainRadioNetDevicePhy");
Ptr<MobilityModel> WurMainRadioNetDevicePhy::GetMobility() const {
	return m_mobility;
}

Ptr<NetDevice> WurMainRadioNetDevicePhy::GetDevice() const {
	return m_netdevice;
}

Ptr<WurMainRadioNetDeviceChannel> WurMainRadioNetDevicePhy::GetChannel() const {
	return m_channel;
}

void WurMainRadioNetDevicePhy::SetChannel(
    Ptr<WurMainRadioNetDeviceChannel> channel) {
	m_channel = channel;
}
bool WurMainRadioNetDevicePhy::IsAwake() const {
	return m_stateHelper->IsAwake();
}

void WurMainRadioNetDevicePhy::SetMobility(Ptr<MobilityModel> mobility) {
	m_mobility = mobility;
}

void WurMainRadioNetDevicePhy::SetDevice(Ptr<WurMainRadioNetDevice> device) {
	m_netdevice = device;
}

void WurMainRadioNetDevicePhy::NotifyTxBegin(Ptr<const WurMainRadioPpdu> psdu,
					     double txPowerW) {
	// m_phyTxBeginTrace (psdu, txPowerW);
}

void WurMainRadioNetDevicePhy::SetTxOkCallback(TxOkCallback txOkCallback) {
	m_txOkCallback = txOkCallback;
}

void WurMainRadioNetDevicePhy::NotifyTxEnd(Ptr<const WurMainRadioPpdu> ppdu) {
	NS_ASSERT(ppdu != nullptr);
	NS_ASSERT(ppdu->GetPsdu() != nullptr);
	NS_ASSERT(ppdu->GetPsdu()->GetPacket() != nullptr);
	m_txOkCallback(ppdu->GetPsdu()->GetPacket());
}

void WurMainRadioNetDevicePhy::NotifyTxDrop(Ptr<const WurMainRadioPpdu> psdu) {
	// for (auto& mpdu : *PeekPointer (psdu))
	//  {
	//    m_phyTxDropTrace (mpdu->GetProtocolDataUnit ());
	//  }
}

void WurMainRadioNetDevicePhy::NotifyRxBegin(Ptr<const WurMainRadioPpdu> psdu) {
	// for (auto& mpdu : *PeekPointer (psdu))
	//  {
	//    m_phyRxBeginTrace (mpdu->GetProtocolDataUnit ());
	//  }
}

void WurMainRadioNetDevicePhy::NotifyRxEnd(Ptr<const WurMainRadioPpdu> psdu) {
	// for (auto& mpdu : *PeekPointer (psdu))
	//  {
	// m_phyRxEndTrace (psdu);
	//  }
}

void WurMainRadioNetDevicePhy::NotifyRxDrop(Ptr<const WurMainRadioPpdu> psdu,
					    std::string reason) {
	// for (auto& mpdu : *PeekPointer (psdu))
	// {
	// m_phyRxDropTrace (psdu, reason);
	//}
}

void WurMainRadioNetDevicePhy::SetReceiveOkCallback(RxOkCallback callback) {
	m_rxOkCallback = callback;
}

WurMainRadioNetDevicePhyStateHelper::MainRadioState_t
WurMainRadioNetDevicePhy::GetState() {
	return m_stateHelper->GetState();
}

void WurMainRadioNetDevicePhy::SetState(
    WurMainRadioNetDevicePhyStateHelper::MainRadioState_t state) {
	m_stateHelper->phyState = state;
}

TypeId WurMainRadioNetDevicePhy::GetTypeId(void) {
	static TypeId tid =
	    TypeId("ns3::WurMainRadioNetDevicePhy")
		.SetParent<Object>()
		.SetGroupName("Wur")
		.AddAttribute(
		    "RxSensitivity",
		    "The energy of a received signal should be higher than "
		    "this threshold (dBm) for the PHY to detect the signal.",
		    DoubleValue(-100.0),
		    MakeDoubleAccessor(
			&WurMainRadioNetDevicePhy::SetRxSensitivity,
			&WurMainRadioNetDevicePhy::GetRxSensitivity),
		    MakeDoubleChecker<double>())
		.AddAttribute(
		    "TxGain", "Transmission gain (dB).", DoubleValue(0.0),
		    MakeDoubleAccessor(&WurMainRadioNetDevicePhy::SetTxGain,
				       &WurMainRadioNetDevicePhy::GetTxGain),
		    MakeDoubleChecker<double>())
		.AddAttribute(
		    "RxGain", "Reception gain (dB).", DoubleValue(0.0),
		    MakeDoubleAccessor(&WurMainRadioNetDevicePhy::SetRxGain,
				       &WurMainRadioNetDevicePhy::GetRxGain),
		    MakeDoubleChecker<double>());
	return tid;
}

void WurMainRadioNetDevicePhy::EndTx(Ptr<WurMainRadioPpdu> ppdu) {
	// change state to idle
	NS_LOG_FUNCTION_NOARGS();
	SetState(WurMainRadioNetDevicePhyStateHelper::IDLE);
	NotifyTxEnd(ppdu);
}
}  // namespace ns3

