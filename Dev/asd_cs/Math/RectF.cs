﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	/// <summary>
	/// 長方形(浮動小数点)の構造体
	/// </summary>
	[System.Runtime.InteropServices.StructLayout(System.Runtime.InteropServices.LayoutKind.Sequential)]
	public struct RectF
	{
		/// <summary>
		/// 左上X座標
		/// </summary>
		public float X;

		/// <summary>
		/// 左上Y座標
		/// </summary>
		public float Y;

		/// <summary>
		/// 横幅
		/// </summary>
		public float Width;

		/// <summary>
		/// 縦幅
		/// </summary>
		public float Height;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="x">左上X座標</param>
		/// <param name="y">左上Y座標</param>
		/// <param name="width">横幅</param>
		/// <param name="height">縦幅</param>
		public RectF(float x, float y, float width, float height)
			: this()
		{
			this.X = x;
			this.Y = y;
			this.Width = width;
			this.Height = height;
		}

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="position">左上座標</param>
		/// <param name="size">大きさ</param>
		public RectF(Vector2DF position, Vector2DF size)
			: this(position.X, position.Y, size.X, size.Y)
		{
		}

		/// <summary>
		/// 左上座標を取得する。
		/// </summary>
		public Vector2DF Position
		{
			get { return new Vector2DF(X, Y); }
		}

		/// <summary>
		/// 大きさを取得する。
		/// </summary>
		public Vector2DF Size
		{
			get { return new Vector2DF(Width, Height); }
		}

		/// <summary>
		/// 四隅の座標を(X,Y)、(X+Width,Y)、(X+Width,Y+Height)、(X,Y+Height)の順に配列として取得する。
		/// </summary>
		public Vector2DF[] Vertexes
		{
			get
			{
				var ret = new Vector2DF[4];

				ret[0] = new Vector2DF(X, Y);
				ret[1] = new Vector2DF(X + Width, Y);
				ret[2] = new Vector2DF(X + Width, Y + Height);
				ret[3] = new Vector2DF(X, Y + Height);

				return ret;
			}
		}

		/// <summary>
		/// RectI型に変換する。
		/// </summary>
		/// <returns>RectI型</returns>
		public RectI ToI()
		{
			return new RectI((int)X, (int)Y, (int)Width, (int)Height);
		}

		public override bool Equals(object obj)
		{
			if (obj is RectF)
			{
				return (RectF)obj == this;
			}
			return false;
		}

		public static bool operator ==(RectF lop, RectF rop)
		{
			return lop.X == rop.X && lop.Y == rop.Y && lop.Width == rop.Width && lop.Height == rop.Height;
		}

		public static bool operator !=(RectF lop, RectF rop)
		{
			return !(lop == rop);
		}
	}
}
