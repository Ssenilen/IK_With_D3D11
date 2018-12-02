// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
// DX11 ���� ���
#include <D3D11.h>
#include <D3DX11.h>
#include <mmsystem.h>
#include <math.h>
#include <d3dcompiler.h>
#include <D3DX10math.h>
#include <iostream>
#include <string>

#define FRAME_BUFFER_WIDTH	1024
#define FRAME_BUFFER_HEIGHT	768

#define VS_SLOT_CAMERA			0x00
#define VS_SLOT_WORLD_MATRIX	0x01

#define RANDOM_COLOR	D3DXCOLOR((rand() * 0xFFFFFF / RAND_MAX))
#define ASPECT_RATIO	(float(FRAME_BUFFER_WIDTH) / float(FRAME_BUFFER_HEIGHT))

// �׽�Ʈ�� ���� �ܼ� ����
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")

// �Է� ���� Define
#define DIR_FORWARD		0x01
#define DIR_BACKWARD	0x02
#define DIR_LEFT		0x04
#define DIR_RIGHT		0x08
#define DIR_UP			0x10
#define DIR_DOWN		0x20