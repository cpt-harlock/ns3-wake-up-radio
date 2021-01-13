#ifndef WUR_MAIN_RADIO_NET_DEVICE_DUMMY_PHY_H
#define WUR_MAIN_RADIO_NET_DEVICE_DUMMY_PHY_H

#include "wur-main-radio-net-device-phy.h"
#include "wur-main-radio-net-device-channel.h"
#include "wur-main-radio-ppdu.h"
#include "ns3/nstime.h"
namespace ns3 {
class WurMainRadioNetDeviceDummyPhy : public WurMainRadioNetDevicePhy {
       public:
        void StartReceivePreamble(Ptr<WurMainRadioPpdu>, double);
        void StartRx(Ptr<WurMainRadioPpdu>, double);
        Time GetPreambleDetectionDuration() { return PREAMBLE_DURATION; }
        Time GetHeaderDuration() { return PREAMBLE_DURATION; }
        void StartReceiveHeader(Ptr<WurMainRadioPpdu>);
        void StartReceivePayload(Ptr<WurMainRadioPsdu>);
        void EndReceivePayload(Ptr<WurMainRadioPsdu>);
        void TurnOn();
        void TurnOff();
        virtual void StartTx(Ptr<WurMainRadioPsdu>);

       private:
        const uint BIT_PER_SECONDS =  1000;
        const Time PREAMBLE_DURATION = MilliSeconds(1);
        Ptr<WurMainRadioPpdu> currentRxPacket;
        Ptr<WurMainRadioPpdu> currentTxPacket;
};

}  // namespace ns3

#endif /* WUR_MAIN_RADIO_NET_DEVICE_DUMMY_PHY_H */
