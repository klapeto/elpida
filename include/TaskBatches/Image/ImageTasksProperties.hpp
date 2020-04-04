/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2018  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

#ifndef IMAGETASKSPROPERIES_HPP
#define IMAGETASKSPROPERIES_HPP

#include <string>
#include <vector>

#include "TaskBatches/QtTaskBatchWrapper.hpp"
#include "ImageTaskBatch.hpp"

namespace Ui
{
	class ImageTasksProperties;
}
namespace Elpida
{

	class ImageTasksProperties final : public QtTaskBatchWrapper
	{
	Q_OBJECT
	public:

		const TaskBatch& getTaskBatch() const
		{
			return _taskBatch;
		}

		void reconfigureTaskBatch() override;
		void validateConfiguration() override;

		ImageTasksProperties();
		~ImageTasksProperties();

	private slots:
		void on_pbSelectInput_clicked();
		void on_pbSelectOutput_clicked();
		void on_chkOutputImage_stateChanged(int state);

	private:
		std::vector<QMetaObject::Connection> _connections;
		ImageTaskBatch _taskBatch;
		std::string _inputImage;
		std::string _outputImage;
		Ui::ImageTasksProperties* _ui;
		bool _outputEnabled;
	};
}  // namespace Elpida

#endif // IMAGETASKSPROPERIES_HPP
