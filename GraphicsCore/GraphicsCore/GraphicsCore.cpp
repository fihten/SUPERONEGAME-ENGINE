#include "GraphicsCore.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "Cameras.h"
#include "auto_ptr.h"
#include "ParseBlendState.h"
#include <algorithm>
#include <sstream>
#include <D3DX11tex.h>
#include <fstream>

GraphicsCore* GraphicsCore::pGraphicsCore = nullptr;

GraphicsCore* GraphicsCore::instance()
{
	if (pGraphicsCore == nullptr)
		pGraphicsCore = new GraphicsCore();
	return pGraphicsCore;
}

void GraphicsCore::init(HINSTANCE instanceHandle, int show, WNDPROC WndProc, DRAW_FUNC drawFunc, UINT width, UINT height, bool windowed, bool enable4xMsaa)
{
	this->drawFunc = drawFunc;

	mWidth = width;
	mHeight = height;

	// Set cameras aspects
	for (int i = 0; i < CAMERAS_NUMBER; ++i)
		cameras()[i].setAspectRatio((float)mWidth / (float)mHeight);

	mEnable4xMsaa = enable4xMsaa;

	mWindowed = windowed;

	initWindow(instanceHandle, show, WndProc);

	// 1. Create the ID3D11Device and ID3D11DeviceContext interfaces using the D3D11CreateDevice function.

	UINT flag = 0;
#if defined(_DEBUG)
	flag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL pFeatureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
	D3D_FEATURE_LEVEL featureLevel;

	D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		flag,
		pFeatureLevels,
		1,
		D3D11_SDK_VERSION,
		&device,
		&featureLevel,
		&context
	);

	// 2. Check 4X MSAA quality level support using the ID3D11Device::CheckMultisampleQualityLevels method.

	device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality);

	// 3. Describe the characteristics of the swap chain we are going to create by filling out an instance of 
	// the DXGI_SWAP_CHAIN_DESC structure

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	if (enable4xMsaa)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;
	sd.OutputWindow = mWindow;
	sd.Windowed = windowed;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	// 4. Query the IDXGIFactory instance that was used to create the device, and create an IDXGISwapChain instance
	
	IDXGIDevice* dxgiDevice = 0;
	device->QueryInterface(__uuidof(IDXGIDevice), (void**)& dxgiDevice);

	IDXGIAdapter* dxgiAdapter = 0;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)& dxgiAdapter);

	// Finally got the IDXGIFactory interface
	IDXGIFactory* dxgiFactory = 0;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	// Now, create the SwapChain
	dxgiFactory->CreateSwapChain(device, &sd, &mSwapChain);

	// Release our required COM interfaces (because we are done with them)
	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();

	// 5. Create a render target view to the swap chain's back buffer
	ID3D11Texture2D* backBuffer;
	mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	device->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView);

	backBuffer->Release();

	// 6. Create the depth/stencil buffer and its associated depth/stencil view

	D3D11_TEXTURE2D_DESC dsDesc;
	dsDesc.Width = width;
	dsDesc.Height = height;
	dsDesc.MipLevels = 1;
	dsDesc.ArraySize = 1;
	dsDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	if (enable4xMsaa)
	{
		dsDesc.SampleDesc.Count = 4;
		dsDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	else
	{
		dsDesc.SampleDesc.Count = 1;
		dsDesc.SampleDesc.Quality = 0;
	}
	dsDesc.Usage = D3D11_USAGE_DEFAULT;
	dsDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsDesc.CPUAccessFlags = 0;
	dsDesc.MiscFlags = 0;

	device->CreateTexture2D(&dsDesc, 0, &mDepthStencilBuffer);
	device->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView);

	// 7. Bind the render target view and depth/stencil view to the output merger stage of the
	// rendering pipeline so that they can be used by Direct3D

	context->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	// 8. Set the viewport

	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = static_cast<float>(mWidth);
	vp.Height = static_cast<float>(mHeight);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	context->RSSetViewports(1, &vp);

	// 9. Process shaders

	processShaders(device);

	// 10. load cameras cfgs
	loadCamerasCFGs();

	// 11. Init objects' selection 
	initRoughObjectsSelection();
	initFineObjectsSelection();
	initRoughObjectsSelectionBySegments();
	initFineObjectsSelectionBySegments();

	// 12. Init photogrammetry
	initCalculationOfTextureIntegrals();
	initDefinitionOfTheSamePoints();
}

void GraphicsCore::startFrame()
{
	flt4 white(1, 1, 1, 1);
	context->ClearRenderTargetView(mRenderTargetView, white.getBuf());

	context->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

void GraphicsCore::endFrame()
{
	mSwapChain->Present(0, 0);
}

void GraphicsCore::draw(Mesh& mesh)
{
	param_index = 0;
	setVariablesOnGPU(mesh);
	setGeometryOnGPU(mesh);
	setPipelineStates(mesh);

	string_id technique_name_id = mesh.getTechnique();
	string_id pass_name_id = mesh.getPass();

	auto* pass = ResourceManager::instance()->getPass(technique_name_id, pass_name_id);
	if (pass == nullptr)
		return;

	pass->Apply(0, context);
	if (ResourceManager::instance()->isThereAGeometryShaderInThePass(technique_name_id, pass_name_id))
		context->Draw(mesh.getVerticesCount(), 0);
	else
		context->DrawIndexed(mesh.getIndicesCount(), 0, 0);
}

bool GraphicsCore::initWindow(HINSTANCE instanceHandle, int show, WNDPROC WndProc)
{
	// The first task to creating a window is to describe some of its
	// characteristics by filling out a WNDCLASS structure.
	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = instanceHandle;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"CANVAS";

	// Next we register this WNDCLASS instance with Windows so
	// that we can create a window based on it.
	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass FAILED", 0, 0);
		return false;
	}

	// With our WNDCLASS instance registered, we can create a
	// window with the CreateWindow function. This function
	// returns a handle to the window it creates (an HWND).
	// If the creation failed, the handle will have the value
	// of zero. A window handle is a way to refer to the window,
	// which is internally managed by Windows. Many of the Win32 API
	// functions that operate on windows require an HWND so that
	// they know what window to act on.

	mWindow = CreateWindow(
		L"CANVAS", // Registered WNDCLASS instance to use
		L"CORE", // window title
		WS_OVERLAPPEDWINDOW, // style flags
		CW_USEDEFAULT, // x-coordinate
		CW_USEDEFAULT, // y-coordinate
		mWidth, // width
		mHeight, // height
		0, // parent window
		0, // menu handle
		instanceHandle, // app instance
		0 // extra creation parameters
	);
	if (mWindow == 0)
	{
		MessageBox(0, L"CreateWindow FAILED", 0, 0);
		return false;
	}

	// Even though we just created a window, it is not initially
	// shown. Therefore, the final step is to show and update the
	// window we just created, which can be done with the following
	// two function calls. Observe that we pass the handle to the
	// window we want to show and update so that these functions know
	// which window to show and update.
	ShowWindow(mWindow, show);
	UpdateWindow(mWindow);

	return true;
}

int GraphicsCore::run()
{
	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		// If there are Window messages then process them.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// Otherwise, do animation/game stuff.
		else
		{
			drawFunc(this);
		}
	}
	return (int)msg.wParam;
}

void GraphicsCore::resize(UINT width, UINT height)
{
	if (!mDepthStencilBuffer)
		return;
	if (!mDepthStencilView)
		return;
	if (!mRenderTargetView)
		return;
	if (!mSwapChain)
		return;
	if (!device)
		return;
	if (!context)
		return;

	// Destroy depth/stencil buffer and depth/stencil view
	mDepthStencilBuffer->Release();
	mDepthStencilView->Release();

	// Destroy RenderTarget view
	mRenderTargetView->Release();

	// Resize RenderTarget
	mSwapChain->ResizeBuffers(2, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	D3D11_TEXTURE2D_DESC dsDesc;
	dsDesc.Width = width;
	dsDesc.Height = height;
	dsDesc.MipLevels = 1;
	dsDesc.ArraySize = 1;
	dsDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	if (mEnable4xMsaa)
	{
		dsDesc.SampleDesc.Count = 4;
		dsDesc.SampleDesc.Quality = m4xMsaaQuality;
	}
	else
	{
		dsDesc.SampleDesc.Count = 1;
		dsDesc.SampleDesc.Quality = 0;
	}
	dsDesc.Usage = D3D11_USAGE_DEFAULT;
	dsDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsDesc.CPUAccessFlags = 0;
	dsDesc.MiscFlags = 0;

	device->CreateTexture2D(&dsDesc, 0, &mDepthStencilBuffer);
	device->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView);

	ID3D11Texture2D* backBuffer;
	mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)(&backBuffer));
	device->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView);
	backBuffer->Release();

	context->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = static_cast<float>(width);
	vp.Height = static_cast<float>(height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	context->RSSetViewports(1, &vp);

	mWidth = width;
	mHeight = height;

	// Set cameras aspects
	for (int i = 0; i < CAMERAS_NUMBER; ++i)
		cameras()[i].setAspectRatio((float)mWidth / (float)mHeight);
}

const flt4x4* GraphicsCore::getFloat4x4(Mesh& mesh, string_id var, const VariableLocation& location) const
{
	if (location.name == cameras_id)
	{
		int index = location.index;
		if (location.field == wvp_id)
			return &cameras()[index].getViewProj();
		if (location.field == vp_id)
			return &cameras()[index].getViewProj();
		if (location.field == v_id)
			return &cameras()[index].getView();
		if (location.field == p_id)
			return &cameras()[index].getProj();
	}
	if (location.name == position_in_scene_id)
		return &mesh.getPos4x4();
	if (location.name == inverse_transpose_of_position_in_scene_id)
		return &mesh.getPosInvTr();
	
	return mesh.getFlt4x4(param_index);
}

const flt4* GraphicsCore::getFloat4(Mesh& mesh, string_id var, const VariableLocation& location) const
{
	if (location.name == cameras_id)
	{
		int index = location.index;
		if (location.field == eye_pos_id)
			return &cameras()[index].getEyePos4();
		if (location.field == fwd_id)
			return &cameras()[index].getFwd4();
	}
	if (location.name == position_in_scene_id)
		return &mesh.getPos4();
	return mesh.getFlt4(param_index);
}

const flt3* GraphicsCore::getFloat3(Mesh& mesh, string_id var, const VariableLocation& location) const
{
	if (location.name == cameras_id)
	{
		int index = location.index;
		if (location.field == eye_pos_id)
			return &cameras()[index].getEyePos();
		if (location.field == fwd_id)
			return &cameras()[index].getFwd();
	}
	if (location.name == position_in_scene_id)
		return &mesh.getPos3();
	return mesh.getFlt3(param_index);
}

const flt2* GraphicsCore::getFloat2(Mesh& mesh, string_id var, const VariableLocation& location) const
{
	return mesh.getFlt2(param_index);
}

const float* GraphicsCore::getFloat1(Mesh& mesh, string_id var, const VariableLocation& location) const
{
	return mesh.getFloat(param_index);
}

void* GraphicsCore::getStruct(Mesh& mesh, string_id var, const StructResource& sr, int* bytes)
{
	string_id technique_name_id = mesh.getTechnique();

	char* structData = (char*)std::malloc(sr.bytes * sr.elementsCount);
	for (int e = 0; e < sr.elementsCount; ++e)
	{
		char* elementData = structData + e * sr.bytes;
		for (int f = 0; f < sr.fieldsCount; ++f)
		{
			auto& fr = sr.fields[f];

			ParamKey pk{ var,e,fr.name };

			char* fieldPtr = elementData + fr.offset;
			if (fr.type == float_id)
			{
				float& floatVariable = *((float*)(fieldPtr));
				mesh.getParam(param_index, floatVariable);
			}
			if (fr.type == float2_id)
			{
				flt2& float2Variable = *((flt2*)(fieldPtr));
				mesh.getParam(param_index, float2Variable);
			}
			if (fr.type == float3_id)
			{
				flt3& float3Variable = *((flt3*)(fieldPtr));
				mesh.getParam(param_index, float3Variable);
			}
			if (fr.type == float4_id)
			{
				flt4& float4Variable = *((flt4*)(fieldPtr));
				mesh.getParam(param_index, float4Variable);
			}
			param_index++;
		}
	}
	if (bytes)
		*bytes = sr.bytes * sr.elementsCount;

	return structData;
}

ID3D11Buffer* GraphicsCore::getVertexBuffer(
	const Mesh& mesh,
	uint32_t* elementSize,
	string_id* technique,
	string_id* pass,
	bool structured
) const
{
	(*elementSize) = 0;

	string_id technique_name_id = technique ? *technique : mesh.getTechnique();
	string_id pass_name_id = pass ? *pass : mesh.getPass();

	const std::vector<InputLayoutResource::StreamInfo>* streamsInfo = ResourceManager::instance()->getStreamsInfo(technique_name_id, pass_name_id);
	if (streamsInfo == nullptr)
		return nullptr;

	for (const auto& si : *streamsInfo)
		(*elementSize) += si.size;
	
	if (mesh.gpuReadyData)
		(*elementSize) = mesh.elementSize;

	ID3D11Buffer* mVB = ResourceManager::instance()->getVertexBuffer(technique_name_id, pass_name_id, mesh.id, structured);
	if (mesh.gpuReadyData)
		mVB = nullptr;

	if (mVB == nullptr)
	{
		uint32_t verticesCount = mesh.getVerticesCount();
		if (mesh.gpuReadyData)
			verticesCount = mesh.verticesCount;

		uint32_t bytes = verticesCount * (*elementSize);

		// Create vertex buffer
		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = bytes;
		vbd.BindFlags = structured ? D3D11_BIND_SHADER_RESOURCE : D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = structured ? D3D11_RESOURCE_MISC_BUFFER_STRUCTURED : 0;
		vbd.StructureByteStride = *elementSize;

		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = mesh.gpuReadyData;
		if (vinitData.pSysMem == nullptr)
			vinitData.pSysMem = fetchVerticesFromMesh(mesh, streamsInfo);

		device->CreateBuffer(&vbd, &vinitData, &mVB);

		ResourceManager::instance()->registerVertexBuffer(technique_name_id, pass_name_id, mesh.id, mVB, structured);
		if (mesh.gpuReadyData == nullptr)
			free((void*)vinitData.pSysMem);
	}
	return mVB;
}

void* GraphicsCore::fetchVerticesFromMesh(
	const Mesh& mesh,
	const std::vector<InputLayoutResource::StreamInfo>* streamsInfo
) const
{
	uint32_t elementSize = 0;
	for (const auto& si : *streamsInfo)
		elementSize += si.size;

	uint32_t verticesCount = mesh.getVerticesCount();
	uint32_t bytes = verticesCount * elementSize;

	char* data = (char*)std::malloc(bytes);
	char* dataElement = data;
	for (int i = 0; i < verticesCount; ++i)
	{
		for (const auto& si : *streamsInfo)
		{
			const void* stream = mesh.getStream(si.name, si.type);
			switch (si.type)
			{
			case FLT1:
			{
				const std::vector<flt1>& s = *(const std::vector<flt1>*)stream;
				std::copy((char*)(&s[i]), (char*)(&s[i]) + si.size, dataElement);
				break;
			}
			case FLT2:
			{
				const std::vector<flt2>& s = *(const std::vector<flt2>*)stream;
				std::copy((char*)(&s[i]), (char*)(&s[i]) + si.size, dataElement);
				break;
			}
			case FLT3:
			{
				const std::vector<flt3>& s = *(const std::vector<flt3>*)stream;
				std::copy((char*)(&s[i]), (char*)(&s[i]) + si.size, dataElement);
				break;
			}
			case FLT4:
			{
				const std::vector<flt4>& s = *(const std::vector<flt4>*)stream;
				std::copy((char*)(&s[i]), (char*)(&s[i]) + si.size, dataElement);
				break;
			}
			}
			dataElement += si.size;
		}
	}
	return (void*)data;
}

ID3D11ShaderResourceView* GraphicsCore::getVertexBufferSRV(
	const Mesh& mesh,
	string_id* technique,
	string_id* pass
) const
{
	string_id technique_name_id = technique ? *technique : mesh.getTechnique();
	string_id pass_name_id = pass ? *pass : mesh.getPass();

	ID3D11ShaderResourceView* ret =
		ResourceManager::instance()->getVertexBufferSRV(
			technique_name_id,
			pass_name_id,
			mesh.id
		);
	if (ret)
		return ret;

	uint32_t elementSize = 0;
	ID3D11Buffer* vertexBuffer = 
		getVertexBuffer(
			mesh, 
			&elementSize, 
			technique, 
			pass,
			true
		);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement = 0;
	srvDesc.BufferEx.Flags = 0;
	srvDesc.BufferEx.NumElements = mesh.getVerticesCount();

	device->CreateShaderResourceView(vertexBuffer, &srvDesc, &ret);

	ResourceManager::instance()->registerVertexBufferSRV(technique_name_id, pass_name_id, mesh.id, ret);

	return ret;
}

ID3D11Buffer* GraphicsCore::getIndexBuffer(
	const Mesh& mesh,
	string_id* technique,
	string_id* pass,
	bool structured
) const
{
	string_id technique_name_id = technique ? *technique : mesh.getTechnique();
	string_id pass_name_id = pass ? *pass : mesh.getPass();

	if (ResourceManager::instance()->isThereAGeometryShaderInThePass(technique_name_id, pass_name_id))
		return nullptr;

	ID3D11Buffer* mIB = ResourceManager::instance()->getIndexBuffer(technique_name_id, pass_name_id, mesh.id, structured);
	if (mIB == nullptr)
	{
		// Create index buffer
		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = mesh.getIndicesCount() * sizeof(uint32_t);
		ibd.BindFlags = structured ? D3D11_BIND_SHADER_RESOURCE : D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = structured ? D3D11_RESOURCE_MISC_BUFFER_STRUCTURED : 0;
		ibd.StructureByteStride = sizeof(uint32_t);

		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = mesh.getIndicies()->data();

		device->CreateBuffer(&ibd, &iinitData, &mIB);

		ResourceManager::instance()->registerIndexBuffer(technique_name_id, pass_name_id, mesh.id, mIB, structured);
	}
	return mIB;
}

ID3D11ShaderResourceView* GraphicsCore::getIndexBufferSRV(
	const Mesh& mesh,
	string_id* technique,
	string_id* pass
) const
{
	string_id technique_name_id = technique ? *technique : mesh.getTechnique();
	string_id pass_name_id = pass ? *pass : mesh.getPass();

	ID3D11ShaderResourceView* ret =
		ResourceManager::instance()->getIndexBufferSRV(
			technique_name_id,
			pass_name_id,
			mesh.id
		);
	if (ret)
		return ret;

	ID3D11Buffer* indexBuffer = getIndexBuffer(mesh, technique, pass, true);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement = 0;
	srvDesc.BufferEx.Flags = 0;
	srvDesc.BufferEx.NumElements = mesh.getIndicesCount();

	device->CreateShaderResourceView(indexBuffer, &srvDesc, &ret);

	ResourceManager::instance()->registerIndexBufferSRV(technique_name_id, pass_name_id, mesh.id, ret);

	return ret;
}

ID3D11ShaderResourceView* GraphicsCore::getImage(Mesh& mesh, string_id var) const
{
	string_id name_id;
	mesh.getParam(param_index, name_id);

	ID3D11ShaderResourceView* img = ResourceManager::instance()->getImage(name_id);
	if (img)
		return img;

	char texturesFolder[200];
	int sz = sizeof texturesFolder / sizeof * texturesFolder;
	GetEnvironmentVariableA("TEXTURES", texturesFolder, sz);

	const char* name = StringManager::toString(name_id);
	std::string texturePath = std::string(texturesFolder) + '\\' + name;

	D3DX11CreateShaderResourceViewFromFileA(
		device, texturePath.c_str(), 0, 0, &img, 0
	);

	ResourceManager::instance()->registerImage(name_id, img);

	return img;
}

ID3D11ShaderResourceView* GraphicsCore::getImagesArray(Mesh& mesh, string_id var) const
{
	string_id name_id;

	mesh.getParam(param_index, name_id);

	ID3D11ShaderResourceView* imgsArr = ResourceManager::instance()->getImagesArray(name_id);
	if (imgsArr)
		return imgsArr;

	std::string name(StringManager::toString(name_id));

	std::vector<std::string> texNames;
	int start = 0;
	int end = name.find_first_of(";\0", start);
	while (end != std::string::npos)
	{
		std::string texName = name.substr(start, end - start);
		texNames.push_back(texName);

		start = end + 1;
		end = name.find_first_of(";\0", start);
	}

	char texturesFolder[200];
	int sz = sizeof texturesFolder / sizeof * texturesFolder;
	GetEnvironmentVariableA("TEXTURES", texturesFolder, sz);

	int size = texNames.size();
	std::vector<ID3D11Texture2D*> texes(size);
	for (int i = 0; i < size; ++i)
	{
		std::string& name = texNames[i];
		std::string texturePath = std::string(texturesFolder) + '\\' + name;

		D3DX11_IMAGE_LOAD_INFO loadInfo;
		loadInfo.Width = D3DX11_FROM_FILE;
		loadInfo.Height = D3DX11_FROM_FILE;
		loadInfo.Depth = D3DX11_FROM_FILE;
		loadInfo.FirstMipLevel = 0;
		loadInfo.MipLevels = D3DX11_FROM_FILE;
		loadInfo.Usage = D3D11_USAGE_STAGING;
		loadInfo.BindFlags = 0;
		loadInfo.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		loadInfo.MiscFlags = 0;
		loadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		loadInfo.Filter = D3DX11_FILTER_LINEAR;
		loadInfo.MipFilter = D3DX11_FILTER_LINEAR;
		loadInfo.pSrcInfo = 0;
		D3DX11CreateTextureFromFileA(
			device,
			texturePath.c_str(),
			&loadInfo,
			0,
			(ID3D11Resource * *)& texes[i],
			0
		);
	}

	D3D11_TEXTURE2D_DESC texElementDesc;
	texes[0]->GetDesc(&texElementDesc);
	D3D11_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width = texElementDesc.Width;
	texArrayDesc.Height = texElementDesc.Height;
	texArrayDesc.MipLevels = texElementDesc.MipLevels;
	texArrayDesc.ArraySize = size;
	texArrayDesc.Format = texElementDesc.Format;
	texArrayDesc.SampleDesc.Count = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = 0;
	ID3D11Texture2D* texArray = 0;
	device->CreateTexture2D(&texArrayDesc, 0, &texArray);

	for (int texElement = 0; texElement < size; ++texElement)
	{
		for (int mipLevel = 0; mipLevel < texElementDesc.MipLevels; ++mipLevel)
		{
			D3D11_MAPPED_SUBRESOURCE mappedTex2D;
			context->Map(texes[texElement], mipLevel, D3D11_MAP_READ, 0, &mappedTex2D);
			context->UpdateSubresource(
				texArray,
				D3D11CalcSubresource(
					mipLevel,
					texElement,
					texElementDesc.MipLevels
				),
				0,
				mappedTex2D.pData,
				mappedTex2D.RowPitch,
				mappedTex2D.DepthPitch
			);
			context->Unmap(texes[texElement], mipLevel);
		}
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texArrayDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	viewDesc.Texture2DArray.MostDetailedMip = 0;
	viewDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
	viewDesc.Texture2DArray.FirstArraySlice = 0;
	viewDesc.Texture2DArray.ArraySize = size;
	
	imgsArr = 0;
	device->CreateShaderResourceView(texArray, &viewDesc, &imgsArr);

	texArray->Release();
	for (int i = 0; i < size; ++i)
		texes[i]->Release();

	ResourceManager::instance()->registerImagesArray(name_id, imgsArr);

	return imgsArr;
}

void GraphicsCore::setFloat4x4sOnGPU(Mesh& mesh)
{
	string_id technique_name_id = mesh.getTechnique();

	std::map<string_id, Float4x4Resource>& flt4x4s =
		ResourceManager::instance()->getFloat4x4s(technique_name_id);
	for (auto& f4x4 : flt4x4s)
	{
		const flt4x4* v = getFloat4x4(mesh, f4x4.first, f4x4.second.location);
		param_index++;
		if (v == nullptr)
			continue;
		f4x4.second.ptr->SetMatrix(reinterpret_cast<const float*>(v));
	}
}

void GraphicsCore::setFloat4sOnGPU(Mesh& mesh)
{
	string_id technique_name_id = mesh.getTechnique();

	std::map<string_id, Float4Resource>& flt4s =
		ResourceManager::instance()->getFloat4s(technique_name_id);
	for (auto& f4 : flt4s)
	{
		const flt4* v = getFloat4(mesh, f4.first, f4.second.location);
		param_index++;
		if (v == nullptr)
			continue;
		f4.second.ptr->SetRawValue(reinterpret_cast<const void*>(v), 0, 4 * sizeof(float));
	}
}

void GraphicsCore::setFloat3sOnGPU(Mesh& mesh)
{
	string_id technique_name_id = mesh.getTechnique();

	std::map<string_id, Float3Resource>& flt3s =
		ResourceManager::instance()->getFloat3s(technique_name_id);
	for (auto& f3 : flt3s)
	{
		const flt3* v = getFloat3(mesh, f3.first, f3.second.location);
		param_index++;
		if (v == nullptr)
			continue;
		f3.second.ptr->SetRawValue(reinterpret_cast<const void*>(v), 0, 3 * sizeof(float));
	}
}

void GraphicsCore::setFloat2sOnGPU(Mesh& mesh)
{
	string_id technique_name_id = mesh.getTechnique();

	std::map<string_id, Float2Resource>& flt2s =
		ResourceManager::instance()->getFloat2s(technique_name_id);
	for (auto& f2 : flt2s)
	{
		const flt2* v = getFloat2(mesh, f2.first, f2.second.location);
		param_index++;
		if (v == nullptr)
			continue;
		f2.second.ptr->SetRawValue(reinterpret_cast<const void*>(v), 0, 2 * sizeof(float));
	}
}

void GraphicsCore::setFloat1sOnGPU(Mesh& mesh)
{
	string_id technique_name_id = mesh.getTechnique();

	std::map<string_id, Float1Resource>& flt1s =
		ResourceManager::instance()->getFloat1s(technique_name_id);
	for (auto& f1 : flt1s)
	{
		const float* v = getFloat1(mesh, f1.first, f1.second.location);
		param_index++;
		if (v == nullptr)
			continue;
		f1.second.ptr->SetRawValue(reinterpret_cast<const void*>(v), 0, sizeof(float));
	}
}

void GraphicsCore::setStructsOnGPU(Mesh& mesh)
{
	string_id technique_name_id = mesh.getTechnique();

	std::map<string_id, StructResource>& structs =
		ResourceManager::instance()->getStructures(technique_name_id);
	for (auto& s : structs)
	{
		int bytes = 0;
		auto_ptr structData = getStruct(mesh, s.first, s.second, &bytes);
		s.second.ptr->SetRawValue(structData.ptr, 0, bytes);
	}
}

void GraphicsCore::setTexturesOnGPU(Mesh& mesh)
{
	string_id technique_name_id = mesh.getTechnique();

	std::map<string_id, Texture2dResource>& textures =
		ResourceManager::instance()->getTextures(technique_name_id);
	for (auto& t : textures)
	{
		ID3D11ShaderResourceView* tex = getImage(mesh, t.first);
		param_index++;
		t.second.tex->SetResource(tex);
	}
}

void GraphicsCore::setTexturesArraysOnGPU(Mesh& mesh)
{
	string_id technique_name_id = mesh.getTechnique();

	std::map<string_id, Texture2dArrayResource>& texArrs =
		ResourceManager::instance()->getTexturesArrays(technique_name_id);
	for (auto& ta : texArrs)
	{
		ID3D11ShaderResourceView* texArr = getImagesArray(mesh, ta.first);
		param_index++;
		ta.second.texArr->SetResource(texArr);
	}
}

void GraphicsCore::setVariablesOnGPU(Mesh& mesh)
{
	setFloat4sOnGPU(mesh);
	setFloat3sOnGPU(mesh);
	setFloat2sOnGPU(mesh);
	setFloat1sOnGPU(mesh);
	setFloat4x4sOnGPU(mesh);
	setStructsOnGPU(mesh);
	setTexturesOnGPU(mesh);
	setTexturesArraysOnGPU(mesh);
}

void GraphicsCore::setGeometryOnGPU(Mesh& mesh)
{
	uint32_t elementSize = 0;
	ID3D11Buffer* mVB = getVertexBuffer(mesh, &elementSize);
	if (mVB == nullptr)
		return;

	string_id technique_name_id = mesh.getTechnique();
	string_id pass_name_id = mesh.getPass();

	ID3D11Buffer* mIB = getIndexBuffer(mesh);
	if (mIB ==  nullptr && !ResourceManager::instance()->isThereAGeometryShaderInThePass(technique_name_id, pass_name_id))
		return;

	D3D_PRIMITIVE_TOPOLOGY primTopology;
	switch (ResourceManager::instance()->getPrimitiveType(technique_name_id, pass_name_id))
	{
	case PassResource::POINT:
		primTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		break;
	
	case PassResource::LINE:
		primTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		break;
	
	case PassResource::LINEADJ:
		primTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;
		break;

	case PassResource::NONE:
	case PassResource::TRIANGLE:
		primTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		break;

	case PassResource::TRIANGLEADJ:
		primTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;

	default:
		primTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}
	context->IASetPrimitiveTopology(primTopology);

	auto* inputLayout = ResourceManager::instance()->getInputLayout(technique_name_id, pass_name_id);
	if (inputLayout == nullptr)
		return;

	context->IASetInputLayout(inputLayout);

	uint32_t offset = 0;
	context->IASetVertexBuffers(0, 1, &mVB, &elementSize, &offset);
	if (!ResourceManager::instance()->isThereAGeometryShaderInThePass(technique_name_id, pass_name_id))
		context->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
}

ID3D11BlendState* GraphicsCore::getBlendState(Mesh& mesh)
{
	string_id blend_state_id = mesh.getBlendState();
	if (blend_state_id == string_id(-1))
		return nullptr;

	ID3D11BlendState* blendState = ResourceManager::instance()->getBlendState(blend_state_id);
	if (blendState)
		return blendState;

	D3D11_BLEND_DESC blendDesc;
	parseBlendState(StringManager::toString(blend_state_id), blendDesc);

	device->CreateBlendState(&blendDesc, &blendState);
	ResourceManager::instance()->registerBlendState(blend_state_id, blendState);

	return blendState;
}

void GraphicsCore::setBlendState(Mesh& mesh)
{
	ID3D11BlendState* blendState = getBlendState(mesh);
	context->OMSetBlendState(blendState, NULL, 0xffffffff);
}

void GraphicsCore::setPipelineStates(Mesh& mesh)
{
	setBlendState(mesh);
}

void GraphicsCore::initRoughObjectsSelection()
{
	char shadersFolder[200];
	int sz = sizeof shadersFolder / sizeof * shadersFolder;
	GetEnvironmentVariableA("SPECIAL_PURPOSE_SHADERS", shadersFolder, sz);
	
	std::string sRoughObjectsSelection = std::string(shadersFolder) + "\\RoughObjectsSelection.fx";
	
	DWORD shaderFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;
	HRESULT res = D3DX11CompileFromFileA(sRoughObjectsSelection.c_str(), 0, 0, 0, "fx_5_0", shaderFlags, 0, 0, &compiledShader, &compilationMsgs, 0);
	if (res != S_OK)
	{
		MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0, MB_OK);
		return;
	}
	D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), 0, device, &mRoughObjectsSelectionFX);

	mRoughObjectsSelectionTech = mRoughObjectsSelectionFX->GetTechniqueByName("RoughObjectsSelection");

	mBoundingSpheres = mRoughObjectsSelectionFX->GetVariableByName("boundingSpheres")->AsShaderResource();
	mSpheresCount = mRoughObjectsSelectionFX->GetVariableByName("spheresCount");
	mSelectorFrustumRough = mRoughObjectsSelectionFX->GetVariableByName("selectorFrustum");
	mSelectedObjects = mRoughObjectsSelectionFX->GetVariableByName("selectedObjects")->AsUnorderedAccessView();
	mV = mRoughObjectsSelectionFX->GetVariableByName("V")->AsMatrix();

	D3D11_BUFFER_DESC inputDesc;
	inputDesc.Usage = D3D11_USAGE_DEFAULT;
	inputDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	inputDesc.ByteWidth = sizeof(uint32_t) * MAX_BOUNDING_SPHERES_COUNT;
	inputDesc.CPUAccessFlags = 0;
	inputDesc.StructureByteStride = sizeof uint32_t;
	inputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	device->CreateBuffer(&inputDesc, 0, &mInputSelectedObjectsBuffer);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.Flags = 0;
	uavDesc.Buffer.NumElements = MAX_BOUNDING_SPHERES_COUNT;
	device->CreateUnorderedAccessView(mInputSelectedObjectsBuffer, &uavDesc, &mSelectedObjectsUAV);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement = 0;
	srvDesc.BufferEx.Flags = 0;
	srvDesc.BufferEx.NumElements = MAX_BOUNDING_SPHERES_COUNT;
	device->CreateShaderResourceView(mInputSelectedObjectsBuffer, &srvDesc, &mSelectedObjectsSRV);

	D3D11_BUFFER_DESC outputDesc;
	outputDesc.Usage = D3D11_USAGE_STAGING;
	outputDesc.BindFlags = 0;
	outputDesc.ByteWidth = sizeof(uint32_t) * MAX_BOUNDING_SPHERES_COUNT;
	outputDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	outputDesc.StructureByteStride = sizeof uint32_t;
	outputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	device->CreateBuffer(&outputDesc, 0, &mOutputSelectedObjectsBuffer);

	inputDesc.Usage = D3D11_USAGE_DEFAULT;
	inputDesc.ByteWidth = MAX_BOUNDING_SPHERES_COUNT * sizeof(flt4);
	inputDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	inputDesc.CPUAccessFlags = 0;
	inputDesc.StructureByteStride = sizeof flt4;
	inputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	device->CreateBuffer(&inputDesc, nullptr, &mBoundingSpheresBuffer);

	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement = 0;
	srvDesc.BufferEx.Flags = 0;
	srvDesc.BufferEx.NumElements = MAX_BOUNDING_SPHERES_COUNT;
	device->CreateShaderResourceView(mBoundingSpheresBuffer, &srvDesc, &mBoundingSpheresBufferSRV);
}

void GraphicsCore::updateBoundingSpheres(flt4 spheres[])
{
	context->UpdateSubresource(mBoundingSpheresBuffer, 0, 0, spheres, 0, 0);
}

void GraphicsCore::setBoundingSpheresRoughByFrustum()
{
	mBoundingSpheres->SetResource(mBoundingSpheresBufferSRV);
}

void GraphicsCore::setSpheresCount(uint32_t spheresCount)
{
	mSpheresCount->SetRawValue(&spheresCount, 0, sizeof uint32_t);
	this->spheresCount = spheresCount;
}

void GraphicsCore::setSelectorFrustumRough(Frustum& selectorFrustum)
{
	mSelectorFrustumRough->SetRawValue(&selectorFrustum, 0, sizeof Frustum);
}

void GraphicsCore::setSelectedObjectsForWritingRoughByFrustum()
{
	mSelectedObjects->SetUnorderedAccessView(mSelectedObjectsUAV);
}

void GraphicsCore::setV(const flt4x4& V)
{
	mV->SetMatrix((const float*)(&V));
}

void GraphicsCore::findRoughlySelectedObjects()
{
	mRoughObjectsSelectionTech->GetPassByName("P0")->Apply(0, context);

	uint32_t threads_x = std::ceil(float(spheresCount) / 256.0f);
	context->Dispatch(threads_x, 1, 1);
	context->CSSetShader(0, 0, 0);
}

void GraphicsCore::traverseRoughlySelectedObjects(SelectedObjectVisitor* visitor)
{
	context->CopyResource(mOutputSelectedObjectsBuffer, mInputSelectedObjectsBuffer);
	
	D3D11_MAPPED_SUBRESOURCE mappedData;
	context->Map(mOutputSelectedObjectsBuffer, 0, D3D11_MAP_READ, 0, &mappedData);

	uint32_t* selectedObjects = (uint32_t*)mappedData.pData;
	for (uint32_t i = 0; i < spheresCount; ++i)
	{
		if (selectedObjects[i] == 0)
			continue;
		(*visitor)(i);
	}

	context->Unmap(mOutputSelectedObjectsBuffer, 0);
}

void GraphicsCore::initFineObjectsSelection()
{
	char shadersFolder[200];
	int sz = sizeof shadersFolder / sizeof * shadersFolder;
	GetEnvironmentVariableA("SPECIAL_PURPOSE_SHADERS", shadersFolder, sz);

	std::string sFineObjectsSelection = std::string(shadersFolder) + "\\FineObjectsSelection.fx";

	DWORD shaderFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;
	HRESULT res = D3DX11CompileFromFileA(sFineObjectsSelection.c_str(), 0, 0, 0, "fx_5_0", shaderFlags, 0, 0, &compiledShader, &compilationMsgs, 0);
	if (res != S_OK)
	{
		MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0, MB_OK);
		return;
	}
	D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), 0, device, &mFineObjectsSelectionFX);

	mFineObjectsSelectionTech = mFineObjectsSelectionFX->GetTechniqueByName("FineObjectsSelection");
	mSelectorFrustumFine = mFineObjectsSelectionFX->GetVariableByName("selectorFrustum");
	mSelectorFrustumDiagonals = mFineObjectsSelectionFX->GetVariableByName("selectorFrustumDiagonals")->AsShaderResource();
	mVFine = mFineObjectsSelectionFX->GetVariableByName("view")->AsMatrix();
	mThreshold = mFineObjectsSelectionFX->GetVariableByName("threshold");
	mVertices = mFineObjectsSelectionFX->GetVariableByName("vertices")->AsShaderResource();
	mIndicies = mFineObjectsSelectionFX->GetVariableByName("indicies")->AsShaderResource();
	mObjectsInfo = mFineObjectsSelectionFX->GetVariableByName("objectsInfo")->AsShaderResource();
	mRoughlySelectedObjects = mFineObjectsSelectionFX->GetVariableByName("roughlySelectedObjects")->AsShaderResource();
	mObjectsCount = mFineObjectsSelectionFX->GetVariableByName("objectsCount");
	mSelectedTriangles = mFineObjectsSelectionFX->GetVariableByName("selectedTriangles")->AsUnorderedAccessView();

	D3D11_BUFFER_DESC inputDesc;
	inputDesc.Usage = D3D11_USAGE_DEFAULT;
	inputDesc.ByteWidth = 4 * sizeof(Segment);
	inputDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	inputDesc.CPUAccessFlags = 0;
	inputDesc.StructureByteStride = sizeof Segment;
	inputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	device->CreateBuffer(&inputDesc, nullptr, &mSelectorFrustumDiagonalsBuffer);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement = 0;
	srvDesc.BufferEx.Flags = 0;
	srvDesc.BufferEx.NumElements = 4;
	device->CreateShaderResourceView(mSelectorFrustumDiagonalsBuffer, &srvDesc, &mSelectorFrustumDiagonalsBufferSRV);

	inputDesc.Usage = D3D11_USAGE_DEFAULT;
	inputDesc.ByteWidth = MAX_OBJECTS_COUNT * MAX_VERTICES_COUNT * sizeof(flt3);
	inputDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	inputDesc.CPUAccessFlags = 0;
	inputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	inputDesc.StructureByteStride = sizeof flt3;
	device->CreateBuffer(&inputDesc, nullptr, &mVerticesBuffer);

	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement = 0;
	srvDesc.BufferEx.Flags = 0;
	srvDesc.BufferEx.NumElements = MAX_OBJECTS_COUNT * MAX_VERTICES_COUNT;
	device->CreateShaderResourceView(mVerticesBuffer, &srvDesc, &mVerticesBufferSRV);

	inputDesc.Usage = D3D11_USAGE_DEFAULT;
	inputDesc.ByteWidth = MAX_OBJECTS_COUNT * MAX_TRIANGLES_COUNT * 3 * sizeof(uint32_t);
	inputDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	inputDesc.CPUAccessFlags = 0;
	inputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	inputDesc.StructureByteStride = sizeof(uint32_t);
	device->CreateBuffer(&inputDesc, nullptr, &mIndicesBuffer);

	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement = 0;
	srvDesc.BufferEx.Flags = 0;
	srvDesc.BufferEx.NumElements = MAX_OBJECTS_COUNT * MAX_TRIANGLES_COUNT * 3;
	device->CreateShaderResourceView(mIndicesBuffer, &srvDesc, &mIndicesBufferSRV);

	inputDesc.Usage = D3D11_USAGE_DEFAULT;
	inputDesc.ByteWidth = MAX_BOUNDING_SPHERES_COUNT * sizeof(ObjectInfo);
	inputDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	inputDesc.CPUAccessFlags = 0;
	inputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	inputDesc.StructureByteStride = sizeof ObjectInfo;
	device->CreateBuffer(&inputDesc, nullptr, &mObjectsInfoBuffer);

	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement = 0;
	srvDesc.BufferEx.Flags = 0;
	srvDesc.BufferEx.NumElements = MAX_BOUNDING_SPHERES_COUNT;
	device->CreateShaderResourceView(mObjectsInfoBuffer, &srvDesc, &mObjectsInfoBufferSRV);

	inputDesc.Usage = D3D11_USAGE_DEFAULT;
	inputDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	inputDesc.ByteWidth = sizeof(uint32_t) * MAX_BOUNDING_SPHERES_COUNT;
	inputDesc.CPUAccessFlags = 0;
	inputDesc.StructureByteStride = sizeof uint32_t;
	inputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	device->CreateBuffer(&inputDesc, 0, &mInputSelectedTrianglesBuffer);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.Flags = 0;
	uavDesc.Buffer.NumElements = MAX_BOUNDING_SPHERES_COUNT;
	device->CreateUnorderedAccessView(mInputSelectedTrianglesBuffer, &uavDesc, &selectedTrianglesUAV);

	D3D11_BUFFER_DESC outputDesc;
	outputDesc.Usage = D3D11_USAGE_STAGING;
	outputDesc.BindFlags = 0;
	outputDesc.ByteWidth = sizeof(uint32_t) * MAX_BOUNDING_SPHERES_COUNT;
	outputDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	outputDesc.StructureByteStride = sizeof uint32_t;
	outputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	device->CreateBuffer(&outputDesc, 0, &mOutputSelectedTrianglesBuffer);
}

void GraphicsCore::setSelectorFrustumFine(Frustum& selectorFrustum)
{
	mSelectorFrustumFine->SetRawValue(&selectorFrustum, 0, sizeof Frustum);
}

void GraphicsCore::updateSelectorFrustumDiagonals(Segment diagonals[])
{
	context->UpdateSubresource(mSelectorFrustumDiagonalsBuffer, 0, 0, diagonals, 0, 0);
}

void GraphicsCore::setSelectorFrustumDiagonals()
{
	mSelectorFrustumDiagonals->SetResource(mSelectorFrustumDiagonalsBufferSRV);
}

void GraphicsCore::setVFine(const flt4x4& V)
{
	mVFine->SetMatrix((float*)(&V));
}

void GraphicsCore::setThreshold(float threshold)
{
	mThreshold->SetRawValue(&threshold, 0, sizeof(float));
}

void GraphicsCore::updateVertices(flt3 vertices[])
{
	context->UpdateSubresource(mVerticesBuffer, 0, 0, vertices, 0, 0);
}

void GraphicsCore::setVerticesFineByFrustum()
{
	mVertices->SetResource(mVerticesBufferSRV);
}

void GraphicsCore::updateIndices(uint32_t indices[])
{
	context->UpdateSubresource(mIndicesBuffer, 0, 0, indices, 0, 0);
}

void GraphicsCore::setIndicesFineByFrustum()
{
	mIndicies->SetResource(mIndicesBufferSRV);
}

void GraphicsCore::updateObjectsInfo(ObjectInfo objectsInfo[], uint32_t count)
{
	context->UpdateSubresource(mObjectsInfoBuffer, 0, 0, objectsInfo, 0, 0);

	maxTrianglesCount = 0;
	for (int i = 0; i < count; i++)
		maxTrianglesCount = std::max<uint32_t>(maxTrianglesCount, objectsInfo[i].trianglesCount);
}

void GraphicsCore::setObjectsInfoFineByFrustum()
{
	mObjectsInfo->SetResource(mObjectsInfoBufferSRV);
}

void GraphicsCore::initSelectedTrianglesWithZeros()
{
	static uint32_t zeros[MAX_BOUNDING_SPHERES_COUNT];
	context->UpdateSubresource(mInputSelectedTrianglesBuffer, 0, 0, zeros, 0, 0);
}

void GraphicsCore::setSelectedTrianglesWriteFineByFrustum()
{
	mSelectedTriangles->SetUnorderedAccessView(selectedTrianglesUAV);
}

void GraphicsCore::setRoughlySelectedObjects()
{
	mRoughlySelectedObjects->SetResource(mSelectedObjectsSRV);
}

void GraphicsCore::setObjectsCount(uint32_t objectsCount)
{
	mObjectsCount->SetRawValue(&objectsCount, 0, sizeof uint32_t);
	spheresCount = objectsCount;
}

void GraphicsCore::checkIntersection()
{
	uint32_t threads_x = std::ceil(float(spheresCount) / 1.0f);
	uint32_t threads_y = std::ceil(float(maxTrianglesCount) / 256.0f);
	context->Dispatch(threads_x, threads_y, 1);
}

void GraphicsCore::traverseFineSelectedObjects(SelectedObjectVisitor* visitor)
{
	context->CopyResource(mOutputSelectedTrianglesBuffer, mInputSelectedTrianglesBuffer);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	context->Map(mOutputSelectedTrianglesBuffer, 0, D3D11_MAP_READ, 0, &mappedData);

	for (int i = 0; i < spheresCount; ++i)
	{
		if (((uint32_t*)mappedData.pData)[i] > 0)
			visitor->operator()(i);
	}

	context->Unmap(mOutputSelectedTrianglesBuffer, 0);
}

void GraphicsCore::applyContextForFineSelection()
{
	const auto& pass = mFineObjectsSelectionTech->GetPassByName("P0");
	pass->Apply(0, context);
}

void GraphicsCore::initRoughObjectsSelectionBySegments()
{
	char shadersFolder[200];
	int sz = sizeof shadersFolder / sizeof * shadersFolder;
	GetEnvironmentVariableA("SPECIAL_PURPOSE_SHADERS", shadersFolder, sz);

	std::string sRoughObjectsSelectionBySegments = std::string(shadersFolder) + "\\RoughObjectsSelectionBySegments.fx";

	DWORD shaderFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;
	HRESULT res = D3DX11CompileFromFileA(sRoughObjectsSelectionBySegments.c_str(), 0, 0, 0, "fx_5_0", shaderFlags, 0, 0, &compiledShader, &compilationMsgs, 0);
	if (res != S_OK)
	{
		MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0, MB_OK);
		return;
	}
	D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), 0, device, &mRoughObjectsSelectionBySegmentsFX);

	mRoughObjectsSelectionBySegmentsTech = mRoughObjectsSelectionBySegmentsFX->GetTechniqueByName("RoughObjectsSelectionBySegments");

	mSelectingSegments = mRoughObjectsSelectionBySegmentsFX->GetVariableByName("selectingSegments")->AsShaderResource();
	mSelectingSegmentsCount = mRoughObjectsSelectionBySegmentsFX->GetVariableByName("selectingSegmentsCount");

	mBoundingSpheresRoughBySegments = mRoughObjectsSelectionBySegmentsFX->GetVariableByName("boundingSpheres")->AsShaderResource();
	mBoundingSpheresCountRoughBySegments = mRoughObjectsSelectionBySegmentsFX->GetVariableByName("boundingSpheresCount");

	mSelectedObjectsRoughBySegments = mRoughObjectsSelectionBySegmentsFX->GetVariableByName("selectedObjects")->AsUnorderedAccessView();

	D3D11_BUFFER_DESC inputDesc;
	inputDesc.Usage = D3D11_USAGE_DEFAULT;
	inputDesc.ByteWidth = MAX_SELECTING_SEGMENTS_COUNT * sizeof(Segment);
	inputDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	inputDesc.CPUAccessFlags = 0;
	inputDesc.StructureByteStride = sizeof Segment;
	inputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	device->CreateBuffer(&inputDesc, nullptr, &mSelectingSegmentsBuffer);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement = 0;
	srvDesc.BufferEx.Flags = 0;
	srvDesc.BufferEx.NumElements = MAX_SELECTING_SEGMENTS_COUNT;
	device->CreateShaderResourceView(mSelectingSegmentsBuffer, &srvDesc, &mSelectingSegmentsBufferSRV);

	inputDesc.Usage = D3D11_USAGE_DEFAULT;
	inputDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	inputDesc.ByteWidth = sizeof(uint32_t) * MAX_SELECTING_SEGMENTS_COUNT * MAX_BOUNDING_SPHERES_COUNT;
	inputDesc.CPUAccessFlags = 0;
	inputDesc.StructureByteStride = sizeof uint32_t;
	inputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	device->CreateBuffer(&inputDesc, 0, &mSelectedObjectsRoughBySegmentsBuffer);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.Flags = 0;
	uavDesc.Buffer.NumElements = MAX_SELECTING_SEGMENTS_COUNT * MAX_BOUNDING_SPHERES_COUNT;
	device->CreateUnorderedAccessView(mSelectedObjectsRoughBySegmentsBuffer, &uavDesc, &mSelectedObjectsRoughBySegmentsUAV);

	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement = 0;
	srvDesc.BufferEx.Flags = 0;
	srvDesc.BufferEx.NumElements = MAX_SELECTING_SEGMENTS_COUNT * MAX_BOUNDING_SPHERES_COUNT;
	device->CreateShaderResourceView(mSelectedObjectsRoughBySegmentsBuffer, &srvDesc, &mSelectedObjectsRoughBySegmentsSRV);
}

void GraphicsCore::updateSelectingSegments(Segment segments[])
{
	context->UpdateSubresource(mSelectingSegmentsBuffer, 0, 0, segments, 0, 0);
}

void GraphicsCore::setSelectingSegmentsRoughBySegments()
{
	mSelectingSegments->SetResource(mSelectingSegmentsBufferSRV);
}

void GraphicsCore::setSelectingSegmentsCountRoughBySegments(uint32_t count)
{
	mSelectingSegmentsCount->SetRawValue(&count, 0, sizeof uint32_t);
	selectingSegmentsCount = count;
}

void GraphicsCore::setBoundingSpheresRoughBySegments()
{
	mBoundingSpheresRoughBySegments->SetResource(mBoundingSpheresBufferSRV);
}

void GraphicsCore::setBoundingSpheresCountRoughBySegments(uint32_t count)
{
	mBoundingSpheresCountRoughBySegments->SetRawValue(&count, 0, sizeof uint32_t);
	spheresCount = count;
}

void GraphicsCore::setSelectedObjectsWriteRoughBySegments()
{
	mSelectedObjectsRoughBySegments->SetUnorderedAccessView(mSelectedObjectsRoughBySegmentsUAV);
}

void GraphicsCore::findRoughlySelectedObjectsBySegments()
{
	mRoughObjectsSelectionBySegmentsTech->GetPassByName("P0")->Apply(0, context);

	uint32_t threads_x = std::ceil(float(selectingSegmentsCount) / 4.0f);
	uint32_t threads_y = std::ceil(float(spheresCount) / 256.0f);
	uint32_t threads_z = 1.0f;
	context->Dispatch(threads_x, threads_y, threads_z);
	context->CSSetShader(0, 0, 0);
}

void GraphicsCore::initFineObjectsSelectionBySegments()
{
	char shadersFolder[200];
	int sz = sizeof shadersFolder / sizeof * shadersFolder;
	GetEnvironmentVariableA("SPECIAL_PURPOSE_SHADERS", shadersFolder, sz);

	std::string sFineObjectsSelectionBySegments = std::string(shadersFolder) + "\\FineObjectsSelectionBySegments.fx";

	DWORD shaderFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;
	HRESULT res = D3DX11CompileFromFileA(sFineObjectsSelectionBySegments.c_str(), 0, 0, 0, "fx_5_0", shaderFlags, 0, 0, &compiledShader, &compilationMsgs, 0);
	if (res != S_OK)
	{
		MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0, MB_OK);
		return;
	}
	D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), 0, device, &mFineObjectsSelectionBySegmentsFX);

	mFineObjectsSelectionBySegmentsTech = mFineObjectsSelectionBySegmentsFX->GetTechniqueByName("FineObjectsSelectionBySegments");

	mSelectingSegmentsFineBySegments = mFineObjectsSelectionBySegmentsFX->GetVariableByName("selectingSegments")->AsShaderResource();
	mSelectedObjectsFineBySegments = mFineObjectsSelectionBySegmentsFX->GetVariableByName("selectedObjects")->AsShaderResource();

	mSelectingSegmentsCountFineBySegments = mFineObjectsSelectionBySegmentsFX->GetVariableByName("selectingSegmentsCount");
	mObjectsCountFineBySegments = mFineObjectsSelectionBySegmentsFX->GetVariableByName("objectsCount");

	mVerticesFineBySegments = mFineObjectsSelectionBySegmentsFX->GetVariableByName("vertices")->AsShaderResource();
	mIndicesFineBySegments = mFineObjectsSelectionBySegmentsFX->GetVariableByName("indicies")->AsShaderResource();
	mObjectsInfoFineBySegments = mFineObjectsSelectionBySegmentsFX->GetVariableByName("objectsInfo")->AsShaderResource();

	mDistancesToClosestObjects = mFineObjectsSelectionBySegmentsFX->GetVariableByName("distancesToClosestObjects")->AsUnorderedAccessView();
	mClosestObjects = mFineObjectsSelectionBySegmentsFX->GetVariableByName("closestObjects")->AsUnorderedAccessView();

	D3D11_BUFFER_DESC inputDesc;
	inputDesc.Usage = D3D11_USAGE_DEFAULT;
	inputDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	inputDesc.ByteWidth = sizeof(uint32_t) * MAX_SELECTING_SEGMENTS_COUNT;
	inputDesc.CPUAccessFlags = 0;
	inputDesc.StructureByteStride = sizeof(uint32_t);
	inputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	device->CreateBuffer(&inputDesc, 0, &mDistancesToClosestObjectsBuffer);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.Flags = 0;
	uavDesc.Buffer.NumElements = MAX_SELECTING_SEGMENTS_COUNT;
	device->CreateUnorderedAccessView(mDistancesToClosestObjectsBuffer, &uavDesc, &mDistancesToClosestObjectsUAV);

	inputDesc.Usage = D3D11_USAGE_DEFAULT;
	inputDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	inputDesc.ByteWidth = sizeof(uint32_t) * MAX_SELECTING_SEGMENTS_COUNT;
	inputDesc.CPUAccessFlags = 0;
	inputDesc.StructureByteStride = sizeof(uint32_t);
	inputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	device->CreateBuffer(&inputDesc, 0, &mClosestObjectsBuffer);

	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.Flags = 0;
	uavDesc.Buffer.NumElements = MAX_SELECTING_SEGMENTS_COUNT;
	device->CreateUnorderedAccessView(mClosestObjectsBuffer, &uavDesc, &mClosestObjectsUAV);

	D3D11_BUFFER_DESC outputDesc;
	outputDesc.Usage = D3D11_USAGE_STAGING;
	outputDesc.BindFlags = 0;
	outputDesc.ByteWidth = sizeof(uint32_t) * MAX_SELECTING_SEGMENTS_COUNT;
	outputDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	outputDesc.StructureByteStride = sizeof uint32_t;
	outputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	device->CreateBuffer(&outputDesc, 0, &mOutputClosestObjectsBuffer);
}

void GraphicsCore::setSelectingSegmentsFineBySegments()
{
	mSelectingSegmentsFineBySegments->SetResource(mSelectingSegmentsBufferSRV);
}

void GraphicsCore::setSelectedObjectsFineBySegments()
{
	mSelectedObjectsFineBySegments->SetResource(mSelectedObjectsRoughBySegmentsSRV);
}

void GraphicsCore::setSelectingSegmentsCountFineBySegments(uint32_t count)
{
	mSelectingSegmentsCountFineBySegments->SetRawValue(&count, 0, sizeof uint32_t);
	selectingSegmentsCount = count;
}

void GraphicsCore::setObjectsCountFineBySegments(uint32_t count)
{
	mObjectsCountFineBySegments->SetRawValue(&count, 0, sizeof uint32_t);
	spheresCount = count;
}

void GraphicsCore::setVerticesFineBySegments()
{
	mVerticesFineBySegments->SetResource(mVerticesBufferSRV);
}

void GraphicsCore::setIndicesFineBySegments()
{
	mIndicesFineBySegments->SetResource(mIndicesBufferSRV);
}

void GraphicsCore::setObjectsInfoFineBySegments()
{
	mObjectsInfoFineBySegments->SetResource(mObjectsInfoBufferSRV);
}

void GraphicsCore::setDistancesToClosestObjects()
{
	mDistancesToClosestObjects->SetUnorderedAccessView(mDistancesToClosestObjectsUAV);
}

void GraphicsCore::setClosestObjects()
{
	mClosestObjects->SetUnorderedAccessView(mClosestObjectsUAV);
}

void GraphicsCore::initDistancesToClosestObjects()
{
	uint32_t maxInts[MAX_SELECTING_SEGMENTS_COUNT];
	for (int i = 0; i < MAX_SELECTING_SEGMENTS_COUNT; i++)
		maxInts[i] = UINT_MAX;

	context->UpdateSubresource(mDistancesToClosestObjectsBuffer, 0, 0, maxInts, 0, 0);
}

void GraphicsCore::initClosestObjects()
{
	uint32_t invalidObjects[MAX_SELECTING_SEGMENTS_COUNT];
	for (int i = 0; i < MAX_SELECTING_SEGMENTS_COUNT; i++)
		invalidObjects[i] = uint32_t(-1);

	context->UpdateSubresource(mClosestObjectsBuffer, 0, 0, invalidObjects, 0, 0);
}

void GraphicsCore::findSelectedObjectsFineBySegments()
{
	mFineObjectsSelectionBySegmentsTech->GetPassByName("CalculationOfDistancesToClosestObjects")->Apply(0, context);

	uint32_t threads_x = std::ceil(float(maxTrianglesCount) / 256.0f);
	uint32_t threads_y = std::ceil(float(spheresCount) / 1.0f);
	uint32_t threads_z = std::ceil(float(selectingSegmentsCount) / 4.0f);
	context->Dispatch(threads_x, threads_y, threads_z);
	context->CSSetShader(0, 0, 0);

	mFineObjectsSelectionBySegmentsTech->GetPassByName("SearchOfClosestObjects")->Apply(0, context);

	threads_x = std::ceil(float(maxTrianglesCount) / 256.0f);
	threads_y = std::ceil(float(spheresCount) / 1.0f);
	threads_z = std::ceil(float(selectingSegmentsCount) / 4.0f);
	context->Dispatch(threads_x, threads_y, threads_z);
	context->CSSetShader(0, 0, 0);
}

void GraphicsCore::getSelectedObjectsFineBySegments(uint32_t objects[], uint32_t count)
{
	context->CopyResource(mOutputClosestObjectsBuffer, mClosestObjectsBuffer);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	context->Map(mOutputClosestObjectsBuffer, 0, D3D11_MAP_READ, 0, &mappedData);

	for (int i = 0; i < count; i++)
		objects[i] = ((uint32_t*)mappedData.pData)[i];
	
	context->Unmap(mOutputClosestObjectsBuffer, 0);
}

void GraphicsCore::initCalculationOfTextureIntegrals()
{
	char shadersFolder[200];
	int sz = sizeof shadersFolder / sizeof * shadersFolder;
	GetEnvironmentVariableA("SPECIAL_PURPOSE_SHADERS", shadersFolder, sz);

	std::string sCalculationOfTextureIntegrals = std::string(shadersFolder) + "\\CalculationOfTextureIntegrals.fx";

	DWORD shaderFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;
	HRESULT res = D3DX11CompileFromFileA(sCalculationOfTextureIntegrals.c_str(), 0, 0, 0, "fx_5_0", shaderFlags, 0, 0, &compiledShader, &compilationMsgs, 0);
	if (res != S_OK)
	{
		MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0, MB_OK);
		return;
	}
	D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), 0, device, &mCalculationOfTextureIntegralsFX);

	mCalculationOfTextureIntegralsTech = mCalculationOfTextureIntegralsFX->GetTechniqueByName("CalculationOfTextureIntegrals");

	mTextureToIntegrate = mCalculationOfTextureIntegralsFX->GetVariableByName("tex")->AsShaderResource();
	mIntegrals = mCalculationOfTextureIntegralsFX->GetVariableByName("integrals")->AsUnorderedAccessView();

	mXleft = mCalculationOfTextureIntegralsFX->GetVariableByName("x_left");
	mXright = mCalculationOfTextureIntegralsFX->GetVariableByName("x_right");

	mYbottom = mCalculationOfTextureIntegralsFX->GetVariableByName("y_bottom");
	mYtop = mCalculationOfTextureIntegralsFX->GetVariableByName("y_top");

	mAngle0 = mCalculationOfTextureIntegralsFX->GetVariableByName("angle0");
	mScale0 = mCalculationOfTextureIntegralsFX->GetVariableByName("scale0");

	mAngle1 = mCalculationOfTextureIntegralsFX->GetVariableByName("angle1");
	mScale1 = mCalculationOfTextureIntegralsFX->GetVariableByName("scale1");
}

void GraphicsCore::openTextureA(const std::string& path)
{
	char texturesFolder[200];
	int sz = sizeof texturesFolder / sizeof * texturesFolder;
	GetEnvironmentVariableA("TEXTURES", texturesFolder, sz);

	std::string texturePath = std::string(texturesFolder) + '\\' + path;

	D3DX11CreateShaderResourceViewFromFileA(
		device, texturePath.c_str(), 0, 0, &mTextureToIntegrateAsrv, 0
	);

	ID3D11Texture2D* tex;
	mTextureToIntegrateAsrv->GetResource((ID3D11Resource * *)& tex);

	D3D11_TEXTURE2D_DESC tex_desc;
	tex->GetDesc(&tex_desc);

	widthOfA = tex_desc.Width;
	heightOfA = tex_desc.Height;

	tex_desc.Width -= 2 * RADIUS_OF_AREA_IN_TEXELS;
	tex_desc.Height -= 2 * RADIUS_OF_AREA_IN_TEXELS;
	tex_desc.MipLevels = 1;
	tex_desc.ArraySize = RADIUS_OF_AREA_IN_TEXELS;
	tex_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	tex_desc.CPUAccessFlags = 0;
	tex_desc.MiscFlags = 0;
	device->CreateTexture2D(&tex_desc, nullptr, &mIntegralsAtex);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;
	uav_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
	uav_desc.Texture2DArray.MipSlice = 0;
	uav_desc.Texture2DArray.FirstArraySlice = 0;
	uav_desc.Texture2DArray.ArraySize = RADIUS_OF_AREA_IN_TEXELS;
	device->CreateUnorderedAccessView(mIntegralsAtex, &uav_desc, &mIntegralsAuav);

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	srv_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srv_desc.Texture2DArray.MostDetailedMip = 0;
	srv_desc.Texture2DArray.MipLevels = 1;
	srv_desc.Texture2DArray.FirstArraySlice = 0;
	srv_desc.Texture2DArray.ArraySize = RADIUS_OF_AREA_IN_TEXELS;
	device->CreateShaderResourceView(mIntegralsAtex, &srv_desc, &mIntegralsAsrv);

	tex_desc.Width = widthOfA;
	tex_desc.Height = heightOfA;
	tex_desc.MipLevels = 1;
	tex_desc.ArraySize = 1;
	tex_desc.Format = DXGI_FORMAT_R32_UINT;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	tex_desc.CPUAccessFlags = 0;
	tex_desc.MiscFlags = 0;
	device->CreateTexture2D(&tex_desc, nullptr, &mMapAtoBtex);

	uav_desc.Format = DXGI_FORMAT_R32_UINT;
	uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uav_desc.Texture2D.MipSlice = 0;
	device->CreateUnorderedAccessView(mMapAtoBtex, &uav_desc, &mMapAtoBuav);

	tex_desc.MipLevels = 1;
	tex_desc.ArraySize = 1;
	tex_desc.Format = DXGI_FORMAT_R32_UINT;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.Usage = D3D11_USAGE_STAGING;
	tex_desc.BindFlags = 0;
	tex_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	tex_desc.MiscFlags = 0;
	device->CreateTexture2D(&tex_desc, nullptr, &mMapAtoBtexCopy);

	tex_desc.MipLevels = 1;
	tex_desc.ArraySize = 1;
	tex_desc.Format = DXGI_FORMAT_R32_UINT;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	tex_desc.CPUAccessFlags = 0;
	tex_desc.MiscFlags = 0;
	device->CreateTexture2D(&tex_desc, nullptr, &mErrorOfTheSamePointsDefinitionTex);

	uav_desc.Format = DXGI_FORMAT_R32_UINT;
	uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uav_desc.Texture2D.MipSlice = 0;
	device->CreateUnorderedAccessView(mErrorOfTheSamePointsDefinitionTex, &uav_desc, &mErrorOfTheSamePointsDefinitionUAV);
}

void GraphicsCore::openTextureB(const std::string& path)
{
	char texturesFolder[200];
	int sz = sizeof texturesFolder / sizeof * texturesFolder;
	GetEnvironmentVariableA("TEXTURES", texturesFolder, sz);

	std::string texturePath = std::string(texturesFolder) + '\\' + path;

	D3DX11CreateShaderResourceViewFromFileA(
		device, texturePath.c_str(), 0, 0, &mTextureToIntegrateBsrv, 0
	);

	ID3D11Texture2D* tex;
	mTextureToIntegrateBsrv->GetResource((ID3D11Resource * *)& tex);

	D3D11_TEXTURE2D_DESC tex_desc;
	tex->GetDesc(&tex_desc);

	widthOfB = tex_desc.Width;
	heightOfB = tex_desc.Height;

	tex_desc.Width -= 2 * RADIUS_OF_AREA_IN_TEXELS;
	tex_desc.MipLevels = 1;
	tex_desc.ArraySize = NUMBER_OF_TEXTURE_INTEGRALS;
	tex_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	tex_desc.CPUAccessFlags = 0;
	tex_desc.MiscFlags = 0;
	device->CreateTexture2D(&tex_desc, nullptr, &mHorisontalIntegralsBtex);

	tex_desc.Height -= 2 * RADIUS_OF_AREA_IN_TEXELS;
	device->CreateTexture2D(&tex_desc, nullptr, &mVerticalIntegralsBtex);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;
	uav_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
	uav_desc.Texture2DArray.MipSlice = 0;
	uav_desc.Texture2DArray.FirstArraySlice = 0;
	uav_desc.Texture2DArray.ArraySize = NUMBER_OF_TEXTURE_INTEGRALS;

	device->CreateUnorderedAccessView(mHorisontalIntegralsBtex, &uav_desc, &mHorisontalIntegralsBuav);
	device->CreateUnorderedAccessView(mVerticalIntegralsBtex, &uav_desc, &mVerticalIntegralsBuav);

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	srv_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srv_desc.Texture2DArray.MostDetailedMip = 0;
	srv_desc.Texture2DArray.MipLevels = 1;
	srv_desc.Texture2DArray.FirstArraySlice = 0;
	srv_desc.Texture2DArray.ArraySize = NUMBER_OF_TEXTURE_INTEGRALS;
	device->CreateShaderResourceView(mHorisontalIntegralsBtex, &srv_desc, &mHorisontalIntegralsBsrv);
	device->CreateShaderResourceView(mVerticalIntegralsBtex, &srv_desc, &mIntegralsBsrv);
}

void GraphicsCore::calculateIntegralsOfTextureA()
{
	mTextureToIntegrate->SetResource(mTextureToIntegrateAsrv);
	mIntegrals->SetUnorderedAccessView(mIntegralsAuav);

	int radius = -RADIUS_OF_AREA_IN_TEXELS;
	mXleft->SetRawValue(&radius, 0, sizeof radius);
	mYbottom->SetRawValue(&radius, 0, sizeof radius);

	radius = RADIUS_OF_AREA_IN_TEXELS;
	mXright->SetRawValue(&radius, 0, sizeof radius);
	mYtop->SetRawValue(&radius, 0, sizeof radius);

	float angle = 0;
	float scale = 1;
	mAngle0->SetRawValue(&angle, 0, sizeof angle);
	mScale0->SetRawValue(&scale, 0, sizeof scale);
	mAngle1->SetRawValue(&angle, 0, sizeof angle);
	mScale1->SetRawValue(&scale, 0, sizeof scale);

	mCalculationOfTextureIntegralsTech->GetPassByName("P0")->Apply(0, context);

	uint32_t groups_x = std::ceil((float)(widthOfA - 2 * RADIUS_OF_AREA_IN_TEXELS) / 16.0f);
	uint32_t groups_y = std::ceil((float)(heightOfA - 2 * RADIUS_OF_AREA_IN_TEXELS) / 16.0f);
	uint32_t groups_z = std::ceil((float)(RADIUS_OF_AREA_IN_TEXELS) / 4.0f);
	context->Dispatch(groups_x, groups_y, groups_z);

	ID3D11UnorderedAccessView* nullUAV = nullptr;
	context->CSSetUnorderedAccessViews(0, 1, &nullUAV, 0);
	
	context->CSSetShader(0, 0, 0);
}

void GraphicsCore::calculateIntegralsOfTextureB()
{
	mTextureToIntegrate->SetResource(mTextureToIntegrateBsrv);
	mHorisontalIntegrals->SetUnorderedAccessView(mHorisontalIntegralsBuav);

	mCalculationOfTextureIntegralsTech->GetPassByName("AlongUaxis")->Apply(0, context);

	uint32_t groups_x = std::ceil((float)(widthOfB - 2 * RADIUS_OF_AREA_IN_TEXELS) / 16.0f);
	uint32_t groups_y = std::ceil((float)(heightOfB) / 16.0f);
	uint32_t groups_z = std::ceil((float)(NUMBER_OF_TEXTURE_INTEGRALS) / 4.0f);
	context->Dispatch(groups_x, groups_y, groups_z);

	ID3D11UnorderedAccessView* nullUAVs[2] = {
		nullptr, nullptr };
	context->CSSetUnorderedAccessViews(0, 2, nullUAVs, 0);

	mHorisontalIntegralsInput->SetResource(mHorisontalIntegralsBsrv);
	mVerticalIntegrals->SetUnorderedAccessView(mVerticalIntegralsBuav);

	mCalculationOfTextureIntegralsTech->GetPassByName("AlongVaxis")->Apply(0, context);

	groups_x = std::ceil((float)(widthOfB - 2 * RADIUS_OF_AREA_IN_TEXELS) / 16.0f);
	groups_y = std::ceil((float)(heightOfB - 2 * RADIUS_OF_AREA_IN_TEXELS) / 16.0f);
	groups_z = std::ceil((float)(NUMBER_OF_TEXTURE_INTEGRALS) / 4.0f);
	context->Dispatch(groups_x, groups_y, groups_z);

	context->CSSetUnorderedAccessViews(0, 2, nullUAVs, 0);

	context->CSSetShader(0, 0, 0);
}

void GraphicsCore::initDefinitionOfTheSamePoints()
{
	char shadersFolder[200];
	int sz = sizeof shadersFolder / sizeof * shadersFolder;
	GetEnvironmentVariableA("SPECIAL_PURPOSE_SHADERS", shadersFolder, sz);

	char* buffer = nullptr;
	uint32_t bufferSize = 0;
#ifdef SAVE_COMPILED_SHADER
	std::string sObjectFile = std::string(shadersFolder) + "\\DefineTheSamePointsOnTwoImages.fxo";
	std::ifstream objectFile_in(sObjectFile.c_str(), std::ifstream::binary);
	if (objectFile_in)
	{
		objectFile_in.seekg(0, objectFile_in.end);
		bufferSize = objectFile_in.tellg();
		objectFile_in.seekg(0, objectFile_in.beg);

		buffer = new char[bufferSize];

		objectFile_in.read(buffer, bufferSize);
	}
	else
	{
		std::string sDefinitionOfTheSamePoints = std::string(shadersFolder) + "\\DefineTheSamePointsOnTwoImages.fx";

		DWORD shaderFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		shaderFlags |= D3D10_SHADER_DEBUG;
		shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

		ID3D10Blob* compiledShader = 0;
		ID3D10Blob* compilationMsgs = 0;
		HRESULT res = D3DX11CompileFromFileA(sDefinitionOfTheSamePoints.c_str(), 0, 0, 0, "fx_5_0", shaderFlags, 0, 0, &compiledShader, &compilationMsgs, 0);
		if (res != S_OK)
		{
			MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0, MB_OK);
			return;
		}

		buffer = (char*)compiledShader->GetBufferPointer();
		bufferSize = compiledShader->GetBufferSize();

		std::ofstream objectFile_out(sObjectFile.c_str(), std::ifstream::binary);
		objectFile_out.write(buffer, bufferSize);
		objectFile_out.close();
	}
#else
	std::string sDefinitionOfTheSamePoints = std::string(shadersFolder) + "\\DefineTheSamePointsOnTwoImages.fx";

	DWORD shaderFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;
	HRESULT res = D3DX11CompileFromFileA(sDefinitionOfTheSamePoints.c_str(), 0, 0, 0, "fx_5_0", shaderFlags, 0, 0, &compiledShader, &compilationMsgs, 0);
	if (res != S_OK)
	{
		MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0, MB_OK);
		return;
	}

	buffer = (char*)compiledShader->GetBufferPointer();
	bufferSize = compiledShader->GetBufferSize();
#endif
	auto res1 = D3DX11CreateEffectFromMemory((LPVOID)buffer, bufferSize, 0, device, &mDefinitionOfTheSamePointsFX);

	mDefineTheSamePointsOnTwoImagesTech = mDefinitionOfTheSamePointsFX->GetTechniqueByName("DefineTheSamePointsOnTwoImages");

	mIntegralsOfA = mDefinitionOfTheSamePointsFX->GetVariableByName("integralsOfA")->AsShaderResource();
	mIntegralsOfB = mDefinitionOfTheSamePointsFX->GetVariableByName("integralsOfB")->AsShaderResource();

	mMapAtoB = mDefinitionOfTheSamePointsFX->GetVariableByName("mapAtoB")->AsUnorderedAccessView();
	mErrorOfTheSamePointsDefinition = mDefinitionOfTheSamePointsFX->GetVariableByName("error")->AsUnorderedAccessView();
}

void GraphicsCore::defineTheSamePoints()
{
	mIntegralsOfA->SetResource(mIntegralsAsrv);
	mIntegralsOfB->SetResource(mIntegralsBsrv);

	mMapAtoB->SetUnorderedAccessView(mMapAtoBuav);
	mErrorOfTheSamePointsDefinition->SetUnorderedAccessView(mErrorOfTheSamePointsDefinitionUAV);

	mDefineTheSamePointsOnTwoImagesTech->GetPassByName("InitializeError")->Apply(0, context);

	uint32_t groups_x = std::ceil((float)(widthOfA - 2 * RADIUS_OF_AREA_IN_TEXELS) / 32.0f);
	uint32_t groups_y = std::ceil((float)(heightOfA - 2 * RADIUS_OF_AREA_IN_TEXELS) / 32.0f);
	uint32_t groups_z = 1;
	context->Dispatch(groups_x, groups_y, groups_z);

	mDefineTheSamePointsOnTwoImagesTech->GetPassByName("CalculateError")->Apply(0, context);

	groups_x = std::ceil((float)((widthOfA - 2 * RADIUS_OF_AREA_IN_TEXELS) * (heightOfA - 2 * RADIUS_OF_AREA_IN_TEXELS)) / 32.0f);
	groups_y = std::ceil((float)((widthOfB - 2 * RADIUS_OF_AREA_IN_TEXELS) * (heightOfB - 2 * RADIUS_OF_AREA_IN_TEXELS)) / 32.0f);
	groups_z = 1;
	context->Dispatch(groups_x, groups_y, groups_z);

	mDefineTheSamePointsOnTwoImagesTech->GetPassByName("MapAontoB")->Apply(0, context);

	groups_x = std::ceil((float)((widthOfA - 2 * RADIUS_OF_AREA_IN_TEXELS) * (heightOfA - 2 * RADIUS_OF_AREA_IN_TEXELS)) / 32.0f);
	groups_y = std::ceil((float)((widthOfB - 2 * RADIUS_OF_AREA_IN_TEXELS) * (heightOfB - 2 * RADIUS_OF_AREA_IN_TEXELS)) / 32.0f);
	groups_z = 1;
	context->Dispatch(groups_x, groups_y, groups_z);
	context->CSSetShader(0, 0, 0);
}

flt2 GraphicsCore::mapAtoB(flt2& uvA)
{
	context->CopyResource(mMapAtoBtexCopy, mMapAtoBtex);

	D3D11_MAPPED_SUBRESOURCE texData;
	context->Map(mMapAtoBtexCopy, 0, D3D11_MAP_READ, 0, &texData);

	float posInAx = widthOfA * uvA.x() - 0.5f;
	float posInAy = heightOfA * uvA.y() - 0.5f;

	posInAx -= RADIUS_OF_AREA_IN_TEXELS;
	posInAy -= RADIUS_OF_AREA_IN_TEXELS;

	uint32_t uiPosInAx = std::min<float>(std::max<float>(posInAx, 0.0f), widthOfA - 2 * RADIUS_OF_AREA_IN_TEXELS - 1);
	uint32_t uiPosInAy = std::min<float>(std::max<float>(posInAy, 0.0f), heightOfA - 2 * RADIUS_OF_AREA_IN_TEXELS - 1);

	char* pRow = (char*)texData.pData + uiPosInAy * texData.RowPitch;
	uint32_t mappedPixel = *((uint32_t*)pRow + uiPosInAx);

	uint32_t uiPosInBx = mappedPixel % (widthOfB - 2 * RADIUS_OF_AREA_IN_TEXELS);
	uint32_t uiPosInBy = mappedPixel / (widthOfB - 2 * RADIUS_OF_AREA_IN_TEXELS);

	uiPosInBx += RADIUS_OF_AREA_IN_TEXELS;
	uiPosInBy += RADIUS_OF_AREA_IN_TEXELS;

	flt2 uvB(((float)(uiPosInBx)+0.5f) / widthOfB, ((float)(uiPosInBy)+0.5f) / heightOfB);

	context->Unmap(mMapAtoBtexCopy, 0);

	return uvB;
}