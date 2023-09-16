//
// Created by klapeto on 10/9/2023.
//

#ifndef ELPIDA_INCLUDE_ELPIDA_CORE_REPEAT_HPP
#define ELPIDA_INCLUDE_ELPIDA_CORE_REPEAT_HPP


#define REPEAT_5(x) x;x;x;x;x
#define REPEAT_10(x) REPEAT_5(x);REPEAT_5(x)
#define REPEAT_50(x) REPEAT_10(x);REPEAT_10(x);REPEAT_10(x);REPEAT_10(x);REPEAT_10(x)
#define REPEAT_100(x) REPEAT_50(x);REPEAT_50(x)
#define REPEAT_500(x) REPEAT_100(x);REPEAT_100(x);REPEAT_100(x);REPEAT_100(x);REPEAT_100(x)
#define REPEAT_1000(x) REPEAT_500(x);REPEAT_500(x)
#define REPEAT_5000(x) REPEAT_1000(x);REPEAT_1000(x);REPEAT_1000(x);REPEAT_1000(x);REPEAT_1000(x)
#define REPEAT_10000(x) REPEAT_5000(x);REPEAT_5000(x)

#endif //ELPIDA_INCLUDE_ELPIDA_CORE_REPEAT_HPP