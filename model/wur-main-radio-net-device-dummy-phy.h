#ifndef WUR_MAIN_RADIO_NET_DEVICE_DUMMY_PHY_H
#define WUR_MAIN_RADIO_NET_DEVICE_DUMMY_PHY_H

#include "contrib/wake-up-radio/model/wur-main-radio-net-device-phy.h"
#include "contrib/wake-up-radio/model/wur-main-radio-ppdu.h"
#include "ns3/nstime.h"
namespace ns3 {
class WurMainRadioNetDeviceDummyPhy : public WurMainRadioNetDevicePhy {
       public:
        void StartReceivePreamble(Ptr<const WurMainRadioPpdu>, double);
        void StartRx(Ptr<const WurMainRadioPpdu>, double);
        double GetRxGain() const { return RX_GAIN; };
        double GetRxSensitivity() const { return RX_SENSITIVITY; };
        Time GetPreambleDetectionDuration() { return PREAMBLE_DURATION; }
        Time GetHeaderDuration() { return PREAMBLE_DURATION; }
        void StartReceiveHeader(Ptr<const WurMainRadioPpdu>);
        void StartReceivePayload(Ptr<const WurMainRadioPpdu>);
        void EndReceivePayload(Ptr<WurMainRadioPpdu>);

       private:
        const double RX_SENSITIVITY = -100;
        const double RX_GAIN = 2.15;
        const Time PREAMBLE_DURATION = MilliSeconds(1);
};

}  // namespace ns3

#endif /* WUR_MAIN_RADIO_NET_DEVICE_DUMMY_PHY_H */
