#include "wur-main-radio-net-device-dummy-device.h"

#include "ns3/mac48-address.h"

namespace ns3 {
NS_LOG_COMPONENT_DEFINE("WurMainRadioNetDeviceDummyDevice");
bool WurMainRadioNetDeviceDummyDevice::IsBroadcast() const { return true; }

Address WurMainRadioNetDeviceDummyDevice::GetBroadcast() const {
        return Mac48Address::GetBroadcast();
}

bool WurMainRadioNetDeviceDummyDevice::IsMulticast() const { return false; }

Address WurMainRadioNetDeviceDummyDevice::GetMulticast(
    Ipv4Address multicastGroup) const {
        return Mac48Address::GetMulticast(multicastGroup);
}

bool WurMainRadioNetDeviceDummyDevice::IsPointToPoint() const { return false; }

}  // namespace ns3

