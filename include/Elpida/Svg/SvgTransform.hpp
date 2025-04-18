//
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SVG_SVGTRANSFORM_HPP
#define ELPIDA_SVG_SVGTRANSFORM_HPP

#include <cmath>
#include <string_view>
#include <Elpida/Svg/SvgUtilities.hpp>
#include <Elpida/Svg/SvgConfig.hpp>

namespace Elpida
{
	class SvgTransform final
	{
	public:
		/*
		 * 	A C E
		 *  B D F
		 *  0 0 1
		 */

		enum Matrix
		{
			A, B, C, D, E, F
		};

		bool IsIdentity() const
		{
			return t[A] == 1.0
			       && t[B] == 0.0
			       && t[C] == 0.0
			       && t[D] == 1.0
			       && t[E] == 0.0
			       && t[F] == 0.0;
		}

		SvgTransform &Translate(const SvgFloat tx, const SvgFloat ty)
		{
			SvgTransform transform;
			transform.SetTranslation(tx, ty);
			PreMultiply(transform);
			return *this;
		}

		SvgTransform &Scale(const SvgFloat sx, const SvgFloat sy)
		{
			SvgTransform transform;
			transform.SetScale(sx, sy);
			PreMultiply(transform);
			return *this;
		}

		SvgTransform &RotateRadians(const SvgFloat radians)
		{
			SvgTransform transform;
			transform.SetRotation(radians);
			PreMultiply(transform);
			return *this;
		}

		SvgTransform &RotateDegrees(const SvgFloat degrees)
		{
			return RotateRadians(degrees / SvgFloat(180.0) * SvgUtilities::Pi);
		}

		SvgTransform &SkewX(const SvgFloat angle)
		{
			SvgTransform transform;
			transform.SetSkewX(angle);
			PreMultiply(transform);
			return *this;
		}

		SvgTransform &SkewY(const SvgFloat angle)
		{
			SvgTransform transform;
			transform.SetSkewY(angle);
			PreMultiply(transform);
			return *this;
		}

		SvgFloat operator[](const std::size_t i) const
		{
			return t[i];
		}

		void AsIdentity()
		{
			t[A] = 1.0;
			t[B] = 0.0;
			t[C] = 0.0;
			t[D] = 1.0;
			t[E] = 0.0;
			t[F] = 0.0;
		}

		void SetTranslation(const SvgFloat tx, const SvgFloat ty)
		{
			t[A] = 1.0;
			t[B] = 0.0;
			t[C] = 0.0;
			t[D] = 1.0;
			t[E] = tx;
			t[F] = ty;
		}

		void SetScale(const SvgFloat sx, const SvgFloat sy)
		{
			t[A] = sx;
			t[B] = 0.0;
			t[C] = 0.0;
			t[D] = sy;
			t[E] = 0.0;
			t[F] = 0.0;
		}

		void SetSkewX(const SvgFloat a)
		{
			t[A] = 1.0;
			t[B] = 0.0;
			t[C] = std::tan(a);
			t[D] = 1.0;
			t[E] = 0.0;
			t[F] = 0.0;
		}

		void SetSkewY(const SvgFloat a)
		{
			t[A] = 1.0;
			t[B] = std::tan(a);
			t[C] = 0.0;
			t[D] = 1.0;
			t[E] = 0.0;
			t[F] = 0.0;
		}

		void SetRotation(const SvgFloat a)
		{
			const SvgFloat cs = std::cos(a);
			const SvgFloat sn = std::sin(a);

			t[A] = cs;
			t[B] = sn;
			t[C] = -sn;
			t[D] = cs;
			t[E] = 0.0;
			t[F] = 0.0;
		}

		void Multiply(const SvgTransform &other)
		{
			const auto &o = other.t;

			const auto a = t[A] * o[A] + t[C] * o[B];
			const auto c = t[A] * o[C] + t[C] * o[D];
			const auto e = t[A] * o[E] + t[C] * o[F] + t[E];
			const auto b = t[B] * o[A] + t[D] * o[B];
			const auto d = t[B] * o[C] + t[D] * o[D];
			const auto f = t[B] * o[E] + t[D] * o[F] + t[F];

			t[A] = a;
			t[B] = b;
			t[C] = c;
			t[D] = d;
			t[E] = e;
			t[F] = f;
		}

		void PreMultiply(const SvgTransform &other)
		{
			// TODO: non copy version
			auto copy = other;
			copy.Multiply(*this);
			*this = copy;
		}

		void Inverse(const SvgTransform &other)
		{
			const SvgFloat det = other.t[0] * other.t[3] - other.t[2] * other.t[1];
			if (det > -1e-6 && det < 1e-6)
			{
				AsIdentity();
				return;
			}
			const SvgFloat invdet = SvgFloat(1.0) / det;
			t[0] = other.t[3] * invdet;
			t[2] = -other.t[2] * invdet;
			t[4] = (other.t[2] * other.t[5] - other.t[3] * other.t[4]) * invdet;
			t[1] = -other.t[1] * invdet;
			t[3] = other.t[0] * invdet;
			t[5] = (other.t[1] * other.t[4] - other.t[0] * other.t[5]) * invdet;
		}

		void ApplyToPoint(SvgFloat &x, SvgFloat &y) const
		{
			const auto tx = t[A] * x + t[C] * y + t[E];
			const auto ty = t[B] * x + t[D] * y + t[F];
			x = tx;
			y = ty;
		}

		void ApplyToPoint(SvgFloat &dx, SvgFloat &dy, const SvgFloat x, const SvgFloat y) const
		{
			dx = x * t[0] + y * t[2] + t[4];
			dy = x * t[1] + y * t[3] + t[5];
		}

		void ApplyToVector(SvgFloat &dx, SvgFloat &dy, const SvgFloat x, const SvgFloat y) const
		{
			dx = x * t[0] + y * t[2];
			dy = x * t[1] + y * t[3];
		}

		SvgTransform()
			: t{1.0, 0.0, 0.0, 1.0, 0.0, 0.0}
		{
		}

		explicit SvgTransform(SvgFloat values[6])
			: t{values[0], values[1], values[2], values[3], values[4], values[5]}
		{
		}

		explicit SvgTransform(const SvgFloat a, const SvgFloat b, const SvgFloat c, const SvgFloat d, const SvgFloat e,
		                      const SvgFloat f)
			: t{a, b, c, d, e, f}
		{
		}

		explicit SvgTransform(std::string_view view);

		SvgTransform(const SvgTransform &) = default;
		SvgTransform &operator=(const SvgTransform &) = default;
		~SvgTransform() = default;

	private:
		SvgFloat t[6];
	};
} // Elpida

#endif //ELPIDA_SVG_SVGTRANSFORM_HPP
