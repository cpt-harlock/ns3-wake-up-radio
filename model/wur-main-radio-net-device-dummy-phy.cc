#include "wur-main-radio-net-device-dummy-phy.h"

#include "contrib/wake-up-radio/model/wur-main-radio-net-device-phy-state-helper.h"
#include "contrib/wake-up-radio/model/wur-main-radio-net-device-phy.h"
#include "contrib/wake-up-radio/model/wur-main-radio-ppdu.h"
#include "contrib/wake-up-radio/model/wur-main-radio-psdu.h"
#include "ns3/nstime.h"
#include "ns3/simulator.h"
#include "src/wifi/model/wifi-phy-state.h"
namespace ns3 {
NS_LOG_COMPONENT_DEFINE("WurMainRadioNetDeviceDummyPhy");

void WurMainRadioNetDeviceDummyPhy::StartReceivePreamble(
    Ptr<WurMainRadioPpdu> ppdu, double rxPowerW) {
        NS_LOG_FUNCTION(this << *ppdu << rxPowerW);
        // Ptr<const WurMainRadioPpdu> psdu = ppdu->GetPsdu();
        // Ptr<Event> event =
        // m_interference.Add(ppdu, txVector, rxDuration, rxPowerW);

        // already managed at channel level
        // if (m_state->GetState() == WifiPhyState::OFF) {
        //        NS_LOG_DEBUG("Cannot start RX because device is OFF");
        //        if (endRx > (Simulator::Now() + m_state->GetDelayUntilIdle()))
        //        {
        //                MaybeCcaBusyDuration();
        //        }
        //        return;
        //}

        if (ppdu->IsTruncatedTx()) {
                NS_LOG_DEBUG(
                    "Packet reception stopped because transmitter has been "
                    "switched off");
                // if (endRx > (Simulator::Now() +
                // m_state->GetDelayUntilIdle())) {
                //        MaybeCcaBusyDuration();
                //}
                return;
        }

        switch (GetState()) {
                case WurMainRadioNetDevicePhyStateHelper::RX:

                        NS_LOG_DEBUG("Drop packet because already in Rx (power="
                                     << rxPowerW << "W)");
                        NotifyRxDrop(ppdu, "Already receiving packet");
                        // if (endRx >
                        //    (Simulator::Now() + m_state->GetDelayUntilIdle()))
                        //    {
                        //        // that packet will be noise _after_ the
                        //        // reception of the currently-received
                        //        // packet.
                        //        MaybeCcaBusyDuration();
                        //}

                        break;
                case WurMainRadioNetDevicePhyStateHelper::TX:
                        NS_LOG_DEBUG("Drop packet because already in Tx (power="
                                     << rxPowerW << "W)");
                        NotifyRxDrop(ppdu, "Already in Tx");
                        // if (endRx >
                        //    (Simulator::Now() + m_state->GetDelayUntilIdle()))
                        //    {
                        //        // that packet will be noise _after_ the
                        //        // transmission of the currently-transmitted
                        //        // packet.
                        //        MaybeCcaBusyDuration();
                        //}
                        break;
                // case WurMainRadioNetDevicePhyStateHelper::CCA_BUSY:
                //        if (m_currentEvent != 0) {
                //                if (m_frameCaptureModel != 0 &&
                //                    m_frameCaptureModel->IsInCaptureWindow(
                //                        m_timeLastPreambleDetected) &&
                //                    m_frameCaptureModel->CaptureNewFrame(
                //                        m_currentEvent, event)) {
                //                        AbortCurrentReception(
                //                            FRAME_CAPTURE_PACKET_SWITCH);
                //                        NS_LOG_DEBUG("Switch to new packet");
                //                        StartRx(event, rxPowerW);
                //                } else {
                //                        NS_LOG_DEBUG(
                //                            "Drop packet because already in Rx
                //                            "
                //                            "(power="
                //                            << rxPowerW << "W)");
                //                        NotifyRxDrop(psdu, RXING);
                //                        if (endRx >
                //                            (Simulator::Now() +
                //                             m_state->GetDelayUntilIdle())) {
                //                                // that packet will be noise
                //                                // _after_ the reception of
                //                                the
                //                                // currently-received packet.
                //                                MaybeCcaBusyDuration();
                //                        }
                //                }
                //        } else {
                //                StartRx(event, rxPowerW);
                //        }
                //        break;
                case WurMainRadioNetDevicePhyStateHelper::IDLE:
                        StartRx(ppdu, rxPowerW);
                        break;
                case WurMainRadioNetDevicePhyStateHelper::OFF:
                        NS_LOG_DEBUG("Drop packet because in sleep mode");
                        NotifyRxDrop(ppdu, "Sleeping");
                        // if (endRx >
                        //    (Simulator::Now() + m_state->GetDelayUntilIdle()))
                        //    {
                        //        // that packet will be noise _after_ the sleep
                        //        // period.
                        //        MaybeCcaBusyDuration();
                        //}
                        break;
                default:
                        NS_FATAL_ERROR("Invalid WifiPhy state.");
                        break;
        }
}

void WurMainRadioNetDeviceDummyPhy::StartRx(Ptr<WurMainRadioPpdu> ppdu,
                                            double rxPowerW) {
        NS_LOG_FUNCTION(this << *ppdu << rxPowerW);
        NS_LOG_DEBUG("sync to signal (power=" << rxPowerW << "W)");
        SetState(WurMainRadioNetDevicePhyStateHelper::RX);
        // fixed preamble duration
        Time startOfPreambleDuration = PREAMBLE_DURATION;
        // set current receiving packet
        currentRxPacket = ppdu;
        // schedule header reception after preamble reception
        Simulator::Schedule(startOfPreambleDuration,
                            &WurMainRadioNetDeviceDummyPhy::StartReceiveHeader,
                            this, ppdu);
}

void WurMainRadioNetDeviceDummyPhy::StartReceiveHeader(
    Ptr<WurMainRadioPpdu> ppdu) {
        NS_LOG_FUNCTION(this << *ppdu);
        NotifyRxBegin(ppdu);
        Time headerDuration =
            Seconds((double)ppdu->GetPpduHeaderLength() / BIT_PER_SECONDS);
        Simulator::Schedule(headerDuration,
                            &WurMainRadioNetDeviceDummyPhy::StartReceivePayload,
                            this, ppdu->GetPsdu());
}

void WurMainRadioNetDeviceDummyPhy::StartReceivePayload(
    Ptr<WurMainRadioPsdu> psdu) {
        NS_LOG_FUNCTION(this << *psdu);
        Time payloadDuration =
            Seconds((psdu->GetPacket()->GetSize() * 8.0) / BIT_PER_SECONDS);
        Simulator::Schedule(payloadDuration,
                            &WurMainRadioNetDeviceDummyPhy::EndReceivePayload,
                            this, psdu);
        NS_LOG_DEBUG("Receiving PSDU");
}
void WurMainRadioNetDeviceDummyPhy::EndReceivePayload(
    Ptr<WurMainRadioPsdu> psdu) {
        NS_LOG_FUNCTION(this << *psdu);
        // trace rx end
        NotifyRxEnd(psdu);
        SetState(WurMainRadioNetDevicePhyStateHelper::IDLE);
        // removing current receiving packet
        currentRxPacket = nullptr;
        // notify MAC of ok reception
        if (!m_rxOkCallback.IsNull()) {
                m_rxOkCallback(psdu->GetPacket());
        }
}

void WurMainRadioNetDeviceDummyPhy::TurnOn() {
        if (GetState() == WurMainRadioNetDevicePhyStateHelper::OFF)
                SetState(WurMainRadioNetDevicePhyStateHelper::IDLE);
}

void WurMainRadioNetDeviceDummyPhy::TurnOff() {
        if(GetState() != WurMainRadioNetDevicePhyStateHelper::OFF) {
                if(currentRxPacket != nullptr)
                        currentRxPacket->SetTruncatedRx();
                if(currentTxPacket != nullptr)
                        currentTxPacket->SetTruncatedTx();
                SetState(WurMainRadioNetDevicePhyStateHelper::OFF);
        }
}

}  // namespace ns3
