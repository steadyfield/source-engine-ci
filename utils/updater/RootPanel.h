//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef MDRIPPERMAIN_H
#define MDRIPPERMAIN_H
#ifdef _WIN32
#pragma once
#endif

#include "matsys_controls/QCGenerator.h"
#include <vgui_controls/Frame.h>
#include <FileSystem.h>
#include "vgui/IScheme.h"

using namespace vgui;


//--------------------------------------------------------------------------------------------------------------
/**
 * Status of the download thread, as set in RequestContext::status.
 */
enum HTTPStatus_t
{
	HTTP_INVALID = -1,
	HTTP_CONNECTING = 0,///< This is set in the main thread before the download thread starts.
	HTTP_FETCH,			///< The download thread sets this when it starts reading data.
	HTTP_DONE,			///< The download thread sets this if it has read all the data successfully.
	HTTP_ABORTED,		///< The download thread sets this if it aborts because it's RequestContext::shouldStop has been set.
	HTTP_ERROR			///< The download thread sets this if there is an error connecting or downloading.  Partial data may be present, so the main thread can check.
};

//--------------------------------------------------------------------------------------------------------------
/**
 * Error encountered in the download thread, as set in RequestContext::error.
 */
enum HTTPError_t
{
	HTTP_ERROR_NONE = 0,
	HTTP_ERROR_ZERO_LENGTH_FILE,
	HTTP_ERROR_CONNECTION_CLOSED,
	HTTP_ERROR_INVALID_URL,			///< InternetCrackUrl failed
	HTTP_ERROR_INVALID_PROTOCOL,	///< URL didn't start with http:// or https://
	HTTP_ERROR_CANT_BIND_SOCKET,
	HTTP_ERROR_CANT_CONNECT,
	HTTP_ERROR_NO_HEADERS,			///< Cannot read HTTP headers
	HTTP_ERROR_FILE_NONEXISTENT,
	HTTP_ERROR_MAX
};

enum { BufferSize = 256 };	///< BufferSize is used extensively within the download system to size char buffers.

//-----------------------------------------------------------------------------
// Purpose: Main dialog for media browser
//-----------------------------------------------------------------------------
class RootPanel : public Frame
{
	DECLARE_CLASS_SIMPLE(RootPanel, Frame );

public:

	RootPanel(Panel *parent, const char *name );
	virtual ~RootPanel();
	void SetProgress(float p);
		
protected:
	
	virtual void OnClose();
	virtual void OnCommand( const char *command );
	virtual void PerformLayout();

private:

	vgui::Button* m_pUpdateButton;
	vgui::ProgressBar* m_pUpdateProgress;
};


extern RootPanel* g_pRootPanel;


struct RequestContext_t
{
	inline RequestContext_t()
	{
		memset(this, 0, sizeof(RequestContext_t));
	}

	/**
	 * The main thread sets this when it wants to abort the download,
	 * or it is done reading data from a finished download.
	 */
	bool			shouldStop;

	/**
	 * The download thread sets this when it is exiting, so the main thread
	 * can delete the RequestContext_t.
	 */
	bool			threadDone;

	bool			bIsBZ2;					///< true if the file is a .bz2 file that should be uncompressed at the end of the download.  Set and used by main thread.
	bool			bAsHTTP;				///< true if downloaded via HTTP and not ingame protocol.  Set and used by main thread
	unsigned int	nRequestID;				///< request ID for ingame download

	HTTPStatus_t	status;					///< Download thread status
	DWORD			fetchStatus;			///< Detailed status info for the download
	HTTPError_t		error;					///< Detailed error info

	char			baseURL[BufferSize];	///< Base URL (including http://).  Set by main thread.
	char			urlPath[BufferSize];	///< Path to be appended to base URL.  Set by main thread.
	char			serverURL[BufferSize];	///< Server URL (IP:port, loopback, etc).  Set by main thread, and used for HTTP Referer header.


	/**
	 * The file's timestamp, as returned in the HTTP Last-Modified header.
	 * Saved to ensure partial download resumes match the original cached data.
	 */
	char			cachedTimestamp[BufferSize];

	DWORD			nBytesTotal;			///< Total bytes in the file
	DWORD			nBytesCurrent;			///< Current bytes downloaded
	DWORD			nBytesCached;			///< Amount of data present in cacheData.

	/**
	 * Buffer for the full file data.  Allocated/deleted by the download thread
	 * (which idles until the data is not needed anymore)
	 */
	unsigned char* data;

	/**
	 * Buffer for partial data from previous failed downloads.
	 * Allocated/deleted by the main thread (deleted after download thread is done)
	 */
	unsigned char* cacheData;

	// Used purely by the download thread - internal data -------------------
	void* hOpenResource;			///< Handle created by InternetOpen
	void* hDataResource;			///< Handle created by InternetOpenUrl

	/**
	 * For any user data we may want to attach to a context
	 */
	void* pUserData;
};



struct RequestContext_t;
class CHttpDownloader;
class KeyValues;

//----------------------------------------------------------------------------------------

class IDownloadHandler
{
public:
	virtual void		OnConnecting(CHttpDownloader* pDownloader) = 0;
	virtual void		OnFetch(CHttpDownloader* pDownloader) = 0;

	// Called when the download is done successfully, errors out, or is aborted.
	// NOTE: pDownloader should NOT be deleted from within OnDownloadComplete().
	// pData contains the downloaded data for processing.
	virtual void		OnDownloadComplete(CHttpDownloader* pDownloader, const unsigned char* pData) = 0;
};


class UpdaterDownloadHandler : public IDownloadHandler
{
public:
	virtual void		OnConnecting(CHttpDownloader* pDownloader);
	virtual void		OnFetch(CHttpDownloader* pDownloader);

	// Called when the download is done successfully, errors out, or is aborted.
	// NOTE: pDownloader should NOT be deleted from within OnDownloadComplete().
	// pData contains the downloaded data for processing.
	virtual void		OnDownloadComplete(CHttpDownloader* pDownloader, const unsigned char* pData);

	char FileUrl[1024];
};



class GameDownloadHandler : public IDownloadHandler
{
public:
	virtual void		OnConnecting(CHttpDownloader* pDownloader);
	virtual void		OnFetch(CHttpDownloader* pDownloader);

	// Called when the download is done successfully, errors out, or is aborted.
	// NOTE: pDownloader should NOT be deleted from within OnDownloadComplete().
	// pData contains the downloaded data for processing.
	virtual void		OnDownloadComplete(CHttpDownloader* pDownloader, const unsigned char* pData);
};
//----------------------------------------------------------------------------------------

//
// Generic downloader class - downloads a single file on its own thread, and maintains
// state for that data.
//
// TODO: Derive from CBaseThinker and remove explicit calls to Think() - will make this
// class less bug-prone (easy to forget to call Think()).
//
class CHttpDownloader
{
public:
	// Pass in a callback 
	CHttpDownloader(IDownloadHandler* pHandler = NULL);
	~CHttpDownloader();

	//
	// Download the file at the given URL (HTTP/HTTPS support only)
	// pGamePath - Game path where we should put the file - can be NULL if we don't
	//    want to save to the file to disk
	// pUserData - Passed back to IDownloadHandler, if one has been set - can be NULL
	// pBytesDownloaded - If non-NULL, # of bytes downloaded written.
	// Returns true on success.
	//
	bool				BeginDownload(const char* pURL, const char* pGamePath = NULL,
		void* pUserData = NULL, uint32* pBytesDownloaded = NULL);

	//
	// Abort the download (if there is one), wait for the download to shutdown and
	// do cleanup.
	//
	void				AbortDownloadAndCleanup();

	inline bool			IsDone() const { return m_bDone; }	// Download done?
	inline bool			CanDelete() const { return m_pThreadState == NULL; }	// Can free?
	inline HTTPStatus_t	GetStatus() const { return m_nHttpStatus; }
	inline HTTPError_t	GetError() const { return m_nHttpError; }
	inline void* GetUserData() const { return m_pUserData; }
	inline uint32		GetBytesDownloaded() const { return m_uBytesDownloaded; }
	inline uint32		GetSize() const { return m_uSize; }	// File size in bytes - NOTE: Not valid until the download is complete, aborted, or errored out
	inline const char* GetURL() const { return m_szURL; }

	void				Think();

	KeyValues* GetOgsRow(int nErrorCounter) const;

	static const char* GetHttpErrorToken(HTTPError_t nError);

private:
	bool				CleanupThreadIfDone();
	void				InvokeHandler();

	RequestContext_t* m_pThreadState;
	float				m_flNextThinkTime;
	bool				m_bDone;
	HTTPError_t			m_nHttpError;
	HTTPStatus_t		m_nHttpStatus;
	uint32				m_uBytesDownloaded;
	uint32* m_pBytesDownloaded;	// Passed into BeginDownload()
	uint32				m_uSize;
	IDownloadHandler* m_pHandler;
	void* m_pUserData;
	char				m_szURL[512];

	// Use this to make sure that AbortDownloadAndCleanup isn't executed simultaneously
	// by two threads. This was causing use-after-free crashes during shutdown.
	CThreadMutex		m_lock;
};


#endif // MDRIPPERMAIN_H
