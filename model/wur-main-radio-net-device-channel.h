#ifndef WUR_MAIN_RADIO_NET_DEVICE_CHANNEL
#define WUR_MAIN_RADIO_NET_DEVICE_CHANNEL

#include "ns3/channel.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/type-id.h"
#include "wur-main-radio-net-device-phy.h"
#include "wur-main-radio-ppdu.h"
namespace ns3 {
/**
 * \brief no cazz
 */
class WurMainRadioNetDeviceChannel : public Channel {
       public:
        static TypeId GetTypeId(void);
        WurMainRadioNetDeviceChannel() : Channel() {}
        virtual ~WurMainRadioNetDeviceChannel() {};
        // inherited from channel
        virtual std::size_t GetNDevices(void) const;
        virtual Ptr<NetDevice> GetDevice(std::size_t i) const;

        /**
         * Add WurMainRadioNetDevicePhy to the PHY list
         * \param phy to be added
         */
        void Add(Ptr<WurMainRadioNetDevicePhy> phy);
        /**
         * \param loss the new propagation loss model.
         */
        void SetPropagationLossModel(const Ptr<PropagationLossModel> loss);
        /**
         * \param delay the new propagation delay model.
         */
        void SetPropagationDelayModel(const Ptr<PropagationDelayModel> delay);

        /**
         * \param sender the PHY object from which the packet is originating.
         * \param ppdu the PPDU to send
         * \param txPowerDbm the TX power associated to the packet, in dBm
         *
         * This method should not be invoked by normal users. It is
         * currently invoked only from WurMainRadioNetDevicePhy::StartTx.  The
         * channel attempts to deliver the PPDU to all other
         * WurMainRadioNetDevicePhy objects on the channel (except for the
         * sender).
         */
        void Send(Ptr<WurMainRadioNetDevicePhy> sender,
                  Ptr<const WurMainRadioPpdu> ppdu, double txPowerDbm) const;

       private:
        /**
         * A vector of pointers to WurMainRadioNetDevicePhy.
         */
        typedef std::vector<Ptr<WurMainRadioNetDevicePhy>> PhyList;

        /**
         * This method is scheduled by Send for each associated
         * WurMainRadioNetDevicePhy. The method then calls the corresponding
         * WurMainRadioNetDevicePhy that the first bit of the PPDU has arrived.
         *
         * \param receiver the device to which the packet is destined
         * \param ppdu the PPDU being sent
         * \param txPowerDbm the TX power associated to the packet being sent
         * (dBm)
         */
        static void Receive(Ptr<WurMainRadioNetDevicePhy> receiver,
                            Ptr<WurMainRadioPpdu> ppdu, double txPowerDbm);

        PhyList m_phyList;  //!< List of WurMainRadioPhys connected to this
                            //!< YansWifiChannel
        Ptr<PropagationLossModel> m_loss;    //!< Propagation loss model
        Ptr<PropagationDelayModel> m_delay;  //!< Propagation delay model
};
}  // namespace ns3

#endif /* WUR_MAIN_RADIO_NET_DEVICE_CHANNEL */
