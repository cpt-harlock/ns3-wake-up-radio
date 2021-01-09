#include "wur-main-radio-net-device.h"
#include "wur-common-mac.h"
#include "ns3/node.h"
#include <bits/stdint-uintn.h>

#include "contrib/wake-up-radio/model/wur-main-radio-net-device-phy.h"

namespace ns3 {
Ptr<WurCommonMac> WurMainRadioNetDevice::GetMac() const { return m_mac; }

Ptr<WurMainRadioNetDevicePhy> WurMainRadioNetDevice::GetPhy() const {
        return m_phy;
}

uint32_t WurMainRadioNetDevice::GetIfIndex() const { return m_ifIndex; }

void WurMainRadioNetDevice::SetMac(const Ptr<WurCommonMac> mac) {
        m_mac = mac;
        CompleteConfig();
}

void WurMainRadioNetDevice::SetPhy(const Ptr<WurMainRadioNetDevicePhy> phy) {
        m_phy = phy;
        CompleteConfig();
}

void WurMainRadioNetDevice::SetIfIndex(const uint32_t index) {
        m_ifIndex = index;
}

Ptr<Channel> WurMainRadioNetDevice::GetChannel() const {
        return m_phy->GetChannel();
}

void WurMainRadioNetDevice::SetAddress(Address address) {
        m_mac->SetAddress(address);
}

Address WurMainRadioNetDevice::GetAddress() const {
        return m_mac->GetAddress();
}

uint16_t WurMainRadioNetDevice::GetMtu() const { return m_mtu; }

bool WurMainRadioNetDevice::SetMtu(const uint16_t mtu) {
        if (mtu > MAX_MSDU_SIZE) return false;
        m_mtu = mtu;
        return true;
}

bool WurMainRadioNetDevice::IsLinkUp() const {
        return m_phy != 0 && m_phy->IsAwake();
}
void WurMainRadioNetDevice::AddLinkChangeCallback(Callback<void> callback) {
        m_linkChanges.ConnectWithoutContext(callback);
}

Ptr<Node> WurMainRadioNetDevice::GetNode() const { return m_node; }

void WurMainRadioNetDevice::SetNode(const Ptr<Node> node) {
        m_node = node;
        CompleteConfig();
}

void WurMainRadioNetDevice::SetReceiveCallback(NetDevice::ReceiveCallback cb) {
        m_forwardUp = cb;
}

void
WurMainRadioNetDevice::SetPromiscReceiveCallback (PromiscReceiveCallback cb)
{
  m_promiscRx = cb;
  m_mac->SetPromisc();
}


}  // namespace ns3
