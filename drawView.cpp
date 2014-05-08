// drawView.cpp : implementation of the CDrawView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "drawView.h"
#include "Line.h"
using namespace Gdiplus;

CDrawView::CDrawView()
	: m_BitmapImage(1024, 768)
	, m_GraphicsImage(&m_BitmapImage)
	, m_Pen(Color(0,0,0))
	, current_shape(0)
	, saved_or_loaded(false)
{
}

BOOL CDrawView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

LRESULT CDrawView::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CPaintDC dc(m_hWnd);

	Graphics graphics(dc);
	graphics.DrawImage(&m_BitmapImage, 0, 0);

	if (shape)
	{
		shape->draw(graphics);
	}

	return 0;
}

LRESULT CDrawView::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_start.X = GET_X_LPARAM(lParam);
	m_start.Y = GET_Y_LPARAM(lParam);

	shape = ShapeMaker::makeShape(current_shape);

	// Gets the current color of pen
	Gdiplus::Color currentColor;
	m_Pen.GetColor(&currentColor);

	// Flyweight: Gets a shared pointer to Pen from singleton
	std::shared_ptr<Pen> myPen = PenWeight::get().getPen(currentColor, m_Pen.GetWidth());
	shape->setPen(myPen);
	shape->setStartPoint(m_start);

	return 0;
}

LRESULT CDrawView::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_end.X = GET_X_LPARAM(lParam);
	m_end.Y = GET_Y_LPARAM(lParam);

	if (shape)
	{

		shape->setEndPoint(m_end);
		shape->draw(m_GraphicsImage);
		RedrawWindow();

		undo_list.push_back(shape);
		redo_list.clear();
		shape.reset();
	}

	return 0;
}

LRESULT CDrawView::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (shape)
	{
		m_end.X = GET_X_LPARAM(lParam);
		m_end.Y = GET_Y_LPARAM(lParam);

		shape->setEndPoint(m_end);

		RedrawWindow();
	}
	return 0;
}
