#include "stdafx.h"
#include "vJoy.h"

vJoyDevice::vJoyDevice()
{
	for (int i = 0; i < 32; i++)
		Dpad[i] = 0;

	for (int i = 0; i < 32; i++)
	{
		unsigned int bitmask = 1 << i;
		GetButton((vJoyButtonID)(vJoyButtonID::Button1 + i))->setButton(&m_iReport.lButtons, bitmask);
	}
	for (int i = 0; i < 32; i++)
	{
		unsigned int bitmask = 1 << i;
		GetButton((vJoyButtonID)(vJoyButtonID::Button1 + i + 32))->setButton(&m_iReport.lButtonsEx1, bitmask);
	}
	for (int i = 0; i < 32; i++)
	{
		unsigned int bitmask = 1 << i;
		GetButton((vJoyButtonID)(vJoyButtonID::Button1 + i + 64))->setButton(&m_iReport.lButtonsEx2, bitmask);
	}
	for (int i = 0; i < 32; i++)
	{
		unsigned int bitmask = 1 << i;
		GetButton((vJoyButtonID)(vJoyButtonID::Button1 + i + 96))->setButton(&m_iReport.lButtonsEx3, bitmask);
	}

	GetButton(vJoyButtonID::DPAD1_LEFT)->setDPad(&Dpad[0]);
	GetButton(vJoyButtonID::DPAD1_UP_LEFT)->setDPad(&Dpad[1]);
	GetButton(vJoyButtonID::DPAD1_UP)->setDPad(&Dpad[2]);
	GetButton(vJoyButtonID::DPAD1_UP_RIGHT)->setDPad(&Dpad[3]);
	GetButton(vJoyButtonID::DPAD1_RIGHT)->setDPad(&Dpad[4]);
	GetButton(vJoyButtonID::DPAD1_DOWN_RIGHT)->setDPad(&Dpad[5]);
	GetButton(vJoyButtonID::DPAD1_DOWN)->setDPad(&Dpad[6]);
	GetButton(vJoyButtonID::DPAD1_DOWN_LEFT)->setDPad(&Dpad[7]);

	GetButton(vJoyButtonID::DPAD2_LEFT)->setDPad(&Dpad[8]);
	GetButton(vJoyButtonID::DPAD2_UP_LEFT)->setDPad(&Dpad[9]);
	GetButton(vJoyButtonID::DPAD2_UP)->setDPad(&Dpad[10]);
	GetButton(vJoyButtonID::DPAD2_UP_RIGHT)->setDPad(&Dpad[11]);
	GetButton(vJoyButtonID::DPAD2_RIGHT)->setDPad(&Dpad[12]);
	GetButton(vJoyButtonID::DPAD2_DOWN_RIGHT)->setDPad(&Dpad[13]);
	GetButton(vJoyButtonID::DPAD2_DOWN)->setDPad(&Dpad[14]);
	GetButton(vJoyButtonID::DPAD2_DOWN_LEFT)->setDPad(&Dpad[15]);

	GetButton(vJoyButtonID::DPAD3_LEFT)->setDPad(&Dpad[16]);
	GetButton(vJoyButtonID::DPAD3_UP_LEFT)->setDPad(&Dpad[17]);
	GetButton(vJoyButtonID::DPAD3_UP)->setDPad(&Dpad[18]);
	GetButton(vJoyButtonID::DPAD3_UP_RIGHT)->setDPad(&Dpad[19]);
	GetButton(vJoyButtonID::DPAD3_RIGHT)->setDPad(&Dpad[20]);
	GetButton(vJoyButtonID::DPAD3_DOWN_RIGHT)->setDPad(&Dpad[21]);
	GetButton(vJoyButtonID::DPAD3_DOWN)->setDPad(&Dpad[22]);
	GetButton(vJoyButtonID::DPAD3_DOWN_LEFT)->setDPad(&Dpad[23]);

	GetButton(vJoyButtonID::DPAD4_LEFT)->setDPad(&Dpad[24]);
	GetButton(vJoyButtonID::DPAD4_UP_LEFT)->setDPad(&Dpad[25]);
	GetButton(vJoyButtonID::DPAD4_UP)->setDPad(&Dpad[26]);
	GetButton(vJoyButtonID::DPAD4_UP_RIGHT)->setDPad(&Dpad[27]);
	GetButton(vJoyButtonID::DPAD4_RIGHT)->setDPad(&Dpad[28]);
	GetButton(vJoyButtonID::DPAD4_DOWN_RIGHT)->setDPad(&Dpad[29]);
	GetButton(vJoyButtonID::DPAD4_DOWN)->setDPad(&Dpad[30]);
	GetButton(vJoyButtonID::DPAD4_DOWN_LEFT)->setDPad(&Dpad[31]);

	//Initialization
	memset(&m_iReport, 0, sizeof(m_iReport));
	loadAxis(vJoyButton::X, HID_USAGE_X, &m_iReport.wAxisX, vJoyAxisID::axis_X);
	loadAxis(vJoyButton::Y, HID_USAGE_Y, &m_iReport.wAxisY, vJoyAxisID::axis_Y);
	loadAxis(vJoyButton::Z, HID_USAGE_Z, &m_iReport.wAxisZ, vJoyAxisID::axis_Z);
	loadAxis(vJoyButton::RX, HID_USAGE_RX, &m_iReport.wAxisXRot, vJoyAxisID::axis_RX);
	loadAxis(vJoyButton::RY, HID_USAGE_RY, &m_iReport.wAxisYRot, vJoyAxisID::axis_RY);
	loadAxis(vJoyButton::RZ, HID_USAGE_RZ, &m_iReport.wAxisZRot, vJoyAxisID::axis_RZ);
	loadAxis(vJoyButton::SL0, HID_USAGE_SL0, &m_iReport.wSlider, vJoyAxisID::axis_SL0);
	loadAxis(vJoyButton::SL1, HID_USAGE_SL1, &m_iReport.wDial, vJoyAxisID::axis_SL1);
	loadTriggers(vJoyButton::XTR, HID_USAGE_X, &m_iReport.wAxisX, vJoyAxisID::axis_X);
	loadTriggers(vJoyButton::YTR, HID_USAGE_Y, &m_iReport.wAxisY, vJoyAxisID::axis_Y);
	loadTriggers(vJoyButton::ZTR, HID_USAGE_Z, &m_iReport.wAxisZ, vJoyAxisID::axis_Z);
	loadTriggers(vJoyButton::RXTR, HID_USAGE_X, &m_iReport.wAxisXRot, vJoyAxisID::axis_RX);
	loadTriggers(vJoyButton::RYTR, HID_USAGE_Y, &m_iReport.wAxisYRot, vJoyAxisID::axis_RY);
	loadTriggers(vJoyButton::RZTR, HID_USAGE_Z, &m_iReport.wAxisZRot, vJoyAxisID::axis_RZ);
	loadTriggers(vJoyButton::SL0TR, HID_USAGE_SL0, &m_iReport.wSlider, vJoyAxisID::axis_SL0);
	loadTriggers(vJoyButton::SL1TR, HID_USAGE_SL1, &m_iReport.wDial, vJoyAxisID::axis_SL1);
	loadAxisInv(vJoyButton::XINV, HID_USAGE_X, &m_iReport.wAxisX, vJoyAxisID::axis_X);
	loadAxisInv(vJoyButton::YINV, HID_USAGE_Y, &m_iReport.wAxisY, vJoyAxisID::axis_Y);
	loadAxisInv(vJoyButton::ZINV, HID_USAGE_Z, &m_iReport.wAxisZ, vJoyAxisID::axis_Z);
	loadAxisInv(vJoyButton::RXINV, HID_USAGE_RX, &m_iReport.wAxisXRot, vJoyAxisID::axis_RX);
	loadAxisInv(vJoyButton::RYINV, HID_USAGE_RY, &m_iReport.wAxisYRot, vJoyAxisID::axis_RY);
	loadAxisInv(vJoyButton::RZINV, HID_USAGE_RZ, &m_iReport.wAxisZRot, vJoyAxisID::axis_RZ);
	loadAxisInv(vJoyButton::SL0INV, HID_USAGE_SL0, &m_iReport.wSlider, vJoyAxisID::axis_SL0);
	loadAxisInv(vJoyButton::SL1INV, HID_USAGE_SL1, &m_iReport.wDial, vJoyAxisID::axis_SL1);
	loadRing(vJoyAxisMoveID::XY_CW, HID_USAGE_X, &m_iReport.wAxisX, HID_USAGE_Y, &m_iReport.wAxisY, vJoyAxisID::axis_XY);
	loadRing(vJoyAxisMoveID::XY_CN, HID_USAGE_X, &m_iReport.wAxisX, HID_USAGE_Y, &m_iReport.wAxisY, vJoyAxisID::axis_XY);
	loadRing(vJoyAxisMoveID::ZRZ_CW, HID_USAGE_Z, &m_iReport.wAxisZ, HID_USAGE_RZ, &m_iReport.wAxisZRot, vJoyAxisID::axis_ZRZ);
	loadRing(vJoyAxisMoveID::ZRZ_CN, HID_USAGE_Z, &m_iReport.wAxisZ, HID_USAGE_RZ, &m_iReport.wAxisZRot, vJoyAxisID::axis_ZRZ);
	loadRing(vJoyAxisMoveID::RXRY_CW, HID_USAGE_RX, &m_iReport.wAxisXRot, HID_USAGE_RY, &m_iReport.wAxisYRot, vJoyAxisID::axis_RXRY);
	loadRing(vJoyAxisMoveID::RXRY_CN, HID_USAGE_RX, &m_iReport.wAxisXRot, HID_USAGE_RY, &m_iReport.wAxisYRot, vJoyAxisID::axis_RXRY);
	loadRing(vJoyAxisMoveID::SL0SL1_CW, HID_USAGE_SL0, &m_iReport.wSlider, HID_USAGE_SL1, &m_iReport.wDial, vJoyAxisID::axis_SL0SL1);
	loadRing(vJoyAxisMoveID::SL0SL1_CN, HID_USAGE_SL0, &m_iReport.wSlider, HID_USAGE_SL1, &m_iReport.wDial, vJoyAxisID::axis_SL0SL1);
	for (int i = vJoyAxisMoveID::XY_LEFT; i <= vJoyAxisMoveID::XY_L_DL_CN; i++)
		loadRing((vJoyAxisMoveID)i, HID_USAGE_X, &m_iReport.wAxisX, HID_USAGE_Y, &m_iReport.wAxisY, vJoyAxisID::axis_XY);
	for (int i = vJoyAxisMoveID::ZRZ_LEFT; i <= vJoyAxisMoveID::ZRZ_L_DL_CN; i++)
		loadRing((vJoyAxisMoveID)i, HID_USAGE_Z, &m_iReport.wAxisZ, HID_USAGE_RZ, &m_iReport.wAxisZRot, vJoyAxisID::axis_ZRZ);
	m_iReport.bHats = -1;
	m_iReport.bHatsEx1 = -1;
	m_iReport.bHatsEx2 = -1;
	m_iReport.bHatsEx3 = -1;

	GetButton(vJoyButton::DPAD1_LEFT)->Release();
	GetButton(vJoyButton::DPAD1_UP_LEFT)->Release();
	GetButton(vJoyButton::DPAD1_UP)->Release();
	GetButton(vJoyButton::DPAD1_UP_RIGHT)->Release();
	GetButton(vJoyButton::DPAD1_RIGHT)->Release();
	GetButton(vJoyButton::DPAD1_DOWN_RIGHT)->Release();
	GetButton(vJoyButton::DPAD1_DOWN)->Release();
	GetButton(vJoyButton::DPAD1_DOWN_LEFT)->Release();
	GetButton(vJoyButton::DPAD2_LEFT)->Release();
	GetButton(vJoyButton::DPAD2_UP_LEFT)->Release();
	GetButton(vJoyButton::DPAD2_UP)->Release();
	GetButton(vJoyButton::DPAD2_UP_RIGHT)->Release();
	GetButton(vJoyButton::DPAD2_RIGHT)->Release();
	GetButton(vJoyButton::DPAD2_DOWN_RIGHT)->Release();
	GetButton(vJoyButton::DPAD2_DOWN)->Release();
	GetButton(vJoyButton::DPAD2_DOWN_LEFT)->Release();
	GetButton(vJoyButton::DPAD3_LEFT)->Release();
	GetButton(vJoyButton::DPAD3_UP_LEFT)->Release();
	GetButton(vJoyButton::DPAD3_UP)->Release();
	GetButton(vJoyButton::DPAD3_UP_RIGHT)->Release();
	GetButton(vJoyButton::DPAD3_RIGHT)->Release();
	GetButton(vJoyButton::DPAD3_DOWN_RIGHT)->Release();
	GetButton(vJoyButton::DPAD3_DOWN)->Release();
	GetButton(vJoyButton::DPAD3_DOWN_LEFT)->Release();
	GetButton(vJoyButton::DPAD4_LEFT)->Release();
	GetButton(vJoyButton::DPAD4_UP_LEFT)->Release();
	GetButton(vJoyButton::DPAD4_UP)->Release();
	GetButton(vJoyButton::DPAD4_UP_RIGHT)->Release();
	GetButton(vJoyButton::DPAD4_RIGHT)->Release();
	GetButton(vJoyButton::DPAD4_DOWN_RIGHT)->Release();
	GetButton(vJoyButton::DPAD4_DOWN)->Release();
	GetButton(vJoyButton::DPAD4_DOWN_LEFT)->Release();
}

vJoyDevice::~vJoyDevice()
{
}

void vJoyDevice::Init(HWND hWnd, bool verbose)
{
	WORD dll, drv;
	DriverMatch(&dll, &drv);
	echo(I18N.vJoy_version, dll >> 8 & 0xF, dll >> 4 & 0xF, dll & 0xF, drv >> 8 & 0xF, drv >> 4 & 0xF, drv & 0xF);
}

BOOL vJoyDevice::Active()
{
	return m_devID > 0;
}

BOOL vJoyDevice::Open(int DevID, bool verbose)
{
	VjdStat status = GetVJDStatus(DevID);

	switch (status)
	{
	case VJD_STAT_OWN:
	case VJD_STAT_FREE: break;
	case VJD_STAT_BUSY: { echo(I18N.vJoy_busy, DevID); return FALSE; }
	case VJD_STAT_MISS:
	default: { if (verbose) { echo(I18N.vJoy_cantuse, DevID); } return FALSE; }
	}

	if (!AcquireVJD(DevID))
	{
		echo(I18N.vJoy_failed_acquire, DevID);
		return FALSE;
	}

	m_iReport.bDevice = m_devID = DevID;
	if (verbose)
		echo(I18N.vJoy_acquire, DevID);

	memcpy(&m_iReportDefault, &m_iReport, sizeof(m_iReport));
	return Update();
}

void vJoyDevice::loadTriggers(vJoyButtonID id, int hid_axis, long* data, unsigned char axis)
{
	m_buttons[id].setTrigger(data, axis);
	m_buttons[id].Release();
}

void vJoyDevice::loadAxis(vJoyButtonID id, int hid_axis, long* data, unsigned char axis)
{
	m_buttons[id].setAxis(data, axis);
	m_buttons[id].Release();
}

void vJoyDevice::loadAxisInv(vJoyButtonID id, int hid_axis, long* data, unsigned char axis)
{
	m_buttons[id].setAxisInv(data, axis);
	m_buttons[id].Release();
}

void vJoyDevice::loadRing(vJoyAxisMoveID id, int hid_axis, long* data, int hid_axis2, long* data2, unsigned char axis)
{
	m_axis[id].setRing(data, data2, id, axis);
	m_axis[id].Release();
}

double vJoyDevice::distSum(Point p, Point arr[], int n)
{
	double sum = 0;
	for (int i = 0; i < n; i++)
	{
		double distx = abs(arr[i].x - p.x);
		double disty = abs(arr[i].y - p.y);
		sum += sqrt((distx * distx) + (disty * disty));
	}

	// Return the sum of Euclidean Distances
	return sum;
}

Point vJoyDevice::geometricMedian(Point arr[], int n)
{
	double lower_limit = 0.01;
	double test_distance = 2;
	Point current_point = { 0, 0 };
	Point test_point[] = { { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 } };

	for (int i = 0; i < n; i++)
	{
		current_point.x += arr[i].x;
		current_point.y += arr[i].y;
	}

	current_point.x /= n;
	current_point.y /= n;
	double minimum_distance = distSum(current_point, arr, n);

	int k = 0;
	while (k < n)
	{
		for (int i = 0; /*i < n, */i != k; i++)
		{
			Point newpoint;
			newpoint.x = arr[i].x;
			newpoint.y = arr[i].y;
			double newd = distSum(newpoint, arr, n);

			if (newd < minimum_distance)
			{
				minimum_distance = newd;
				current_point.x = newpoint.x;
				current_point.y = newpoint.y;
			}
		}
		k++;
	}

	int flag = 0;
	while (test_distance > lower_limit)
	{
		flag = 0;

		for (int i = 0; i < 4; i++)
		{
			Point newpoint;
			newpoint.x = current_point.x + (double)test_distance * test_point[i].x;
			newpoint.y = current_point.y + (double)test_distance * test_point[i].y;
			double newd = distSum(newpoint, arr, n);

			if (newd < minimum_distance)
			{
				minimum_distance = newd;
				current_point.x = newpoint.x;
				current_point.y = newpoint.y;
				flag = 1;
				break;
			}
		}

		if (flag == 0)
			test_distance /= 2;
	}

	// Return the geometric median
	return { current_point.x, current_point.y };
}

void vJoyDevice::ClearState()
{
	for (int i = 0; i < vJoyButtonID::button_Count; i++)
		GetButton((vJoyButtonID)i)->SetPushed(false);

	memcpy(&m_iReport, &m_iReportDefault, sizeof(m_iReport));

	if (!GetButton(vJoyButtonID::DPAD1_LEFT)->isPushed()) { Dpad[0] = 0; }
	if (!GetButton(vJoyButtonID::DPAD1_UP_LEFT)->isPushed()) { Dpad[1] = 0; }
	if (!GetButton(vJoyButtonID::DPAD1_UP)->isPushed()) { Dpad[2] = 0; }
	if (!GetButton(vJoyButtonID::DPAD1_UP_RIGHT)->isPushed()) { Dpad[3] = 0; }
	if (!GetButton(vJoyButtonID::DPAD1_RIGHT)->isPushed()) { Dpad[4] = 0; }
	if (!GetButton(vJoyButtonID::DPAD1_DOWN_RIGHT)->isPushed()) { Dpad[5] = 0; }
	if (!GetButton(vJoyButtonID::DPAD1_DOWN)->isPushed()) { Dpad[6] = 0; }
	if (!GetButton(vJoyButtonID::DPAD1_DOWN_LEFT)->isPushed()) { Dpad[7] = 0; }
	if (!GetButton(vJoyButtonID::DPAD2_LEFT)->isPushed()) { Dpad[8] = 0; }
	if (!GetButton(vJoyButtonID::DPAD2_UP_LEFT)->isPushed()) { Dpad[9] = 0; }
	if (!GetButton(vJoyButtonID::DPAD2_UP)->isPushed()) { Dpad[10] = 0; }
	if (!GetButton(vJoyButtonID::DPAD2_UP_RIGHT)->isPushed()) { Dpad[11] = 0; }
	if (!GetButton(vJoyButtonID::DPAD2_RIGHT)->isPushed()) { Dpad[12] = 0; }
	if (!GetButton(vJoyButtonID::DPAD2_DOWN_RIGHT)->isPushed()) { Dpad[13] = 0; }
	if (!GetButton(vJoyButtonID::DPAD2_DOWN)->isPushed()) { Dpad[14] = 0; }
	if (!GetButton(vJoyButtonID::DPAD2_DOWN_LEFT)->isPushed()) { Dpad[15] = 0; }
	if (!GetButton(vJoyButtonID::DPAD3_LEFT)->isPushed()) { Dpad[16] = 0; }
	if (!GetButton(vJoyButtonID::DPAD3_UP_LEFT)->isPushed()) { Dpad[17] = 0; }
	if (!GetButton(vJoyButtonID::DPAD3_UP)->isPushed()) { Dpad[18] = 0; }
	if (!GetButton(vJoyButtonID::DPAD3_UP_RIGHT)->isPushed()) { Dpad[19] = 0; }
	if (!GetButton(vJoyButtonID::DPAD3_RIGHT)->isPushed()) { Dpad[20] = 0; }
	if (!GetButton(vJoyButtonID::DPAD3_DOWN_RIGHT)->isPushed()) { Dpad[21] = 0; }
	if (!GetButton(vJoyButtonID::DPAD3_DOWN)->isPushed()) { Dpad[22] = 0; }
	if (!GetButton(vJoyButtonID::DPAD3_DOWN_LEFT)->isPushed()) { Dpad[23] = 0; }
	if (!GetButton(vJoyButtonID::DPAD4_LEFT)->isPushed()) { Dpad[24] = 0; }
	if (!GetButton(vJoyButtonID::DPAD4_UP_LEFT)->isPushed()) { Dpad[25] = 0; }
	if (!GetButton(vJoyButtonID::DPAD4_UP)->isPushed()) { Dpad[26] = 0; }
	if (!GetButton(vJoyButtonID::DPAD4_UP_RIGHT)->isPushed()) { Dpad[27] = 0; }
	if (!GetButton(vJoyButtonID::DPAD4_RIGHT)->isPushed()) { Dpad[28] = 0; }
	if (!GetButton(vJoyButtonID::DPAD4_DOWN_RIGHT)->isPushed()) { Dpad[29] = 0; }
	if (!GetButton(vJoyButtonID::DPAD4_DOWN)->isPushed()) { Dpad[30] = 0; }
	if (!GetButton(vJoyButtonID::DPAD4_DOWN_LEFT)->isPushed()) { Dpad[31] = 0; }
}

void vJoyDevice::UpdateState()
{
	const double π = 3.141592653589793238462643;
	const double λ = 0.707106781186547524400844;

	for (int i = 0; i < 32; i += 8)
	{
		Point arr[8];
		int n = 0;
		double result = -1;
		if (Dpad[0 + i]) { arr[n].x = -1; arr[n].y = 0; n++; }
		if (Dpad[1 + i]) { arr[n].x = -λ; arr[n].y = λ; n++; }
		if (Dpad[2 + i]) { arr[n].x = 0; arr[n].y = 1; n++; }
		if (Dpad[3 + i]) { arr[n].x = λ; arr[n].y = λ; n++; }
		if (Dpad[4 + i]) { arr[n].x = 1; arr[n].y = 0; n++; }
		if (Dpad[5 + i]) { arr[n].x = λ; arr[n].y = -λ; n++; }
		if (Dpad[6 + i]) { arr[n].x = 0; arr[n].y = -1; n++; }
		if (Dpad[7 + i]) { arr[n].x = -λ; arr[n].y = -λ; n++; }

		Point median = geometricMedian(arr, n);

		if ((abs(median.x) > 0.001 || abs(median.y) > 0.001) && n)
		{
			result = atan2(median.x, median.y);
			if (result < 0)
				result += (2 * π);
			result = result * (4 / π);
		}

		switch (i)
		{
		case 0: { m_iReport.bHats = (result == -1) ? -1 : (long)((int)result * 4500); break; }
		case 8: { m_iReport.bHatsEx1 = (result == -1) ? -1 : (long)((int)result * 4500); break; }
		case 16: { m_iReport.bHatsEx2 = (result == -1) ? -1 : (long)((int)result * 4500); break; }
		case 24: { m_iReport.bHatsEx3 = (result == -1) ? -1 : (long)((int)result * 4500); break; }
		}
	}
}

BOOL vJoyDevice::Update()
{
	if (m_devID <= 0)
		return FALSE;

	if (!UpdateVJD(m_devID, &m_iReport))
	{
		echo(I18N.vJoy_failed_update, m_devID);
		Close();
		return FALSE;
	}

	return TRUE;
}

vJoyButton* vJoyDevice::GetButton(vJoyButtonID id)
{
	if (id >= vJoyButtonID::none && id < vJoyButtonID::button_Count)
		return &m_buttons[id];
	return &m_buttons[0];
}

vJoyButton* vJoyDevice::GetAxis(vJoyAxisMoveID id)
{
	if (id >= vJoyAxisMoveID::axismove_none && id < vJoyAxisMoveID::axismove_Count)
		return &m_axis[id];
	return &m_axis[0];
}

void vJoyDevice::Close()
{
	if (m_devID > 0)
	{
		m_devID = 0;
		static std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
		if (std::chrono::system_clock::now() - start < std::chrono::milliseconds(1000))
			return;
		else
			start = std::chrono::system_clock::now();
		ClearState();
		Update();
		RelinquishVJD(m_devID);
	}
}
