// SixAxisSensorInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "motor test 20151013.h"
#include "SixAxisSensorInfoDlg.h"
#include "afxdialogex.h"


// CSixAxisSensorInfoDlg 对话框

IMPLEMENT_DYNAMIC(CSixAxisSensorInfoDlg, CDialogEx)

CSixAxisSensorInfoDlg::CSixAxisSensorInfoDlg(FTWrapper* ft,CWnd* pParent /*=NULL*/)
	: CDialogEx(CSixAxisSensorInfoDlg::IDD, pParent)
{
	mFTWrapper = ft;
	mFTSensor = ft->GetFTSensor();
}

CSixAxisSensorInfoDlg::~CSixAxisSensorInfoDlg()
{
}

void CSixAxisSensorInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SensorInformationDlg)
	DDX_Control(pDX, LABEL_TEMP_COMPENSATION, mLabelTemperatureCompensation);
	DDX_Control(pDX, LABEL_OUTPUT_RANGE, mLabelOutputRange);
	DDX_Control(pDX, LABEL_OUTPUT_POLARITY, mLabelOutputPolarity);
	DDX_Control(pDX, LABEL_OUTPUT_MODE, mLabelOutputMode);
	DDX_Control(pDX, LABEL_CALIBRATION_DATE, mLabelCalibrationDate);
	DDX_Control(pDX, LIST_CALIBRATIONS, mListCalibrations);
	DDX_Control(pDX, LABEL_NUM_CHANNELS, mLabelNumChannels);
	DDX_Control(pDX, LABEL_FAMILY, mLabelFamily);
	DDX_Control(pDX, LABEL_SERIAL_NUMBER, mLabelSerialNumber);
	DDX_Control(pDX, LABEL_BODY_STYLE, mLabelBodyStyle);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, LABEL_AXIS_MAX_1, mLabelAxisMax[0]);
	DDX_Control(pDX, LABEL_AXIS_MAX_2, mLabelAxisMax[1]);
	DDX_Control(pDX, LABEL_AXIS_MAX_3, mLabelAxisMax[2]);
	DDX_Control(pDX, LABEL_AXIS_MAX_4, mLabelAxisMax[3]);
	DDX_Control(pDX, LABEL_AXIS_MAX_5, mLabelAxisMax[4]);
	DDX_Control(pDX, LABEL_AXIS_MAX_6, mLabelAxisMax[5]);
	DDX_Control(pDX, LABEL_AXIS_1, mLabelAxis[0]);
	DDX_Control(pDX, LABEL_AXIS_2, mLabelAxis[1]);
	DDX_Control(pDX, LABEL_AXIS_3, mLabelAxis[2]);
	DDX_Control(pDX, LABEL_AXIS_4, mLabelAxis[3]);
	DDX_Control(pDX, LABEL_AXIS_5, mLabelAxis[4]);
	DDX_Control(pDX, LABEL_AXIS_6, mLabelAxis[5]);
}


BEGIN_MESSAGE_MAP(CSixAxisSensorInfoDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSixAxisSensorInfoDlg::OnBnClickedOk)
	ON_LBN_SELCHANGE(LIST_CALIBRATIONS, &CSixAxisSensorInfoDlg::OnLbnSelchangeCalibrations)
END_MESSAGE_MAP()


// CSixAxisSensorInfoDlg 消息处理程序


void CSixAxisSensorInfoDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
BOOL CSixAxisSensorInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString bodyStyle = mFTSensor->GetBodyStyle();
	CString family = mFTSensor->GetFamily();
	short numChannels = mFTSensor->GetNumChannels();
	CString strNumChannels;
	strNumChannels.Format("%d", numChannels);
	CString serial = mFTSensor->GetSerial();

	//	HWND hwnd1 = mLabelSerialNumber.GetSafeHwnd();
	//	HWND hwnd2 = mLabelBodyStyle.GetSafeHwnd();
	//	HWND hwnd3 = mLabelFamily.GetSafeHwnd();
	//	HWND hwnd4 = mLabelNumChannels.GetSafeHwnd();

	mLabelSerialNumber.SetWindowText(serial);
	mLabelBodyStyle.SetWindowText(bodyStyle);
	mLabelFamily.SetWindowText(family);
	mLabelNumChannels.SetWindowText(strNumChannels);

	_Calibrations calibrations = mFTSensor->GetCalibrations();
	short N = (short)(calibrations.GetCount());
	for (short i=1; i<=N; i++) {
		_Calibration calibration=calibrations.GetItem(COleVariant(i,VT_I2));
		CString part = calibration.GetPartNumber();
		mListCalibrations.AddString(part);
	}
	mListCalibrations.SetCurSel(mFTWrapper->GetActiveCalibrationIndex() - 1);
	UpdateCalibrationInfo();

	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CSixAxisSensorInfoDlg::UpdateCalibrationInfo()
{
	_Calibration calibration = mFTWrapper->GetActiveCalibration();
/*
	mLabelCalibrationDate.SetWindowText("DATE");
	mLabelTemperatureCompensation.SetWindowText("TEMP");
	mLabelOutputMode.SetWindowText("MODE");
	mLabelOutputRange.SetWindowText("RANGE");
	mLabelOutputPolarity.SetWindowText("POLARITY");
*/
	COleDateTime date(calibration.GetCalDate());
	mLabelCalibrationDate.SetWindowText(date.Format(VAR_DATEVALUEONLY));

	CString tempcomp = calibration.GetTempCompAvailable()?"Yes":"No";
	mLabelTemperatureCompensation.SetWindowText(tempcomp);

	CString outputMode = calibration.GetOutputMode();
	mLabelOutputMode.SetWindowText(outputMode);

	double range = calibration.GetOutputRange();
	CString outputRange;
	outputRange.Format("%3.1f V", range);
	mLabelOutputRange.SetWindowText(outputRange);

	CString outputPolarity = calibration.GetOutputBipolar()?"Bipolar":"Unipolar";
	mLabelOutputPolarity.SetWindowText(outputPolarity);

	for (int i=0; i<6; i++) {
		CString name = calibration.GetAxisName(COleVariant(short(i+1), VT_I2));
		mLabelAxis[i].SetWindowText(name + " :");
		CString maxLoad;
		maxLoad.Format("%4.1f ", calibration.GetAxisMaxLoad(COleVariant(short(i+1), VT_I2)));
		if (name.GetAt(0) == 'F' || name.GetAt(0) == 'f') {
			maxLoad += calibration.GetForceUnits();
		}
		else {
			maxLoad += calibration.GetTorqueUnits();
		}
		mLabelAxisMax[i].SetWindowText(maxLoad);
	}
}

void CSixAxisSensorInfoDlg::OnLbnSelchangeCalibrations()
{
	// TODO: 在此添加控件通知处理程序代码
	mFTWrapper->SetActiveCalibration((short)(mListCalibrations.GetCurSel()+1));
	UpdateCalibrationInfo();
}
