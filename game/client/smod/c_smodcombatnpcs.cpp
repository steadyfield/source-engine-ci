//========= Made by The SMOD13 Team, some rights reserved. ============//
//
// Purpose: Client-Side rep for the "Combat-Ready" NPCs
//
// TODO: This entire file is a giant fucking mess. Is there any better way to do this?
//
//=============================================================================//
#include "cbase.h"
#include "c_ai_basenpc.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class C_KleinerCombatEnenmy : public C_AI_BaseNPC
{
public:
	DECLARE_CLASS(C_KleinerCombatEnenmy, C_AI_BaseNPC);
	DECLARE_CLIENTCLASS();

	C_KleinerCombatEnenmy();
	virtual			~C_KleinerCombatEnenmy();

private:
	C_KleinerCombatEnenmy(const C_KleinerCombatEnenmy &); // not defined, not accessible
};

IMPLEMENT_CLIENTCLASS_DT(C_KleinerCombatEnenmy, DT_NPC_KleinerCombatEnenmy, CNPC_KleinerCombatEnenmy)
END_RECV_TABLE()

C_KleinerCombatEnenmy::C_KleinerCombatEnenmy()
{
}

C_KleinerCombatEnenmy::~C_KleinerCombatEnenmy()
{
}

class C_EliCombatEnenmy : public C_AI_BaseNPC
{
public:
	DECLARE_CLASS(C_EliCombatEnenmy, C_AI_BaseNPC);
	DECLARE_CLIENTCLASS();

	C_EliCombatEnenmy();
	virtual			~C_EliCombatEnenmy();

private:
	C_EliCombatEnenmy(const C_EliCombatEnenmy &); // not defined, not accessible
};

IMPLEMENT_CLIENTCLASS_DT(C_EliCombatEnenmy, DT_NPC_EliCombatEnenmy, CNPC_EliCombatEnenmy)
END_RECV_TABLE()

C_EliCombatEnenmy::C_EliCombatEnenmy()
{
}

C_EliCombatEnenmy::~C_EliCombatEnenmy()
{
}

class C_MossmanCombatEnenmy : public C_AI_BaseNPC
{
public:
	DECLARE_CLASS(C_MossmanCombatEnenmy, C_AI_BaseNPC);
	DECLARE_CLIENTCLASS();

	C_MossmanCombatEnenmy();
	virtual			~C_MossmanCombatEnenmy();

private:
	C_MossmanCombatEnenmy(const C_MossmanCombatEnenmy &); // not defined, not accessible
};

IMPLEMENT_CLIENTCLASS_DT(C_MossmanCombatEnenmy, DT_NPC_MossmanCombatEnenmy, CNPC_MossmanCombatEnenmy)
END_RECV_TABLE()

C_MossmanCombatEnenmy::C_MossmanCombatEnenmy()
{
}

C_MossmanCombatEnenmy::~C_MossmanCombatEnenmy()
{
}

class C_GManCombatEnenmy : public C_AI_BaseNPC
{
public:
	DECLARE_CLASS(C_GManCombatEnenmy, C_AI_BaseNPC);
	DECLARE_CLIENTCLASS();

	C_GManCombatEnenmy();
	virtual			~C_GManCombatEnenmy();

private:
	C_GManCombatEnenmy(const C_GManCombatEnenmy &); // not defined, not accessible
};

IMPLEMENT_CLIENTCLASS_DT(C_GManCombatEnenmy, DT_NPC_GManCombatEnenmy, CNPC_GManCombatEnenmy)
END_RECV_TABLE()

C_GManCombatEnenmy::C_GManCombatEnenmy()
{
}

C_GManCombatEnenmy::~C_GManCombatEnenmy()
{
}

class C_BreenCombatEnenmy : public C_AI_BaseNPC
{
public:
	DECLARE_CLASS(C_BreenCombatEnenmy, C_AI_BaseNPC);
	DECLARE_CLIENTCLASS();

	C_BreenCombatEnenmy();
	virtual			~C_BreenCombatEnenmy();

private:
	C_BreenCombatEnenmy(const C_BreenCombatEnenmy &); // not defined, not accessible
};

IMPLEMENT_CLIENTCLASS_DT(C_BreenCombatEnenmy, DT_NPC_BreenCombatEnenmy, CNPC_BreenCombatEnenmy)
END_RECV_TABLE()

C_BreenCombatEnenmy::C_BreenCombatEnenmy()
{
}

C_BreenCombatEnenmy::~C_BreenCombatEnenmy()
{
}

class C_MagnussonCombatEnenmy : public C_AI_BaseNPC
{
public:
	DECLARE_CLASS(C_MagnussonCombatEnenmy, C_AI_BaseNPC);
	DECLARE_CLIENTCLASS();

	C_MagnussonCombatEnenmy();
	virtual			~C_MagnussonCombatEnenmy();

private:
	C_MagnussonCombatEnenmy(const C_MagnussonCombatEnenmy &); // not defined, not accessible
};

IMPLEMENT_CLIENTCLASS_DT(C_MagnussonCombatEnenmy, DT_NPC_MagnussonCombatEnenmy, CNPC_MagnussonCombatEnenmy)
END_RECV_TABLE()

C_MagnussonCombatEnenmy::C_MagnussonCombatEnenmy()
{
}

C_MagnussonCombatEnenmy::~C_MagnussonCombatEnenmy()
{
}


class C_KleinerCombat : public C_AI_BaseNPC
{
public:
	DECLARE_CLASS(C_KleinerCombat, C_AI_BaseNPC);
	DECLARE_CLIENTCLASS();

	C_KleinerCombat();
	virtual			~C_KleinerCombat();

private:
	C_KleinerCombat(const C_KleinerCombat &); // not defined, not accessible
};

IMPLEMENT_CLIENTCLASS_DT(C_KleinerCombat, DT_NPC_KleinerCombat, CNPC_KleinerCombat)
END_RECV_TABLE()

C_KleinerCombat::C_KleinerCombat()
{
}

C_KleinerCombat::~C_KleinerCombat()
{
}

class C_EliCombat : public C_AI_BaseNPC
{
public:
	DECLARE_CLASS(C_EliCombat, C_AI_BaseNPC);
	DECLARE_CLIENTCLASS();

	C_EliCombat();
	virtual			~C_EliCombat();

private:
	C_EliCombat(const C_EliCombat &); // not defined, not accessible
};

IMPLEMENT_CLIENTCLASS_DT(C_EliCombat, DT_NPC_EliCombat, CNPC_EliCombat)
END_RECV_TABLE()

C_EliCombat::C_EliCombat()
{
}

C_EliCombat::~C_EliCombat()
{
}

class C_MossmanCombat : public C_AI_BaseNPC
{
public:
	DECLARE_CLASS(C_MossmanCombat, C_AI_BaseNPC);
	DECLARE_CLIENTCLASS();

	C_MossmanCombat();
	virtual			~C_MossmanCombat();

private:
	C_MossmanCombat(const C_MossmanCombat &); // not defined, not accessible
};

IMPLEMENT_CLIENTCLASS_DT(C_MossmanCombat, DT_NPC_MossmanCombat, CNPC_MossmanCombat)
END_RECV_TABLE()

C_MossmanCombat::C_MossmanCombat()
{
}

C_MossmanCombat::~C_MossmanCombat()
{
}

class C_GManCombat : public C_AI_BaseNPC
{
public:
	DECLARE_CLASS(C_GManCombat, C_AI_BaseNPC);
	DECLARE_CLIENTCLASS();

	C_GManCombat();
	virtual			~C_GManCombat();

private:
	C_GManCombat(const C_GManCombat &); // not defined, not accessible
};

IMPLEMENT_CLIENTCLASS_DT(C_GManCombat, DT_NPC_GManCombat, CNPC_GManCombat)
END_RECV_TABLE()

C_GManCombat::C_GManCombat()
{
}

C_GManCombat::~C_GManCombat()
{
}

class C_BreenCombat : public C_AI_BaseNPC
{
public:
	DECLARE_CLASS(C_BreenCombat, C_AI_BaseNPC);
	DECLARE_CLIENTCLASS();

	C_BreenCombat();
	virtual			~C_BreenCombat();

private:
	C_BreenCombat(const C_BreenCombat &); // not defined, not accessible
};

IMPLEMENT_CLIENTCLASS_DT(C_BreenCombat, DT_NPC_BreenCombat, CNPC_BreenCombat)
END_RECV_TABLE()

C_BreenCombat::C_BreenCombat()
{
}

C_BreenCombat::~C_BreenCombat()
{
}

class C_MagnussonCombat : public C_AI_BaseNPC
{
public:
	DECLARE_CLASS(C_MagnussonCombat, C_AI_BaseNPC);
	DECLARE_CLIENTCLASS();

	C_MagnussonCombat();
	virtual			~C_MagnussonCombat();

private:
	C_MagnussonCombat(const C_MagnussonCombat &); // not defined, not accessible
};

IMPLEMENT_CLIENTCLASS_DT(C_MagnussonCombat, DT_NPC_MagnussonCombat, CNPC_MagnussonCombat)
END_RECV_TABLE()

C_MagnussonCombat::C_MagnussonCombat()
{
}

C_MagnussonCombat::~C_MagnussonCombat()
{
}


class C_AlyxCombatEnenmy : public C_AI_BaseNPC
{
public:
	DECLARE_CLASS(C_AlyxCombatEnenmy, C_AI_BaseNPC);
	DECLARE_CLIENTCLASS();

	C_AlyxCombatEnenmy();
	virtual			~C_AlyxCombatEnenmy();

private:
	C_AlyxCombatEnenmy(const C_AlyxCombatEnenmy &); // not defined, not accessible
};

IMPLEMENT_CLIENTCLASS_DT(C_AlyxCombatEnenmy, DT_NPC_AlyxCombatEnenmy, CNPC_AlyxCombatEnenmy)
END_RECV_TABLE()

C_AlyxCombatEnenmy::C_AlyxCombatEnenmy()
{
}

C_AlyxCombatEnenmy::~C_AlyxCombatEnenmy()
{
}

class C_AlyxCombat : public C_AI_BaseNPC
{
public:
	DECLARE_CLASS(C_AlyxCombat, C_AI_BaseNPC);
	DECLARE_CLIENTCLASS();

	C_AlyxCombat();
	virtual			~C_AlyxCombat();

private:
	C_AlyxCombat(const C_AlyxCombat &); // not defined, not accessible
};

IMPLEMENT_CLIENTCLASS_DT(C_AlyxCombat, DT_NPC_AlyxCombat, CNPC_AlyxCombat)
END_RECV_TABLE()

C_AlyxCombat::C_AlyxCombat()
{
}

C_AlyxCombat::~C_AlyxCombat()
{
}


class C_BarneyCombatEnenmy : public C_AI_BaseNPC
{
public:
	DECLARE_CLASS(C_BarneyCombatEnenmy, C_AI_BaseNPC);
	DECLARE_CLIENTCLASS();

	C_BarneyCombatEnenmy();
	virtual			~C_BarneyCombatEnenmy();

private:
	C_BarneyCombatEnenmy(const C_BarneyCombatEnenmy &); // not defined, not accessible
};

IMPLEMENT_CLIENTCLASS_DT(C_BarneyCombatEnenmy, DT_NPC_BarneyCombatEnenmy, CNPC_BarneyCombatEnenmy)
END_RECV_TABLE()

C_BarneyCombatEnenmy::C_BarneyCombatEnenmy()
{
}

C_BarneyCombatEnenmy::~C_BarneyCombatEnenmy()
{
}

class C_BarneyCombat : public C_AI_BaseNPC
{
public:
	DECLARE_CLASS(C_BarneyCombat, C_AI_BaseNPC);
	DECLARE_CLIENTCLASS();

	C_BarneyCombat();
	virtual			~C_BarneyCombat();

private:
	C_BarneyCombat(const C_BarneyCombat &); // not defined, not accessible
};

IMPLEMENT_CLIENTCLASS_DT(C_BarneyCombat, DT_NPC_BarneyCombat, CNPC_BarneyCombat)
END_RECV_TABLE()

C_BarneyCombat::C_BarneyCombat()
{
}

C_BarneyCombat::~C_BarneyCombat()
{
}


class C_OdessaCombatEnenmy : public C_AI_BaseNPC
{
public:
	DECLARE_CLASS(C_OdessaCombatEnenmy, C_AI_BaseNPC);
	DECLARE_CLIENTCLASS();

	C_OdessaCombatEnenmy();
	virtual			~C_OdessaCombatEnenmy();

private:
	C_OdessaCombatEnenmy(const C_OdessaCombatEnenmy &); // not defined, not accessible
};

IMPLEMENT_CLIENTCLASS_DT(C_OdessaCombatEnenmy, DT_NPC_OdessaCombatEnenmy, CNPC_OdessaCombatEnenmy)
END_RECV_TABLE()

C_OdessaCombatEnenmy::C_OdessaCombatEnenmy()
{
}

C_OdessaCombatEnenmy::~C_OdessaCombatEnenmy()
{
}

class C_OdessaCombat : public C_AI_BaseNPC
{
public:
	DECLARE_CLASS(C_OdessaCombat, C_AI_BaseNPC);
	DECLARE_CLIENTCLASS();

	C_OdessaCombat();
	virtual			~C_OdessaCombat();

private:
	C_OdessaCombat(const C_OdessaCombat &); // not defined, not accessible
};

IMPLEMENT_CLIENTCLASS_DT(C_OdessaCombat, DT_NPC_OdessaCombat, CNPC_OdessaCombat)
END_RECV_TABLE()

C_OdessaCombat::C_OdessaCombat()
{
}

C_OdessaCombat::~C_OdessaCombat()
{
}


class C_CombineAlly : public C_AI_BaseNPC
{
public:
	DECLARE_CLASS(C_CombineAlly, C_AI_BaseNPC);
	DECLARE_CLIENTCLASS();

	C_CombineAlly();
	virtual			~C_CombineAlly();

private:
	C_CombineAlly(const C_CombineAlly &); // not defined, not accessible
};

IMPLEMENT_CLIENTCLASS_DT(C_CombineAlly, DT_NPC_CombineAlly, CNPC_CombineAlly)
END_RECV_TABLE()

C_CombineAlly::C_CombineAlly()
{
}

C_CombineAlly::~C_CombineAlly()
{
}


class C_CitizenEnenmy : public C_AI_BaseNPC
{
public:
	DECLARE_CLASS(C_CitizenEnenmy, C_AI_BaseNPC);
	DECLARE_CLIENTCLASS();

	C_CitizenEnenmy();
	virtual			~C_CitizenEnenmy();

private:
	C_CitizenEnenmy(const C_CitizenEnenmy &); // not defined, not accessible
};

IMPLEMENT_CLIENTCLASS_DT(C_CitizenEnenmy, DT_NPC_CitizenEnenmy, CNPC_CitizenEnenmy)
END_RECV_TABLE()

C_CitizenEnenmy::C_CitizenEnenmy()
{
}

C_CitizenEnenmy::~C_CitizenEnenmy()
{
}

class C_MetropoliceAlly : public C_AI_BaseNPC
{
public:
	DECLARE_CLASS(C_MetropoliceAlly, C_AI_BaseNPC);
	DECLARE_CLIENTCLASS();

	C_MetropoliceAlly();
	virtual			~C_MetropoliceAlly();

private:
	C_MetropoliceAlly(const C_MetropoliceAlly &); // not defined, not accessible
};

IMPLEMENT_CLIENTCLASS_DT(C_MetropoliceAlly, DT_NPC_MetropoliceAlly, CNPC_MetropoliceAlly)
END_RECV_TABLE()

C_MetropoliceAlly::C_MetropoliceAlly()
{
}

C_MetropoliceAlly::~C_MetropoliceAlly()
{
}

class C_GordonEnenmy : public C_AI_BaseNPC
{
public:
	DECLARE_CLASS(C_GordonEnenmy, C_AI_BaseNPC);
	DECLARE_CLIENTCLASS();

	C_GordonEnenmy();
	virtual			~C_GordonEnenmy();

private:
	C_GordonEnenmy(const C_GordonEnenmy &); // not defined, not accessible
};

IMPLEMENT_CLIENTCLASS_DT(C_GordonEnenmy, DT_NPC_GordonEnenmy, CNPC_GordonEnenmy)
END_RECV_TABLE()

C_GordonEnenmy::C_GordonEnenmy()
{
}

C_GordonEnenmy::~C_GordonEnenmy()
{
}

class C_Gordon : public C_AI_BaseNPC
{
public:
	DECLARE_CLASS(C_Gordon, C_AI_BaseNPC);
	DECLARE_CLIENTCLASS();

	C_Gordon();
	virtual			~C_Gordon();

private:
	C_Gordon(const C_Gordon &); // not defined, not accessible
};

IMPLEMENT_CLIENTCLASS_DT(C_Gordon, DT_NPC_Gordon, CNPC_Gordon)
END_RECV_TABLE()

C_Gordon::C_Gordon()
{
}

C_Gordon::~C_Gordon()
{
}




class C_MonkCombatEnenmy : public C_AI_BaseNPC
{
public:
	DECLARE_CLASS(C_MonkCombatEnenmy, C_AI_BaseNPC);
	DECLARE_CLIENTCLASS();

	C_MonkCombatEnenmy();
	virtual			~C_MonkCombatEnenmy();

private:
	C_MonkCombatEnenmy(const C_MonkCombatEnenmy &); // not defined, not accessible
};

IMPLEMENT_CLIENTCLASS_DT(C_MonkCombatEnenmy, DT_NPC_MonkCombatEnenmy, CNPC_MonkCombatEnenmy)
END_RECV_TABLE()

C_MonkCombatEnenmy::C_MonkCombatEnenmy()
{
}

C_MonkCombatEnenmy::~C_MonkCombatEnenmy()
{
}

class C_MonkCombat : public C_AI_BaseNPC
{
public:
	DECLARE_CLASS(C_MonkCombat, C_AI_BaseNPC);
	DECLARE_CLIENTCLASS();

	C_MonkCombat();
	virtual			~C_MonkCombat();

private:
	C_MonkCombat(const C_MonkCombat &); // not defined, not accessible
};

IMPLEMENT_CLIENTCLASS_DT(C_MonkCombat, DT_NPC_MonkCombat, CNPC_MonkCombat)
END_RECV_TABLE()

C_MonkCombat::C_MonkCombat()
{
}

C_MonkCombat::~C_MonkCombat()
{
}
