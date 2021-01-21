#ifndef WUR_COMMON_PPDU_H
#define WUR_COMMON_PPDU_H
#include "ns3/simple-ref-count.h"
namespace ns3 {
	class WurCommonPpdu : public SimpleRefCount<WurCommonPpdu> {
		public:
			bool IsTruncatedTx() { return m_isTruncatedTx; }
			void SetTruncatedTx() { m_isTruncatedTx = true; }
		private:
			bool m_isTruncatedTx = false;
	};
}
#endif /* WUR_COMMON_PPDU_H */
