//
// Created by klapeto on 8/4/2023.
//

#ifndef ELPIDA_OSINFOMODEL_HPP_
#define ELPIDA_OSINFOMODEL_HPP_

#include <string>

#include "Models/Abstractions/Model.hpp"

namespace Elpida::Application
{

	class OsInfoModel: public Model
	{
	 public:

		[[nodiscard]]
		const std::string& GetCategory() const;

		[[nodiscard]]
		const std::string& GetName() const;

		[[nodiscard]]
		const std::string& GetVersion() const;

		OsInfoModel(std::string category, std::string name, std::string version);
		OsInfoModel(const OsInfoModel&) = delete;
		OsInfoModel(OsInfoModel&&) = delete;
		OsInfoModel& operator=(const OsInfoModel&) = delete;
		OsInfoModel& operator=(OsInfoModel&&) = delete;
		~OsInfoModel() override = default;
	 private:
		std::string _category;
		std::string _name;
		std::string _version;
	};

} // Application

#endif //ELPIDA_OSINFOMODEL_HPP_
