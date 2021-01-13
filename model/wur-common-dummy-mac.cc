#include "wur-common-dummy-mac.h"
#include "ns3/mac8-address.h"
#include "ns3/wur-main-radio-psdu.h"
#include "wur-main-radio-net-device-phy-state-helper.h"
#include "ns3/log.h"
#include "wur-main-radio-net-device-phy.h"
#include "wur-main-radio-net-device.h"

NS_LOG_COMPONENT_DEFINE("WurCommonDummyMac");

namespace ns3 {
void WurCommonDummyMac::StartWurMechanism() {
        //TODO: send a wur packet? 
}

void OnDataRx(Ptr<Packet> packet) {
        //se lo mettemo in saccoccia
        NS_LOG_INFO("MAC received packet"); 
}
        

void WurCommonDummyMac::SetAddress(Address address) {
        m_address.ConvertFrom(address);
}

Address WurCommonDummyMac::GetAddress() const {
        return m_address;
}

void WurCommonDummyMac::SetPromisc() {}

void WurCommonDummyMac::StartDataTx() {
        if(m_mainRadioPhy->GetState() == WurMainRadioNetDevicePhyStateHelper::OFF)
                m_mainRadioPhy->TurnOn();
        if(m_mainRadioPhy->GetState() == WurMainRadioNetDevicePhyStateHelper::IDLE) {
                Ptr<Packet> packet = m_txqueue.front();
                WurMainRadioPsdu temp(packet);
                m_txqueue.erase(m_txqueue.begin());
                Ptr<WurMainRadioPsdu> psdu(&temp);
                m_mainRadioPhy->StartTx(psdu);
        }

}
        
}
