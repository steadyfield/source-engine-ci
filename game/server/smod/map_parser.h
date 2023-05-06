#include "igamesystem.h"

class CMapScriptParser : public CAutoGameSystem
{
public:
	CMapScriptParser( char const *name ) : CAutoGameSystem( name )
	{
	}

	virtual void LevelInitPostEntity( void );
	virtual void LevelShutdownPostEntity( void );
	virtual void ParseScriptFile( const char* filename, bool official = false );
	virtual void ParseEntities( KeyValues *keyvalues );

	KeyValues* m_pMapScript;
};
extern CMapScriptParser *GetMapScriptParser();
