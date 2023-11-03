//
// Created by klapeto on 3/11/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGTRANSFORM_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGTRANSFORM_HPP

#include <cmath>

namespace Elpida
{

	class SvgTransform
	{
	public:

		void SetIdentity()
		{
			t[0] = 1.0f; t[1] = 0.0f;
			t[2] = 0.0f; t[3] = 1.0f;
			t[4] = 0.0f; t[5] = 0.0f;
		}

		void SetTranslation(float tx, float ty)
		{
			t[0] = 1.0f; t[1] = 0.0f;
			t[2] = 0.0f; t[3] = 1.0f;
			t[4] = tx;   t[5] = ty;
		}

		void SetScale(float sx, float sy)
		{
			t[0] = sx;   t[1] = 0.0f;
			t[2] = 0.0f; t[3] = sy;
			t[4] = 0.0f; t[5] = 0.0f;
		}

		void SetSkewX(float angle)
		{
			t[0] = 1.0f; t[1] = 0.0f;
			t[2] = tanf(angle); t[3] = 1.0f;
			t[4] = 0.0f; t[5] = 0.0f;
		}

		void SetSkewY(float angle)
		{
			t[0] = 1.0f; t[1] = tanf(angle);
			t[2] = 0.0f; t[3] = 1.0f;
			t[4] = 0.0f; t[5] = 0.0f;
		}

		void SetRotation(float angle)
		{
			float cs = cosf(angle), sn = sinf(angle);
			t[0] = cs; t[1] = sn;
			t[2] = -sn; t[3] = cs;
			t[4] = 0.0f; t[5] = 0.0f;
		}

		void Multiply(const SvgTransform& other)
		{
			auto s = other.t;
			float t0 = t[0] * s[0] + t[1] * s[2];
			float t2 = t[2] * s[0] + t[3] * s[2];
			float t4 = t[4] * s[0] + t[5] * s[2] + s[4];
			t[1] = t[0] * s[1] + t[1] * s[3];
			t[3] = t[2] * s[1] + t[3] * s[3];
			t[5] = t[4] * s[1] + t[5] * s[3] + s[5];
			t[0] = t0;
			t[2] = t2;
			t[4] = t4;
		}

		void Inverse(SvgTransform& other)
		{
			auto det = (double)t[0] * t[3] - (double)t[2] * t[1];
			if (det > -1e-6 && det < 1e-6) {
				SetIdentity();
				return;
			}
			auto inv = other.t;
			auto invdet = 1.0 / det;
			inv[0] = (float)(t[3] * invdet);
			inv[2] = (float)(-t[2] * invdet);
			inv[4] = (float)(((double)t[2] * t[5] - (double)t[3] * t[4]) * invdet);
			inv[1] = (float)(-t[1] * invdet);
			inv[3] = (float)(t[0] * invdet);
			inv[5] = (float)(((double)t[1] * t[4] - (double)t[0] * t[5]) * invdet);
		}


		void Premultiply(const SvgTransform& other)
		{
			auto copy = other;
			copy.Multiply(*this);
			*this = copy;
		}

		constexpr SvgTransform()
			: t{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f}
		{

		}

		constexpr SvgTransform(float a, float b, float c, float d, float e, float f)
			: t{a,b,c,d,e,f}
		{

		}
	private:
		float t[6];
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGTRANSFORM_HPP
