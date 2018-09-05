
// Main.h : header file
//

#pragma once
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include "glm/glm.hpp"
#include "matrix_transform.hpp"
#include "afxcmn.h"
#include "Camera.h"

// glm provides vector, matrix classes like glsl
// Typedefs to make code more readable 

typedef glm::mat3 mat3;
typedef glm::mat4 mat4;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;

//const float pi = 3.14159265; // For portability across platforms

// RayTracerDlg dialog
class RayTracerDlg : public CDialogEx
{
// Construction
public:
	RayTracerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION7_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg
		
	static void Raytrace(Camera * cam);
	void OnBnClickedOk();
	CProgressCtrl RenderProgress;
	afx_msg void OnBnClickedRenderspheres();
};
