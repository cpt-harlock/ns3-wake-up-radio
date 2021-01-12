#ifndef WUR_MAIN_RADIO_NET_DEVICE_H
#define WUR_MAIN_RADIO_NET_DEVICE_H

#include "wur-main-radio-net-device-channel.h"
#include "ns3/net-device.h"
namespace ns3 {

class WurMainRadioNetDevicePhy;
class WurCommonMac;

class WurMainRadioNetDevice : public NetDevice {
       public:
        static TypeId GetTypeId(void);

        WurMainRadioNetDevice();
        virtual ~WurMainRadioNetDevice();
        void SetMac(const Ptr<WurCommonMac> mac);
        void SetPhy(const Ptr<WurMainRadioNetDevicePhy> phy);
        Ptr<WurCommonMac> GetMac(void) const;
        Ptr<WurMainRadioNetDevicePhy> GetPhy(void) const;

        // inherited from NetDevice class
        void SetIfIndex(const uint32_t index);
        uint32_t GetIfIndex(void) const;
        Ptr<Channel> GetChannel(void) const;
        void SetAddress(Address address);
        Address GetAddress(void) const;
        bool SetMtu(const uint16_t mtu);
        uint16_t GetMtu(void) const;
        //bool IsLinkUp(void) const;
        //void AddLinkChangeCallback(Callback<void> callback);
        //virtual bool IsBroadcast(void) const = 0;
        //virtual Address GetBroadcast(void) const = 0;
        //virtual bool IsMulticast(void) const = 0;
        //virtual Address GetMulticast(Ipv4Address multicastGroup) const = 0;
        //virtual bool IsPointToPoint(void) const = 0;
        //virtual bool IsBridge(void) const = 0;
        virtual bool Send(Ptr<Packet> packet, const Address& dest,
                          uint16_t protocolNumber);
        Ptr<Node> GetNode(void) const;
        void SetNode(const Ptr<Node> node);
        //bool NeedsArp(void) const;
        void SetReceiveCallback(NetDevice::ReceiveCallback cb);
        //Address GetMulticast(Ipv6Address addr) const;
        bool SendFrom(Ptr<Packet> packet, const Address& source,
                      const Address& dest, uint16_t protocolNumber);
        void SetPromiscReceiveCallback(PromiscReceiveCallback cb);
        bool SupportsSendFrom(void) const;
        void TurnOn(void) { m_phy->TurnOn(); }
        void TurnOff(void) { m_phy->TurnOff(); }

       protected:
        virtual void CompleteConfig(void) = 0;

       private:
        // max size of MAC Service Data Unit
        const uint16_t MAX_MSDU_SIZE = 2000;
        Ptr<WurCommonMac> m_mac;
        NetDevice::ReceiveCallback m_forwardUp;  //!< forward up callback
        NetDevice::PromiscReceiveCallback
            m_promiscRx;  //!< promiscuous receive callback

        mutable uint16_t m_mtu;  //!< MTU
        uint32_t m_ifIndex;      //!< IF index
        bool m_configComplete;   //!< configuration complete
        Ptr<WurMainRadioNetDevicePhy> m_phy;
        Ptr<Node> m_node;
        TracedCallback<> m_linkChanges;
};
}  // namespace ns3

#endif /* WUR_MAIN_RADIO_NET_DEVICE_H */
