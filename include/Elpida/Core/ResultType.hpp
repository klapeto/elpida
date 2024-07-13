//
// Created by klapeto on 27/2/2023.
//

#ifndef ELPIDA_RESULTTYPE_HPP
#define ELPIDA_RESULTTYPE_HPP

namespace Elpida
{

	/**
	 * @brief Defines what kind a score is.
	 */
	enum class ResultType
	{
		/**
		 * @brief The score it measured in units/s.
		 */
		Throughput,

		/**
		 * @brief The score is execution time.
		 */
		Time,

		/**
		 * @brief The score is implementation defined.
		 */
		Custom
	};
}
#endif //ELPIDA_RESULTTYPE_HPP
