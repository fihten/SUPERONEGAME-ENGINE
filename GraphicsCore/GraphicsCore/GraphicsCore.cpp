#include "GraphicsCore.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "Cameras.h"

void GraphicsCore::init(HINSTANCE instanceHandle, int show, WNDPROC WndProc, DRAW_FUNC drawFunc, UINT width, UINT height, bool windowed, bool enable4xMsaa)
{
	this->drawFunc = drawFunc;

	mWidth = width;
	mHeight = height;

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
		sd.SampleDesc.Quality = m4xMsaaQuality;
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
	std::string sTechnique = mesh.getTechnique();
	std::string sPass = mesh.getPass();

	std::map<std::string, ID3DX11EffectMatrixVariable*> matrices;
	resourceManager.getMatrices(sTechnique, matrices);
	for (auto& m : matrices)
	{
		std::string place = resourceManager.getVariableLocation(sTechnique, m.first);
		size_t pos = place.find("cameras[");
		if (pos == 0)
		{
			size_t beg = 7;
			size_t end = place.find(']', beg);

			int index = std::atoi(std::string(place, beg, end - beg).c_str());
			
			beg = end + 2;
			std::string what(place, beg, std::string::npos);
			if (what == std::string("WVP"))
			{
				const flt4x4& v = cameras[index].getView();
				const flt4x4& p = cameras[index].getProj();
				flt4x4 wvp = v * p;

				m.second->SetMatrix(reinterpret_cast<float*>(&wvp));
			}

			continue;
		}
	}

	const std::vector<ResourceManager::InputLayoutStreamInfo>* streamsInfo = resourceManager.getStreamsInfo(sTechnique, sPass);
	if (streamsInfo == nullptr)
		return;

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
			case Mesh::FLT1:
			{
				const std::vector<flt1>& s = *(const std::vector<flt1>*)stream;
				std::copy((char*)(&s[i]), (char*)(&s[i]) + si.size, dataElement);
				break; 
			}
			case Mesh::FLT2:
			{
				const std::vector<flt2>& s = *(const std::vector<flt2>*)stream;
				std::copy((char*)(&s[i]), (char*)(&s[i]) + si.size, dataElement);
				break;
			}
			case Mesh::FLT3:
			{
				const std::vector<flt3>& s = *(const std::vector<flt3>*)stream;
				std::copy((char*)(&s[i]), (char*)(&s[i]) + si.size, dataElement);
				break;
			}
			case Mesh::FLT4:
			{
				const std::vector<flt4>& s = *(const std::vector<flt4>*)stream;
				std::copy((char*)(&s[i]), (char*)(&s[i]) + si.size, dataElement);
				break;
			}
			}
			dataElement += si.size;
		}
	}

	// Create vertex buffer
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = bytes;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = data;

	ID3D11Buffer* mVB;
	device->CreateBuffer(&vbd, &vinitData, &mVB);

	free((void*)data);

	// Create index buffer
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = mesh.getIndicesCount() * sizeof(uint32_t);
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = mesh.getIndicies()->data();

	ID3D11Buffer* mIB;
	device->CreateBuffer(&ibd, &iinitData, &mIB);

	auto* inputLayout = resourceManager.getInputLayout(sTechnique, sPass);
	if (inputLayout == nullptr)
		return;

	context->IASetInputLayout(inputLayout);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	uint32_t offset = 0;
	context->IASetVertexBuffers(0, 1, &mVB, &elementSize, &offset);
	context->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	mIB->Release();
	mVB->Release();

	auto* pass = resourceManager.getPass(sTechnique, sPass);
	if (pass == nullptr)
		return;

	pass->Apply(0, context);
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
		CW_USEDEFAULT, // width
		CW_USEDEFAULT, // height
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
}