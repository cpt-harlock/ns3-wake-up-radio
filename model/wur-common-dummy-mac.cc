#include "wur-common-dummy-mac.h"
#include "ns3/log-macros-enabled.h"
#include "ns3/mac8-address.h"
#include "ns3/object.h"
#include "ns3/wur-main-radio-psdu.h"
#include "wur-main-radio-net-device-phy-state-helper.h"
#include "ns3/log.h"
#include "wur-main-radio-net-device-phy.h"
#include "wur-main-radio-net-device.h"
#include "wur-main-radio-net-device-channel.h"
#include <string>

NS_LOG_COMPONENT_DEFINE("WurCommonDummyMac");

namespace ns3 {
void WurCommonDummyMac::StartWurMechanism() {
        //TODO: send a wur packet? 
        //TODO: for now, dummy implementation
        m_stateHelper->SetState(WurCommonMacStateHelper::SENDING_WUR_MESSAGE);
	NS_LOG_FUNCTION_NOARGS();
        OnWurMechanismSuccess();
}

void WurCommonDummyMac::OnDataRx(Ptr<Packet> packet) {
        //se lo mettemo in saccoccia
       NS_LOG_FUNCTION(packet);
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
                m_txqueue.erase(m_txqueue.begin());
                NS_LOG_INFO(" WurCommonDummyMac::StartDataTx m_txqueue size " + std::to_string(m_txqueue.size()));
                Ptr<WurMainRadioPsdu> psdu = Create<WurMainRadioPsdu>(packet);
                m_mainRadioPhy->StartTx(psdu);
        }

}
        
}
