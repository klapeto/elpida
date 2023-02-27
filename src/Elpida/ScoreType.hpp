//
// Created by klapeto on 27/2/2023.
//

#ifndef ELPIDA_SCORETYPE_HPP
#define ELPIDA_SCORETYPE_HPP

namespace Elpida {

	/**
	 * @brief Defines what kind a score is.
	 */
	enum class ScoreType {
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
#endif //ELPIDA_SCORETYPE_HPP
