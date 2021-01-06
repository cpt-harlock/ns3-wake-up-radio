#include "contrib/wake-up-radio/model/wur-main-radio-net-device-phy.h"

#include "ns3/mobility-model.h"
#include "ns3/net-device.h"
namespace ns3 {
Ptr<MobilityModel> WurMainRadioNetDevicePhy::GetMobility() const {
        return m_mobility;
}

Ptr<NetDevice> WurMainRadioNetDevicePhy::GetDevice() const {
        return m_netdevice;
}

bool WurMainRadioNetDevicePhy::IsAwake() const {
        return m_statehelper->IsAwake();
}
}  // namespace ns3

