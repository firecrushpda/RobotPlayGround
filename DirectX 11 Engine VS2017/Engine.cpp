#include "Engine.h"
#include "GJKCollision.h"

bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
	//timer.Start();

	if (!this->render_window.Initialize(this, hInstance, window_title, window_class, width, height))
		return false;

	if (!gfx.Initialize(this->render_window.GetHWND(), width, height))
		return false;

	HKL defaultLanguageLayout = LoadKeyboardLayout(L"0x04090409", KLF_SUBSTITUTE_OK);
	DWORD dwThreadID = GetCurrentThreadId();
	HKL hCurKeyboard = GetKeyboardLayout(dwThreadID);
	if (hCurKeyboard != defaultLanguageLayout)
	{
		UINT i;
		HKL hklCurrent;
		UINT uLayouts;
		HKL * lpList;

		uLayouts = GetKeyboardLayoutList(0, NULL);
		lpList = (HKL*)malloc(uLayouts * sizeof(HKL));
		uLayouts = GetKeyboardLayoutList(uLayouts, lpList);

		for (i = 0; i < uLayouts; i++)
		{
			hklCurrent = *(lpList + i);
			if (hklCurrent == defaultLanguageLayout)
			{
				ActivateKeyboardLayout(hklCurrent, 0);
			}
		}
	}

	return true;
}

bool Engine::ProcessMessages()
{
	return this->render_window.ProcessMessages();
}

void Engine::Update(float dt)
{
	at += dt / 1000;
	float t = (FLOAT)(at * 0.2);
	
	while (!keyboard.CharBufferIsEmpty())
	{
		unsigned char ch = keyboard.ReadChar();
	}

	while (!keyboard.KeyBufferIsEmpty())
	{
		KeyboardEvent kbe = keyboard.ReadKey();
		unsigned char keycode = kbe.GetKeyCode();
		if (kbe.IsPress())
		{
			if (keycode == 'C')
			{
				gfx.ChangeHdrImage();
			}

			if (keycode == 'M')
			{
				auto anisize = gfx.model->GetFBXModel()->GetAnimationSize();
				gfx.model->GetFBXModel()->m_kpreAnimaionIndex = gfx.model->GetFBXModel()->m_kcurrentAnimaionIndex;
				gfx.model->GetFBXModel()->m_kcurrentAnimaionIndex += 1;
				if (gfx.model->GetFBXModel()->m_kcurrentAnimaionIndex > anisize - 1)
				{
					gfx.model->GetFBXModel()->m_kcurrentAnimaionIndex = anisize - 1;
					gfx.model->GetFBXModel()->preanimtime = gfx.model->GetFBXModel()->GetCurrentAnimationTime(gfx.model->GetAccTime());
				}
			}

			if (keycode == 'N')
			{
				gfx.model->GetFBXModel()->m_kpreAnimaionIndex = gfx.model->GetFBXModel()->m_kcurrentAnimaionIndex;
				gfx.model->GetFBXModel()->m_kcurrentAnimaionIndex -= 1;
				if (gfx.model->GetFBXModel()->m_kcurrentAnimaionIndex < 0)
				{
					gfx.model->GetFBXModel()->preanimtime = gfx.model->GetFBXModel()->GetCurrentAnimationTime(gfx.model->GetAccTime());
					gfx.model->GetFBXModel()->m_kcurrentAnimaionIndex = 0;
				}
			}
		}
	}

	while (!mouse.EventBufferIsEmpty())
	{
		MouseEvent me = mouse.ReadEvent();
		if (mouse.IsRightDown())
		{
			if (me.GetType() == MouseEvent::EventType::RAW_MOVE)
			{
				//camera move mouse
				if (gfx.camera.GetCameraType() == 0)
				{
					this->gfx.camera.AdjustRotation((float)me.GetPosY() * 0.01f, (float)me.GetPosX() * 0.01f, 0);
				}
				else if (gfx.camera.GetCameraType() == 1)
				{

				}
				
			}
		}
	}

	const float cameraSpeed = 0.02f;

	//collision
	BoundingOrientedBox cp1 = gfx.quad_left->UpdateCollision();
	BoundingOrientedBox cp5 = gfx.quad_ground->UpdateCollision();

#pragma region collision detection and move adjust

	float fDist;
	float fdistance = -1;

	XMVECTOR adjustvector = XMVectorZero();
	XMVECTOR adjustrotvector = XMVectorZero();
	
	auto pspeed = gfx.player->MoveSpeed;
	auto fspeed = gfx.player->Gravity;
	
	if (keyboard.KeyIsPressed('W'))
	{
		adjustvector += this->gfx.camera.GetForwardVector() * pspeed;
	}
	if (keyboard.KeyIsPressed('S'))
	{
		adjustvector += this->gfx.camera.GetBackwardVector() * pspeed;
	}
	if (keyboard.KeyIsPressed('A'))
	{
		adjustvector += this->gfx.camera.GetLeftVector() * pspeed;
	}
	if (keyboard.KeyIsPressed('D'))
	{
		adjustvector += this->gfx.camera.GetRightVector() * pspeed;
	}

	if (keyboard.KeyIsPressed('Q'))
	{
		adjustrotvector += XMVectorSet(0.0f, 0.01f, 0.0f, 0.0f);
	}

	if (keyboard.KeyIsPressed('E'))
	{
		adjustrotvector += XMVectorSet(0.0f, -0.01f, 0.0f, 0.0f);
	}

	if (keyboard.KeyIsPressed('X'))//VK_SPACE
	{
		adjustvector += this->gfx.camera.GetUpVector() * pspeed;
	}
	if (keyboard.KeyIsPressed('Z'))
	{
		adjustvector += this->gfx.camera.GetDownVector() * pspeed;
	}

	//player fall every sec
	adjustvector += this->gfx.camera.GetDownVector() * fspeed;

	if (keyboard.KeyIsPressed(VK_SPACE))
	{
		gfx.player->StartJump();
	}

	//gjk
	auto temppos = gfx.model->GetPositionVector() + adjustvector;
	auto temprot = gfx.model->GetRotationVector() + adjustrotvector;
	auto tempscl = gfx.model->sclVector;
	auto quadcol = gfx.model->UpdateSphereCollision(temppos, temprot, tempscl);
	for (size_t i = 0; i < gfx.obstacles.size(); i++)
	{
		gfx.gjktest = XMVectorZero();
		auto targetcollision = gfx.obstacles.at(i)->UpdateCollision();
		gfx.gjkcol = gfx.gjk->gjk(targetcollision, quadcol, gfx.gjktest);
		if (gfx.gjkcol)
		{
			adjustvector -= gfx.gjktest;
		}
	}
	

	//final
	this->gfx.model->AdjustPosition(adjustvector);
	this->gfx.model->AdjustRotation(adjustrotvector);
	
#pragma endregion

	//camera move keyboard
	if (gfx.camera.GetCameraType() == 0)
	{
		//free move
		if (keyboard.KeyIsPressed('I'))
		{
			this->gfx.camera.AdjustPosition(this->gfx.camera.GetForwardVector() * cameraSpeed * dt);
		}
		if (keyboard.KeyIsPressed('K'))
		{
			this->gfx.camera.AdjustPosition(this->gfx.camera.GetBackwardVector() * cameraSpeed * dt);
		}
		if (keyboard.KeyIsPressed('J'))
		{
			this->gfx.camera.AdjustPosition(this->gfx.camera.GetLeftVector() * cameraSpeed * dt);
		}
		if (keyboard.KeyIsPressed('L'))
		{
			this->gfx.camera.AdjustPosition(this->gfx.camera.GetRightVector() * cameraSpeed * dt);
		}

		if (keyboard.KeyIsPressed('O'))
		{
			this->gfx.camera.AdjustPosition(this->gfx.camera.GetUpVector() * cameraSpeed * dt);
		}
		if (keyboard.KeyIsPressed('P'))
		{
			this->gfx.camera.AdjustPosition(this->gfx.camera.GetDownVector() * cameraSpeed * dt);
		}
	}
	else if (gfx.camera.GetCameraType() == 1)
	{
		//focus perspective
	}
	
	//gfx.model->GetFBXModel()->m_kcurrentAnimaionIndex = 0;
	//gfx.model->collider->colliderAdjustTransform = XMVectorSet(0, 0, -200, 0);
	gfx.model->Update(dt);
	gfx.player->Update(dt);

	/*gfx.quad_left->AdjustRotation(XMFLOAT3(0.01f, 0.0f, 0.0f));
	gfx.quad_right->AdjustRotation(XMFLOAT3(0.01f, 0.0f, 0.0f));
	gfx.quad_up->AdjustRotation(XMFLOAT3(0.01f, 0.0f, 0.0f));
	gfx.quad_down->AdjustRotation(XMFLOAT3(0.01f, 0.0f, 0.0f));*/
	
	//update ray with camera center
	auto rot = gfx.camera.GetRotationFloat3();
	auto rotmat = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
	XMVECTOR camTarget = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotmat);

	gfx.primeray.origin = gfx.camera.GetPositionVector();
	gfx.primeray.direction = XMVector3Normalize(camTarget);

	//gfx.inte = cp1.Intersects(cp2);

	/*auto quadpos = gfx.quad->GetPositionFloat3();
	if (quadpos.x > 100.0f)
		moveswitch = false;

	if (quadpos.x <= -100.0f)
		moveswitch = true;

	gfx.quad->AdjustRotation(XMFLOAT3(0.01f, 0.01f, 0.01f));
	if (moveswitch == true)
		gfx.quad->SetPosition(XMFLOAT3(quadpos.x + 1, quadpos.y, quadpos.z)); 
	else 
		gfx.quad->SetPosition(XMFLOAT3(quadpos.x - 1, quadpos.y, quadpos.z));*/

}

void Engine::RenderFrame(float dt)
{
	this->gfx.RenderFrame(dt);
}

