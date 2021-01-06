#ifndef WUR_MAIN_RADIO_NET_DEVICE_PHY_STATE_HELPER
#define WUR_MAIN_RADIO_NET_DEVICE_PHY_STATE_HELPER

#include "ns3/object.h"
namespace ns3 {
class WurMainRadioNetDevicePhyStateHelper : public Object

{
       public:
        bool IsAwake() { return phyState == ON; }
        void GoToSleep() { phyState = OFF; }
        void WakeUp() { phyState = ON; }

       private:
        typedef enum MAIN_RADIO_STATE {
                ON,
                OFF,
        } MainRadioState_t;

        MainRadioState_t phyState;
};
}  // namespace ns3

#endif
