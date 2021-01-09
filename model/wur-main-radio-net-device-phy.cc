#include "contrib/wake-up-radio/model/wur-main-radio-net-device-phy.h"

#include "contrib/wake-up-radio/model/wur-main-radio-net-device-channel.h"
#include "contrib/wake-up-radio/model/wur-main-radio-ppdu.h"
#include "ns3/mobility-model.h"
#include "ns3/net-device.h"
#include <string>
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

void WurMainRadioNetDevicePhy::SetChannel(Ptr<WurMainRadioNetDeviceChannel> channel) {
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

void WurMainRadioNetDevicePhy::TurnOn() {
        m_stateHelper->WakeUp();
}

void WurMainRadioNetDevicePhy::TurnOff() {
        m_stateHelper->GoToSleep();
}

void
WurMainRadioNetDevicePhy::NotifyTxBegin (Ptr<const WurMainRadioPpdu> psdu, double txPowerW)
{
  for (auto& mpdu : *PeekPointer (psdu))
    {
      m_phyTxBeginTrace (mpdu->GetProtocolDataUnit (), txPowerW);
    }
}

void
WurMainRadioNetDevicePhy::NotifyTxEnd (Ptr<const WurMainRadioPpdu> psdu)
{
  for (auto& mpdu : *PeekPointer (psdu))
    {
      m_phyTxEndTrace (mpdu->GetProtocolDataUnit ());
    }
}

void
WurMainRadioNetDevicePhy::NotifyTxDrop (Ptr<const WurMainRadioPpdu> psdu)
{
  for (auto& mpdu : *PeekPointer (psdu))
    {
      m_phyTxDropTrace (mpdu->GetProtocolDataUnit ());
    }
}

void
WurMainRadioNetDevicePhy::NotifyRxBegin (Ptr<const WurMainRadioPpdu> psdu)
{
  for (auto& mpdu : *PeekPointer (psdu))
    {
      m_phyRxBeginTrace (mpdu->GetProtocolDataUnit ());
    }
}

void
WurMainRadioNetDevicePhy::NotifyRxEnd (Ptr<const WurMainRadioPpdu> psdu)
{
  for (auto& mpdu : *PeekPointer (psdu))
    {
      m_phyRxEndTrace (mpdu->GetProtocolDataUnit ());
    }
}

void
WurMainRadioNetDevicePhy::NotifyRxDrop (Ptr<const WurMainRadioPpdu> psdu,std::string reason)
{
  for (auto& mpdu : *PeekPointer (psdu))
    {
      m_phyRxDropTrace (mpdu->GetProtocolDataUnit (), reason);
    }
}
}  // namespace ns3

