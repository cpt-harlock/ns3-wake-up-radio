#include "wur-shared-mac-dummy-impl.h"
#include "ns3/mac8-address.h"
#include "ns3/object.h"
#include "wur-common-phy.h"
#include "wur-common-psdu.h"
#include "wur-common-net-device.h"

namespace ns3 {
void WurSharedMacDummyImpl::SetAddress(Address address) {
	m_address = Mac8Address::ConvertFrom(address);
}
Address WurSharedMacDummyImpl::GetAddress() const { return m_address; }
void WurSharedMacDummyImpl::StartWurTxMechanismImpl() {
	// dummy invocation of OnWurTxMechanismSuccess
	OnWurTxMechanismSuccess();
}
void WurSharedMacDummyImpl::StartWurRxMechanismImpl() {
	// dummy invocation of OnWurRxMechanismSuccess
	OnWurRxMechanismSuccess();
}
void WurSharedMacDummyImpl::OnDataRx(Ptr<Packet> packet) {
	// TODO: print packet
}
void WurSharedMacDummyImpl::OnWurRx(Ptr<Packet> packet) {
	// TODO: print packet
}
void WurSharedMacDummyImpl::StartDataTx() {
	GetMainRadioPhy()->TurnOn();
	Ptr<WurCommonPsdu> ppdu = Create<WurCommonPsdu>();
}
void WurSharedMacDummyImpl::StartDataRx() { GetMainRadioPhy()->TurnOn(); }
void WurSharedMacDummyImpl::SetPromisc(void) {}



TypeId WurSharedMacDummyImpl::WurSharedMacDummyImplHeader::GetTypeId(void) {
	static TypeId tid = TypeId("ns3::WurSharedMacDummyImpl::WurSharedMacDummyImplHeader")
				.SetParent<Chunk>()
				.SetGroupName("Wur");
	return tid;
}

TypeId WurSharedMacDummyImpl::WurSharedMacDummyImplHeader::GetInstanceTypeId() const {
	return GetTypeId();
}

uint32_t WurSharedMacDummyImpl::WurSharedMacDummyImplHeader::GetSerializedSize() const {
	return sizeof(uint8_t) * 2;
};

void WurSharedMacDummyImpl::WurSharedMacDummyImplHeader::Serialize(Buffer::Iterator start) const {
	uint8_t temp;
	m_from.CopyTo(&temp);
	start.WriteU8(temp);
	m_to.CopyTo(&temp);
	start.WriteU8(temp);
}
uint32_t WurSharedMacDummyImpl::WurSharedMacDummyImplHeader::Deserialize(Buffer::Iterator start) {
	Buffer::Iterator i = start;
	m_from = Mac8Address(i.ReadU8());
	m_to = Mac8Address(i.ReadU8());
	return i.GetDistanceFrom(start);
}

void WurSharedMacDummyImpl::WurSharedMacDummyImplHeader::Print(std::ostream &os) const {
	os << m_from << " " << m_to;
}


}  // namespace ns3
