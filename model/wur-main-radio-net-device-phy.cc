#include "contrib/wake-up-radio/model/wur-main-radio-net-device-phy.h"

#include <string>

#include "contrib/wake-up-radio/model/wur-main-radio-net-device-channel.h"
#include "contrib/wake-up-radio/model/wur-main-radio-net-device-phy-state-helper.h"
#include "contrib/wake-up-radio/model/wur-main-radio-ppdu.h"
#include "ns3/double.h"
#include "ns3/mobility-model.h"
#include "ns3/net-device.h"
namespace ns3 {
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

void WurMainRadioNetDevicePhy::SetDevice(Ptr<NetDevice> device) {
        m_netdevice = device;
}

void WurMainRadioNetDevicePhy::NotifyTxBegin(Ptr<const WurMainRadioPpdu> psdu,
                                             double txPowerW) {
        // m_phyTxBeginTrace (psdu, txPowerW);
}

void WurMainRadioNetDevicePhy::NotifyTxEnd(Ptr<const WurMainRadioPpdu> psdu) {
        // m_phyTxEndTrace (mpdu->GetProtocolDataUnit ());
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
                    DoubleValue(-101.0),
                    MakeDoubleAccessor(&WurMainRadioNetDevicePhy::SetRxSensitivity,
                                       &WurMainRadioNetDevicePhy::GetRxSensitivity),
                    MakeDoubleChecker<double>())
                .AddAttribute("TxGain", "Transmission gain (dB).",
                              DoubleValue(0.0),
                              MakeDoubleAccessor(&WurMainRadioNetDevicePhy::SetTxGain,
                                                 &WurMainRadioNetDevicePhy::GetTxGain),
                              MakeDoubleChecker<double>())
                .AddAttribute("RxGain", "Reception gain (dB).",
                              DoubleValue(0.0),
                              MakeDoubleAccessor(&WurMainRadioNetDevicePhy::SetRxGain,
                                                 &WurMainRadioNetDevicePhy::GetRxGain),
                              MakeDoubleChecker<double>());
        return tid;
}
}  // namespace ns3

