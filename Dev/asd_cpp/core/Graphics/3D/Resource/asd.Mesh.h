﻿
#pragma once

#include <asd.common.Base.h>

#include "../../../asd.Core.Base.h"

#include "../../../asd.ReferenceObject.h"

namespace asd
{
	/**
		@brief	3Dメッシュのクラス
	*/
	class Mesh
		: public IReference
	{
	protected:
		Mesh(){}
		virtual ~Mesh(){}
	public:

		/**
			@brief	頂点を追加する。
			@param	position	座標
			@param	normal		法線
			@param	binormal	従法線
			@param	uv1			UV1
			@param	uv2			UV2
			@param	color		頂点色
			@param	boneWeights	ボーンのウエイト
			@param	boneIndexes	ボーンのインデックス
		*/
		virtual void AddVertex(
			const Vector3DF& position,
			const Vector3DF& normal,
			const Vector3DF& binormal,
			const Vector2DF& uv1,
			const Vector2DF& uv2,
			const Color& color,
			int32_t boneWeights,
			int32_t boneIndexes) = 0;

		/**
			@brief	面を追加する。
			@param	index1	頂点インデックス1
			@param	index2	頂点インデックス2
			@param	index3	頂点インデックス3
			@param	materialIndex	材質インデックス
		*/
		virtual void AddFace(int32_t index1, int32_t index2, int32_t index3, int32_t materialIndex) = 0;

		/**
			@brief	ボーンとの接続設定を追加する。
			@param	targetIndex	対象ボーンインデックス
			@param	boneToMesh	ボーンの行列をメッシュの行列に変換する行列
		*/
		virtual void AddBoneConnector(int32_t targetIndex, const Matrix44& boneToMesh) = 0;

		/**
			@brief	材質を追加する。
			@return	材質のインデックス
		*/
		virtual int32_t AddMaterial() = 0;

		/**
			@brief	設定した値をGPUに送信する。
		*/
		virtual void SendToGPUMemory() = 0;

		/**
		@brief	内部シェーダーを使用する場合のカラーテクスチャを設定する。
		@param	materialIndex	材質のインデックス
		@param	テクスチャ
		@note
		AddMaterialCountを実行した後でないと無効になる。
		*/
		virtual void SetColorTexture(int32_t materialIndex, Texture2D* texture) = 0;

		/**
		@brief	内部シェーダーを使用する場合の法線テクスチャを設定する。
		@param	materialIndex	材質のインデックス
		@param	テクスチャ
		@note
		AddMaterialCountを実行した後でないと無効になる。
		*/
		virtual void SetNormalTexture(int32_t materialIndex, Texture2D* texture) = 0;

		/**
		@brief	内部シェーダーを使用する場合の金属度テクスチャを設定する。
		@param	materialIndex	材質のインデックス
		@param	テクスチャ
		@note
		AddMaterialCountを実行した後でないと無効になる。
		*/
		virtual void SetMetalnessTexture(int32_t materialIndex, Texture2D* texture) = 0;

		/**
		@brief	内部シェーダーを使用する場合の面平滑度テクスチャを設定する。
		@param	materialIndex	材質のインデックス
		@param	テクスチャ
		@note
		AddMaterialCountを実行した後でないと無効になる。
		*/
		virtual void SetSmoothnessTexture(int32_t materialIndex, Texture2D* texture) = 0;

		/**
		@brief	材質を設定する。
		@param	materialIndex	材質のインデックス
		@param	material		材質
		@note
		AddMaterialCountを実行した後でないと無効になる。
		*/
		virtual void SetMaterial(int32_t materialIndex, Material3D* material) = 0;

#if !SWIG
		/**
		@brief	内部シェーダーを使用する場合のカラーテクスチャを設定する。
		@param	materialIndex	材質のインデックス
		@param	テクスチャ
		@note
		AddMaterialCountを実行した後でないと無効になる。
		*/
		void SetColorTexture(int32_t materialIndex, std::shared_ptr<Texture2D> texture)
		{
			SetColorTexture(materialIndex, texture.get());
		}

		/**
		@brief	内部シェーダーを使用する場合の法線テクスチャを設定する。
		@param	materialIndex	材質のインデックス
		@param	テクスチャ
		@note
		AddMaterialCountを実行した後でないと無効になる。
		*/
		void SetNormalTexture(int32_t materialIndex, std::shared_ptr<Texture2D> texture)
		{
			SetNormalTexture(materialIndex, texture.get());
		}

		/**
		@brief	内部シェーダーを使用する場合の金属度テクスチャを設定する。
		@param	materialIndex	材質のインデックス
		@param	テクスチャ
		@note
		AddMaterialCountを実行した後でないと無効になる。
		*/
		void SetMetalnessTexture(int32_t materialIndex, std::shared_ptr<Texture2D> texture)
		{
			SetMetalnessTexture(materialIndex, texture.get());
		}


		/**
		@brief	内部シェーダーを使用する場合の面平滑度テクスチャを設定する。
		@param	materialIndex	材質のインデックス
		@param	テクスチャ
		@note
		AddMaterialCountを実行した後でないと無効になる。
		*/
		void SetSmoothnessTexture(int32_t materialIndex, std::shared_ptr<Texture2D> texture)
		{
			SetSmoothnessTexture(materialIndex, texture.get());
		}

		/**
		@brief	材質を設定する。
		@param	materialIndex	材質のインデックス
		@param	material		材質
		@note
		AddMaterialCountを実行した後でないと無効になる。
		*/
		void SetMaterial(int32_t materialIndex, std::shared_ptr<Material3D> material)
		{
			SetMaterial(materialIndex, material.get());
		}
#endif
	};
};