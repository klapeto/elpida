//
// Created by klapeto on 4/8/2024.
//

#ifndef ELPIDA_HIDDEN_H
#define ELPIDA_HIDDEN_H

#if defined _WIN32 || defined __CYGWIN__
	#define ELPIDA_IMPORT __declspec(dllimport)
	#define ELPIDA_EXPORT __declspec(dllexport)
	#define ELPIDA_HIDDEN
	#define ELPIDA_WEAK_ALIAS(old, new)
#else
	#if __GNUC__ >= 4
		#define ELPIDA_IMPORT __attribute__ ((visibility ("default")))
		#define ELPIDA_EXPORT __attribute__ ((visibility ("default")))
		#define ELPIDA_HIDDEN __attribute__ ((visibility ("hidden")))
		#define ELPIDA_WEAK_ALIAS(old, new) \
			extern __typeof(old) new __attribute__((__weak__, __alias__(#old)))
	#else
		#define ELPIDA_IMPORT
		#define ELPIDA_EXPORT
		#define ELPIDA_HIDDEN
		#define ELPIDA_WEAK_ALIAS(old, new)
	#endif
#endif

#define hidden ELPIDA_HIDDEN
#define weak_alias(old, new) ELPIDA_WEAK_ALIAS(old, new)

#endif //ELPIDA_HIDDEN_H
