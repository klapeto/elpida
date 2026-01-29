/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 7/4/2024.
//

#ifndef ELPIDA_SVGPARSETASK_HPP
#define ELPIDA_SVGPARSETASK_HPP

#include "Elpida/Core/MicroTask.hpp"
#include "Elpida/Svg/SvgDocument.hpp"
#include "Elpida/Xml/XmlElement.hpp"

namespace Elpida
{

	class SvgParseTask : public MicroTask
	{
	public:

		void Prepare(SharedPtr<AbstractTaskData> inputData) override;

		[[nodiscard]]
		SharedPtr<AbstractTaskData> Finalize() override;

		[[nodiscard]]
		Size GetProcessedDataSize() const override;

		SvgParseTask() = default;
		~SvgParseTask() override = default;
	protected:
		void DoRunImpl() override;

		TaskInfo DoGetInfo() const override;

		[[nodiscard]]
		Size GetOperationsPerformedPerRun() override;

		[[nodiscard]]
		UniquePtr<Task> DoDuplicate() const override;
	private:
		SharedPtr<AbstractTaskData> _inputData;
		XmlElement _inputXmlElement;
		SvgDocument _parsedElement;
	};

} // Elpida

#endif //ELPIDA_SVGPARSETASK_HPP
