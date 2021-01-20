#include "wur-main-radio-net-device-dummy-phy.h"

#include "ns3/log-macros-enabled.h"
#include "ns3/nstime.h"
#include "ns3/object.h"
#include "ns3/scheduler.h"
#include "ns3/simulator.h"
#include "ns3/wifi-phy-state.h"
#include "wur-common-mac.h"
#include "wur-main-radio-net-device-channel.h"
#include "wur-main-radio-net-device-phy-state-helper.h"
#include "wur-main-radio-net-device-phy.h"
#include "wur-main-radio-net-device.h"
#include "wur-main-radio-ppdu.h"
#include "wur-main-radio-psdu.h"
namespace ns3 {
NS_LOG_COMPONENT_DEFINE("WurMainRadioNetDeviceDummyPhy");

void WurMainRadioNetDeviceDummyPhy::StartReceivePreamble(
    Ptr<WurMainRadioPpdu> ppdu, double rxPowerW) {
        NS_LOG_FUNCTION(this << rxPowerW);
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
			NS_LOG_INFO("start rx");
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
        // TODO: implement operator for ppdu
        // NS_LOG_FUNCTION(this << *ppdu << rxPowerW);
        NS_LOG_DEBUG("sync to signal (power=" << rxPowerW << "W)");
        SetState(WurMainRadioNetDevicePhyStateHelper::RX);
        // fixed preamble duration
        Time startOfPreambleDuration = PREAMBLE_DURATION;
        // set current receiving packet
        currentRxPacket = ppdu;
        // schedule header reception after preamble reception
        Simulator::ScheduleWithContext(m_netdevice->GetNode()->GetId(), startOfPreambleDuration,
                            &WurMainRadioNetDeviceDummyPhy::StartReceiveHeader,
                            this, ppdu);
}

void WurMainRadioNetDeviceDummyPhy::StartReceiveHeader(
    Ptr<WurMainRadioPpdu> ppdu) {
        // NS_LOG_FUNCTION(this << *ppdu);
        NotifyRxBegin(ppdu);
        Time headerDuration =
            Seconds((double)ppdu->GetPpduHeaderLength() / BIT_PER_SECONDS);
        Simulator::ScheduleWithContext(Simulator::GetContext(),headerDuration,
                            &WurMainRadioNetDeviceDummyPhy::StartReceivePayload,
                            this, ppdu->GetPsdu());
}

void WurMainRadioNetDeviceDummyPhy::StartReceivePayload(
    Ptr<WurMainRadioPsdu> psdu) {
        // NS_LOG_FUNCTION(this << *psdu);
        Time payloadDuration =
            Seconds((psdu->GetPacket()->GetSize() * 8.0) / BIT_PER_SECONDS);
        Simulator::ScheduleWithContext(Simulator::GetContext(),payloadDuration,
                            &WurMainRadioNetDeviceDummyPhy::EndReceivePayload,
                            this, psdu);
        NS_LOG_DEBUG("Receiving PSDU");
}
void WurMainRadioNetDeviceDummyPhy::EndReceivePayload(
    Ptr<WurMainRadioPsdu> psdu) {
        NS_LOG_FUNCTION(this);
        // trace rx end
        // TODO: why we have psdu?
        // NotifyRxEnd(psdu);
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
        if (GetState() != WurMainRadioNetDevicePhyStateHelper::OFF) {
                if (currentRxPacket != nullptr)
                        currentRxPacket->SetTruncatedRx();
                if (currentTxPacket != nullptr)
                        currentTxPacket->SetTruncatedTx();
                SetState(WurMainRadioNetDevicePhyStateHelper::OFF);
        }
}

void WurMainRadioNetDeviceDummyPhy::StartTx(Ptr<WurMainRadioPsdu> psdu) {
        if (GetState() == WurMainRadioNetDevicePhyStateHelper::IDLE) {
                SetState(WurMainRadioNetDevicePhyStateHelper::TX);
                // create a ppdu
		NS_LOG_FUNCTION_NOARGS();
                // TODO: define header duration
                Ptr<WurMainRadioPpdu> ppdu = Create<WurMainRadioPpdu>(psdu, 5);
                // TODO: wrapper around this function, tx end should be
                // scheduled independently of channel invocation
                // The wrapper should call the EndTx function
                Time txDuration = Seconds((double)(ppdu->GetPpduHeaderLength() +
                                                   ppdu->GetPsdu()
                                                       ->GetPacket()
                                                       ->GetSerializedSize()) /
                                          BIT_PER_SECONDS);
                txDuration += PREAMBLE_DURATION;
                Simulator::ScheduleWithContext(Simulator::GetContext(),
                    txDuration, &WurMainRadioNetDevicePhy::EndTx, this, ppdu);
		//TODO: correct formula for gain
                m_channel->Send(this, ppdu, GetTxPower() + GetTxGain());

        }
}

}  // namespace ns3
