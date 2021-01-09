#include "contrib/wake-up-radio/model/wur-main-radio-ppdu.h"
namespace ns3 {
NS_LOG_COMPONENT_DEFINE("WurMainRadioPpdu");
class WurMainRadioPsdu;
Ptr<const WurMainRadioPsdu> WurMainRadioPpdu::GetPsdu(void) const {
        return m_psdu;
}
void WurMainRadioPpdu::SetTruncatedTx() {
        NS_LOG_FUNCTION(this);
        m_truncatedTx = true;
}

}  // namespace ns3
