#include "game.h"

int main()
{
	// 월드
	World world;

	// 제어 변수
	int flag = -1;

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
	GravitySystem gravitySystem;
	Translate translate;
	LandSystem landSystem;
	WallSystem wallSystem(5000);

	HpSystem hpSystem;
	MpSystem mpSystem;

	CameraSystem cameraSystem(50, 5000, 2000, 1280, 720, 0, 0, 1280, 720);
	
	HpBarSystem hpBarSystem;
	MpBarSystem mpBarSystem;
	
	EndSystem endSystem(left, right, &flag);
	
	Render render(&cameraSystem);

	// 컴포넌트
	left->addComponent(new Human(left, "idle"));
	left->addComponent(new Direction(1));
	left->addComponent(new Position());
	left->addComponent(new Velocity());
	left->addComponent(new DeltaPosition());
	left->addComponent(new GravityComponent);
	left->addComponent(new HpComponent(5.0f, 5.0f));
	left->addComponent(new MpComponent(0.0f, 5.0f));
	left->addComponent(new CameraComponent);
	left->addComponent(new Graphic);

	right->addComponent(new Human(right, "idle"));
	right->addComponent(new Direction(-1));
	right->addComponent(new Position(500.0f));
	right->addComponent(new Velocity());
	right->addComponent(new DeltaPosition());
	right->addComponent(new GravityComponent);
	right->addComponent(new HpComponent(5.0f, 5.0f));
	right->addComponent(new MpComponent(0.0f, 5.0f));
	right->addComponent(new CameraComponent);
	right->addComponent(new Graphic);

	leftHpBar->addComponent(new HpBar(left));
	leftHpBar->addComponent(new Position());
	leftHpBar->addComponent(new Graphic(nullptr, true));

	leftMpBar->addComponent(new MpBar(left));
	leftMpBar->addComponent(new Position(0, 100));
	leftMpBar->addComponent(new Graphic(nullptr, true));

	rightHpBar->addComponent(new HpBar(right));
	rightHpBar->addComponent(new Position(1000));
	rightHpBar->addComponent(new Graphic(nullptr, true));

	rightMpBar->addComponent(new MpBar(right));
	rightMpBar->addComponent(new Position(1000, 100));
	rightMpBar->addComponent(new Graphic(nullptr, true));
}
