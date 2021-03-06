//
// DlgHitColumns.h
//
// Copyright (c) Shareaza Development Team, 2002-2007.
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

#include "DlgSkinDialog.h"
#include "CtrlSchemaCombo.h"
#include "Schema.h"


class CSchemaColumnsDlg : public CSkinDialog
{
// Construction
public:
	CSchemaColumnsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
public:
	//{{AFX_DATA(CSchemaColumnsDlg)
	enum { IDD = IDD_SCHEMA_COLUMNS };
	CListCtrl	m_wndColumns;
	CSchemaCombo	m_wndSchemas;
	//}}AFX_DATA

// Attributes
public:
	CSchemaPtr	m_pSchema;
	CList< CSchemaMember* >	m_pColumns;

// Operations
public:
	static BOOL		LoadColumns(CSchemaPtr pSchema, CList< CSchemaMember* >* pColumns);
	static BOOL		SaveColumns(CSchemaPtr pSchema, CList< CSchemaMember* >* pColumns);
	static CMenu*	BuildColumnMenu(CSchemaPtr pSchema, CList< CSchemaMember* >* pColumns = NULL);
	static BOOL		ToggleColumnHelper(CSchemaPtr pSchema, CList< CSchemaMember* >* pSource, CList< CSchemaMember* >* pTarget, UINT nToggleID, BOOL bSave = FALSE);

// Overrides
public:
	//{{AFX_VIRTUAL(CSchemaColumnsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CSchemaColumnsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelChangeSchemas();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
