#ifndef WUR_COMMON_MAC_H
#define WUR_COMMON_MAC_H

/**
 * \brief Class implementing a common MAC layer for the Wake Up Radio
 * paradigm. The purpose of this class is to correctly dispatch data
 * coming from the upper layers to the main radio net device only after
 * completing the wake up procedure. Simmetrically, it should correctly
 * dispatch messages coming from the above layers to upper layers
 */

#include "wur-common-mac-state-helper.h"
#include "wur-main-radio-net-device-phy.h"
#include "ns3/traced-callback.h"
#include "ns3/wur-net-device.h"
#include "ns3/nstime.h"
#include "ns3/object.h"
#include "ns3/timer.h"
namespace ns3 {

class Address;
class WurMainRadioNetDevice;

class WurCommonMac : public Object {
       public:
               WurCommonMac() : Object(), wurSendingTimer(), dataSendingTimer() , dataReceivingTimer(), m_stateHelper(), m_txqueue() {
                       Initialize();
               };
        virtual void SetPromisc(void) = 0;
        virtual void SetAddress(Address) = 0;
        virtual Address GetAddress(void) const = 0;
        static TypeId GetTypeId(void);
        //TODO: reset main and wur device functions
        //TODO: add listeners for phy events
        void SetMainRadioNetDevice(const Ptr<WurMainRadioNetDevice> device);
        Ptr<WurMainRadioNetDevice> GetMainRadioNetDevice(void) const;
        void SetWurNetDevice(const Ptr<WurNetDevice> device);
        Ptr<WurNetDevice> GetWurNetDevice(void) const;
        virtual bool SupportsSendFrom(void) const {return true;};
        //Enqueue a packet at MAC level
        virtual void Enqueue(Ptr<Packet> packet, Address to);
        virtual void StartWurMechanism() = 0;
        virtual void OnDataRx(Ptr<Packet>) = 0;
        void TimerWurMechanismCallback();
        void TimerDataReceivingCallback();
        void TimerDataSendingCallback();
        void StartDataTxWrapper();
        virtual void StartDataTx() = 0;
        void StartDataRx();
        void OnWurMechanismSuccess();
        void NotifyTx(Ptr<Packet>);
       protected: 
        Ptr<WurMainRadioNetDevicePhy> m_mainRadioPhy;
        Ptr<WurMainRadioNetDevice> m_mainRadioNetDevice;
        Ptr<WurNetDevice> m_wurNetDevice;
        std::vector<Ptr<Packet>> m_txqueue;
       private:
        Timer wurSendingTimer;
        Timer dataSendingTimer;
        Timer dataReceivingTimer;
        Ptr<WurCommonMacStateHelper> m_stateHelper;
        const Time WUR_MECHANISM_TIMEOUT = MilliSeconds(20);
        const Time DATA_RECEPTION_TIMEOUT = MilliSeconds(50);
        const Time DATA_TRANSMISSION_TIMEOUT = MilliSeconds(50);
        TracedCallback<Ptr<const Packet>> m_macTxTrace;
        TracedCallback<Ptr<const Packet>> m_macTxDropTrace;
        TracedCallback<Ptr<const Packet>> m_macRxDropTrace;
        TracedCallback<Ptr<const Packet>> m_macPromiscRxTrace;
        TracedCallback<Ptr<const Packet>> m_macRxTrace;
        
        void Initialize();
};
}  // namespace ns3

#endif /* WUR_COMMON_MAC_H */
