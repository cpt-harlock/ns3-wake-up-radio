#ifndef WUR_MAIN_RADIO_NET_DEVICE_PHY
#define WUR_MAIN_RADIO_NET_DEVICE_PHY

/**
 * \brief Class implementing a basic main radio phy for the
 * Wake Up radio architecture.
 * The phy model should have the possibility of being turned
 * ON/OFF and keep track of energy consumption.
 * Other things should be implemented in extending classes.
 */

#include "contrib/wake-up-radio/model/wur-main-radio-ppdu.h"
#include "ns3/mobility-model.h"
#include "ns3/net-device.h"
#include "ns3/object.h"
#include "wur-main-radio-net-device-phy-state-helper.h"
namespace ns3 {

class WurMainRadioNetDevicePhy : public Object {
       public:
        static TypeId GetTypeId(void);
        WurMainRadioNetDevicePhy();
        virtual ~WurMainRadioNetDevicePhy();
        Ptr<NetDevice> GetDevice() const;
        Ptr<MobilityModel> GetMobility() const;
        virtual void StartReceivePreamble(Ptr<WurMainRadioPpdu>,
                                          double rxPower) = 0;
        double GetRxGain() const;
        double GetRxSensitivity() const;
        bool IsAwake() const;

        /**
         * \param callback the callback to invoke
         *        upon successful packet reception.
         */
        // void SetReceiveOkCallback(RxOkCallback callback);
        /**
         * \param callback the callback to invoke
         *        upon erroneous packet reception.
         */
        // void SetReceiveErrorCallback(RxErrorCallback callback);

       protected:
        // defining callback to be invoked when
        // the channel pass a packet to th PHY
        //
       private:
        Ptr<NetDevice> m_netdevice;
        Ptr<MobilityModel> m_mobility;
        Ptr<WurMainRadioNetDevicePhyStateHelper> m_statehelper;
};
}  // namespace ns3
#endif
