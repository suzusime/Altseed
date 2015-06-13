﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.Window
{
	public class EmptyExternal : TestFramework
	{
		//[Test]
		public override void Test(asd.GraphicsDeviceType graphicsType)
		{
			var option = new asd.EngineOption
			{
				GraphicsDevice = graphicsType,
				IsFullScreen = false
			};

			bool closed = false;
			System.Windows.Forms.Form form = new System.Windows.Forms.Form();
			form.FormClosed += (object sender, System.Windows.Forms.FormClosedEventArgs e) =>
				{
					closed = true;
				};
			form.Show();


			// aceを初期化する。
			asd.Engine.InitializeByExternalWindow(form.Handle, new IntPtr(), form.Size.Width, form.Size.Height, new asd.EngineOption());

			int time = 0;

			// aceが進行可能かチェックする。
			while (asd.Engine.DoEvents())
			{
				System.Windows.Forms.Application.DoEvents();
				if (closed) break;

				// aceを更新する。
				asd.Engine.Update();

				if (time == 10) break;
				time++;
			}

			// aceを終了する。
			asd.Engine.Terminate();
		}
	}
}
