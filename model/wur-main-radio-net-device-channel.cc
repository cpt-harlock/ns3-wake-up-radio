#include "wur-main-radio-net-device-channel.h"

#include <bits/c++config.h>

#include <algorithm>

#include "contrib/wake-up-radio/model/wur-main-radio-net-device-phy.h"
#include "contrib/wake-up-radio/model/wur-main-radio-ppdu.h"
#include "ns3/log-macros-enabled.h"
#include "ns3/log.h"
#include "ns3/mobility-model.h"
#include "ns3/net-device.h"
#include "ns3/node.h"
#include "ns3/pointer.h"
#include "ns3/simulator.h"
#include "src/core/model/simulator.h"
#include "src/wifi/model/wifi-utils.h"
#include "wur-main-radio-net-device.h"
#include "wur-common-mac.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("WurMainRadioNetDeviceChannel");

void WurMainRadioNetDeviceChannel::Add(Ptr<WurMainRadioNetDevicePhy> phy) {
        if (std::find(m_phyList.begin(), m_phyList.end(), phy) ==
            m_phyList.end())
                m_phyList.push_back(phy);
}

Ptr<NetDevice> WurMainRadioNetDeviceChannel::GetDevice(std::size_t i) const {
        return m_phyList[i]->GetDevice()->GetObject<NetDevice>();
}

std::size_t WurMainRadioNetDeviceChannel::GetNDevices() const {
        return m_phyList.size();
}
TypeId WurMainRadioNetDeviceChannel::GetTypeId(void) {
        static TypeId tid =
            TypeId("ns3::WurMainRadioNetDeviceChannel")
                .SetParent<Channel>()
                .SetGroupName("WurMainRadio")
                .AddConstructor<WurMainRadioNetDeviceChannel>()
                .AddAttribute("PropagationLossModel",
                              "A pointer to the propagation loss model "
                              "attached to this channel.",
                              PointerValue(),
                              MakePointerAccessor(&WurMainRadioNetDeviceChannel::m_loss),
                              MakePointerChecker<PropagationLossModel>())
                .AddAttribute("PropagationDelayModel",
                              "A pointer to the propagation delay model "
                              "attached to this channel.",
                              PointerValue(),
                              MakePointerAccessor(&WurMainRadioNetDeviceChannel::m_delay),
                              MakePointerChecker<PropagationDelayModel>());
        return tid;
}

void WurMainRadioNetDeviceChannel::Receive(Ptr<WurMainRadioNetDevicePhy> phy,
                                           Ptr<WurMainRadioPpdu> ppdu,
                                           double rxPowerDbm) {
        NS_LOG_FUNCTION(phy << ppdu << rxPowerDbm);
        // I just need to check if the phy is awake.
        // If this is the case, I invoke the processing function
        // Otherwise drop the packet
        if (!phy->IsAwake()) {
                NS_LOG_INFO("Phy asleep, can't receive message");
                return;
        }
        if ((rxPowerDbm + phy->GetRxGain()) < phy->GetRxSensitivity()) {
                NS_LOG_INFO("Received signal too weak to process: "
                            << rxPowerDbm << " dBm");
                return;
        }
        // it is upon the extending class to handle the reception
        phy->StartReceivePreamble(ppdu, DbmToW(rxPowerDbm + phy->GetRxGain()));
}
void WurMainRadioNetDeviceChannel::Send(Ptr<WurMainRadioNetDevicePhy> sender,
                                        Ptr<const WurMainRadioPpdu> ppdu,
                                        double txPowerDbm) const {
        NS_LOG_FUNCTION(this << sender << ppdu << txPowerDbm);
        std::cout << Now().GetSeconds() <<  " WurMainRadioNetDeviceChannel::Send" << std::endl;
        Ptr<MobilityModel> senderMobility = sender->GetMobility();
        NS_ASSERT(senderMobility != 0);
        for (PhyList::const_iterator i = m_phyList.begin();
             i != m_phyList.end(); i++) {
                if (sender != (*i)) {
                        // For now don't account for inter channel interference
                        // nor channel bonding
                        // if ((*i)->GetChannelNumber() !=
                        //    sender->GetChannelNumber()) {
                        //        continue;
                        //}

                        Ptr<MobilityModel> receiverMobility =
                            (*i)->GetMobility()->GetObject<MobilityModel>();
                        // valid only for speed << c :)
                        Time delay =
                            m_delay->GetDelay(senderMobility, receiverMobility);
                        double rxPowerDbm = m_loss->CalcRxPower(
                            txPowerDbm, senderMobility, receiverMobility);
                        NS_LOG_DEBUG(
                            "propagation: txPower="
                            << txPowerDbm << "dbm, rxPower=" << rxPowerDbm
                            << "dbm, "
                            << "distance="
                            << senderMobility->GetDistanceFrom(receiverMobility)
                            << "m, delay=" << delay);
                        Ptr<WurMainRadioPpdu> copy = Copy(ppdu);
                        Ptr<NetDevice> dstNetDevice = (*i)->GetDevice();
                        uint32_t dstNode;
                        if (dstNetDevice == 0) {
                                dstNode = 0xffffffff;
                        } else {
                                dstNode = dstNetDevice->GetNode()->GetId();
                        }

                        Simulator::ScheduleWithContext(
                            dstNode, delay,
                            &WurMainRadioNetDeviceChannel::Receive, (*i), copy,
                            rxPowerDbm);
                }
        }
}

}  // namespace ns3
