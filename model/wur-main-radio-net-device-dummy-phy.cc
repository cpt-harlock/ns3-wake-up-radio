#include "wur-main-radio-net-device-dummy-phy.h"

#include "contrib/wake-up-radio/model/wur-main-radio-net-device-phy-state-helper.h"
#include "contrib/wake-up-radio/model/wur-main-radio-net-device-phy.h"
#include "contrib/wake-up-radio/model/wur-main-radio-ppdu.h"
#include "ns3/simulator.h"
namespace ns3 {
NS_LOG_COMPONENT_DEFINE("WurMainRadioNetDeviceDummyPhy");

void WurMainRadioNetDeviceDummyPhy::StartReceivePreamble(
    Ptr<const WurMainRadioPpdu> ppdu, double rxPowerW) {
        NS_LOG_FUNCTION(this << *ppdu << rxPowerW);
        Time rxDuration = ppdu->GetTxDuration();
        Ptr<const WurMainRadioPpdu> psdu = ppdu->GetPsdu();
        // Ptr<Event> event =
        // m_interference.Add(ppdu, txVector, rxDuration, rxPowerW);
        Time endRx = Simulator::Now() + rxDuration;

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
                        NotifyRxDrop(psdu, "Already receiving packet");
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
                        NotifyRxDrop(psdu, "Already in Tx");
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
                        StartRx(psdu, rxPowerW);
                        break;
                case WurMainRadioNetDevicePhyStateHelper::OFF:
                        NS_LOG_DEBUG("Drop packet because in sleep mode");
                        NotifyRxDrop(psdu, "Sleeping");
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

void WurMainRadioNetDeviceDummyPhy::StartRx(Ptr<const WurMainRadioPpdu> packet,
                                            double rxPowerW) {
        NS_LOG_FUNCTION(this << *packet << rxPowerW);
        NS_LOG_DEBUG("sync to signal (power=" << rxPowerW << "W)");
        SetState(WurMainRadioNetDevicePhyStateHelper::RX);
        Time startOfPreambleDuration = GetPreambleDetectionDuration();
        // schedule header reception after preamble reception
        Simulator::Schedule(startOfPreambleDuration,
                            &WurMainRadioNetDeviceDummyPhy::StartReceiveHeader,
                            this, packet);
}

void WurMainRadioNetDeviceDummyPhy::StartReceiveHeader(
    Ptr<const WurMainRadioPpdu> psdu) {
        NS_LOG_FUNCTION(this << *psdu);
        NotifyRxBegin(psdu);
        // we don't give a fuck about PHY header
        Simulator::Schedule(GetHeaderDuration(),
                            &WurMainRadioNetDeviceDummyPhy::StartReceivePayload,
                            this, psdu);
}

void WurMainRadioNetDeviceDummyPhy::StartReceivePayload(
    Ptr<const WurMainRadioPpdu> psdu) {
        NS_LOG_FUNCTION(this << *psdu);
        Time payloadDuration = psdu->GetPayloadDuration();
        Simulator::Schedule(payloadDuration,
                            &WurMainRadioNetDeviceDummyPhy::EndReceivePayload,
                            this, psdu);
        NS_LOG_DEBUG("Receiving PSDU");
}
void WurMainRadioNetDeviceDummyPhy::EndReceivePayload(
    Ptr<WurMainRadioPpdu> psdu) {
        NS_LOG_FUNCTION(this << *psdu);
        // trace rx end
        NotifyRxEnd(psdu);
        SetState(WurMainRadioNetDevicePhyStateHelper::IDLE);
        // notify MAC of ok reception
        if (!m_rxOkCallback.IsNull()) {
                m_rxOkCallback(psdu);
        }
}

}  // namespace ns3
