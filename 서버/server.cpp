#include "framework.h"
#include "server.h"
#include "engine/engine.h"
#include "game/game.h"

#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)

#include <map>
#include <set>

#include <chrono>

struct Buffer
{
	char data[2048] = {};
	size_t size = 0;
};

struct Client
{
	char id;
	Buffer buffer;
};

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
	LoadStringW(hInstance, IDC_SERVER, szWindowClass, MAX_LOADSTRING);

	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	// 소켓 생성
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	SOCKET hServerSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hServerSocket == INVALID_SOCKET)
		return 0;
	SOCKADDR_IN server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(8000);
	if (bind(hServerSocket, (SOCKADDR*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
		return 0;
	if (listen(hServerSocket, 8) == SOCKET_ERROR)
		return 0;
	WSAAsyncSelect(hServerSocket, hWnd, WM_USER, FD_ACCEPT);

	// 0 1 접속
	std::map<SOCKET, Client> clients;
	while (clients.size() != 2)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
			case WM_USER:
				switch (WSAGETSELECTEVENT(msg.lParam))
				{
				case FD_ACCEPT:
				{
					SOCKADDR_IN client_addr;
					int addrlen = sizeof(client_addr);
					SOCKET hClientSocket = accept(hServerSocket, (SOCKADDR*)&client_addr, &addrlen);
					if (hClientSocket == INVALID_SOCKET)
						break;
					WSAAsyncSelect(hClientSocket, hWnd, WM_USER, FD_READ | FD_CLOSE);
					clients[hClientSocket] = { (char)clients.size() };
					break;
				}
				case FD_READ:
				{
					SOCKET hSocket = (SOCKET)msg.wParam;
					auto it = clients.find(hSocket);
					if (it == clients.end())
						break;

					// do
					LINGER linger = { 1, 0 };
					setsockopt(hSocket, SOL_SOCKET, SO_LINGER, (const char*)&linger, sizeof(linger));
					closesocket(hSocket);
					clients.erase(it);
					break;
				}
				case FD_CLOSE:
				{
					SOCKET hSocket = (SOCKET)msg.wParam;
					auto it = clients.find(hSocket);
					if (it == clients.end())
						break;

					// do
					closesocket(hSocket);
					clients.erase(it);
					break;
				}
				}
				break;
			case WM_QUIT:
				return 0;
			default:
				DispatchMessage(&msg);
			}
		}
	}
	closesocket(hServerSocket);

	// 게임준비를 위한 식별자 전송
	for (auto it = clients.begin(); it != clients.end(); it++)
	{
		char buf[1] = { it->second.id };
		send(it->first, buf, 1, 0);
	}

	// 게임 준비
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
	LandSystem landSystem;
	WallSystem wallSystem(fieldWidth);

	HpSystem hpSystem;
	MpSystem mpSystem;

	CameraSystem cameraSystem(200, fieldWidth, fieldHeight, 1280, 720, 0 - fieldWidth * 50, 0 - fieldHeight * 50, fieldWidth * 100, fieldHeight * 100);

	HpBarSystem hpBarSystem;
	MpBarSystem mpBarSystem;

	EndSystem endSystem(left, right, &flag);

	//Render render(&cameraSystem, hWnd, fieldWidth, fieldHeight);
	/*render.loadImage(L"리소스\\인터페이스\\", L"hpBack.png", "hpBack");
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
	render.loadImage(L"리소스\\휴먼\\", L"skill5.png", "humanSkill5");*/

	// 등록
	world.addSystem(&humanSystem);
	world.addSystem(&beamSystem);
	world.addSystem(&particleSystem);
	world.addSystem(&deleteSystem);
	world.addSystem(&gravitySystem);
	world.addSystem(&translate);
	world.addSystem(&landSystem);
	world.addSystem(&wallSystem);
	world.addSystem(&hpSystem);
	world.addSystem(&mpSystem);
	world.addSystem(&cameraSystem);
	world.addSystem(&hpBarSystem);
	world.addSystem(&mpBarSystem);
	world.addSystem(&endSystem);
	//world.addSystem(&render);

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
	right->addComponent(new Position(fieldWidth - 200, 1000));
	right->addComponent(new Velocity());
	right->addComponent(new DeltaPosition());
	right->addComponent(new GravityComponent);
	right->addComponent(new HpComponent(5.0f, 5.0f));
	right->addComponent(new MpComponent(0.0f, 5.0f));
	right->addComponent(new CameraComponent);
	right->addComponent(new Graphic);
	right->addComponent(new Sound);

	leftHpBar->addComponent(new HpBar(left));
	leftHpBar->addComponent(new Position(100, 32));
	leftHpBar->addComponent(new Graphic(nullptr, true));

	leftMpBar->addComponent(new MpBar(left));
	leftMpBar->addComponent(new Position(100, 64));
	leftMpBar->addComponent(new Graphic(nullptr, true));

	rightHpBar->addComponent(new HpBar(right));
	rightHpBar->addComponent(new Position(1280 - 320 - 100, 32));
	rightHpBar->addComponent(new Graphic(nullptr, true));

	rightMpBar->addComponent(new MpBar(right));
	rightMpBar->addComponent(new Position(1280 - 320 - 100, 64));
	rightMpBar->addComponent(new Graphic(nullptr, true));

	// 준비완료?
	std::set<size_t> ready;
	while (ready.size() != 2)
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
					SOCKET hSocket = (SOCKET)msg.wParam;
					auto it = clients.find(hSocket);
					if (it == clients.end())
						break;

					// do
					char buf[1];
					if (recv(hSocket, buf, 1, 0) == 0 || ready.find(it->second.id) != ready.end() || buf[0] != it->second.id)
					{
						ready.insert(it->second.id);
						// 불량 클라이언트 그냥 레디한걸로 치고 종료시킨다.
						LINGER linger = { 1, 0 };
						setsockopt(hSocket, SOL_SOCKET, SO_LINGER, (const char*)&linger, sizeof(linger));
						closesocket(hSocket);
						clients.erase(it);
						break;
					}
					ready.insert(it->second.id);
					break;
				}
				case FD_CLOSE:
				{
					SOCKET hSocket = (SOCKET)msg.wParam;
					auto it = clients.find(hSocket);
					if (it == clients.end())
						break;

					// do
					ready.insert(it->second.id);
					// 그냥 레디한걸로 치고 종료시킨다.
					closesocket(hSocket);
					clients.erase(it);
					break;
				}
				}
				break;
			case WM_QUIT:
				return 0;
			default:
				DispatchMessage(&msg);
			}
		}
	}

	if (clients.size() == 0)
		return 0;

	// 틱 전송
	for (auto it = clients.begin(); it != clients.end(); it++)
	{
		char buf[1] = {};
		send(it->first, buf, 1, 0);
	}

	// 타이머 초기화
	auto timePoint = std::chrono::steady_clock::now();
	std::chrono::milliseconds term(20);
	std::chrono::milliseconds baseTerm(20);

	// 최초 업데이트
	humanSystem.update();
	beamSystem.update();
	particleSystem.update();
	deleteSystem.update();
	gravitySystem.update();
	translate.update();
	landSystem.update();
	wallSystem.update();
	hpSystem.update();
	mpSystem.update();
	cameraSystem.update();
	hpBarSystem.update();
	mpBarSystem.update();
	endSystem.update();
	//render.update();

	while (flag == -1)
	{
		// 타이머 검사
		if (std::chrono::steady_clock::now() - timePoint >= term)
		{
			// 틱전송
			for (auto it = clients.begin(); it != clients.end(); it++)
			{
				char buf[1] = {};
				send(it->first, buf, 1, 0);
			}

			// 게임 업데이트
			humanSystem.update();
			beamSystem.update();
			particleSystem.update();
			deleteSystem.update();
			gravitySystem.update();
			translate.update();
			landSystem.update();
			wallSystem.update();
			hpSystem.update();
			mpSystem.update();
			cameraSystem.update();
			hpBarSystem.update();
			mpBarSystem.update();
			endSystem.update();

			timePoint += term;
		}
		else if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
			case WM_USER:
				switch (WSAGETSELECTEVENT(msg.lParam))
				{
				case FD_READ:
				{
					SOCKET hSocket = (SOCKET)msg.wParam;
					auto it = clients.find(hSocket);
					if (it == clients.end())
						break;

					char id = it->second.id;
					Buffer& buffer = it->second.buffer;

					buffer.size += recv(hSocket, buffer.data + buffer.size, 1024, 0);
					size_t pos = 0;

					bool 불량클라이언트 = false;

					while (1)
					{
						if (buffer.size - pos < 1)
							break;
						
						auto input = buffer.data[pos];

						// 입력값 범위 검사
						if (input < 0 || input >= 14)
						{
							// 불량클라이언트
							LINGER linger = { 1, 0 };
							setsockopt(hSocket, SOL_SOCKET, SO_LINGER, (const char*)&linger, sizeof(linger));
							closesocket(hSocket);
							clients.erase(it);

							if (clients.empty())
								return 0;

							불량클라이언트 = true;

							break;
						}

						char buf[3] = { char(1), id, input };
						for (auto it = clients.begin(); it != clients.end(); it++)
						{
							send(it->first, buf, 3, 0);
						}

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

						pos++;
					}

					if (불량클라이언트)
					{
						break;
					}
					
					buffer.size -= pos;
					if (buffer.size > 1024)
					{
						// 불량클라이언트// 불량클라이언트
						LINGER linger = { 1, 0 };
						setsockopt(hSocket, SOL_SOCKET, SO_LINGER, (const char*)&linger, sizeof(linger));
						closesocket(hSocket);
						clients.erase(it);

						if (clients.empty())
							return 0;

						break;
					}

					for (size_t i = 0; i < buffer.size; i++)
						buffer.data[i] = buffer.data[i + pos];

					break;
				}
				case FD_CLOSE: // 탈주 시발련
				{
					SOCKET hSocket = (SOCKET)msg.wParam;
					auto it = clients.find(hSocket);
					if (it == clients.end())
						break;

					// do
					closesocket(hSocket);
					clients.erase(it);

					if (clients.empty())
						return 0;
					break;
				}
				}
				break;

			case WM_QUIT:
				return 0;
			default:
				DispatchMessage(&msg);
			}
		}
		else
		{
			// 자투리
			// render.update();
		}
	}

	// 삭제
	for (auto it = clients.begin(); it != clients.end(); it++)
	{
		LINGER linger = { 1, 0 };
		setsockopt(it->first, SOL_SOCKET, SO_LINGER, (const char*)&linger, sizeof(linger));
		closesocket(it->first);
		clients.erase(it);
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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SERVER));
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
