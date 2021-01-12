#ifndef WUR_COMMON_DUMMY_MAC_HEADER_H
#define WUR_COMMON_DUMMY_MAC_HEADER_H

#include <bits/stdint-uintn.h>

#include "ns3/header.h"
#include "src/network/utils/ipv6-address.h"

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
        void SetFrom(uint32_t from) { m_from = from; }
        void SetTo(uint32_t to) { m_to = to; }
        uint32_t GetFrom() { return m_from; }
        uint32_t GetTo() { return m_to; }

       private:
        uint32_t m_from;
        uint32_t m_to;
};
}  // namespace ns3

#endif /* WUR_COMMON_DUMMY_MAC_HEADER_H */
