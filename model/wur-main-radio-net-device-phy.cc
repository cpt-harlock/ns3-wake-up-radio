#include "contrib/wake-up-radio/model/wur-main-radio-net-device-phy.h"

#include "contrib/wake-up-radio/model/wur-main-radio-net-device-channel.h"
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

void WurMainRadioNetDevicePhy::SetChannel(Ptr<WurMainRadioNetDeviceChannel> channel) {
        m_channel = channel;
}
bool WurMainRadioNetDevicePhy::IsAwake() const {
        return m_statehelper->IsAwake();
}

void WurMainRadioNetDevicePhy::SetMobility(Ptr<MobilityModel> mobility) {
        m_mobility = mobility;
}

void WurMainRadioNetDevicePhy::SetDevice(Ptr<NetDevice> device) {
        m_netdevice = device;
}

void WurMainRadioNetDevicePhy::TurnOn() {
        m_statehelper->WakeUp();
}

void WurMainRadioNetDevicePhy::TurnOff() {
        m_statehelper->GoToSleep();
}

}  // namespace ns3

