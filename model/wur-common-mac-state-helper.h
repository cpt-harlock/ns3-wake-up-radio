#ifndef WUR_COMMON_MAC_STATE_HELPER_H
#define WUR_COMMON_MAC_STATE_HELPER_H

#include "ns3/object.h"
#include "ns3/node.h"
namespace ns3 {
class WurCommonMacStateHelper : public Object {
       public:
        typedef enum {
                IDLE,
                SENDING_WUR_MESSAGE,
                SENDING_DATA,
                RECEIVING_DATA,
        } WurCommonMacState;

        WurCommonMacState GetState() const;
        void SetState(WurCommonMacState); 
       private:
        WurCommonMacState m_state;
};
}  // namespace ns3

#endif /* WUR_COMMON_MAC_STATE_HELPER_H */
