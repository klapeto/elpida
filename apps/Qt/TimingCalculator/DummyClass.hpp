//
// Created by klapeto on 16/9/2023.
//

#ifndef ELPIDA_APPS_QT_OVERHEADSCALCULATOR_DUMMYCLASS_HPP
#define ELPIDA_APPS_QT_OVERHEADSCALCULATOR_DUMMYCLASS_HPP

namespace Elpida::Application
{
	class Base
	{
	public:
		virtual void Foo() = 0;
		virtual ~Base() = default;
	};

	class Derived : public Base
	{
	public:
		void Foo() override;
		Derived() = default;
		~Derived() override = default;
	};

} // Application

#endif //ELPIDA_APPS_QT_OVERHEADSCALCULATOR_DUMMYCLASS_HPP
