#ifndef WUR_MAIN_RADIO_NET_DEVICE_DUMMY_DEVICE_H
#define WUR_MAIN_RADIO_NET_DEVICE_DUMMY_DEVICE_H

#include "contrib/wake-up-radio/model/wur-main-radio-net-device.h"
namespace ns3 {
class WurMainRadioNetDeviceDummyDevice : public WurMainRadioNetDevice {
       public:
        bool IsBroadcast(void) const;
        Address GetBroadcast(void) const;
        bool IsMulticast(void) const;
        Address GetMulticast(Ipv4Address multicastGroup) const;
        bool IsPointToPoint(void) const;
        bool IsBridge(void) const;

       protected:
        virtual void CompleteConfig(void) = 0;
};
}  // namespace ns3

#endif /* WUR_MAIN_RADIO_NET_DEVICE_DUMMY_DEVICE_H */
