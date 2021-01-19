#include "wur-common-mac.h"

#include "wur-common-mac-state-helper.h"
#include "ns3/callback.h"
#include "ns3/type-id.h"
#include "wur-main-radio-net-device-channel.h"
#include "wur-main-radio-net-device.h"
namespace ns3 {

TypeId WurCommonMac::GetTypeId() {
        static TypeId tid =
            TypeId("ns3::WurCommonMac")
                .SetParent<Object>()
                .SetGroupName("Wur")
                .AddTraceSource(
                    "MacTx",
                    "A packet has been received from higher layers and is "
                    "being processed in preparation for "
                    "queueing for transmission.",
                    MakeTraceSourceAccessor(&WurCommonMac::m_macTxTrace),
                    "ns3::Packet::TracedCallback")
                .AddTraceSource(
                    "MacTxDrop",
                    "A packet has been dropped in the MAC layer before "
                    "transmission.",
                    MakeTraceSourceAccessor(&WurCommonMac::m_macTxDropTrace),
                    "ns3::Packet::TracedCallback")
                .AddTraceSource(
                    "MacPromiscRx",
                    "A packet has been received by this device, has been "
                    "passed up from the physical layer "
                    "and is being forwarded up the local protocol stack.  This "
                    "is a promiscuous trace.",
                    MakeTraceSourceAccessor(&WurCommonMac::m_macPromiscRxTrace),
                    "ns3::Packet::TracedCallback")
                .AddTraceSource(
                    "MacRx",
                    "A packet has been received by this device, has been "
                    "passed up from the physical layer "
                    "and is being forwarded up the local protocol stack. This "
                    "is a non-promiscuous trace.",
                    MakeTraceSourceAccessor(&WurCommonMac::m_macRxTrace),
                    "ns3::Packet::TracedCallback")
                .AddTraceSource(
                    "MacRxDrop",
                    "A packet has been dropped in the MAC layer after it has "
                    "been passed up from the physical layer.",
                    MakeTraceSourceAccessor(&WurCommonMac::m_macRxDropTrace),
                    "ns3::Packet::TracedCallback");
        return tid;
}

void WurCommonMac::Initialize() {
        // init timers
        wurSendingTimer.SetFunction(&WurCommonMac::TimerWurMechanismCallback,
                                    this);
        dataSendingTimer.SetFunction(&WurCommonMac::TimerDataSendingCallback,
                                     this);
        dataReceivingTimer.SetFunction(
            &WurCommonMac::TimerDataReceivingCallback, this);
}

void WurCommonMac::Enqueue(Ptr<Packet> packet, Address to) {
        m_txqueue.push_back(packet);
        std::cout << Now().GetSeconds()
                  << " WurCommonMac::Enqueue total #packets "
                  << m_txqueue.size() << std::endl;
        // if in IDLE state, try send packet immediately
        if (m_stateHelper->GetState() == WurCommonMacStateHelper::IDLE) {
                std::cout << Now().GetSeconds()
                          << " WurCommonMac::Enqueue starting wur mechanism"
                          << std::endl;
                wurSendingTimer.Schedule(WUR_MECHANISM_TIMEOUT);
                StartWurMechanism();
        }
}

void WurCommonMac::TimerWurMechanismCallback() {
        std::cout << Now().GetSeconds()
                  << " WurCommonMac::TimerWurMechanismCallback " << std::endl;
        // failure of previous wur mechanism or idle, try to send data
        if ((m_stateHelper->GetState() ==
                 WurCommonMacStateHelper::SENDING_WUR_MESSAGE ||
             m_stateHelper->GetState() == WurCommonMacStateHelper::IDLE) &&
            !m_txqueue.empty()) {
                wurSendingTimer.Schedule(WUR_MECHANISM_TIMEOUT);
                StartWurMechanism();
                std::cout
                    << Now().GetSeconds()
                    << " WurCommonMac::TimerWurMechanismCallback restarting to send \
                        a new packet"
                    << std::endl;
        } else {
                std::cout << Now().GetSeconds()
                          << " WurCommonMac::TimerWurMechanismCallback no new packet or doing smthng else "
                          << std::endl;
        }
}

void WurCommonMac::TimerDataReceivingCallback() {
        // turn off
        if (m_stateHelper->GetState() ==
            WurCommonMacStateHelper::RECEIVING_DATA) {
                m_mainRadioNetDevice->TurnOff();
                m_stateHelper->SetState(WurCommonMacStateHelper::IDLE);
        }
}

void WurCommonMac::StartDataRx() {
        // invoked on reception of wake up signal from the Wur device mac
        // put  in receive state unless we're already transmitting data
        // this check covers the unlikely case where I need to receive data
        // while sending NOTE: in our architecture, reception overrides the wur
        // sending mechanism see WurCommonMac::OnWurMechanismSuccess
        std::cout << Now().GetSeconds() << " WurCommonMac::StartDataRx"
                  << std::endl;
        if (m_stateHelper->GetState() !=
            WurCommonMacStateHelper::SENDING_DATA) {
                m_stateHelper->SetState(
                    WurCommonMacStateHelper::RECEIVING_DATA);
                m_mainRadioNetDevice->TurnOn();
                dataReceivingTimer.Schedule(DATA_RECEPTION_TIMEOUT);
        }
}

void WurCommonMac::OnWurMechanismSuccess() {
        // If no packet reception activated in the meanwhile and wurSendingTimer
        // is running
        if (m_stateHelper->GetState() ==
                WurCommonMacStateHelper::SENDING_WUR_MESSAGE &&
            wurSendingTimer.IsRunning()) {
                StartDataTxWrapper();
                m_stateHelper->SetState(WurCommonMacStateHelper::SENDING_DATA);
                wurSendingTimer.Remove();
                //wurSendingTimer.Cancel();
        }
}

void WurCommonMac::StartDataTxWrapper() {
        dataSendingTimer.Schedule(DATA_TRANSMISSION_TIMEOUT);
        m_stateHelper->SetState(WurCommonMacStateHelper::SENDING_DATA);
        StartDataTx();
}

void WurCommonMac::TimerDataSendingCallback() {
        std::cout << Now().GetSeconds()
                  << " WurCommonMac::TimerDataSendingCallback " << std::endl;
        // turn off phy and go to idle
        m_stateHelper->SetState(WurCommonMacStateHelper::IDLE);
        m_mainRadioNetDevice->TurnOff();
        // start wurSendingTimer if something else is in tx queue
        if(m_txqueue.size() > 0)
                StartWurMechanism();

}

void WurCommonMac::NotifyTx(Ptr<Packet> packet) {
        std::cout << Now().GetSeconds() << " WurCommonMac::NotifyTx packet " << packet->GetSerializedSize() << std::endl;
        packet->Print(std::cout);
        m_macTxTrace(packet);
}

void WurCommonMac::SetMainRadioNetDevice(
    const Ptr<WurMainRadioNetDevice> device) {
        if (device != nullptr) {
                m_mainRadioNetDevice = device;
                m_mainRadioNetDevice->GetPhy()->SetReceiveOkCallback(
                    MakeCallback(&WurCommonMac::OnDataRx, this));
        }
}
void WurCommonMac::SetMainRadioPhy(const Ptr<WurMainRadioNetDevicePhy> phy) {
        m_mainRadioPhy = phy;
}
Ptr<WurMainRadioNetDevice> WurCommonMac::GetMainRadioNetDevice(void) const {
        return m_mainRadioNetDevice;
}

void WurCommonMac::SetWurNetDevice(const Ptr<WurNetDevice> device) {
        if (device != nullptr) {
                m_wurNetDevice = device;
        }
}

Ptr<WurNetDevice> WurCommonMac::GetWurNetDevice(void) const {
        return m_wurNetDevice;
}

}  // namespace ns3

