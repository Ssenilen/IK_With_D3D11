// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
// DX11 전용 헤더
#include <D3D11.h>
#include <D3DX11.h>
#include <mmsystem.h>
#include <math.h>
#include <d3dcompiler.h>
#include <D3DX10math.h>
#include <iostream>
#include <string>
#include <vector>

#define FRAME_BUFFER_WIDTH	1024
#define FRAME_BUFFER_HEIGHT	768

#define VS_SLOT_CAMERA			0x00
#define VS_SLOT_WORLD_MATRIX	0x01

#define RANDOM_COLOR	D3DXCOLOR((rand() * 0xFFFFFF / RAND_MAX))
#define ASPECT_RATIO	(float(FRAME_BUFFER_WIDTH) / float(FRAME_BUFFER_HEIGHT))

// 테스트를 위한 콘솔 띄우기
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")

// 입력 관련 Define
#define DIR_FORWARD		0x01
#define DIR_BACKWARD	0x02
#define DIR_LEFT		0x04
#define DIR_RIGHT		0x08
#define DIR_UP			0x10
#define DIR_DOWN		0x20