#ifndef WUR_COMMON_MAC_H
#define WUR_COMMON_MAC_H

/**
 * \brief Class implementing a common MAC layer for the Wake Up Radio
 * paradigm. The purpose of this class is to correctly dispatch data
 * coming from the upper layers to the main radio net device only after
 * completing the wake up procedure. Simmetrically, it should correctly
 * dispatch messages coming from the above layers to upper layers
 */

#include "ns3/object.h"
namespace ns3 {
class WurCommonMac : public Object {
};
}  // namespace ns3

#endif /* WUR_COMMON_MAC_H */
