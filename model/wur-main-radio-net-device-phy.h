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
#include "ns3/interference-helper.h"
#include "ns3/mobility-model.h"
#include "ns3/net-device.h"
#include "ns3/object.h"
#include "src/wifi/model/wifi-phy-state-helper.h"
#include "wur-main-radio-net-device-phy-state-helper.h"
namespace ns3 {
class WurMainRadioNetDeviceChannel;

class WurMainRadioNetDevicePhy : public Object {
       public:
        static TypeId GetTypeId(void);
        WurMainRadioNetDevicePhy();
        virtual ~WurMainRadioNetDevicePhy();
        Ptr<WurMainRadioNetDeviceChannel> GetChannel() const;
        Ptr<NetDevice> GetDevice() const;
        Ptr<MobilityModel> GetMobility() const;
        void SetDevice(Ptr<NetDevice>);
        void SetMobility(Ptr<MobilityModel>);
        void SetChannel(Ptr<WurMainRadioNetDeviceChannel>);
        virtual void StartReceivePreamble(Ptr<const WurMainRadioPpdu>,
                                          double rxPower) = 0;
        virtual double GetRxGain() const = 0;
        virtual double GetRxSensitivity() const = 0;
        bool IsAwake() const;
        void TurnOn();
        void TurnOff();
        WurMainRadioNetDevicePhyStateHelper::MainRadioState_t GetState() {
                return m_stateHelper->GetState();
        }
        void SetState(
            WurMainRadioNetDevicePhyStateHelper::MainRadioState_t state) {
                m_stateHelper->phyState = state;
        }

        void NotifyRxBegin(Ptr<const WurMainRadioPpdu>);
        void NotifyRxDrop(Ptr<const WurMainRadioPpdu>, std::string);
        void NotifyRxEnd(Ptr<const WurMainRadioPpdu>);
        void NotifyTxBegin(Ptr<const WurMainRadioPpdu>, double);
        void NotifyTxDrop(Ptr<const WurMainRadioPpdu>);
        void NotifyTxEnd(Ptr<const WurMainRadioPpdu>);

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
        // InterferenceHelper _interference;

        typedef Callback<void, Ptr<WurMainRadioPpdu> > RxOkCallback;
        RxOkCallback  m_rxOkCallback;
       private:
        Ptr<NetDevice> m_netdevice;
        Ptr<MobilityModel> m_mobility;
        Ptr<WurMainRadioNetDevicePhyStateHelper> m_stateHelper;
        Ptr<WurMainRadioNetDeviceChannel> m_channel;
};
}  // namespace ns3
#endif
