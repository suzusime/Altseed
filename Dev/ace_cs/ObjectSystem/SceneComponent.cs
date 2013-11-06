﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public abstract class SceneComponent
	{
		public Scene Owner { get; internal set; }

		protected abstract void OnUpdated();

		internal void Update()
		{
			OnUpdated();
		}
	}
}
