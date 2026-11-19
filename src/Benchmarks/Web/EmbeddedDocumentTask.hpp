/*
 *  Copyright (c) 2026  Ioannis Panagiotopoulos
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

#ifndef ELPIDA_EMBEDDEDDOCUMENTTASK_HPP
#define ELPIDA_EMBEDDEDDOCUMENTTASK_HPP

#include "Elpida/Core/Task.hpp"

namespace Elpida::Benchmarks::Web
{
	// The real-world inputs of the web benchmarks, embedded at build time from Assets/ (see Assets/README.md)
	extern const unsigned char JsonBenchmarkDocument[];
	extern const std::size_t JsonBenchmarkDocument_SIZE;
	extern const unsigned char XmlBenchmarkDocument[];
	extern const std::size_t XmlBenchmarkDocument_SIZE;
	extern const unsigned char RegexBenchmarkText[];
	extern const std::size_t RegexBenchmarkText_SIZE;

	/// Copies a built-in document into the task data, as the input of the next task.
	class EmbeddedDocumentTask : public Task
	{
	public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;
		SharedPtr<AbstractTaskData> Finalize() override;
		Size GetProcessedDataSize() const override;

		EmbeddedDocumentTask(const unsigned char* data, std::size_t size);
		~EmbeddedDocumentTask() override = default;
	protected:
		TaskInfo DoGetInfo() const override;
		void DoRun() override;
		UniquePtr<Task> DoDuplicate() const override;
	private:
		std::shared_ptr<AbstractTaskData> _outputData;
		const unsigned char* _data;
		std::size_t _size;
	};

} // Elpida

#endif //ELPIDA_EMBEDDEDDOCUMENTTASK_HPP
