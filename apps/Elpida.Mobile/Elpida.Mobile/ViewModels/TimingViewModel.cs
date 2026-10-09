// =========================================================================
//
// Elpida Mobile
//
// Copyright (C) 2026 Ioannis Panagiotopoulos
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
//
// You should have received a copy of the GNU General Public License
// =========================================================================

using Elpida.Mobile.Models;

namespace Elpida.Mobile.ViewModels
{
	public class TimingViewModel
	{
		private readonly TimingModel _model;

		public TimingViewModel(TimingModel model)
		{
			_model = model;
		}

		public double NowOverhead => _model.NowOverhead;

		public double LoopOverhead => _model.LoopOverhead;

		public ulong IterationsPerSecond => _model.IterationsPerSecond;
	}
}