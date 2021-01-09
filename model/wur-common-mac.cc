#include "wur-common-mac.h"
namespace ns3 {

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
        // if in IDLE state, try send packet immediately
        if (m_stateHelper->GetState() == WurCommonMacStateHelper::IDLE)
                StartWurMechanism();
        // Start a timer for timeout. Timer should be stopped before
        // firing for completed wur mechanism
        wurSendingTimer.Schedule(WUR_MECHANISM_TIMEOUT);
}

void WurCommonMac::TimerWurMechanismCallback() {
        if (!m_txqueue.empty()) wurSendingTimer.Schedule(WUR_MECHANISM_TIMEOUT);
        // failure of previous wur mechanism or idle, try to send data
        if ((m_stateHelper->GetState() ==
                 WurCommonMacStateHelper::SENDING_WUR_MESSAGE ||
             m_stateHelper->GetState() == WurCommonMacStateHelper::IDLE) &&
            !m_txqueue.empty())
                StartWurMechanism();
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
                StartDataTx();
                m_stateHelper->SetState(WurCommonMacStateHelper::SENDING_DATA);
                wurSendingTimer.Cancel();
        }
}

void WurCommonMac::StartDataTxWrapper() {
        dataSendingTimer.Schedule(DATA_TRANSMISSION_TIMEOUT);
        m_stateHelper->SetState(WurCommonMacStateHelper::SENDING_DATA);
        StartDataTx();
}

void WurCommonMac::TimerDataSendingCallback() {
        // turn off phy and go to idle
        m_stateHelper->SetState(WurCommonMacStateHelper::IDLE);
        m_mainRadioNetDevice->TurnOff();
        // start wurSendingTimer if something else is in tx queue
        TimerWurMechanismCallback();
}

}  // namespace ns3

