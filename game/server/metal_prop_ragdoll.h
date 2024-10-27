#ifndef HL1_PROP_RAGDOLL_H
#define HL1_PROP_RAGDOLL_H
#ifdef _WIN32
#pragma once
#endif

#include "cbase.h"
#include "physics_prop_ragdoll.h"

class CMetalRagdoll : public CRagdollProp
{
	DECLARE_CLASS(CMetalRagdoll, CRagdollProp);

public:
	CMetalRagdoll(void);
	~CMetalRagdoll(void);

	void Spawn();
	void Precache();
	virtual void TraceAttack(const CTakeDamageInfo &info, const Vector &dir, trace_t *ptr, CDmgAccumulator *pAccumulator);

	void SetBloodColor(int color) { m_bloodColor = color; }


private:
	int	m_bloodColor; // color of blood particless
	float m_flGibHealth;

	DECLARE_DATADESC();
};

CMetalRagdoll* CreateMetalServerRagdoll(CBaseAnimating* pAnimating, int forceBone, const CTakeDamageInfo& info, int collisionGroup, bool bUseLRURetirement = false);

#endif // HL1_PROP_RAGDOLL_H