//
// DiscoveryServices.h
//
// Copyright (c) Shareaza Development Team, 2002-2012.
// This file is part of SHAREAZA (shareaza.sourceforge.net)
//
// Shareaza is free software; you can redistribute it
// and/or modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// Shareaza is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Shareaza; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#pragma once

#include "HttpRequest.h"


// TODO: Remove Network locks
// TODO: Add support for concurrent webcache requests


class CDiscoveryService
{
public:
	enum Type
	{
		dsNull, dsGnutella, dsWebCache, dsServerMet, dsBlocked
	};

	enum SubType
	{
		dsOldBootStrap, dsGnutellaTCP, dsGnutella2TCP, dsGnutellaUDPHC, dsGnutella2UDPKHL
	};

	CDiscoveryService(Type nType = dsNull, LPCTSTR pszAddress = NULL);
	virtual ~CDiscoveryService();

	Type		m_nType;
	CString		m_sAddress;
	BOOL		m_bGnutella2;			// Webcache supports Gnutella 2
	BOOL		m_bGnutella1;			// Webcache supports Gnutella
	DWORD		m_tCreated;
	DWORD		m_tAccessed;
	DWORD		m_nAccesses;
	DWORD		m_tUpdated;
	DWORD		m_nUpdates;
	DWORD		m_nHosts;
	DWORD		m_nTotalHosts;
	DWORD		m_nURLs;
	DWORD		m_nTotalURLs;
	DWORD		m_nFailures;
	DWORD		m_nAccessPeriod;
	DWORD		m_nUpdatePeriod;
	SubType		m_nSubType;
	IN_ADDR		m_pAddress;
	WORD		m_nPort;
	CString		m_sPong;

	void		Remove(BOOL bCheck = TRUE);
	void		OnSuccess();
	void		OnFailure();
	void		OnCopyGiven();			//Used in Datagrams.cpp

protected:
	void		OnAccess();
	void		OnGivenHosts();
	void		OnHostAdd(int nCount = 1);
	void		OnURLAdd(int nCount = 1);
	void		Serialize(CArchive& ar, int nVersion);
	BOOL		ResolveGnutella();

	friend class CDiscoveryServices;	
};

class CDiscoveryServices : public CThreadImpl
{
public:
	CDiscoveryServices();
	virtual ~CDiscoveryServices();
	
	enum Mode
	{
		wcmHosts, wcmCaches, wcmUpdate, wcmSubmit, wcmServerMet
	};

protected:
	CList< CDiscoveryService* > m_pList;
	CHttpRequest		m_pRequest;
	CDiscoveryService*	m_pWebCache;
	Mode				m_nWebCache;
	CDiscoveryService*	m_pSubmit;
	PROTOCOLID			m_nLastQueryProtocol;		// Protocol that was queried most recently
	DWORD				m_tUpdated;					// Time a webcache was last updated
	PROTOCOLID			m_nLastUpdateProtocol;		// Protocol that had a service update most recently
	BOOL				m_bFirstTime;
	DWORD				m_tExecute;					// Time the Execute() function was last run
	DWORD				m_tQueried;					// Time a webcache/MET was last queried
	DWORD				m_tMetQueried;				// Time a MET was last queried

public:
	POSITION			GetIterator() const;
	CDiscoveryService*	GetNext(POSITION& pos) const;
	BOOL				Check(CDiscoveryService* pService, CDiscoveryService::Type nType = CDiscoveryService::dsNull) const;
	BOOL				Add(CDiscoveryService* pService);
	BOOL				Add(LPCTSTR pszAddress, int nType, PROTOCOLID nProtocol = PROTOCOL_NULL);
	BOOL				CheckMinimumServices();
//	DWORD				MetQueried() const;
	DWORD				LastExecute() const;
	CDiscoveryService*	GetByAddress(LPCTSTR pszAddress) const;
	CDiscoveryService*	GetByAddress(const IN_ADDR* pAddress, WORD nPort, CDiscoveryService::SubType nSubType );
	BOOL				Load();
	BOOL				Save();
	BOOL				Update();
	BOOL				Execute(BOOL bDiscovery, PROTOCOLID nProtocol, USHORT nForceDiscovery);
	void				Stop();
	void				Clear();
	BOOL				Execute(CDiscoveryService* pService, Mode nMode);
	void				OnResolve(PROTOCOLID nProtocol, LPCTSTR szAddress, const IN_ADDR* pAddress = NULL, WORD nPort = 0);

protected:
	void				Remove(CDiscoveryService* pService, BOOL bCheck = TRUE);
	DWORD				GetCount(int nType = 0, PROTOCOLID nProtocol = PROTOCOL_NULL) const;
	BOOL				CheckWebCacheValid(LPCTSTR pszAddress);
	int					ExecuteBootstraps( int nCount, BOOL bUDP = FALSE, PROTOCOLID nProtocol = PROTOCOL_NULL );
	void				Serialize(CArchive& ar);
	BOOL				RequestRandomService(PROTOCOLID nProtocol);	
	CDiscoveryService*  GetRandomService(PROTOCOLID nProtocol);
	CDiscoveryService*	GetRandomWebCache(PROTOCOLID nProtocol, BOOL bWorkingOnly, CDiscoveryService* pExclude = NULL, BOOL bForUpdate = FALSE);
	BOOL				RequestWebCache(CDiscoveryService* pService, Mode nMode, PROTOCOLID nProtocol);
	void				OnRun();
	BOOL				RunWebCacheGet(BOOL bCache);
	BOOL				RunWebCacheUpdate();
	BOOL				RunServerMet();
	BOOL				SendWebCacheRequest(CString strURL, CString& strOutput);
	BOOL				EnoughServices() const;
	void				AddDefaults();
	void				MergeURLs();

	friend class CDiscoveryService;

private:
	CDiscoveryServices(const CDiscoveryServices&);
	CDiscoveryServices& operator=(const CDiscoveryServices&);
};

extern CDiscoveryServices DiscoveryServices;
