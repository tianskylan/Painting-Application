// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "drawView.h"
#include "MainFrm.h"

#include <string>
#include <fstream>

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if (CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
		return TRUE;

	return m_view.PreTranslateMessage(pMsg);
}

BOOL CMainFrame::OnIdle()
{
	UIUpdateToolBar();
	return FALSE;
}

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// create command bar window
	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	// attach menu
	m_CmdBar.AttachMenu(GetMenu());
	// load command bar images
	m_CmdBar.LoadImages(IDR_MAINFRAME);
	// remove old menu
	SetMenu(NULL);

	HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndCmdBar);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

	CreateSimpleStatusBar();

	m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL, WS_EX_CLIENTEDGE);
	m_view.SetScrollSize(1, 1);
	UIAddToolBar(hWndToolBar);
	UISetCheck(ID_VIEW_TOOLBAR, 1);
	UISetCheck(ID_VIEW_STATUS_BAR, 1);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	return 0;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	bHandled = FALSE;
	return 1;
}

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}

LRESULT CMainFrame::OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: add code to initialize document
	m_view.m_GraphicsImage.Clear(Gdiplus::Color(255, 255, 255));
	m_view.RedrawWindow();

	m_view.undo_list.clear();
	m_view.redo_list.clear();

	m_view.saved_or_loaded = false;
	m_view.filename.clear();

	return 0;
}

LRESULT CMainFrame::OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static BOOL bVisible = TRUE;	// initially visible
	bVisible = !bVisible;
	CReBarCtrl rebar = m_hWndToolBar;
	int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
	rebar.ShowBand(nBandIndex, bVisible);
	UISetCheck(ID_VIEW_TOOLBAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
	::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
	UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainFrame::OnPenColor(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// Initialize this to current color
	Gdiplus::Color currentColor;
	m_view.m_Pen.GetColor(&currentColor);

	COLORREF color = RGB(currentColor.GetR(),currentColor.GetG(),currentColor.GetB());
	// Create dialog box
	CColorDialog test(color);
	test.DoModal();
	// Grab color selected
	color = test.GetColor();
	Gdiplus::Color newColor(GetRValue(color), GetGValue(color), GetBValue(color));

	m_view.m_Pen.SetColor(newColor);

	return 0;
}

LRESULT CMainFrame::OnPenWidth(WORD /*wNotifyCode*/, WORD wID/*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// Remove all the checkmarks
	m_CmdBar.GetMenu().CheckMenuItem(ID_WIDTH_0, MF_UNCHECKED);
	m_CmdBar.GetMenu().CheckMenuItem(ID_WIDTH_1, MF_UNCHECKED);
	m_CmdBar.GetMenu().CheckMenuItem(ID_WIDTH_2, MF_UNCHECKED);
	m_CmdBar.GetMenu().CheckMenuItem(ID_WIDTH_3, MF_UNCHECKED);
	m_CmdBar.GetMenu().CheckMenuItem(ID_WIDTH_4, MF_UNCHECKED);
	m_CmdBar.GetMenu().CheckMenuItem(ID_WIDTH_5, MF_UNCHECKED);

	// Add checkmark to wID
	m_CmdBar.GetMenu().CheckMenuItem(wID, MF_CHECKED);

	// Set pen width accordingly
	switch (wID)
	{
	case ID_WIDTH_0:
		m_view.m_Pen.SetWidth(0.5);
		break;
	case ID_WIDTH_1:
		m_view.m_Pen.SetWidth(1);
		break;
	case ID_WIDTH_2:
		m_view.m_Pen.SetWidth(1.5);
		break;
	case ID_WIDTH_3:
		m_view.m_Pen.SetWidth(2.0);
		break;
	case ID_WIDTH_4:
		m_view.m_Pen.SetWidth(2.5);
		break;
	case ID_WIDTH_5:
		m_view.m_Pen.SetWidth(5);
		break;
	}

	return 0;
}

LRESULT CMainFrame::OnUndo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (m_view.undo_list.size() > 0)
	{
		auto item = m_view.undo_list.back();
		m_view.undo_list.pop_back();

		m_view.redo_list.push_back(item);

		m_view.m_GraphicsImage.Clear(Gdiplus::Color(255, 255, 255));

		for (auto it = m_view.undo_list.begin(); it != m_view.undo_list.end(); ++it)
		{
			it->get()->draw(m_view.m_GraphicsImage);
		}

		m_view.RedrawWindow();
	}

	return 0;
}

LRESULT CMainFrame::OnRedo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (m_view.redo_list.size() > 0)
	{
		auto item = m_view.redo_list.back();
		m_view.redo_list.pop_back();

		m_view.undo_list.push_back(item);

		item.get()->draw(m_view.m_GraphicsImage);

		m_view.RedrawWindow();
	}

	return 0;
}

LRESULT CMainFrame::OnLine(WORD /*wNotifyCode*/, WORD wID/*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// Set current shape to be line
	m_view.current_shape = 0;

	// Remove all the checkmarks
	m_CmdBar.GetMenu().CheckMenuItem(ID_SHAPE_LINE, MF_UNCHECKED);
	m_CmdBar.GetMenu().CheckMenuItem(ID_SHAPE_ELLIPSE, MF_UNCHECKED);
	m_CmdBar.GetMenu().CheckMenuItem(ID_SHAPE_RECTANGLE, MF_UNCHECKED);

	// Add checkmark to wID
	m_CmdBar.GetMenu().CheckMenuItem(wID, MF_CHECKED);

	return 0;
}

LRESULT CMainFrame::OnEllipse(WORD /*wNotifyCode*/, WORD wID/*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// Set current shape to be Ellipse
	m_view.current_shape = 1;

	// Remove all the checkmarks
	m_CmdBar.GetMenu().CheckMenuItem(ID_SHAPE_LINE, MF_UNCHECKED);
	m_CmdBar.GetMenu().CheckMenuItem(ID_SHAPE_ELLIPSE, MF_UNCHECKED);
	m_CmdBar.GetMenu().CheckMenuItem(ID_SHAPE_RECTANGLE, MF_UNCHECKED);

	// Add checkmark to wID
	m_CmdBar.GetMenu().CheckMenuItem(wID, MF_CHECKED);

	return 0;
}

LRESULT CMainFrame::OnRectangle(WORD /*wNotifyCode*/, WORD wID/*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// Set current shape to be Rectangle
	m_view.current_shape = 2;

	// Remove all the checkmarks
	m_CmdBar.GetMenu().CheckMenuItem(ID_SHAPE_LINE, MF_UNCHECKED);
	m_CmdBar.GetMenu().CheckMenuItem(ID_SHAPE_ELLIPSE, MF_UNCHECKED);
	m_CmdBar.GetMenu().CheckMenuItem(ID_SHAPE_RECTANGLE, MF_UNCHECKED);

	// Add checkmark to wID
	m_CmdBar.GetMenu().CheckMenuItem(wID, MF_CHECKED);

	return 0;
}

LRESULT CMainFrame::OnFileSave(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (m_view.saved_or_loaded) // if file has already been loaded or saved
	{
		// Ugly code snippet to save to file "filename"
		CLSID pngClsid;
		CLSIDFromString(L"{557cf406-1a04-11d3-9a73-0000f81ef32e}", &pngClsid);
		m_view.m_BitmapImage.Save(CA2W(m_view.filename.c_str()), &pngClsid);
	}
	else // file has not been saved before
	{
		OnFileSaveAs(wNotifyCode, wID, hWndCtl, bHandled);
	}

	return 0;
}

LRESULT CMainFrame::OnFileSaveAs(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// opens a dialog for save file, does not require file exists
	CFileDialog dialog(false);

	if (IDOK == dialog.DoModal())
	{
		m_view.filename = dialog.m_szFileName;
	}

	// append png to filename
	m_view.filename.append(".png");

	// ugly code snippet to save to file
	CLSID pngClsid;
	CLSIDFromString(L"{557cf406-1a04-11d3-9a73-0000f81ef32e}", &pngClsid);
	m_view.m_BitmapImage.Save(CA2W(m_view.filename.c_str()), &pngClsid);

	// set saved flag
	m_view.saved_or_loaded = true;

	return 0;
}

LRESULT CMainFrame::OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	std::string name;

	// opens a dialog for save file, REQUIRE file exists
	CFileDialog dialog(true);

	if (IDOK == dialog.DoModal())
	{
		name = dialog.m_szFileName;

		// load the image file
		Gdiplus::Bitmap myFile(CA2W(name.c_str()));
		m_view.m_GraphicsImage.DrawImage(&myFile, 0, 0);
	}

	m_view.RedrawWindow();

	m_view.saved_or_loaded = true;
	m_view.filename = name;
	m_view.undo_list.clear();
	m_view.redo_list.clear();

	return 0;
}