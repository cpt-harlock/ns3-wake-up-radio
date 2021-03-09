#include "wur-common-phy.h"

#include "ns3/device-energy-model.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "src/wifi/model/wifi-utils.h"
#include "wur-common-channel.h"
#include "wur-common-net-device.h"
#include "wur-common-ppdu.h"
namespace ns3 {
NS_LOG_COMPONENT_DEFINE("WurCommonPhy");
Ptr<WurCommonChannel> WurCommonPhy::GetChannel() const { return m_channel; }
Ptr<MobilityModel> WurCommonPhy::GetMobility() const { return m_mobility; }
Ptr<WurCommonNetDevice> WurCommonPhy::GetDevice() const { return m_netDevice; }
void WurCommonPhy::StartReceivePreamble(Ptr<WurCommonPpdu> ppdu,
                                        double rxPowerDbm) {
        NS_LOG_FUNCTION(this << rxPowerDbm);
        //add interference event 
        Time rxDuration = ppdu->GetTxDuration ();
        Ptr<RadioEvent> event = m_interference->Add(ppdu, ppdu->GetTxDuration(), DbmToW(rxPowerDbm));
        Ptr<const WurCommonPsdu> psdu = ppdu->GetPsdu();
        Time endRx = Simulator::Now () + rxDuration;
        //TODO: this should be checked at the end, since interruption may occurr at any point of the transmission
        if (ppdu->IsTruncatedTx()) {
                NS_LOG_DEBUG(
                    "Packet reception stopped because transmitter has been "
                    "switched off");
                return;
        }
        switch (GetState()) {
                case WurCommonPhyState::RX:
                        //TODO: in Wifi there is a frame capture model, we may decide to use it
                        //for the moment, no further complexity 
                        NS_LOG_DEBUG("Drop packet because already in Rx (power="
                                     << rxPowerDbm << "dBm)");
                        NS_ASSERT(m_currentEvent != 0);
                        NotifyRxDrop(ppdu, "Already receiving packet");
                        break;
                case WurCommonPhyState::TX:
                        NS_LOG_DEBUG("Drop packet because already in Tx (power="
                                     << rxPowerDbm << "dBm)");
                        NotifyRxDrop(ppdu, "Already in Tx");
                        break;
                case WurCommonPhyState::IDLE:
                        NS_LOG_INFO("start rx");
                        StartRx(event, rxPowerDbm);
                        break;
                case WurCommonPhyState::OFF:
                        NS_LOG_DEBUG("Drop packet because in sleep mode");
                        NotifyRxDrop(ppdu, "Sleeping");
                        break;
                case WurCommonPhyState::DISABLED:
                        NS_LOG_DEBUG("Drop packet as radio is out of energy");
                        NotifyRxDrop(ppdu, "Disabled");
                        break;
                default:
                        NS_FATAL_ERROR("Invalid WurCommonPhy state.");
                        break;
        }
}

void WurCommonPhy::StartRx (Ptr<RadioEvent> event, double rxPowerDbm)
{
  NS_LOG_FUNCTION (this << *event << rxPowerDbm);
  NS_LOG_DEBUG ("sync to signal (power=" << rxPowerDbm << "dBm)");
  m_interference->NotifyRxStart (); //We need to notify it now so that it starts recording events

  if (!m_endPreambleDetectionEvent.IsRunning ())
    {
      Time remainingRxDuration = event->GetDuration () - GetPreambleDetectionDuration();
      //StartReceiveHeader should be implemented by the developer, as we're not imposing any constraint on PHY header format.
      //Dummy implementation (if no PHY header is present) can be to directly invoke StartReceivePayload from StartReceiveHeader
      m_endPreambleDetectionEvent = Simulator::Schedule (GetPreambleDetectionDuration(), &WurCommonPhy::StartReceiveHeader, this, event);
    }
  else if(rxPowerDbm > WToDbm(m_currentEvent->GetRxPowerW())) {
      NS_LOG_DEBUG ("Received a stronger signal during preamble detection: drop current packet and switch to new packet");
      NotifyRxDrop (m_currentEvent->GetPsdu (), "Switching to new packet during preamble detection");
      m_interference->NotifyRxEnd ();
      m_endPreambleDetectionEvent.Cancel ();
      m_interference->NotifyRxStart ();
      Time startOfPreambleDuration = GetPreambleDetectionDuration ();
      Time remainingRxDuration = event->GetDuration () - startOfPreambleDuration;
      m_endPreambleDetectionEvent = Simulator::Schedule (startOfPreambleDuration, &WurCommonPhy::StartReceiveHeader, this, event);
  }  
  else
    {
      NS_LOG_DEBUG ("Drop packet because RX is already decoding preamble");
      NotifyRxDrop (event->GetPsdu (),"Busy decoding preamble");
      return;
    }
  m_currentEvent = event;
}

void WurCommonPhy::StartReceivePayload (Ptr<RadioEvent> event)
{
  NS_LOG_FUNCTION (this << *event);
  NS_ASSERT (m_endPhyRxEvent.IsExpired ());
  NS_ASSERT (m_endRxEvent.IsExpired ());
  bool canReceivePayload;
  RadioInterferenceHelper::SnrPer snrPer;
  snrPer = m_interference->CalculateHtPhyHeaderSnrPer (event);
  NS_LOG_DEBUG ("snr(dB)=" << RatioToDb (snrPer.snr) << ", per=" << snrPer.per);
  canReceivePayload = (m_random->GetValue () > snrPer.per);
  Time payloadDuration = event->GetEndTime () - event->GetStartTime () - ( GetPreambleDetectionDuration() + GetPhyHeaderDuration()); 
  if (canReceivePayload) //PHY reception succeeded
    {
          ChangeState(RX);
          m_endRxEvent = Simulator::Schedule (payloadDuration, &WurCommonPhy::EndReceive, this, event);
          NS_LOG_DEBUG ("Receiving PSDU");
          return;
    }
  else //PHY reception failed
    {
      NS_LOG_DEBUG ("Drop packet because ");
      NotifyRxDrop (event->GetPsdu (), SIG_A_FAILURE);
    }
  m_endRxEvent = Simulator::Schedule (payloadDuration, &WurCommonPhy::ResetReceive, this, event);
}

void WurCommonPhy::ResetReceive (Ptr<RadioEvent> event)
{
  NS_LOG_FUNCTION (this << *event);

  ChangeState(IDLE);
  NS_ASSERT (event->GetEndTime () == Simulator::Now ());
  m_interference->NotifyRxEnd ();
  m_currentEvent = 0;
}

void WurCommonPhy::TurnOn() {
        NS_LOG_FUNCTION_NOARGS();
        if (m_state == WurCommonPhyState::OFF) {
                ChangeState(WurCommonPhyState::IDLE);
        }
}

void WurCommonPhy::TurnOff() {
        NS_LOG_FUNCTION_NOARGS();
        if (m_state != WurCommonPhyState::OFF && m_state != WurCommonPhyState::DISABLED) {
                ChangeState(WurCommonPhyState::OFF);
                if (m_rxPacket != nullptr) m_rxPacket->SetTruncatedRx();
                if (m_txPacket != nullptr) m_txPacket->SetTruncatedTx();
        }
}
void WurCommonPhy::SetMobility(Ptr<MobilityModel> mobility) {
        m_mobility = mobility;
}

TypeId WurCommonPhy::GetTypeId() {
        static TypeId tid =
            TypeId("ns3::WurCommonPhy").SetParent<Object>().SetGroupName("Wur");
        return tid;
}
void WurCommonPhy::SetChannel(Ptr<WurCommonChannel> channel) {
        m_channel = channel;
}
void WurCommonPhy::SetDevice(Ptr<WurCommonNetDevice> device) {
        m_netDevice = device;
}

void WurCommonPhy::SetEnergyModelCallback(
    DeviceEnergyModel::ChangeStateCallback cb) {
        m_energyModelCallback = cb;
}

void WurCommonPhy::ChangeState(WurCommonPhy::WurCommonPhyState state) {
        NS_LOG_FUNCTION(state);
        //can't set disabled state with ChangeState, must be manually done in the depletion handler
        NS_ASSERT(state != DISABLED);
        if (!m_energyModelCallback.IsNull()) m_energyModelCallback(state);
        //I could be disabled due to previous function call
        //Hence, I only set new state if I wasn't disabled
        if(m_state != DISABLED)
                m_state = state;
        NS_LOG_FUNCTION("Final state" << m_state);
}

/**
 * Invoked when we're out of energy
 */
void WurCommonPhy::EnergyDepletionHandler() {
        NS_LOG_FUNCTION_NOARGS();
        m_state = WurCommonPhyState::DISABLED;
        if(m_rxPacket != nullptr)
                m_rxPacket->SetTruncatedRx();
        if(m_txPacket != nullptr)
                m_txPacket->SetTruncatedTx();
}
/**
 * Invoked when getting out of disabled mode
 */
void WurCommonPhy::EnergyRechargeHandler() {
        //going to off state
        ChangeState(WurCommonPhyState::OFF);
}
        
}  // namespace ns3
