#include "framework.h"
#include "client.h"
#include "game/game.h"

#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)

#define MAX_LOADSTRING 100

WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
HWND hWnd;
MSG msg;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);

	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	// 서버에 접속
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	SOCKET hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		return 0;
	SOCKADDR_IN server_addr = {};
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("124.63.153.236");
	server_addr.sin_port = htons(8000);
	if (connect(hSocket, (SOCKADDR*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
		return 0;
	WSAAsyncSelect(hSocket, hWnd, WM_USER, FD_READ | FD_CLOSE);
	char buffer[2048] = {};
	size_t bufferCount = 0;

	// 식별자 받기
	char id = -1;
	while (id == -1)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
			case WM_USER:
				switch (WSAGETSELECTEVENT(msg.lParam))
				{
				case FD_READ:
				{
					if (recv(hSocket, &id, 1, 0) != 1)
						return 0;
					break;
				}
				case FD_CLOSE:
					return 0;
				}
				break;
			case WM_QUIT:
				return 0;
			default:
				DispatchMessage(&msg);
			}
		}
	}

	// 식별자로 게임 준비
	// 월드
	World world;

	// 제어 변수
	int flag = -1;
	int fieldWidth = 1500;
	int fieldHeight = 1000;

	// 엔티티
	auto left = world.createEntity();
	auto leftHpBar = world.createEntity();
	auto leftMpBar = world.createEntity();
	auto right = world.createEntity();
	auto rightHpBar = world.createEntity();
	auto rightMpBar = world.createEntity();

	// 시스템
	HumanSystem humanSystem;
	BeamSystem beamSystem(&humanSystem);
	ParticleSystem particleSystem;

	DeleteSystem deleteSystem;
	GravitySystem gravitySystem(2.0f);
	Translate translate;
	FrictionSystem frictionSystem;
	LandSystem landSystem;
	WallSystem wallSystem(fieldWidth);

	HpSystem hpSystem;
	MpSystem mpSystem;

	CameraSystem cameraSystem(200, fieldWidth, fieldHeight, 1280, 720, 0-fieldWidth*50, 0- fieldHeight*50, fieldWidth*100, fieldHeight*100);

	HpBarSystem hpBarSystem;
	MpBarSystem mpBarSystem;

	EndSystem endSystem(left, right, &flag);

	Render render(&cameraSystem, hWnd, fieldWidth, fieldHeight);
	render.loadImage(L"리소스\\인터페이스\\", L"hpBack.png", "hpBack");
	render.loadImage(L"리소스\\인터페이스\\", L"hpValue.png", "hpValue");
	render.loadImage(L"리소스\\인터페이스\\", L"hpEffect.png", "hpEffect");
	render.loadImage(L"리소스\\인터페이스\\", L"mpBack.png", "mpBack");
	render.loadImage(L"리소스\\인터페이스\\", L"mpValue.png", "mpValue");
	render.loadImage(L"리소스\\인터페이스\\", L"mpEffect.png", "mpEffect");

	render.loadImage(L"리소스\\파티클\\", L"blood0.png", "blood0");
	render.loadImage(L"리소스\\파티클\\", L"blood1.png", "blood1");
	render.loadImage(L"리소스\\파티클\\", L"spark0.png", "spark0");
	render.loadImage(L"리소스\\파티클\\", L"spark1.png", "spark1");

	render.loadImage(L"리소스\\히트\\", L"hitAttack0.png", "hitAttack0");
	render.loadImage(L"리소스\\히트\\", L"hitAttack1.png", "hitAttack1");
	render.loadImage(L"리소스\\히트\\", L"hitAttack2.png", "hitAttack2");
	render.loadImage(L"리소스\\히트\\", L"hitBeam0.png", "hitBeam0");
	render.loadImage(L"리소스\\히트\\", L"hitBeam1.png", "hitBeam1");
	render.loadImage(L"리소스\\히트\\", L"hitBeam2.png", "hitBeam2");

	render.loadImage(L"리소스\\빔\\", L"beam0.png", "beam0");
	render.loadImage(L"리소스\\빔\\", L"beam1.png", "beam1");

	render.loadImage(L"리소스\\휴먼\\", L"air0.png", "humanAir0");
	render.loadImage(L"리소스\\휴먼\\", L"attack0.png", "humanAttack0");
	render.loadImage(L"리소스\\휴먼\\", L"attack1.png", "humanAttack1");
	render.loadImage(L"리소스\\휴먼\\", L"attack2.png", "humanAttack2");
	render.loadImage(L"리소스\\휴먼\\", L"attack3.png", "humanAttack3");
	render.loadImage(L"리소스\\휴먼\\", L"attack4.png", "humanAttack4");
	render.loadImage(L"리소스\\휴먼\\", L"attack5.png", "humanAttack5");
	render.loadImage(L"리소스\\휴먼\\", L"attack6.png", "humanAttack6");
	render.loadImage(L"리소스\\휴먼\\", L"charge0.png", "humanCharge0");
	render.loadImage(L"리소스\\휴먼\\", L"charge1.png", "humanCharge1");
	render.loadImage(L"리소스\\휴먼\\", L"charge2.png", "humanCharge2");
	render.loadImage(L"리소스\\휴먼\\", L"charge3.png", "humanCharge3");
	render.loadImage(L"리소스\\휴먼\\", L"charge4.png", "humanCharge4");
	render.loadImage(L"리소스\\휴먼\\", L"charge5.png", "humanCharge5");
	render.loadImage(L"리소스\\휴먼\\", L"charge6.png", "humanCharge6");
	render.loadImage(L"리소스\\휴먼\\", L"charge7.png", "humanCharge7");
	render.loadImage(L"리소스\\휴먼\\", L"die0.png", "humanDie0");
	render.loadImage(L"리소스\\휴먼\\", L"die1.png", "humanDie1");
	render.loadImage(L"리소스\\휴먼\\", L"die2.png", "humanDie2");
	render.loadImage(L"리소스\\휴먼\\", L"die3.png", "humanDie3");
	render.loadImage(L"리소스\\휴먼\\", L"die4.png", "humanDie4");
	render.loadImage(L"리소스\\휴먼\\", L"idle0.png", "humanIdle0");
	render.loadImage(L"리소스\\휴먼\\", L"idle1.png", "humanIdle1");
	render.loadImage(L"리소스\\휴먼\\", L"move0.png", "humanMove0");
	render.loadImage(L"리소스\\휴먼\\", L"move1.png", "humanMove1");
	render.loadImage(L"리소스\\휴먼\\", L"skill0.png", "humanSkill0");
	render.loadImage(L"리소스\\휴먼\\", L"skill1.png", "humanSkill1");
	render.loadImage(L"리소스\\휴먼\\", L"skill2.png", "humanSkill2");
	render.loadImage(L"리소스\\휴먼\\", L"skill3.png", "humanSkill3");
	render.loadImage(L"리소스\\휴먼\\", L"skill4.png", "humanSkill4");
	render.loadImage(L"리소스\\휴먼\\", L"skill5.png", "humanSkill5");

	Audio audio;
	audio.loadSound("리소스\\city.wav", true, "bgm");
	audio.loadSound("리소스\\pak.wav", false, "pak");
	audio.loadSound("리소스\\swing.wav", false, "swing");
	audio.loadSound("리소스\\hitBeam.wav", false, "hitBeam");
	audio.loadSound("리소스\\skill.wav", false, "skill");
	audio.loadSound("리소스\\beam.wav", false, "beam");
	audio.loadSound("리소스\\gathering.wav", false, "gathering");

	// 등록
	world.addSystem(&humanSystem);
	world.addSystem(&beamSystem);
	world.addSystem(&particleSystem);
	world.addSystem(&deleteSystem);
	world.addSystem(&gravitySystem);
	world.addSystem(&translate);
	world.addSystem(&landSystem);
	world.addSystem(&wallSystem);
	world.addSystem(&frictionSystem);
	world.addSystem(&hpSystem);
	world.addSystem(&mpSystem);
	world.addSystem(&cameraSystem);
	world.addSystem(&hpBarSystem);
	world.addSystem(&mpBarSystem);
	world.addSystem(&endSystem);
	world.addSystem(&render);
	world.addSystem(&audio);

	SetWindowTextA(hWnd, "시스템 등록 완료");

	// 컴포넌트
	left->addComponent(new Human(left, "idle"));
	left->addComponent(new Direction(1));
	left->addComponent(new Position(200, 1000));
	left->addComponent(new Velocity());
	left->addComponent(new DeltaPosition());
	left->addComponent(new GravityComponent);
	left->addComponent(new HpComponent(5.0f, 5.0f));
	left->addComponent(new MpComponent(0.0f, 5.0f));
	left->addComponent(new CameraComponent);
	left->addComponent(new Graphic);
	left->addComponent(new Sound);

	right->addComponent(new Human(right, "idle"));
	right->addComponent(new Direction(-1));
	right->addComponent(new Position(fieldWidth-200, 1000));
	right->addComponent(new Velocity());
	right->addComponent(new DeltaPosition());
	right->addComponent(new GravityComponent);
	right->addComponent(new HpComponent(5.0f, 5.0f));
	right->addComponent(new MpComponent(0.0f, 5.0f));
	right->addComponent(new CameraComponent);
	right->addComponent(new Graphic);
	right->addComponent(new Sound);

	SetWindowTextA(hWnd, "휴먼 완료");

	leftHpBar->addComponent(new HpBar(left));
	leftHpBar->addComponent(new Position(100, 32));
	leftHpBar->addComponent(new Graphic(nullptr, true));

	leftMpBar->addComponent(new MpBar(left));
	leftMpBar->addComponent(new Position(100, 64));
	leftMpBar->addComponent(new Graphic(nullptr, true));

	rightHpBar->addComponent(new HpBar(right));
	rightHpBar->addComponent(new Position(1280-320-100, 32));
	rightHpBar->addComponent(new Graphic(nullptr, true));

	rightMpBar->addComponent(new MpBar(right));
	rightMpBar->addComponent(new Position(1280-320-100, 64));
	rightMpBar->addComponent(new Graphic(nullptr, true));

	SetWindowTextA(hWnd, "컴포넌트 완료");

	// 준비완료 송신
	char buf[1] = { id };
	if (send(hSocket, buf, 1, 0) != 1)
	{
		return 0;
	}

	// 틱 수신
	char tick = 1;
	while (tick)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
			case WM_USER:
				switch (WSAGETSELECTEVENT(msg.lParam))
				{
				case FD_READ:
				{
					if (recv(hSocket, &tick, 1, 0) != 1)
						return 0;
					break;
				}
				case FD_CLOSE:
					return 0;
				}
				break;
			case WM_QUIT:
				return 0;
			default:
				DispatchMessage(&msg);
			}
		}
	}

	// 최초 업데이트
	humanSystem.update();
	beamSystem.update();
	particleSystem.update();
	deleteSystem.update();
	gravitySystem.update();
	translate.update();
	landSystem.update();
	wallSystem.update();
	frictionSystem.update();
	hpSystem.update();
	mpSystem.update();
	cameraSystem.update();
	hpBarSystem.update();
	mpBarSystem.update();
	endSystem.update();
	audio.update();
	audio.playSound("bgm", 1.0f);
	
	while (flag == -1)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
			case WM_NCLBUTTONDOWN:
			case WM_NCLBUTTONUP:
			case WM_NCMOUSEMOVE:
				break;
			case WM_USER:
				switch (WSAGETSELECTEVENT(msg.lParam))
				{
				case FD_READ:
				{
					WSAAsyncSelect(hSocket, hWnd, WM_USER, FD_CLOSE);
					bufferCount += recv(hSocket, buffer + bufferCount, 1024, 0);
					size_t pos = 0;

					bool flag2 = true;
					while (flag2)
					{
						if (bufferCount - pos < 1)
							break;

						switch (buffer[pos])
						{
						case 0: // 틱
							// 게임 업데이트
							humanSystem.update();
							beamSystem.update();
							particleSystem.update();
							deleteSystem.update();
							gravitySystem.update();
							translate.update();
							landSystem.update();
							wallSystem.update();
							frictionSystem.update();
							hpSystem.update();
							mpSystem.update();
							cameraSystem.update();
							hpBarSystem.update();
							mpBarSystem.update();
							endSystem.update();
							audio.update();
							if (flag != -1)
								return 0;

							pos++;
							break;
						case 1: // 입력
						{
							if (bufferCount - pos < 3)
							{
								flag2 = false;
								break;
							}

							char id = buffer[pos + 1]; // id
							if (id < 0 || id >= 2)
								return 0;

							char input = buffer[pos + 2]; // input
							if (input < 0 || input >= 14)
								return 0;

							// 입력처리
							switch (id)
							{
							case 0:
								left->getComponent<Human>()->input(input);
								break;
							case 1:
								right->getComponent<Human>()->input(input);
								break;
							}
							
							pos += 3;
							break;
						}
						default: // 불량서버
							return 0;
						}
					}

					bufferCount -= pos;
					if (bufferCount > 1024)
						return 0;

					for (size_t i = 0; i < bufferCount; i++)
						buffer[i] = buffer[i + pos];


					WSAAsyncSelect(hSocket, hWnd, WM_USER, FD_READ | FD_CLOSE);
					break;
				}
				case FD_CLOSE: // 서버가 뒤짐
					return 0;
				}
				break;

				// 장치이벤트
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			case WM_KEYUP:
			case WM_SYSKEYUP:
			{
				bool keyState = !(msg.lParam & 0b10000000000000000000000000000000);
				bool isRepeat = msg.lParam & 0b01000000000000000000000000000000;

				if (keyState && isRepeat)
					break;

				char value = -100;

				switch (msg.wParam)
				{
				case VK_LEFT:
					value = 0;
					break;
				case VK_RIGHT:
					value = 1;
					break;
				case VK_UP:
					value = 2;
					break;
				case VK_DOWN:
					value = 3;
					break;
				case 'a':
				case 'A':
					value = 4;
					break;
				case 's':
				case 'S':
					value = 5;
					break;
				case 'd':
				case 'D':
					value = 6;
					break;
				}

				if (!keyState)
				{
					value += 7;
				}
				if (value < 0 || value >= 14)
					break;

				send(hSocket, &value, 1, 0);
				break;
			}

			case WM_QUIT:
				return 0;
			default:
				DispatchMessage(&msg);
			}
		}
		else
		{
			// 자투리
			render.update();
		}
	}

	return (int)msg.wParam;
}



ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
