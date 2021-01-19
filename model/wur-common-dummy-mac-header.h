#ifndef WUR_COMMON_DUMMY_MAC_HEADER_H
#define WUR_COMMON_DUMMY_MAC_HEADER_H

#include <bits/stdint-uintn.h>

#include "ns3/header.h"
#include "ns3/mac8-address.h"
#include "ns3/ipv6-address.h"

namespace ns3 {
class WurCommonDummyMacHeader : public Header {
        using Chunk::Deserialize;

       public:
        WurCommonDummyMacHeader(){};
        virtual ~WurCommonDummyMacHeader(){};
        static TypeId GetTypeId(void);
        virtual TypeId GetInstanceTypeId (void) const;
        virtual uint32_t GetSerializedSize(void) const;
        virtual void Serialize(Buffer::Iterator start) const;
        virtual uint32_t Deserialize(Buffer::Iterator start);
        virtual void Print(std::ostream &os) const;
        void SetFrom(uint8_t from) { m_from = Mac8Address(from); }
        void SetTo(uint8_t to) { m_to = Mac8Address(to); }
        void SetFrom(Address from) { m_from.ConvertFrom(from); }
        void SetTo(Address to) { m_to.ConvertFrom(to); }
        Mac8Address GetFrom() { return m_from; }
        Mac8Address GetTo() { return m_to; }

       private:
       Mac8Address  m_from;
       Mac8Address m_to;
};
}  // namespace ns3

#endif /* WUR_COMMON_DUMMY_MAC_HEADER_H */
