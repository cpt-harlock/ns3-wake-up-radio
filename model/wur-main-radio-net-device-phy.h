#ifndef WUR_MAIN_RADIO_NET_DEVICE_PHY
#define WUR_MAIN_RADIO_NET_DEVICE_PHY

/**
 * \brief Class implementing a basic main radio phy for the
 * Wake Up radio architecture.
 * The phy model should have the possibility of being turned
 * ON/OFF and keep track of energy consumption.
 * Other things should be implemented in extending classes.
 */

#include "ns3/interference-helper.h"
#include "ns3/mobility-model.h"
#include "ns3/net-device.h"
#include "ns3/object.h"
#include "ns3/wur-main-radio-psdu.h"
#include "src/wifi/model/wifi-phy-state-helper.h"
#include "wur-main-radio-net-device-phy-state-helper.h"
#include "wur-main-radio-ppdu.h"

namespace ns3 {
class WurMainRadioNetDeviceChannel;
class WurCommonMac;
class WurMainRadioNetDevice;

class WurMainRadioNetDevicePhy : public Object {
       private:
        Ptr<MobilityModel> m_mobility;
        double m_rxGainDb;
        double m_txGainDb;
        double m_rxSensitivityDb;

       protected:
        Ptr<WurMainRadioNetDeviceChannel> m_channel;
        Ptr<WurMainRadioNetDevice> m_netdevice;

       public:
        typedef Callback<void, Ptr<Packet> > RxOkCallback;
        typedef Callback<void, Ptr<Packet> > TxOkCallback;
        static TypeId GetTypeId(void);
        WurMainRadioNetDevicePhy() {
                m_stateHelper =
                    CreateObject<WurMainRadioNetDevicePhyStateHelper>();
        };
        virtual ~WurMainRadioNetDevicePhy(){};
        Ptr<WurMainRadioNetDeviceChannel> GetChannel() const;
        Ptr<NetDevice> GetDevice() const;
        Ptr<MobilityModel> GetMobility() const;
        void SetDevice(Ptr<WurMainRadioNetDevice>);
        void SetMobility(Ptr<MobilityModel>);
        void SetChannel(Ptr<WurMainRadioNetDeviceChannel>);
        virtual void StartReceivePreamble(Ptr<WurMainRadioPpdu>,
                                          double rxPower) = 0;
        bool IsAwake() const;
        virtual void TurnOn() = 0;
        virtual void TurnOff() = 0;

        void NotifyRxBegin(Ptr<const WurMainRadioPpdu>);
        void NotifyRxDrop(Ptr<const WurMainRadioPpdu>, std::string);
        void NotifyRxEnd(Ptr<const WurMainRadioPpdu>);
        void NotifyTxBegin(Ptr<const WurMainRadioPpdu>, double);
        void NotifyTxDrop(Ptr<const WurMainRadioPpdu>);
        void NotifyTxEnd(Ptr<const WurMainRadioPpdu>);

        virtual void StartTx(Ptr<WurMainRadioPsdu>) = 0;

        WurMainRadioNetDevicePhyStateHelper::MainRadioState_t GetState(void);
        void SetState(WurMainRadioNetDevicePhyStateHelper::MainRadioState_t);
        void SetReceiveOkCallback(RxOkCallback);
        void SetTxOkCallback(TxOkCallback);
        void EndTx(Ptr<WurMainRadioPpdu>);
        /**
         * \param callback the callback to invoke
         *        upon successful packet reception.
         */
        /**
         * \param callback the callback to invoke
         *        upon erroneous packet reception.
         */
        // void SetReceiveErrorCallback(RxErrorCallback callback);
        //
        void SetRxGain(double rxGainDb) { m_rxGainDb = rxGainDb; };
        void SetTxGain(double txGainDb) { m_txGainDb = txGainDb; };
        void SetRxSensitivity(double rxSensitivityDb) {
                m_rxSensitivityDb = rxSensitivityDb;
        };

        double GetRxGain() const { return m_rxGainDb; };
        double GetTxGain() const { return m_txGainDb; };
        double GetRxSensitivity() const { return m_rxSensitivityDb; };

       protected:
        // defining callback to be invoked when
        // the channel pass a packet to th PHY
        //
        // InterferenceHelper _interference;

        RxOkCallback m_rxOkCallback;
        TxOkCallback m_txOkCallback;
        Ptr<WurMainRadioNetDevicePhyStateHelper> m_stateHelper;
};
}  // namespace ns3
#endif
