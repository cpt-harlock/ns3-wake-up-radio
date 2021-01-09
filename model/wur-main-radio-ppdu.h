#ifndef WUR_MAIN_RADIO_PPDU_H
#define WUR_MAIN_RADIO_PPDU_H

#include "ns3/simple-ref-count.h"
#include "ns3/packet.h"
namespace ns3 {

/**
 * \brief Simple container for Wur Main Radio packets, extensible in any
 * possible way
 */
class WurMainRadioPpdu : public Packet {};
}  // namespace ns3
#endif /* WUR_MAIN_RADIO_PPDU_H */
