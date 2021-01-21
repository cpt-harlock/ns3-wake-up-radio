#ifndef WUR_COMMON_PHY_H
#define WUR_COMMON_PHY_H
#include "ns3/mobility-model.h"
#include "ns3/net-device.h"
#include "ns3/object.h"
namespace ns3 {
class WurCommonPpdu;
class WurCommonPhy : public Object {
       public:
	Ptr<MobilityModel> GetMobility() const;
	Ptr<NetDevice> GetDevice() const;
	virtual double GetRxGain() const;
	virtual double GetRxSensitivity() const;
	virtual void StartReceivePreamble(Ptr<WurCommonPpdu> ppdu, double powerDbm);
};
}  // namespace ns3
#endif /* WUR_COMMON_PHY_H */
