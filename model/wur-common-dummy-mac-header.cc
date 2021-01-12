#include "wur-common-dummy-mac-header.h"

#include <bits/stdint-uintn.h>

#include "ns3/buffer.h"
#include "ns3/type-id.h"

namespace ns3 {
TypeId WurCommonDummyMacHeader::GetTypeId(void) {
        static TypeId tid = TypeId("ns3::WurCommonDummyMacHeader")
                                .SetParent<Chunk>()
                                .SetGroupName("Wur");
        return tid;
}

TypeId WurCommonDummyMacHeader::GetInstanceTypeId() const {
        return GetTypeId();
}

uint32_t WurCommonDummyMacHeader::GetSerializedSize() const {
        return sizeof(uint32_t) * 2;
};

void WurCommonDummyMacHeader::Serialize(Buffer::Iterator start) const {
        start.WriteU32(m_from);
        start.WriteU32(m_to);
}
uint32_t WurCommonDummyMacHeader::Deserialize(Buffer::Iterator start) {
        Buffer::Iterator i = start;
        m_from = i.ReadU32();
        m_to = i.ReadU32();
        return i.GetDistanceFrom(start);
}

void WurCommonDummyMacHeader::Print(std::ostream &os) const {
        os << m_from << " " << m_to;
}

}  // namespace ns3

