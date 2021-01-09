#include "wur-common-mac-state-helper.h"


namespace ns3 {

void WurCommonMacStateHelper::SetState(WurCommonMacState state) {
       m_state = state; 
}

WurCommonMacStateHelper::WurCommonMacState WurCommonMacStateHelper::GetState() const {
        return m_state;
}
        
}
