//
// Created by klapeto on 17/4/2023.
//

#ifndef ELPIDA_CONTROLLER_HPP_
#define ELPIDA_CONTROLLER_HPP_

namespace Elpida::Application
{

	template<typename T>
	class Controller
	{
	 public:
		explicit Controller(T& model)
			: _model(model)
		{

		}
		Controller(const Controller&) = delete;
		Controller(Controller&&) noexcept = default;
		Controller& operator=(const Controller&) = delete;
		Controller& operator=(Controller&&) noexcept = default;
		~Controller() = default;
	 protected:
		T& _model;
	};

} // Application

#endif //ELPIDA_CONTROLLER_HPP_
