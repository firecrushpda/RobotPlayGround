#include "Graphics.h"
#include "../FBXModel.h"



bool Graphics::Initialize(HWND hwnd, int width, int height)
{
	this->windowWidth = width;
	this->windowHeight = height;
	//this->fpsTimer.Start();

	if (!InitializeDirectX(hwnd))
		return false;

	if (!InitializeShaders())
		return false;

	model = new Model;
	model->Initialize("robot.fbx", device.Get(), deviceContext.Get(), NULL, cb_vs_vertexshader,1);//IKhaitteru.fbx robot.fbx Idle (1).fbx
	model->transfomrMatirx = XMMatrixSet(
		1, 0, 0, 0,
		0, 0, -1, 0,
		0, 1, 0, 0,
		0, 0, 0, 1
	);
	model->GetFBXModel()->CreateFBXAnimationOnly("robot_idle.fbx", model->GetFBXModel());
	model->GetFBXModel()->CreateFBXAnimationOnly("robot_run.fbx", model->GetFBXModel());
	model->AdjustPosition(0, 2, 0);
	model->GetFBXModel()->m_kcurrentAnimaionIndex = 0;
	model->collider->collisiontype = CollsionType::player;
	//model->collider->colliderAdjustScale = XMVectorSet(1, 1, 1.0, 0);//XMVectorSet(0.6, 0.6, 1.0, 0);

	skybox = new Skybox;
	skybox->Initialize(device.Get(), deviceContext.Get(), cb_vs_vertexshader);

	quad = new Model;
	quad->Initialize("Data//quad.fbx", device.Get(), deviceContext.Get(), NULL, cb_vs_vertexshader, 0);
	quad->AdjustPosition(0, 0, 0);
	quad->scl = XMFLOAT3(1, 1, 1);
	quad->useDebugMesh = false;
	quad->sclVector = XMLoadFloat3(&quad->scl);

	drone = new Model;
	drone->Initialize("Data//drone-test.fbx", device.Get(), deviceContext.Get(), NULL, cb_vs_vertexshader,0);
	drone->AdjustPosition(0, 0, 0);
	drone->scl = XMFLOAT3(0.5f, 0.5f, 0.5f);
	drone->useDebugMesh = false;
	drone->sclVector = XMLoadFloat3(&drone->scl);

	quad_left = new Model;
	quad_left->Initialize("Data//quadtest.fbx", device.Get(), deviceContext.Get(), NULL, cb_vs_vertexshader, 0);
	quad_left->AdjustPosition(5, 0, -5);
	//quad_left->AdjustRotation(0, 0.7,0);
	quad_left->scl = XMFLOAT3(1, 1, 1);
	quad_left->sclVector = XMLoadFloat3(&quad_left->scl);
	quad_left->useDebugMesh = false;
	obstacles.push_back(quad_left);

	/*quad_right = new Model;
	quad_right->Initialize("Data//quad.fbx", device.Get(), deviceContext.Get(), NULL, cb_vs_vertexshader, 0);
	quad_right->AdjustPosition(5, 0, 5);
	quad_right->scl = XMFLOAT3(3, 3, 3);
	quad_right->sclVector = XMLoadFloat3(&quad_right->scl);*/

	/*quad_up = new Model;
	quad_up->Initialize("Data//quad.fbx", device.Get(), deviceContext.Get(), NULL, cb_vs_vertexshader, 0);
	quad_up->AdjustPosition(-5, 0, 5);
	quad_up->scl = XMFLOAT3(3, 3, 3);
	quad_up->sclVector = XMLoadFloat3(&quad_up->scl);*/

	/*quad_down = new Model;
	quad_down->Initialize("Data//quad.fbx", device.Get(), deviceContext.Get(), NULL, cb_vs_vertexshader, 0);
	quad_down->AdjustPosition(-5, 0, -5);
	
	quad_down->scl = XMFLOAT3(3, 3, 3);
	quad_down->sclVector = XMLoadFloat3(&quad_down->scl);*/

	quad_ground = new Model;
	quad_ground->Initialize("Data//ground.fbx", device.Get(), deviceContext.Get(), NULL, cb_vs_vertexshader, 0);
	quad_ground->AdjustPosition(0,-2, 0);
	quad_ground->scl = XMFLOAT3(20, 1, 20);
	quad_ground->sclVector = XMLoadFloat3(&quad_ground->scl);
	quad_ground->useDebugMesh = false;
	obstacles.push_back(quad_ground);

	gjk = new GJKCollision;
	gjktest = XMVectorZero();
#pragma region material sphere
	/*sphere_AluminiumInsulator = new Model;
	sphere_AluminiumInsulator->Initialize("Data//Sphere.fbx", device.Get(), deviceContext.Get(), NULL, cb_vs_vertexshader,false);
	sphere_AluminiumInsulator->AdjustPosition(150, -50, 0);
	sphere_AluminiumInsulator->sclVector = XMLoadFloat3(&quad->scl);

	sphere_CamoFabric = new Model;
	sphere_CamoFabric->Initialize("Data//Sphere.fbx", device.Get(), deviceContext.Get(), NULL, cb_vs_vertexshader, false);
	sphere_CamoFabric->AdjustPosition(200, -50, 0);
	sphere_CamoFabric->sclVector = XMLoadFloat3(&quad->scl);

	sphere_GlassVisor = new Model;
	sphere_GlassVisor->Initialize("Data//Sphere.fbx", device.Get(), deviceContext.Get(), NULL, cb_vs_vertexshader, false);
	sphere_GlassVisor->AdjustPosition(250, -50, 0);
	sphere_GlassVisor->sclVector = XMLoadFloat3(&quad->scl);

	sphere_Gold = new Model;
	sphere_Gold->Initialize("Data//Sphere.fbx", device.Get(), deviceContext.Get(), NULL, cb_vs_vertexshader, false);
	sphere_Gold->AdjustPosition(300, -50, 0);
	sphere_Gold->sclVector = XMLoadFloat3(&quad->scl);

	sphere_GunMetal = new Model;
	sphere_GunMetal->Initialize("Data//Sphere.fbx", device.Get(), deviceContext.Get(), NULL, cb_vs_vertexshader, false);
	sphere_GunMetal->AdjustPosition(150, 0, 0);
	sphere_GunMetal->sclVector = XMLoadFloat3(&quad->scl);

	sphere_IronOld = new Model;
	sphere_IronOld->Initialize("Data//Sphere.fbx", device.Get(), deviceContext.Get(), NULL, cb_vs_vertexshader, false);
	sphere_IronOld->AdjustPosition(200, 0, 0);
	sphere_IronOld->sclVector = XMLoadFloat3(&quad->scl);

	sphere_Rubber = new Model;
	sphere_Rubber->Initialize("Data//Sphere.fbx", device.Get(), deviceContext.Get(), NULL, cb_vs_vertexshader, false);
	sphere_Rubber->AdjustPosition(250, 0, 0);
	sphere_Rubber->sclVector = XMLoadFloat3(&quad->scl);

	sphere_SuperHeroFabric = new Model;
	sphere_SuperHeroFabric->Initialize("Data//Sphere.fbx", device.Get(), deviceContext.Get(), NULL, cb_vs_vertexshader, false);
	sphere_SuperHeroFabric->AdjustPosition(300, 0, 0);
	sphere_SuperHeroFabric->sclVector = XMLoadFloat3(&quad->scl);

	terrian= new Model;
	terrian->Initialize("Data//terrian_01.fbx", device.Get(), deviceContext.Get(), NULL, cb_vs_vertexshader, false);
	terrian->AdjustPosition(400, 0, 0);
	terrian->scl = XMFLOAT3(1000,1000,1000);
	terrian->sclVector = XMLoadFloat3(&terrian->scl);*/
#pragma endregion

	primeray.origin = XMVectorZero();
	primeray.direction = g_XMIdentityR2;

	if (!InitializeScene())
		return false;

	if (!InitializeIBLStatus())
		return false;

	DWORD s_indices[] =
	{
		0, 1, 2, //FRONT
		0, 2, 3, //FRONT
		4, 7, 6, //BACK 
		4, 6, 5, //BACK
		3, 2, 6, //RIGHT SIDE
		3, 6, 7, //RIGHT SIDE
		4, 5, 1, //LEFT SIDE
		4, 1, 0, //LEFT SIDE
		1, 5, 6, //TOP
		1, 6, 2, //TOP
		0, 3, 7, //BOTTOM
		0, 7, 4, //BOTTOM
	};

	//Setup ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	//io.Fonts->AddFontFromFileTTF("Data/Fonts/HGRSMP.TTF", 10.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(this->device.Get(), this->deviceContext.Get());
	ImGui::StyleColorsDark();

	return true;
}

void Graphics::RenderFrame(float dt)
{

	CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(this->windowWidth), static_cast<float>(this->windowHeight));;
	this->deviceContext->RSSetViewports(1, &viewport);
	this->deviceContext->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), this->depthStencilView.Get());

	float bgcolor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	this->deviceContext->ClearRenderTargetView(this->renderTargetView.Get(), bgcolor);
	this->deviceContext->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->deviceContext->OMSetBlendState(this->blendState.Get(), NULL, 0xFFFFFFFF);

	deviceContext->IASetInputLayout(vertexshader_skyBox.GetInputLayout());
	deviceContext->VSSetShader(vertexshader_skyBox.GetShader(), NULL, 0);
	deviceContext->PSSetShader(pixelshader_skyBox.GetShader(), NULL, 0);
	deviceContext->RSSetState(this->rasterizerState.Get());
	deviceContext->OMSetDepthStencilState(depthUnenableStencilState.Get(), 0);

	auto viewmat = camera.GetViewMatrix();
	viewmat.r[3] = g_XMIdentityR3;
	skybox->model->transfomrMatirx = XMMatrixIdentity();
	//skybox->model->SetTexture(skybox->textureView2.Get());
	this->deviceContext->PSSetShaderResources(0, 1, skybox->textureView.GetAddressOf());
	skybox->model->Draw(viewmat * camera.GetProjectionMatrix());

	this->deviceContext->IASetInputLayout(this->vertexshader.GetInputLayout());
	this->deviceContext->RSSetState(this->rasterizerState_CullFront.Get());
	this->deviceContext->OMSetDepthStencilState(this->depthStencilState.Get(), 0);
	this->deviceContext->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());
	this->deviceContext->VSSetShader(vertexshader.GetShader(), NULL, 0);
	this->deviceContext->PSSetShader(pixelmatshader.GetShader(), NULL, 0);

	UINT offset = 0;
	static float alpha = 1.0f;
	static float roughness = 0.5f;
	static float anisotropic = 0.0f;
	static float clearcoat = 0.0f;
	static float clearcoatGloss = 1.0f;
	static float sheen = 0.0f;
	static float sheenTint = 0.0f;
	static float specular = 0.5f;
	static float specularTint = 1.0f;
	static float subsurface = 0.0f;
	static float metallic = 0.0f;
	static XMFLOAT3 baseColor = cb_ps_BSDFData.data.baseColor;
	static XMFLOAT3 lightcol = cb_ps_lightdata.data.lightCol;
	static XMFLOAT3 lightpos = cb_ps_lightdata.data.lightPos1;

	{ //Pavement Cube Texture
		//Update Constant Buffer
		this->cb_ps_pixelshader.data.alpha = alpha;
		this->cb_ps_pixelshader.data.metallic = metallic;
		this->cb_ps_pixelshader.data.roughness = roughness;
		this->cb_ps_pixelshader.data.albedo = XMFLOAT3(1, 1, 1);
		this->cb_ps_pixelshader.data.ao = 1.0f;
		this->cb_ps_pixelshader.data.cameraPosition = camera.GetPositionFloat3();
		this->cb_ps_pixelshader.data.normaltype = 1;
		this->cb_ps_pixelshader.ApplyChanges();
		this->deviceContext->PSSetConstantBuffers(0, 1, this->cb_ps_pixelshader.GetAddressOf());

		this->cb_ps_lightdata.data.lightCol = lightcol;
		this->cb_ps_lightdata.data.lightPos1 = lightpos;
		this->cb_ps_lightdata.ApplyChanges();
		this->deviceContext->PSSetConstantBuffers(1, 1, this->cb_ps_lightdata.GetAddressOf());

#pragma region Draw Player Character

		this->deviceContext->PSSetShaderResources(0, 1, &skyIBLSRV);//  
		this->deviceContext->PSSetShaderResources(1, 1, &envMapSRV);//
		this->deviceContext->PSSetShaderResources(2, 1, &brdfLUTSRV);

		this->deviceContext->PSSetShaderResources(3, 1, this->Robot_Albedo.GetAddressOf());
		this->deviceContext->PSSetShaderResources(4, 1, this->Robot_Normal.GetAddressOf());
		this->deviceContext->PSSetShaderResources(5, 1, this->Robot_Metallic.GetAddressOf());
		this->deviceContext->PSSetShaderResources(6, 1, this->Robot_Roughness.GetAddressOf());
		this->deviceContext->PSSetShaderResources(9, 1, this->Robot_Emission.GetAddressOf());
		this->deviceContext->PSSetShaderResources(10, 1, this->Robot_Occlusion.GetAddressOf());

		//this->deviceContext->PSSetShader(pixelshader.GetShader(), NULL, 0);
		cb_ps_BSDFData.data.roughness = roughness;
		cb_ps_BSDFData.data.anisotropic = anisotropic;
		cb_ps_BSDFData.data.clearcoat = clearcoat;
		cb_ps_BSDFData.data.clearcoatGloss = clearcoatGloss;
		cb_ps_BSDFData.data.sheen = sheen;
		cb_ps_BSDFData.data.sheenTint = sheenTint;
		cb_ps_BSDFData.data.specular = specular;
		cb_ps_BSDFData.data.specularTint = specularTint;
		cb_ps_BSDFData.data.subsurface = subsurface;
		cb_ps_BSDFData.data.metallic = metallic;
		cb_ps_BSDFData.data.baseColor = baseColor;
		cb_ps_BSDFData.data.usepurecolor = 0;
		cb_ps_BSDFData.ApplyChanges();
		this->deviceContext->PSSetConstantBuffers(2, 1, this->cb_ps_BSDFData.GetAddressOf());

		model->scl = XMFLOAT3(0.01f, 0.01f, 0.01f);
		model->sclVector = XMLoadFloat3(&model->scl);
		if (debugswitch)
		{
			model->Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());
		}
		if (model->useDebugMesh) {
			cb_ps_BSDFData.data.baseColor = XMFLOAT3(1, 0, 0);
			cb_ps_BSDFData.data.usepurecolor = 1;
			cb_ps_BSDFData.ApplyChanges();
			model->DrawP(camera.GetViewMatrix() * camera.GetProjectionMatrix());
		}

#pragma endregion

		this->cb_ps_pixelshader.data.normaltype = 0;
		this->cb_ps_pixelshader.ApplyChanges();

#pragma region Draw Floor Block

		this->deviceContext->PSSetShaderResources(3, 1, this->LabBlock_Albedo.GetAddressOf());//LabBlock_Albedo
		this->deviceContext->PSSetShaderResources(4, 1, this->LabBlock_Normal.GetAddressOf());
		this->deviceContext->PSSetShaderResources(5, 1, this->LabBlock_Metallic.GetAddressOf());
		this->deviceContext->PSSetShaderResources(6, 1, this->LabBlock_Roughness.GetAddressOf());

		ID3D11ShaderResourceView* nullSRV[1] = { NULL };
		this->deviceContext->PSSetShaderResources(9, 1, nullSRV);
		this->deviceContext->PSSetShaderResources(10, 1, nullSRV);
		//nullSRV[0]->Release();

		if (debugswitch)
		{
			cb_ps_BSDFData.data.baseColor = baseColor;
			cb_ps_BSDFData.data.usepurecolor = 0;
			cb_ps_BSDFData.ApplyChanges();
			
			quad_left->Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());

			quad_ground->Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());
		}
		if (quad->useDebugMesh) {

			cb_ps_BSDFData.data.baseColor = XMFLOAT3(1, 0, 0);

			cb_ps_BSDFData.data.usepurecolor = 1;
			cb_ps_BSDFData.ApplyChanges();

			quad_left->DrawP(camera.GetViewMatrix() * camera.GetProjectionMatrix());
		}
#pragma endregion

#pragma region Draw Floor

		this->deviceContext->PSSetShaderResources(3, 1, this->LabFloor_Albedo.GetAddressOf());
		this->deviceContext->PSSetShaderResources(4, 1, this->LabFloor_Normal.GetAddressOf());
		this->deviceContext->PSSetShaderResources(5, 1, this->LabFloor_Metallic.GetAddressOf());
		this->deviceContext->PSSetShaderResources(6, 1, this->LabFloor_Roughness.GetAddressOf());
		this->deviceContext->PSSetShaderResources(10, 1, this->LabFloor_Occlusion.GetAddressOf());

		this->deviceContext->PSSetShaderResources(9, 1, nullSRV);

		if (debugswitch)
		{
			cb_ps_BSDFData.data.baseColor = baseColor;
			cb_ps_BSDFData.data.usepurecolor = 0;
			cb_ps_BSDFData.ApplyChanges();
			quad_ground->Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());
		}
		if (quad->useDebugMesh) {
			cb_ps_BSDFData.data.baseColor = XMFLOAT3(1, 0, 0);
			cb_ps_BSDFData.data.usepurecolor = 1;
			cb_ps_BSDFData.ApplyChanges();
			quad_ground->DrawP(camera.GetViewMatrix() * camera.GetProjectionMatrix());
		}

#pragma endregion

#pragma region Draw Drone

		this->deviceContext->PSSetShaderResources(3, 1, this->drone_Albedo.GetAddressOf());
		this->deviceContext->PSSetShaderResources(4, 1, this->drone_Normal.GetAddressOf());
		this->deviceContext->PSSetShaderResources(5, 1, this->drone_Metallic.GetAddressOf());
		this->deviceContext->PSSetShaderResources(6, 1, this->drone_Roughness.GetAddressOf());
		//this->deviceContext->PSSetShaderResources(7, 1, this->Leather_Height.GetAddressOf());

		this->deviceContext->PSSetShaderResources(9, 1, nullSRV);
		this->deviceContext->PSSetShaderResources(10, 1, nullSRV);
		if (debugswitch)
		{
			cb_ps_BSDFData.data.baseColor = baseColor;
			cb_ps_BSDFData.data.usepurecolor = 0;
			cb_ps_BSDFData.ApplyChanges();
			drone->Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());
		}
		if (quad->useDebugMesh) {

			cb_ps_BSDFData.data.baseColor = XMFLOAT3(1, 0, 0);

			cb_ps_BSDFData.data.usepurecolor = 1;
			cb_ps_BSDFData.ApplyChanges();
			drone->DrawP(camera.GetViewMatrix() * camera.GetProjectionMatrix());
		}
	}

#pragma endregion

#pragma region draw material sphere

	//cb_ps_BSDFData.data.usepurecolor = 0;
	//cb_ps_BSDFData.ApplyChanges();
	//this->deviceContext->PSSetShaderResources(3, 1, this->AluminiumInsulator_Albedo.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(4, 1, this->AluminiumInsulator_Normal.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(5, 1, this->AluminiumInsulator_Metallic.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(6, 1, this->AluminiumInsulator_Roughness.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(7, 1, this->AluminiumInsulator_Height.GetAddressOf());
	////sphere_AluminiumInsulator->Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());

	//this->deviceContext->PSSetShaderResources(3, 1, this->CamoFabric_Albedo.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(4, 1, this->CamoFabric_Normal.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(5, 1, this->CamoFabric_Metallic.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(6, 1, this->CamoFabric_Roughness.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(7, 1, this->CamoFabric_Height.GetAddressOf());
	////sphere_CamoFabric->Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());

	//this->deviceContext->PSSetShaderResources(3, 1, this->GlassVisor_Albedo.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(4, 1, this->GlassVisor_Normal.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(5, 1, this->GlassVisor_Metallic.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(6, 1, this->GlassVisor_Roughness.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(7, 1, this->GlassVisor_Height.GetAddressOf());
	////sphere_GlassVisor->Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());

	//this->deviceContext->PSSetShaderResources(3, 1, this->Gold_Albedo.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(4, 1, this->Gold_Normal.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(5, 1, this->Gold_Metallic.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(6, 1, this->Gold_Roughness.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(7, 1, this->Gold_Height.GetAddressOf());
	////sphere_Gold->Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());

	//this->deviceContext->PSSetShaderResources(3, 1, this->GunMetal_Albedo.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(4, 1, this->GunMetal_Normal.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(5, 1, this->GunMetal_Metallic.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(6, 1, this->GunMetal_Roughness.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(7, 1, this->GunMetal_Height.GetAddressOf());
	////sphere_GunMetal->Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());

	//this->deviceContext->PSSetShaderResources(3, 1, this->IronOld_Albedo.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(4, 1, this->IronOld_Normal.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(5, 1, this->IronOld_Metallic.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(6, 1, this->IronOld_Roughness.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(7, 1, this->IronOld_Height.GetAddressOf());
	////sphere_IronOld->Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());

	//this->deviceContext->PSSetShaderResources(3, 1, this->Rubber_Albedo.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(4, 1, this->Rubber_Normal.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(5, 1, this->Rubber_Metallic.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(6, 1, this->Rubber_Roughness.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(7, 1, this->Rubber_Height.GetAddressOf());
	////sphere_Rubber->Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());

	//this->deviceContext->PSSetShaderResources(3, 1, this->SuperHeroFabric_Albedo.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(4, 1, this->SuperHeroFabric_Normal.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(5, 1, this->SuperHeroFabric_Metallic.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(6, 1, this->SuperHeroFabric_Roughness.GetAddressOf());
	//this->deviceContext->PSSetShaderResources(7, 1, this->SuperHeroFabric_Height.GetAddressOf());
	////sphere_SuperHeroFabric->Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());

	////terrian->Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());
#pragma endregion

#pragma region Draw Text
	/*static int fpsCounter = 0;
	static std::string fpsString = "FPS: 0";
	fpsCounter += 1;
	if (fpsTimer.GetMilisecondsElapsed() > 1000.0)
	{
		fpsString = "FPS: " + std::to_string(fpsCounter);
		fpsCounter = 0;
		fpsTimer.Restart();
	}*/
	spriteBatch->Begin();
	
	auto text = "collision detect: " + std::to_string(gjkcol) + " mtk: " + std::to_string(XMVectorGetX(gjktest)) + " " + std::to_string(XMVectorGetY(gjktest)) + " " + std::to_string(XMVectorGetZ(gjktest));
	//auto text = "press mouse right to change the view port/npress space key to rise the camera /npress Z key to rise the camera /n";
	//auto text = "";

	spriteFont->DrawString(spriteBatch.get(), StringConverter::StringToWide(text).c_str(), DirectX::XMFLOAT2(0, 0), DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));
	spriteBatch->End();
#pragma endregion

#pragma region Draw ImGui

	//static int counter = 0;
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	//Create ImGui Test Window

	ImGui::Begin("tool bar");
	ImGui::Checkbox("show model", &debugswitch);
	ImGui::DragFloat("alpha", &alpha, 0.1f, 0.0f, 1.0f);
	ImGui::DragFloat("Roughness", &roughness, 0.1f, 0.0f, 1.0f);
	ImGui::DragFloat("metallic", &metallic, 0.1f, 0.0f, 1.0f);
	ImGui::DragFloat("Anisotropic", &anisotropic, 0.1f, 0.0f, 1.0f);
	ImGui::DragFloat("Clearcoat", &clearcoat, 0.1f, 0.0f, 1.0f);
	ImGui::DragFloat("ClearcoatGloss", &clearcoatGloss, 0.1f, 0.0f, 1.0f);
	ImGui::DragFloat("Sheen", &sheen, 0.1f, 0.0f, 1.0f);
	ImGui::DragFloat("SheenTint", &sheenTint, 0.1f, 0.0f, 1.0f);
	ImGui::DragFloat("Specular", &specular, 0.1f, 0.0f, 1.0f);
	ImGui::DragFloat("SpecularTint", &specularTint, 0.1f, 0.0f, 1.0f);
	ImGui::DragFloat("Subsurface", &subsurface, 0.1f, 0.0f, 1.0f);
	ImGui::DragFloat3("BaseColor", &baseColor.x, 0.1f, 0.0f, 1.0f);
	ImGui::DragFloat3("LightColor", &lightcol.x, 10.0f, -1000.0f, 1000.0f);
	ImGui::DragFloat3("LightPos", &lightpos.x, 10.0f, -1000.0f, 1000.0f);

	ImGui::End();
	//Assemble Together Draw Data
	ImGui::Render();
	//Render Draw Data
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

#pragma endregion

	this->swapchain->Present(0, NULL);
}

bool Graphics::InitializeDirectX(HWND hwnd)
{
	try
	{
		std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

		if (adapters.size() < 1)
		{
			ErrorLogger::Log("No IDXGI Adapters found.");
			return false;
		}

		DXGI_SWAP_CHAIN_DESC scd = { 0 };

		scd.BufferDesc.Width = this->windowWidth;
		scd.BufferDesc.Height = this->windowHeight;
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;

		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount = 1;
		scd.OutputWindow = hwnd;
		scd.Windowed = TRUE;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		HRESULT hr;
		hr = D3D11CreateDeviceAndSwapChain(adapters[0].pAdapter, //IDXGI Adapter
			D3D_DRIVER_TYPE_UNKNOWN,
			NULL, //FOR SOFTWARE DRIVER TYPE
			NULL, //FLAGS FOR RUNTIME LAYERS
			NULL, //FEATURE LEVELS ARRAY
			0, //# OF FEATURE LEVELS IN ARRAY
			D3D11_SDK_VERSION,
			&scd, //Swapchain description
			this->swapchain.GetAddressOf(), //Swapchain Address
			this->device.GetAddressOf(), //Device Address
			NULL, //Supported feature level
			this->deviceContext.GetAddressOf()); //Device Context Address

		COM_ERROR_IF_FAILED(hr, "Failed to create device and swapchain.");

		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
		hr = this->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
		COM_ERROR_IF_FAILED(hr, "GetBuffer Failed.");

		hr = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->renderTargetView.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create render target view.");

		//Describe our Depth/Stencil Buffer
		CD3D11_TEXTURE2D_DESC depthStencilTextureDesc(DXGI_FORMAT_D24_UNORM_S8_UINT, this->windowWidth, this->windowHeight);
		depthStencilTextureDesc.MipLevels = 1;
		depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		hr = this->device->CreateTexture2D(&depthStencilTextureDesc, NULL, this->depthStencilBuffer.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil buffer.");

		hr = this->device->CreateDepthStencilView(this->depthStencilBuffer.Get(), NULL, this->depthStencilView.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil view.");

		this->deviceContext->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), this->depthStencilView.Get());

		//Create depth stencil state
		CD3D11_DEPTH_STENCIL_DESC depthstencildesc(D3D11_DEFAULT);
		depthstencildesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

		hr = this->device->CreateDepthStencilState(&depthstencildesc, this->depthStencilState.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil state.");

		depthstencildesc.DepthEnable = false;//
		depthstencildesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthstencildesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		hr = this->device->CreateDepthStencilState(&depthstencildesc, this->depthUnenableStencilState.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil state.");

		depthstencildesc = {};
		depthstencildesc.DepthEnable = true;
		depthstencildesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthstencildesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		hr = this->device->CreateDepthStencilState(&depthstencildesc, this->IBLskydepthStencilState.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil state.");
		

		//Create & set the Viewport
		CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(this->windowWidth), static_cast<float>(this->windowHeight));;
		this->deviceContext->RSSetViewports(1, &viewport);

		//Create Rasterizer State
		CD3D11_RASTERIZER_DESC rasterizerDesc(D3D11_DEFAULT);
		rasterizerDesc.DepthClipEnable = false;
		hr = this->device->CreateRasterizerState(&rasterizerDesc, this->rasterizerState.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state.");

		//Create Rasterizer State for culling front
		CD3D11_RASTERIZER_DESC rasterizerDesc_CullFront(D3D11_DEFAULT);
		rasterizerDesc_CullFront.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc_CullFront.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		//rasterizerDesc_CullFront.DepthClipEnable = true;//
		hr = this->device->CreateRasterizerState(&rasterizerDesc_CullFront, this->rasterizerState_CullFront.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state.");

		//Create Rasterizer State for iblsky
		CD3D11_RASTERIZER_DESC rasterizerDesc_iblsky = {};
		rasterizerDesc_iblsky.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc_iblsky.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		rasterizerDesc_iblsky.DepthClipEnable = true;//
		hr = this->device->CreateRasterizerState(&rasterizerDesc_iblsky, this->IBLskyrasterizerState.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state.");

		////Create Rasterizer State
		//CD3D11_RASTERIZER_DESC rasterizerDesc(D3D11_DEFAULT);
		//rasterizerDesc.DepthClipEnable = false;
		//hr = this->device->CreateRasterizerState(&rasterizerDesc, this->rasterizerState.GetAddressOf());
		//COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state.");

		////Create Rasterizer State for culling front
		//CD3D11_RASTERIZER_DESC rasterizerDesc_CullFront(D3D11_DEFAULT);
		//rasterizerDesc_CullFront.FillMode = D3D11_FILL_SOLID;
		//rasterizerDesc_CullFront.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		//hr = this->device->CreateRasterizerState(&rasterizerDesc_CullFront, this->rasterizerState_CullFront.GetAddressOf());
		//COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state.");

		//Create Blend State
		D3D11_RENDER_TARGET_BLEND_DESC rtbd = { 0 };
		rtbd.BlendEnable = true;
		rtbd.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		rtbd.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		rtbd.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		rtbd.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		rtbd.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
		rtbd.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

		D3D11_BLEND_DESC blendDesc = { 0 };
		blendDesc.RenderTarget[0] = rtbd;

		hr = this->device->CreateBlendState(&blendDesc, this->blendState.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create blend state.");

		spriteBatch = std::make_unique<DirectX::SpriteBatch>(this->deviceContext.Get());
		spriteFont = std::make_unique<DirectX::SpriteFont>(this->device.Get(), L"Data\\Fonts\\comic_sans_ms_16.spritefont");
		//spriteFont = std::make_unique<DirectX::SpriteFont>(this->device.Get(), L"Data\\Fonts\\japanese.spritefont");


		//Create sampler description for sampler state
		CD3D11_SAMPLER_DESC sampDesc(D3D11_DEFAULT);
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;// D3D11_TEXTURE_ADDRESS_WRAP D3D11_TEXTURE_ADDRESS_BORDER
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;//D3D11_COMPARISON_NEVER D3D11_COMPARISON_LESS_EQUAL D3D11_COMPARISON_ALWAYS
		sampDesc.BorderColor[0] = 0;
		sampDesc.BorderColor[1] = 0;
		sampDesc.BorderColor[2] = 0;
		sampDesc.BorderColor[3] = 0;
		//sampDesc.MaxAnisotropy = 16;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		sampDesc.MinLOD = 0;
		hr = this->device->CreateSamplerState(&sampDesc, this->samplerState.GetAddressOf()); //Create sampler state
		COM_ERROR_IF_FAILED(hr, "Failed to create sampler state.");
	}
	catch (COMException & exception)
	{
		ErrorLogger::Log(exception);
		return false;
	}
	return true;
}

bool Graphics::InitializeShaders()
{

	std::wstring shaderfolder = L"";
#pragma region DetermineShaderPath
	if (IsDebuggerPresent() == TRUE)
	{
#ifdef _DEBUG //Debug Mode
	#ifdef _WIN64 //x64
			shaderfolder = L"..\\x64\\Debug\\";
	#else  //x86 (Win32)
			shaderfolder = L"..\\Debug\\";
	#endif
	#else //Release Mode
	#ifdef _WIN64 //x64
			shaderfolder = L"..\\x64\\Release\\";
	#else  //x86 (Win32)
			shaderfolder = L"..\\Release\\";
	#endif
#endif
	}

	D3D11_INPUT_ELEMENT_DESC layout3D[] =
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
		{"NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
		{"TANGENT", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
	};

	UINT numElements3D = ARRAYSIZE(layout3D);

	if (!vertexshader.Initialize(this->device, shaderfolder + L"vertexshader.cso", layout3D, numElements3D))
		return false;

	if (!pixelshader.Initialize(this->device, shaderfolder + L"pixelshader.cso"))
		return false;

	if (!pixelmatshader.Initialize(this->device, shaderfolder + L"pixelmatshader.cso"))
		return false;

	if (!vertexshader_skyBox.Initialize(this->device, shaderfolder + L"vertexshader_skyBox.cso", layout3D, numElements3D))
		return false;

	if (!pixelshader_skyBox.Initialize(this->device, shaderfolder + L"pixelshader_skyBox.cso"))
		return false;

	if (!ConvolutionPixelShader.Initialize(this->device, shaderfolder + L"ConvolutionPixelShader.cso"))
		return false;

	if (!PrefilterMapPixelShader.Initialize(this->device, shaderfolder + L"PrefilterMapPixelShader.cso"))
		return false;

	if (!IntegrateBRDFPixelShader.Initialize(this->device, shaderfolder + L"IntegrateBRDFPixelShader.cso"))
		return false;

	if (!DisneyBSDFPixelShader.Initialize(this->device, shaderfolder + L"pixelDisneyBSDFshader.cso"))
		return false;

	return true;
}

bool Graphics::InitializeScene()
{
	try
	{
		//Textured Square
		Vertex v[] =
		{
			Vertex(-0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,0.0f,0.0f,0.0f), //FRONT Bottom Left   - [0]
			Vertex(-0.5f,   0.5f, -0.5f, 0.0f, 0.0f,0.0f,0.0f,0.0f), //FRONT Top Left      - [1]
			Vertex(0.5f,   0.5f, -0.5f, 1.0f, 0.0f,0.0f,0.0f,0.0f), //FRONT Top Right     - [2]
			Vertex(0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,0.0f,0.0f,0.0f), //FRONT Bottom Right   - [3]
			Vertex(-0.5f,  -0.5f, 0.5f, 0.0f, 1.0f,0.0f,0.0f,0.0f), //BACK Bottom Left   - [4]
			Vertex(-0.5f,   0.5f, 0.5f, 0.0f, 0.0f,0.0f,0.0f,0.0f), //BACK Top Left      - [5]
			Vertex(0.5f,   0.5f, 0.5f, 1.0f, 0.0f,0.0f,0.0f,0.0f), //BACK Top Right     - [6]
			Vertex(0.5f,  -0.5f, 0.5f, 1.0f, 1.0f,0.0f,0.0f,0.0f), //BACK Bottom Right   - [7]
		};

		//Load Vertex Data
		HRESULT hr = this->vertexBuffer.Initialize(this->device.Get(), v, ARRAYSIZE(v));
		COM_ERROR_IF_FAILED(hr, "Failed to initialize vertex buffer.");

		DWORD indices[] =
		{
			0, 1, 2, //FRONT
			0, 2, 3, //FRONT
			4, 7, 6, //BACK 
			4, 6, 5, //BACK
			3, 2, 6, //RIGHT SIDE
			3, 6, 7, //RIGHT SIDE
			4, 5, 1, //LEFT SIDE
			4, 1, 0, //LEFT SIDE
			1, 5, 6, //TOP
			1, 6, 2, //TOP
			0, 3, 7, //BOTTOM
			0, 7, 4, //BOTTOM
		};

		//Load Index Data

		hr = this->indicesBuffer.Initialize(this->device.Get(), indices, ARRAYSIZE(indices));
		COM_ERROR_IF_FAILED(hr, "Failed to initialize index buffer.");

#pragma region Create Material

		//Load Texture
		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\seamless_grass.jpg", nullptr, grassTexture.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\pinksquare.jpg", nullptr, pinkTexture.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\seamless_pavement.jpg", nullptr, pavementTexture.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\Leather_Roughness.png", nullptr, Leather_Roughness.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\Leather_Albedo.png", nullptr, Leather_Albedo.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\Leather_Height.png", nullptr, Leather_Height.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\Leather_Metallic.png", nullptr, Leather_Metallic.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\Leather_Normal.png", nullptr, Leather_Normal.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

#pragma region laboratory block

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\LabBlock_roughness.jpeg", nullptr, LabBlock_Roughness.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\LabBlock_albedo.jpeg", nullptr, LabBlock_Albedo.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\LabBlock_normal.jpeg", nullptr, LabBlock_Normal.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\LabBlock_metallic.jpeg", nullptr, LabBlock_Metallic.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

#pragma endregion

#pragma region DropBlock
		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\model_Kitava_Roughness.png", nullptr, DropBlock_Roughness.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\model_Kitava_BaseColor.png", nullptr, DropBlock_Albedo.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\model_Kitava_Normal.png", nullptr, DropBlock_Normal.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\model_Kitava_Metallic.png", nullptr, DropBlock_Metallic.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

#pragma endregion


#pragma region laboratory floor

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\LabFloor_roughness.jpeg", nullptr, LabFloor_Roughness.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\LabFloor_albedo.jpeg", nullptr, LabFloor_Albedo.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\LabFloor_normal.jpeg", nullptr, LabFloor_Normal.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\LabFloor_metallic.jpeg", nullptr, LabFloor_Metallic.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\LabFloor_AO.jpeg", nullptr, LabFloor_Occlusion.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");
		

#pragma endregion
#pragma region drone

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\Final_lambert1_Roughness.png", nullptr, drone_Roughness.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\Final_lambert1_BaseColor.png", nullptr, drone_Albedo.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\Final_lambert1_Normal.png", nullptr, drone_Normal.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\Final_lambert1_Metallic.png", nullptr, drone_Metallic.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

#pragma endregion
#pragma region AluminiumInsulator

		//hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\AluminiumInsulator_Roughness.png", nullptr, AluminiumInsulator_Roughness.GetAddressOf());
		//COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		//hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\AluminiumInsulator_Albedo.png", nullptr, AluminiumInsulator_Albedo.GetAddressOf());
		//COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		//hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\AluminiumInsulator_Height.png", nullptr, AluminiumInsulator_Height.GetAddressOf());
		//COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		//hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\AluminiumInsulator_Metallic.png", nullptr, AluminiumInsulator_Metallic.GetAddressOf());
		//COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		//hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\AluminiumInsulator_Normal.png", nullptr, AluminiumInsulator_Normal.GetAddressOf());
		//COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

#pragma endregion

#pragma region Wood

		/*hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\Wood_Roughness.png", nullptr, Wood_Roughness.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\Wood_Albedo.png", nullptr, Wood_Albedo.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\Wood_Height.png", nullptr, Wood_Height.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\Wood_Metallic.png", nullptr, Wood_Metallic.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\Wood_Normal.png", nullptr, Wood_Normal.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");*/

#pragma endregion

#pragma region CamoFabric

		/*hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\CamoFabric_Roughness.png", nullptr, CamoFabric_Roughness.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\CamoFabric_Albedo.png", nullptr, CamoFabric_Albedo.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\CamoFabric_Height.png", nullptr, CamoFabric_Height.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\CamoFabric_Metallic.png", nullptr, CamoFabric_Metallic.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\CamoFabric_Normal.png", nullptr, CamoFabric_Normal.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\CamoFabric_Roughness.png", nullptr, CamoFabric_Roughness.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");*/
#pragma endregion

#pragma region GlassVisor

		/*hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\GlassVisor_Albedo.png", nullptr, GlassVisor_Albedo.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\GlassVisor_Height.png", nullptr, GlassVisor_Height.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\GlassVisor_Metallic.png", nullptr, GlassVisor_Metallic.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\GlassVisor_Normal.png", nullptr, GlassVisor_Normal.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\GlassVisor_Roughness.png", nullptr, GlassVisor_Roughness.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");*/

#pragma endregion

#pragma region Gold

		/*hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\Gold_Albedo.png", nullptr, Gold_Albedo.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\Gold_Height.png", nullptr, Gold_Height.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\Gold_Metallic.png", nullptr, Gold_Metallic.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\Gold_Normal.png", nullptr, Gold_Normal.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\Gold_Roughness.png", nullptr, Gold_Roughness.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");*/

#pragma endregion

#pragma region GunMetal

		/*hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\GunMetal_Albedo.png", nullptr, GunMetal_Albedo.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\GunMetal_Height.png", nullptr, GunMetal_Height.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\GunMetal_Metallic.png", nullptr, GunMetal_Metallic.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\GunMetal_Normal.png", nullptr, GunMetal_Normal.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\GunMetal_Roughness.png", nullptr, GunMetal_Roughness.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");
*/

#pragma endregion

#pragma region Ironold material


		//hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\IronOld_Albedo.png", nullptr, IronOld_Albedo.GetAddressOf());
		//COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		//hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\IronOld_Height.png", nullptr, IronOld_Height.GetAddressOf());
		//COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		//hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\IronOld_Metallic.png", nullptr, IronOld_Metallic.GetAddressOf());
		//COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		//hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\IronOld_Normal.png", nullptr, IronOld_Normal.GetAddressOf());
		//COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		//hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\IronOld_Roughness.png", nullptr, IronOld_Roughness.GetAddressOf());
		//COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");
#pragma endregion

#pragma region iron metarial

		//hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\Rubber_Albedo.png", nullptr, Rubber_Albedo.GetAddressOf());
		//COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		//hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\Rubber_Height.png", nullptr, Rubber_Height.GetAddressOf());
		//COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		//hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\Rubber_Metallic.png", nullptr, Rubber_Metallic.GetAddressOf());
		//COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		//hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\Rubber_Normal.png", nullptr, Rubber_Normal.GetAddressOf());
		//COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		//hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\Rubber_Roughness.png", nullptr, Rubber_Roughness.GetAddressOf());
		//COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

#pragma endregion

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\SuperHeroFabric_Albedo.png", nullptr, SuperHeroFabric_Albedo.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\SuperHeroFabric_Height.png", nullptr, SuperHeroFabric_Height.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\SuperHeroFabric_Metallic.png", nullptr, SuperHeroFabric_Metallic.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\SuperHeroFabric_Normal.png", nullptr, SuperHeroFabric_Normal.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\SuperHeroFabric_Roughness.png", nullptr, SuperHeroFabric_Roughness.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\Robot_4.tga.png", nullptr, Robot_Albedo.GetAddressOf());//
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\Robot_4_Emission.tga.png", nullptr, Robot_Emission.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\Robot_4_Metalic.tga.png", nullptr, Robot_Metallic.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\Robot_4_Normals.tga.png", nullptr, Robot_Normal.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\Robot_4_Roughness.tga.png", nullptr, Robot_Roughness.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\Robot_4_Occlusion.tga.png", nullptr, Robot_Occlusion.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

#pragma endregion

		//Initialize Constant Buffer(s)
		hr = this->cb_vs_vertexshader.Initialize(this->device.Get(), this->deviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

		hr = this->cb_ps_pixelshader.Initialize(this->device.Get(), this->deviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

		hr = this->cb_ps_lightdata.Initialize(this->device.Get(), this->deviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");
		cb_ps_lightdata.data.lightPos1 = XMFLOAT3(10, 10, 10);
		cb_ps_lightdata.data.lightPos2 = XMFLOAT3(-10, 3, 5.0f);
		cb_ps_lightdata.data.lightPos3 = XMFLOAT3(5, 3, -5.0f);
		cb_ps_lightdata.data.lightPos4 = XMFLOAT3(-5, 3, -5.0f);
		cb_ps_lightdata.data.lightCol = XMFLOAT3(100.0f,100.0f,100.0f);
		cb_ps_lightdata.ApplyChanges();

		hr = this->cb_ps_BSDFData.Initialize(this->device.Get(), this->deviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");
		cb_ps_BSDFData.data.baseColor = XMFLOAT3(1, 1, 1);//XMFLOAT3(.82f, .67f, .16f);
		cb_ps_BSDFData.data.anisotropic = 0;
		cb_ps_BSDFData.data.clearcoat = 0;
		cb_ps_BSDFData.data.clearcoatGloss = 1;
		cb_ps_BSDFData.data.metallic = 1.0f;
		cb_ps_BSDFData.data.roughness = 1.0f;
		cb_ps_BSDFData.data.sheen = 0;
		cb_ps_BSDFData.data.sheenTint = 0.5;
		cb_ps_BSDFData.data.specular = 0.5;
		cb_ps_BSDFData.data.specularTint = 0;
		cb_ps_BSDFData.data.subsurface = 0;
		cb_ps_BSDFData.data.usepurecolor = 0;
		cb_ps_BSDFData.ApplyChanges();


		hr = this->cb_ps_iblstatus.Initialize(this->device.Get(), this->deviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");

		camera.SetPosition(-10.0f, 5.0f, 0.0f);
		camera.SetRotation(0.46, -3.14, 0.0);
		camera.SetProjectionValues(90.0f, static_cast<float>(windowWidth) / static_cast<float>(windowHeight), 0.1f, 3000.0f);

		player = new Player;
		player->model = this->model;
	}
	catch (COMException & exception)
	{
		ErrorLogger::Log(exception);
		return false;
	}
	return true;
}

bool Graphics::InitializeIBLStatus()
{
	auto start = std::chrono::high_resolution_clock::now();

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->IASetInputLayout(vertexshader_skyBox.GetInputLayout());

	XMFLOAT3 position = XMFLOAT3(0, 0, 0);
	XMFLOAT4X4 camViewMatrix; //camera.GetViewMatrix();
	XMFLOAT4X4 camProjMatrix;// camera.GetProjectionMatrix();
	XMVECTOR tar[] = { XMVectorSet(1, 0, 0, 0), XMVectorSet(-1, 0, 0, 0), XMVectorSet(0, 1, 0, 0), XMVectorSet(0, -1, 0, 0), XMVectorSet(0, 0, 1, 0), XMVectorSet(0, 0, -1, 0) };
	XMVECTOR up[] = { XMVectorSet(0, 1, 0, 0), XMVectorSet(0, 1, 0, 0), XMVectorSet(0, 0, -1, 0), XMVectorSet(0, 0, 1, 0), XMVectorSet(0, 1, 0, 0), XMVectorSet(0, 1, 0, 0) };
	//---

	UINT stride = sizeof(Vertex);//
	UINT offset = 0;
	const float color[4] = { 0.6f, 0.6f, 0.6f, 0.0f };

#pragma region Diffuse IBL
	// DIFFUSE IBL CONVOLUTION

	D3D11_TEXTURE2D_DESC skyIBLDesc;
	ZeroMemory(&skyIBLDesc, sizeof(skyIBLDesc));
	skyIBLDesc.Width = 64;
	skyIBLDesc.Height = 64;
	skyIBLDesc.MipLevels = 1;
	skyIBLDesc.ArraySize = 6;
	skyIBLDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	skyIBLDesc.Usage = D3D11_USAGE_DEFAULT;
	skyIBLDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	skyIBLDesc.CPUAccessFlags = 0;
	skyIBLDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE | D3D11_RESOURCE_MISC_GENERATE_MIPS;
	skyIBLDesc.SampleDesc.Count = 1;
	skyIBLDesc.SampleDesc.Quality = 0;
	//---
	ID3D11RenderTargetView* skyIBLRTV[6];
	//--
	D3D11_RENDER_TARGET_VIEW_DESC skyIBLRTVDesc;
	ZeroMemory(&skyIBLRTVDesc, sizeof(skyIBLRTVDesc));
	skyIBLRTVDesc.Format = skyIBLDesc.Format;
	skyIBLRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	skyIBLRTVDesc.Texture2DArray.ArraySize = 1;
	skyIBLRTVDesc.Texture2DArray.MipSlice = 0;
	//---
	D3D11_SHADER_RESOURCE_VIEW_DESC skyIBLSRVDesc;
	ZeroMemory(&skyIBLSRVDesc, sizeof(skyIBLSRVDesc));
	skyIBLSRVDesc.Format = skyIBLDesc.Format;
	skyIBLSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	skyIBLSRVDesc.TextureCube.MostDetailedMip = 0;
	skyIBLSRVDesc.TextureCube.MipLevels = 1;
	//---
	D3D11_VIEWPORT skyIBLviewport;
	skyIBLviewport.Width = 64;
	skyIBLviewport.Height = 64;
	skyIBLviewport.MinDepth = 0.0f;
	skyIBLviewport.MaxDepth = 1.0f;
	skyIBLviewport.TopLeftX = 0.0f;
	skyIBLviewport.TopLeftY = 0.0f;
	//---

	device->CreateTexture2D(&skyIBLDesc, 0, &skyIBLtex);
	device->CreateShaderResourceView(skyIBLtex, &skyIBLSRVDesc, &skyIBLSRV);
	//deviceContext->GenerateMips(skyIBLSRV);
	for (int i = 0; i < 6; i++) {
		skyIBLRTVDesc.Texture2DArray.FirstArraySlice = i;
		device->CreateRenderTargetView(skyIBLtex, &skyIBLRTVDesc, &skyIBLRTV[i]);
		//-- Cam directions
		XMVECTOR dir = XMVector3Rotate(tar[i], XMQuaternionIdentity());
		XMMATRIX view = DirectX::XMMatrixLookToLH(XMLoadFloat3(&position), dir, up[i]);
		view.r[3] = g_XMIdentityR3;
		XMStoreFloat4x4(&camViewMatrix, DirectX::XMMatrixTranspose(view));//

		XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(90.0f, static_cast<float>(windowWidth) / static_cast<float>(windowHeight), 0.1f, 3000.0f);//     
		//XMStoreFloat4x4(&camProjMatrix, DirectX::XMMatrixTranspose(P));//

		deviceContext->OMSetRenderTargets(1, &skyIBLRTV[i], 0);
		deviceContext->RSSetViewports(1, &skyIBLviewport);
		deviceContext->ClearRenderTargetView(skyIBLRTV[i], color);

		auto vertexBuffer = quad->vertexbuffer.GetAddressOf();
		auto indexBuffer = quad->indexbuffer;

		deviceContext->VSSetShader(vertexshader_skyBox.GetShader(), 0, 0);
		deviceContext->PSSetShader(ConvolutionPixelShader.GetShader(), 0, 0);

		this->cb_vs_vertexshader.data.mat = DirectX::XMMatrixIdentity() * XMLoadFloat4x4(&camViewMatrix);// * XMLoadFloat4x4(&camProjMatrix); dont ask me i dont know why!!!!!!! it just worked!!!!!!!!!!
		//this->cb_vs_vertexshader.data.mat = DirectX::XMMatrixTranspose(this->cb_vs_vertexshader.data.mat);
		this->cb_vs_vertexshader.data.mat_wvp = DirectX::XMMatrixIdentity();
		this->cb_vs_vertexshader.ApplyChanges();
		deviceContext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshader.GetAddressOf());

		this->deviceContext->PSSetShaderResources(0, 1, skybox->textureView.GetAddressOf());//.textureView.GetAddressOf()
		deviceContext->PSSetSamplers(0, 1, samplerState.GetAddressOf());
		deviceContext->IASetInputLayout(vertexshader_skyBox.GetInputLayout());

		deviceContext->RSSetState(IBLskyrasterizerState.Get());
		deviceContext->OMSetDepthStencilState(IBLskydepthStencilState.Get(), 0);

		deviceContext->IASetVertexBuffers(0, 1, vertexBuffer, &stride, &offset);
		deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		deviceContext->DrawIndexed(quad->GetFBXModel()->GetMeshData()->nIndexCount, 0, 0);//.indices.size()
		// Reset the render states we've changed
		/*deviceContext->RSSetState(0);
		deviceContext->OMSetDepthStencilState(0, 0);*/

	}

	for (int i = 0; i < 6; i++) {
		skyIBLRTV[i]->Release();
	}

#pragma endregion
#pragma region Prefilter EnvMap
	// PREFILTER ENVIRONMENT MAP
	unsigned int maxMipLevels = 5;
	D3D11_TEXTURE2D_DESC envMapDesc;
	//ZeroMemory(&skyIBLDesc, sizeof(skyIBLDesc));
	envMapDesc.Width = 256;
	envMapDesc.Height = 256;
	envMapDesc.MipLevels = maxMipLevels;
	envMapDesc.ArraySize = 6;
	envMapDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	envMapDesc.Usage = D3D11_USAGE_DEFAULT;
	envMapDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	envMapDesc.CPUAccessFlags = 0;
	envMapDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE | D3D11_RESOURCE_MISC_GENERATE_MIPS;
	envMapDesc.SampleDesc.Count = 1;
	envMapDesc.SampleDesc.Quality = 0;
	//---
	D3D11_SHADER_RESOURCE_VIEW_DESC envMapSRVDesc;
	ZeroMemory(&envMapSRVDesc, sizeof(envMapSRVDesc));
	envMapSRVDesc.Format = skyIBLDesc.Format;
	envMapSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	envMapSRVDesc.TextureCube.MostDetailedMip = 0;
	envMapSRVDesc.TextureCube.MipLevels = maxMipLevels;
	//--
	ID3D11RenderTargetView* envMapRTV[6];
	//---
	device->CreateTexture2D(&envMapDesc, 0, &envMaptex);
	device->CreateShaderResourceView(envMaptex, &envMapSRVDesc, &envMapSRV);
	for (int mip = 0; mip < maxMipLevels; mip++) {

		D3D11_RENDER_TARGET_VIEW_DESC envMapRTVDesc;
		ZeroMemory(&envMapRTVDesc, sizeof(envMapRTVDesc));
		envMapRTVDesc.Format = skyIBLDesc.Format;
		envMapRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		envMapRTVDesc.Texture2DArray.ArraySize = 1;
		envMapRTVDesc.Texture2DArray.MipSlice = mip;

		unsigned mipWidth = 256 * pow(0.5, mip);
		unsigned mipHeight = 256 * pow(0.5, mip);

		D3D11_VIEWPORT envMapviewport;
		envMapviewport.Width = mipWidth;
		envMapviewport.Height = mipHeight;
		envMapviewport.MinDepth = 0.0f;
		envMapviewport.MaxDepth = 1.0f;
		envMapviewport.TopLeftX = 0.0f;
		envMapviewport.TopLeftY = 0.0f;


		float roughness = (float)mip / (float)(maxMipLevels - 1);
		//float roughness = 0.0;
		for (int i = 0; i < 6; i++) {
			envMapRTVDesc.Texture2DArray.FirstArraySlice = i;
			device->CreateRenderTargetView(envMaptex, &envMapRTVDesc, &envMapRTV[i]);

			//---
			deviceContext->OMSetRenderTargets(1, &envMapRTV[i], 0);
			deviceContext->RSSetViewports(1, &envMapviewport);
			deviceContext->ClearRenderTargetView(envMapRTV[i], color);
			//---

			auto vertexBuffer = quad->vertexbuffer.GetAddressOf();
			auto indexBuffer = quad->indexbuffer;

			deviceContext->VSSetShader(vertexshader_skyBox.GetShader(), 0, 0);
			deviceContext->PSSetShader(PrefilterMapPixelShader.GetShader(), 0, 0);

			XMVECTOR dir = XMVector3Rotate(tar[i], XMQuaternionIdentity());
			XMMATRIX view = DirectX::XMMatrixLookToLH(XMLoadFloat3(&position), dir, up[i]);
			view.r[3] = g_XMIdentityR3;
			XMStoreFloat4x4(&camViewMatrix, view);//DirectX::XMMatrixTranspose(

			XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(90.0f, static_cast<float>(windowWidth) / static_cast<float>(windowHeight), 0.1f, 3000.0f);//  0.5f * XM_PI, 1.0f, 0.1f, 100.0f   
			XMStoreFloat4x4(&camProjMatrix,P);//DirectX::XMMatrixTranspose(

			this->cb_vs_vertexshader.data.mat = DirectX::XMMatrixIdentity() * XMLoadFloat4x4(&camViewMatrix);// * XMLoadFloat4x4(&camProjMatrix); dont ask me i dont know why!!!!!!! it just worked!!!!!!!!!!
			//this->cb_vs_vertexshader.data.mat = DirectX::XMMatrixTranspose(this->cb_vs_vertexshader.data.mat);
			this->cb_vs_vertexshader.data.mat_wvp = DirectX::XMMatrixIdentity();
			this->cb_vs_vertexshader.ApplyChanges();
			deviceContext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshader.GetAddressOf());

			cb_ps_iblstatus.data.roughness = roughness;
			cb_ps_iblstatus.ApplyChanges();
			deviceContext->PSSetConstantBuffers(0, 1, this->cb_ps_iblstatus.GetAddressOf());
			deviceContext->PSSetShaderResources(0, 1, skybox->textureView.GetAddressOf());
			deviceContext->PSSetSamplers(0, 1, samplerState.GetAddressOf());

			deviceContext->RSSetState(IBLskyrasterizerState.Get());
			deviceContext->OMSetDepthStencilState(IBLskydepthStencilState.Get(), 0);

			deviceContext->IASetVertexBuffers(0, 1, vertexBuffer, &stride, &offset);
			deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

			deviceContext->DrawIndexed(quad->GetFBXModel()->GetMeshData()->nIndexCount, 0, 0);

			// Reset the render states we've changed
			/*deviceContext->RSSetState(0);
			deviceContext->OMSetDepthStencilState(0, 0);*/

		}
		for (int i = 0; i < 6; i++) {
			envMapRTV[i]->Release();
		}

	}
#pragma endregion

#pragma region Integrate BRDF LUT
	// INTEGRATE BRDF & CREATE LUT

	D3D11_TEXTURE2D_DESC brdfLUTDesc;
	//ZeroMemory(&skyIBLDesc, sizeof(skyIBLDesc));
	brdfLUTDesc.Width = 512;
	brdfLUTDesc.Height = 512;
	brdfLUTDesc.MipLevels = 0;
	brdfLUTDesc.ArraySize = 1;
	brdfLUTDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
	brdfLUTDesc.Usage = D3D11_USAGE_DEFAULT;
	brdfLUTDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	brdfLUTDesc.CPUAccessFlags = 0;
	brdfLUTDesc.MiscFlags = 0;
	brdfLUTDesc.SampleDesc.Count = 1;
	brdfLUTDesc.SampleDesc.Quality = 0;
	//---
	ID3D11RenderTargetView* brdfLUTRTV;
	//--
	D3D11_RENDER_TARGET_VIEW_DESC brdfLUTRTVDesc;
	ZeroMemory(&brdfLUTRTVDesc, sizeof(brdfLUTRTVDesc));
	brdfLUTRTVDesc.Format = brdfLUTDesc.Format;
	brdfLUTRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	//---
	D3D11_SHADER_RESOURCE_VIEW_DESC brdfLUTSRVDesc;
	ZeroMemory(&brdfLUTSRVDesc, sizeof(brdfLUTSRVDesc));
	brdfLUTSRVDesc.Format = brdfLUTSRVDesc.Format;
	brdfLUTSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	brdfLUTSRVDesc.TextureCube.MostDetailedMip = 0;
	brdfLUTSRVDesc.TextureCube.MipLevels = 1;
	//---
	D3D11_VIEWPORT brdfLUTviewport;
	brdfLUTviewport.Width = 512;
	brdfLUTviewport.Height = 512;
	brdfLUTviewport.MinDepth = 0.0f;
	brdfLUTviewport.MaxDepth = 1.0f;
	brdfLUTviewport.TopLeftX = 0.0f;
	brdfLUTviewport.TopLeftY = 0.0f;
	//---
	device->CreateTexture2D(&brdfLUTDesc, 0, &brdfLUTtex);
	device->CreateRenderTargetView(brdfLUTtex, &brdfLUTRTVDesc, &brdfLUTRTV);
	device->CreateShaderResourceView(brdfLUTtex, &brdfLUTSRVDesc, &brdfLUTSRV);

	deviceContext->OMSetRenderTargets(1, &brdfLUTRTV, 0);
	deviceContext->RSSetViewports(1, &brdfLUTviewport);
	deviceContext->ClearRenderTargetView(brdfLUTRTV, color);

	auto vertexBuffer = quad->vertexbuffer.GetAddressOf();//.GetMesh().at(0).GetVertexBuffer();
	auto indexBuffer = quad->indexbuffer; 

	deviceContext->VSSetShader(vertexshader.GetShader(), 0, 0);
	deviceContext->PSSetShader(IntegrateBRDFPixelShader.GetShader(), 0, 0);

	this->cb_vs_vertexshader.data.mat = XMMatrixTranspose(quad->GetMVPMatrix(camera.GetViewMatrix() * camera.GetProjectionMatrix()));
	//this->cb_vs_vertexshader.data.worldMatrix = DirectX::XMMatrixIdentity();
	this->cb_vs_vertexshader.ApplyChanges();
	deviceContext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshader.GetAddressOf());

	stride = sizeof(Vertex);//Vertex3D
	deviceContext->IASetVertexBuffers(0, 1, vertexBuffer, &stride, &offset);
	deviceContext->IASetInputLayout(vertexshader.GetInputLayout());
	deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	deviceContext->DrawIndexed(quad->GetFBXModel()->GetMeshData()->nIndexCount, 0, 0);

	/*QuadVertexShader->SetShader();
	IntegrateBRDFPixelShader->SetShader();


	ID3D11Buffer* nothing = 0;
	context->IASetVertexBuffers(0, 1, &nothing, &stride, &offset);
	context->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);

	context->Draw(3, 0);*/

	brdfLUTRTV->Release();

	//---JANKY CHEESE---
	CreateWICTextureFromFile(device.Get(), deviceContext.Get(), L"Data/Textures/brdfLUT.png", 0, &brdfLUTSRV);//ibl_brdf_lut.png brdfLUT.png
	//---END CHEESE----
#pragma endregion


	//HRESULT hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\brdfLUT.png", nullptr, brdfLUT.GetAddressOf());
	//COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

	auto stop = std::chrono::high_resolution_clock::now();

	auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	//printf("\n %f", (float)diff.count());
	//std::cout << "\n" << diff.count();

	return true;
}

void Graphics::ChangeHdrImage() {
	hdrImageIdx++;
	if (hdrImageIdx > 3)
		hdrImageIdx = 0;

	if (hdrImageIdx == 0) 
	{
		skybox->textureView = skybox->textureView1;
	}
	else if (hdrImageIdx == 1)
	{ 
		skybox->textureView = skybox->textureView2;
	}
	else if (hdrImageIdx == 2)
	{
		skybox->textureView = skybox->textureView3;
	}
	else if (hdrImageIdx == 3)
	{
		skybox->textureView = skybox->textureView4;
	}

	InitializeIBLStatus();
}
