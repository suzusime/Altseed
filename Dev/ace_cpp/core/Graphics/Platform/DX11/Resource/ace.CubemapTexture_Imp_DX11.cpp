﻿
#include "ace.CubemapTexture_Imp_DX11.h"
#include "../ace.Graphics_Imp_DX11.h"

namespace ace
{
	CubemapTexture_Imp_DX11::CubemapTexture_Imp_DX11(Graphics* graphics, ID3D11Texture2D* texture, ID3D11ShaderResourceView* textureSRV, std::array<std::vector<ID3D11RenderTargetView*>, 6>& textureRTVs)
		: CubemapTexture_Imp(graphics)
		, m_texture(texture)
		, m_textureSRV(textureSRV)
		, m_textureRTVs(textureRTVs)
	{

	}

	CubemapTexture_Imp_DX11::~CubemapTexture_Imp_DX11()
	{
		SafeRelease(m_texture);
		SafeRelease(m_textureSRV);

		for (auto& v : m_textureRTVs)
		{
			for (auto& v_ : v)
			{
				v_->Release();
			}
		}
	}

	CubemapTexture_Imp* CubemapTexture_Imp_DX11::Create(Graphics_Imp* graphics, const achar* front, const achar* left, const achar* back, const achar* right, const achar* top, const achar* bottom)
	{
		auto loadFile = [](const achar* path, std::vector<uint8_t>& dst)-> bool
		{
#if _WIN32
			auto fp = _wfopen(path, L"rb");
			if (fp == nullptr) return false;
#else
			auto fp = fopen(ToUtf8String(path).c_str(), "rb");
			if (fp == nullptr) return false;
#endif
			fseek(fp, 0, SEEK_END);
			auto size = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			dst.resize(size);
			fread(dst.data(), 1, size, fp);
			fclose(fp);

			return true;
		};

		int32_t widthes[6];
		int32_t heights[6];
		std::vector<uint8_t> fileBuffers[6];
		std::array<std::vector<ID3D11RenderTargetView*>, 6> textureRTVs;

		uint8_t* buffers [] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

		const achar* pathes[] = {
			right,
			left,
			top,
			bottom,
			front,
			back,
		};

		auto g = (Graphics_Imp_DX11*) graphics;

		for (int32_t i = 0; i < 6; i++)
		{
			if (!loadFile(pathes[i], fileBuffers[i]))
			{
				return nullptr;
			}
		}

		for (int32_t i = 0; i < 6; i++)
		{
			void* result = nullptr;
			if (ImageHelper::LoadPNGImage(fileBuffers[i].data(), fileBuffers[i].size(), false, widthes[i], heights[i], result))
			{
				buffers[i] = (uint8_t*) result;
			}
			else
			{
				goto End;
			}
		}

		
		auto width = widthes[0];
		auto height = heights[0];

		for (int32_t i = 0; i < 6; i++)
		{
			if (widthes[i] != width) goto End;
			if (heights[i] != height) goto End;
		}

		ID3D11Texture2D* texture = nullptr;
		ID3D11ShaderResourceView* srv = nullptr;

		D3D11_TEXTURE2D_DESC TexDesc;
		TexDesc.Width = width;
		TexDesc.Height = height;
		TexDesc.MipLevels = 1;
		TexDesc.ArraySize = 6;
		TexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		TexDesc.SampleDesc.Count = 1;
		TexDesc.SampleDesc.Quality = 0;
		TexDesc.Usage = D3D11_USAGE_DEFAULT;
		TexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		TexDesc.CPUAccessFlags = 0;
		TexDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;

		// 全てのミップマップを使用
		desc.TextureCube.MipLevels = -1;
		desc.TextureCube.MostDetailedMip = 0;

		D3D11_SUBRESOURCE_DATA data[6];

		for (int32_t i = 0; i < 6; i++)
		{
			data[i].pSysMem = buffers[i];
			data[i].SysMemPitch = width * 4;
			data[i].SysMemSlicePitch = data[i].SysMemPitch * height;
		}

		auto hr = g->GetDevice()->CreateTexture2D(&TexDesc, data, &texture);

		if (FAILED(hr))
		{
			goto End;
		}

		hr = g->GetDevice()->CreateShaderResourceView(texture, &desc, &srv);
		if (FAILED(hr))
		{
			SafeRelease(texture);
			goto End;
		}

		// ミップマップ処理
		texture->GetDesc(&TexDesc);

		for (int32_t dirNum = 0; dirNum < 6; dirNum++)
		{
			for (int32_t mip = 0; mip < TexDesc.MipLevels; mip++)
			{
				ID3D11RenderTargetView* rtv = nullptr;
				D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
				rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
				rtvDesc.Texture2DArray.ArraySize = 1;
				rtvDesc.Texture2DArray.FirstArraySlice = dirNum;
				rtvDesc.Texture2DArray.MipSlice = mip;

				hr = g->GetDevice()->CreateRenderTargetView(texture, &rtvDesc, &rtv);
				if (FAILED(hr))
				{
					goto End;
				}
				textureRTVs[dirNum].push_back(rtv);
			}
		}

		for (int32_t i = 0; i < 6; i++)
		{
			SafeDeleteArray(buffers[i]);
		}
		return new CubemapTexture_Imp_DX11(graphics, texture, srv, textureRTVs);

	End:;

		for (int32_t i = 0; i < 6; i++)
		{
			SafeDeleteArray(buffers[i]);
		}

		for (auto& v : textureRTVs)
		{
			for (auto& v_ : v)
			{
				if (v_ == nullptr) continue;
				v_->Release();
			}
		}

		return nullptr;
	}
}