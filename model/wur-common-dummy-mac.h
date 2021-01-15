#ifndef WUR_COMMON_DUMMY_MAC_H
#define WUR_COMMON_DUMMY_MAC_H
#include "ns3/log-macros-enabled.h"
#include "ns3/log.h"
#include "ns3/mac8-address.h"
#include "wur-common-mac.h"
namespace ns3 {
class WurCommonDummyMac : public WurCommonMac {
        public:
        WurCommonDummyMac()  {}
        virtual ~WurCommonDummyMac() {}
        void StartWurMechanism();
        void OnDataRx(Ptr<Packet>);
        void SetPromisc();
        void SetAddress(Address);
        Address GetAddress() const;
        void StartDataTx();

        private:
        Mac8Address m_address;
};
}  // namespace ns3

#endif /* WUR_COMMON_DUMMY_MAC_H */
