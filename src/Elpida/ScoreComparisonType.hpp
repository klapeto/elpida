//
// Created by klapeto on 27/2/2023.
//

#ifndef ELPIDA_SCORECOMPARISONTYPE_HPP
#define ELPIDA_SCORECOMPARISONTYPE_HPP

namespace Elpida {
	/**
	 * @brief Determines how a score should be determined better or worse than another.
	 */
	enum class ScoreComparisonType {
		/**
		 * @brief The lower the score the better.
		 */
		LowerIsBetter,

		/**
		 * @brief The greater the score the better.
		 */
		GreaterIsBetter
	};
}

#endif //ELPIDA_SCORECOMPARISONTYPE_HPP
