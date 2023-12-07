//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGTRANSFORM_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGTRANSFORM_HPP

#include <cmath>

namespace Elpida
{

	class SvgTransform final
	{
	public:
		void AsIdentity()
		{
			t[0] = 1.0f;
			t[1] = 0.0f;
			t[2] = 0.0f;
			t[3] = 1.0f;
			t[4] = 0.0f;
			t[5] = 0.0f;
		}

		void SetTranslation(float tx, float ty)
		{
			t[0] = 1.0f;
			t[1] = 0.0f;
			t[2] = 0.0f;
			t[3] = 1.0f;
			t[4] = tx;
			t[5] = ty;
		}

		void SetScale(float sx, float sy)
		{
			t[0] = sx;
			t[1] = 0.0f;
			t[2] = 0.0f;
			t[3] = sy;
			t[4] = 0.0f;
			t[5] = 0.0f;
		}

		void SetSkewX(float a)
		{
			t[0] = 1.0f;
			t[1] = 0.0f;
			t[2] = tanf(a);
			t[3] = 1.0f;
			t[4] = 0.0f;
			t[5] = 0.0f;
		}

		void SetSkewY(float a)
		{
			t[0] = 1.0f;
			t[1] = tanf(a);
			t[2] = 0.0f;
			t[3] = 1.0f;
			t[4] = 0.0f;
			t[5] = 0.0f;
		}

		void SetRotation(float a)
		{
			float cs = cosf(a);
			float sn = sinf(a);
			t[0] = cs;
			t[1] = sn;
			t[2] = -sn;
			t[3] = cs;
			t[4] = 0.0f;
			t[5] = 0.0f;
		}

		void Multiply(const SvgTransform& other)
		{
			float t0 = t[0] * other.t[0] + t[1] * other.t[2];
			float t2 = t[2] * other.t[0] + t[3] * other.t[2];
			float t4 = t[4] * other.t[0] + t[5] * other.t[2] + other.t[4];
			t[1] = t[0] * other.t[1] + t[1] * other.t[3];
			t[3] = t[2] * other.t[1] + t[3] * other.t[3];
			t[5] = t[4] * other.t[1] + t[5] * other.t[3] + other.t[5];
			t[0] = t0;
			t[2] = t2;
			t[4] = t4;
		}

		void Inverse(SvgTransform& other)
		{
			double invdet, det = (double)t[0] * t[3] - (double)t[2] * t[1];
			if (det > -1e-6 && det < 1e-6)
			{
				AsIdentity();
				return;
			}
			invdet = 1.0 / det;
			other.t[0] = (float)(t[3] * invdet);
			other.t[2] = (float)(-t[2] * invdet);
			other.t[4] = (float)(((double)t[2] * t[5] - (double)t[3] * t[4]) * invdet);
			other.t[1] = (float)(-t[1] * invdet);
			other.t[3] = (float)(t[0] * invdet);
			other.t[5] = (float)(((double)t[1] * t[4] - (double)t[0] * t[5]) * invdet);
		}

		void PreMultiply(const SvgTransform& other)
		{
			auto copy = other;
			copy.Multiply(*this);
			*this = copy;
		}

		SvgTransform() = default;
		SvgTransform(const SvgTransform&) = default;
		SvgTransform& operator=(const SvgTransform&) = default;
		~SvgTransform() = default;
	private:
		float t[6];
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_SVG_SVGTRANSFORM_HPP
