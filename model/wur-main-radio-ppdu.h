#ifndef WUR_MAIN_RADIO_PPDU_H
#define WUR_MAIN_RADIO_PPDU_H

#include <bits/stdint-uintn.h>

#include "ns3/packet.h"
#include "ns3/simple-ref-count.h"
namespace ns3 {

class WurMainRadioPsdu;
/**
 * \brief Simple container for Wur Main Radio packets, extensible in any
 * possible way
 */
class WurMainRadioPpdu : public SimpleRefCount<WurMainRadioPpdu> {
       public:
        WurMainRadioPpdu(Ptr<const WurMainRadioPsdu> psdu,
                         uint32_t headerLength)
            : m_psdu(psdu),
              m_truncatedTx(false),
              m_headerLength(headerLength){};
        virtual ~WurMainRadioPpdu();

        Ptr<const WurMainRadioPsdu> GetPsdu(void) const;

        bool IsTruncatedTx(void) const { return m_truncatedTx; }
        bool IsTruncatedRx(void) const { return m_truncatedRx; }

        void SetTruncatedTx(void);
        void SetTruncatedRx(void);

        uint32_t GetPpduHeaderLength() const { return m_headerLength; }

       private:
        Ptr<WurMainRadioPsdu> m_psdu;
        bool m_truncatedTx, m_truncatedRx;
        uint32_t m_headerLength;
};
}  // namespace ns3
#endif /* WUR_MAIN_RADIO_PPDU_H */
