//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SVG_SVGTRANSFORM_HPP
#define ELPIDA_SVG_SVGTRANSFORM_HPP

#include <cmath>
#include <string_view>

namespace Elpida
{

	class SvgTransform final
	{
	public:

		double operator[](const std::size_t i) const
		{
			return t[i];
		}

		void AsIdentity()
		{
			t[0] = 1.0f;
			t[1] = 0.0f;
			t[2] = 0.0f;
			t[3] = 1.0f;
			t[4] = 0.0f;
			t[5] = 0.0f;
		}

		void SetTranslation(const double tx, const double ty)
		{
			t[0] = 1.0f;
			t[1] = 0.0f;
			t[2] = 0.0f;
			t[3] = 1.0f;
			t[4] = tx;
			t[5] = ty;
		}

		void SetScale(const double sx, const double sy)
		{
			t[0] = sx;
			t[1] = 0.0f;
			t[2] = 0.0f;
			t[3] = sy;
			t[4] = 0.0f;
			t[5] = 0.0f;
		}

		void SetSkewX(const double a)
		{
			t[0] = 1.0f;
			t[1] = 0.0f;
			t[2] = tan(a);
			t[3] = 1.0f;
			t[4] = 0.0f;
			t[5] = 0.0f;
		}

		void SetSkewY(const double a)
		{
			t[0] = 1.0f;
			t[1] = tan(a);
			t[2] = 0.0f;
			t[3] = 1.0f;
			t[4] = 0.0f;
			t[5] = 0.0f;
		}

		void SetRotation(const double a)
		{
			const double cs = cos(a);
			const double sn = sin(a);
			t[0] = cs;
			t[1] = sn;
			t[2] = -sn;
			t[3] = cs;
			t[4] = 0.0f;
			t[5] = 0.0f;
		}

		void Multiply(const SvgTransform& other)
		{
			const double t0 = t[0] * other.t[0] + t[1] * other.t[2];
			const double t2 = t[2] * other.t[0] + t[3] * other.t[2];
			const double t4 = t[4] * other.t[0] + t[5] * other.t[2] + other.t[4];
			t[1] = t[0] * other.t[1] + t[1] * other.t[3];
			t[3] = t[2] * other.t[1] + t[3] * other.t[3];
			t[5] = t[4] * other.t[1] + t[5] * other.t[3] + other.t[5];
			t[0] = t0;
			t[2] = t2;
			t[4] = t4;
		}

		void Inverse(const SvgTransform& other)
		{
			const double det = other.t[0] * other.t[3] - other.t[2] * other.t[1];
			if (det > -1e-6 && det < 1e-6)
			{
				AsIdentity();
				return;
			}
			const double invdet = 1.0 / det;
			t[0] = other.t[3] * invdet;
			t[2] = -other.t[2] * invdet;
			t[4] = (other.t[2] * other.t[5] - other.t[3] * other.t[4]) * invdet;
			t[1] = -other.t[1] * invdet;
			t[3] = other.t[0] * invdet;
			t[5] = (other.t[1] * other.t[4] - other.t[0] * other.t[5]) * invdet;
		}

		void PreMultiply(const SvgTransform& other)
		{
			auto copy = other;
			copy.Multiply(*this);
			*this = copy;
		}

		void ApplyToPoint(double& dx, double& dy, const double x, const double y) const
		{
			dx = x * t[0] + y * t[2] + t[4];
			dy = x * t[1] + y * t[3] + t[5];
		}

		void ApplyToVector(double& dx, double& dy, const double x, const double y) const
		{
			dx = x * t[0] + y * t[2];
			dy = x * t[1] + y * t[3];
		}

		SvgTransform()
			: t{ 1.0, 0.0, 0.0, 1.0, 0.0, 0.0 }
		{

		}

		explicit SvgTransform(double values[6])
			: t{ values[0], values[1], values[2], values[3], values[4], values[5] }
		{

		}

		explicit SvgTransform(const double a, const double b, const double c, const double d, const double e,
		                      const double f)
			: t{a, b, c, d, e, f}
		{
		}

		explicit SvgTransform(std::string_view view);

		SvgTransform(const SvgTransform&) = default;
		SvgTransform& operator=(const SvgTransform&) = default;
		~SvgTransform() = default;
	private:
		double t[6];
	};

} // Elpida

#endif //ELPIDA_SVG_SVGTRANSFORM_HPP
