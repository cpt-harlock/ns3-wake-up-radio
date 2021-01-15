#ifndef WUR_MAIN_RADIO_PSDU_H
#define WUR_MAIN_RADIO_PSDU_H

#include "ns3/packet.h"
#include "ns3/simple-ref-count.h"
namespace ns3 {
class WurMainRadioPsdu : public SimpleRefCount<WurMainRadioPsdu> {
       public:
        WurMainRadioPsdu(Ptr<Packet> packet) { m_packet = packet;} ;
        virtual ~WurMainRadioPsdu() {};
        Ptr<Packet> GetPacket() const { return m_packet; }
       private:
        Ptr<Packet> m_packet;
};  // namespace ns3

}  // namespace ns3
// namespace ns3

#endif /* WUR_MAIN_RADIO_PSDU_H */
