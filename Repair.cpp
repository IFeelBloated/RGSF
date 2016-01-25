/*****************************************************************************

AvsFilterRemoveGrain/Repair16
Author: Laurent de Soras, 2012
Modified for VapourSynth by Fredrik Mellbin 2013

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/

#include "shared.h"

class ConvSigned
{
};

class ConvUnsigned
{
};

#define AvsFilterRepair16_SORT_AXIS_CPP \
    const float      ma1 = std::max(a1, a8);   \
    const float      mi1 = std::min(a1, a8);   \
    const float      ma2 = std::max(a2, a7);   \
    const float      mi2 = std::min(a2, a7);   \
    const float      ma3 = std::max(a3, a6);   \
    const float      mi3 = std::min(a3, a6);   \
    const float      ma4 = std::max(a4, a5);   \
    const float      mi4 = std::min(a4, a5);

class OpRG01
{
public:
	typedef    ConvSigned    ConvSign;
	static __forceinline float rg(float cr, float a1, float a2, float a3, float a4, float c, float a5, float a6, float a7, float a8, const int chroma) {
		const float        mi = std::min(std::min(
			std::min(std::min(a1, a2), std::min(a3, a4)),
			std::min(std::min(a5, a6), std::min(a7, a8))
			), c);
		const float        ma = std::max(std::max(
			std::max(std::max(a1, a2), std::max(a3, a4)),
			std::max(std::max(a5, a6), std::max(a7, a8))
			), c);

		return (limit(cr, mi, ma));
	}
};

class OpRG02
{
public:
	typedef    ConvSigned    ConvSign;
	static __forceinline float rg(float cr, float a1, float a2, float a3, float a4, float c, float a5, float a6, float a7, float a8, const int chroma) {
		float                a[9] = { a1, a2, a3, a4, c, a5, a6, a7, a8 };

		std::sort(&a[0], (&a[8]) + 1);

		return (limit(cr, a[2 - 1], a[7]));
	}
};

class OpRG03
{
public:
	typedef    ConvSigned    ConvSign;
	static __forceinline float rg(float cr, float a1, float a2, float a3, float a4, float c, float a5, float a6, float a7, float a8, const int chroma) {
		float                a[9] = { a1, a2, a3, a4, c, a5, a6, a7, a8 };

		std::sort(&a[0], (&a[8]) + 1);

		return (limit(cr, a[3 - 1], a[6]));
	}
};

class OpRG04
{
public:
	typedef    ConvSigned    ConvSign;
	static __forceinline float rg(float cr, float a1, float a2, float a3, float a4, float c, float a5, float a6, float a7, float a8, const int chroma) {
		float                a[9] = { a1, a2, a3, a4, c, a5, a6, a7, a8 };

		std::sort(&a[0], (&a[8]) + 1);

		return (limit(cr, a[4 - 1], a[5]));
	}
};

class OpRG05
{
public:
	typedef    ConvSigned    ConvSign;
	static __forceinline float rg(float cr, float a1, float a2, float a3, float a4, float c, float a5, float a6, float a7, float a8, const int chroma) {
		const float mal1 = std::max(std::max(a1, a8), c);
		const float mil1 = std::min(std::min(a1, a8), c);

		const float mal2 = std::max(std::max(a2, a7), c);
		const float mil2 = std::min(std::min(a2, a7), c);

		const float mal3 = std::max(std::max(a3, a6), c);
		const float mil3 = std::min(std::min(a3, a6), c);

		const float mal4 = std::max(std::max(a4, a5), c);
		const float mil4 = std::min(std::min(a4, a5), c);

		const float clipped1 = limit(cr, mil1, mal1);
		const float clipped2 = limit(cr, mil2, mal2);
		const float clipped3 = limit(cr, mil3, mal3);
		const float clipped4 = limit(cr, mil4, mal4);

		const double c1 = std::abs(static_cast<double>(cr) - clipped1);
		const double c2 = std::abs(static_cast<double>(cr) - clipped2);
		const double c3 = std::abs(static_cast<double>(cr) - clipped3);
		const double c4 = std::abs(static_cast<double>(cr) - clipped4);

		const double mindiff = std::min(std::min(c1, c2), std::min(c3, c4));

		if (mindiff == c4)
			return clipped4;
		else if (mindiff == c2)
			return clipped2;
		else if (mindiff == c3)
			return clipped3;
		else
			return clipped1;
	}
};

class OpRG06
{
public:
	typedef    ConvSigned    ConvSign;
	static __forceinline float rg(float cr, float a1, float a2, float a3, float a4, float c, float a5, float a6, float a7, float a8, const int chroma) {
		double maximum, minimum;
		if (chroma) {
			maximum = 0.5;
			minimum = -0.5;
		}
		else {
			maximum = 1.;
			minimum = 0.;
		}

		const float mal1 = std::max(std::max(a1, a8), c);
		const float mil1 = std::min(std::min(a1, a8), c);

		const float mal2 = std::max(std::max(a2, a7), c);
		const float mil2 = std::min(std::min(a2, a7), c);

		const float mal3 = std::max(std::max(a3, a6), c);
		const float mil3 = std::min(std::min(a3, a6), c);

		const float mal4 = std::max(std::max(a4, a5), c);
		const float mil4 = std::min(std::min(a4, a5), c);

		const double d1 = static_cast<double>(mal1) - mil1;
		const double d2 = static_cast<double>(mal2) - mil2;
		const double d3 = static_cast<double>(mal3) - mil3;
		const double d4 = static_cast<double>(mal4) - mil4;

		const float clipped1 = limit(cr, mil1, mal1);
		const float clipped2 = limit(cr, mil2, mal2);
		const float clipped3 = limit(cr, mil3, mal3);
		const float clipped4 = limit(cr, mil4, mal4);

		const double c1 = limit((std::abs(static_cast<double>(cr) - clipped1) * 2.) + d1, minimum, maximum);
		const double c2 = limit((std::abs(static_cast<double>(cr) - clipped2) * 2.) + d2, minimum, maximum);
		const double c3 = limit((std::abs(static_cast<double>(cr) - clipped3) * 2.) + d3, minimum, maximum);
		const double c4 = limit((std::abs(static_cast<double>(cr) - clipped4) * 2.) + d4, minimum, maximum);

		const double mindiff = std::min(std::min(c1, c2), std::min(c3, c4));

		if (mindiff == c4)
			return clipped4;
		else if (mindiff == c2)
			return clipped2;
		else if (mindiff == c3)
			return clipped3;
		else
			return clipped1;
	}
};

class OpRG07
{
public:
	typedef    ConvSigned    ConvSign;
	static __forceinline float rg(float cr, float a1, float a2, float a3, float a4, float c, float a5, float a6, float a7, float a8, const int chroma) {
		const float mal1 = std::max(std::max(a1, a8), c);
		const float mil1 = std::min(std::min(a1, a8), c);

		const float mal2 = std::max(std::max(a2, a7), c);
		const float mil2 = std::min(std::min(a2, a7), c);

		const float mal3 = std::max(std::max(a3, a6), c);
		const float mil3 = std::min(std::min(a3, a6), c);

		const float mal4 = std::max(std::max(a4, a5), c);
		const float mil4 = std::min(std::min(a4, a5), c);

		const double d1 = static_cast<double>(mal1) - mil1;
		const double d2 = static_cast<double>(mal2) - mil2;
		const double d3 = static_cast<double>(mal3) - mil3;
		const double d4 = static_cast<double>(mal4) - mil4;

		const float clipped1 = limit(cr, mil1, mal1);
		const float clipped2 = limit(cr, mil2, mal2);
		const float clipped3 = limit(cr, mil3, mal3);
		const float clipped4 = limit(cr, mil4, mal4);

		const double c1 = std::abs(static_cast<double>(cr) - clipped1) + d1;
		const double c2 = std::abs(static_cast<double>(cr) - clipped2) + d2;
		const double c3 = std::abs(static_cast<double>(cr) - clipped3) + d3;
		const double c4 = std::abs(static_cast<double>(cr) - clipped4) + d4;

		const double mindiff = std::min(std::min(c1, c2), std::min(c3, c4));

		if (mindiff == c4)
			return clipped4;
		else if (mindiff == c2)
			return clipped2;
		else if (mindiff == c3)
			return clipped3;
		else
			return clipped1;
	}
};

class OpRG08
{
public:
	typedef    ConvSigned    ConvSign;
	static __forceinline float rg(float cr, float a1, float a2, float a3, float a4, float c, float a5, float a6, float a7, float a8, const int chroma) {
		double maximum, minimum;
		if (chroma) {
			maximum = 0.5;
			minimum = -0.5;
		}
		else {
			maximum = 1.;
			minimum = 0.;
		}

		const float mal1 = std::max(std::max(a1, a8), c);
		const float mil1 = std::min(std::min(a1, a8), c);

		const float mal2 = std::max(std::max(a2, a7), c);
		const float mil2 = std::min(std::min(a2, a7), c);

		const float mal3 = std::max(std::max(a3, a6), c);
		const float mil3 = std::min(std::min(a3, a6), c);

		const float mal4 = std::max(std::max(a4, a5), c);
		const float mil4 = std::min(std::min(a4, a5), c);

		const double d1 = static_cast<double>(mal1) - mil1;
		const double d2 = static_cast<double>(mal2) - mil2;
		const double d3 = static_cast<double>(mal3) - mil3;
		const double d4 = static_cast<double>(mal4) - mil4;

		const float clipped1 = limit(cr, mil1, mal1);
		const float clipped2 = limit(cr, mil2, mal2);
		const float clipped3 = limit(cr, mil3, mal3);
		const float clipped4 = limit(cr, mil4, mal4);

		const double c1 = limit(std::abs(static_cast<double>(cr) - clipped1) + (d1 * 2.), minimum, maximum);
		const double c2 = limit(std::abs(static_cast<double>(cr) - clipped2) + (d2 * 2.), minimum, maximum);
		const double c3 = limit(std::abs(static_cast<double>(cr) - clipped3) + (d3 * 2.), minimum, maximum);
		const double c4 = limit(std::abs(static_cast<double>(cr) - clipped4) + (d4 * 2.), minimum, maximum);

		const double mindiff = std::min(std::min(c1, c2), std::min(c3, c4));

		if (mindiff == c4)
			return clipped4;
		else if (mindiff == c2)
			return clipped2;
		else if (mindiff == c3)
			return clipped3;
		else
			return clipped1;
	}
};

class OpRG09
{
public:
	typedef    ConvSigned    ConvSign;
	static __forceinline float rg(float cr, float a1, float a2, float a3, float a4, float c, float a5, float a6, float a7, float a8, const int chroma) {
		const float mal1 = std::max(std::max(a1, a8), c);
		const float mil1 = std::min(std::min(a1, a8), c);

		const float mal2 = std::max(std::max(a2, a7), c);
		const float mil2 = std::min(std::min(a2, a7), c);

		const float mal3 = std::max(std::max(a3, a6), c);
		const float mil3 = std::min(std::min(a3, a6), c);

		const float mal4 = std::max(std::max(a4, a5), c);
		const float mil4 = std::min(std::min(a4, a5), c);

		const double d1 = static_cast<double>(mal1) - mil1;
		const double d2 = static_cast<double>(mal2) - mil2;
		const double d3 = static_cast<double>(mal3) - mil3;
		const double d4 = static_cast<double>(mal4) - mil4;

		const double mindiff = std::min(std::min(d1, d2), std::min(d3, d4));

		if (mindiff == d4)
			return limit(cr, mil4, mal4);
		else if (mindiff == d2)
			return limit(cr, mil2, mal2);
		else if (mindiff == d3)
			return limit(cr, mil3, mal3);
		else
			return limit(cr, mil1, mal1);
	}
};

class OpRG10
{
public:
	typedef    ConvUnsigned    ConvSign;
	static __forceinline float rg(float cr, float a1, float a2, float a3, float a4, float c, float a5, float a6, float a7, float a8, const int chroma) {
		const double d1 = std::abs(static_cast<double>(cr) - a1);
		const double d2 = std::abs(static_cast<double>(cr) - a2);
		const double d3 = std::abs(static_cast<double>(cr) - a3);
		const double d4 = std::abs(static_cast<double>(cr) - a4);
		const double d5 = std::abs(static_cast<double>(cr) - a5);
		const double d6 = std::abs(static_cast<double>(cr) - a6);
		const double d7 = std::abs(static_cast<double>(cr) - a7);
		const double d8 = std::abs(static_cast<double>(cr) - a8);
		const double dc = std::abs(static_cast<double>(cr) - c);

		const double mindiff = std::min(std::min(std::min(std::min(d1, d2), std::min(d3, d4)), std::min(std::min(d5, d6), std::min(d7, d8))), dc);

		if (mindiff == d7)
			return a7;
		else if (mindiff == d8)
			return a8;
		else if (mindiff == d6)
			return a6;
		else if (mindiff == d2)
			return a2;
		else if (mindiff == d3)
			return a3;
		else if (mindiff == d1)
			return a1;
		else if (mindiff == d5)
			return a5;
		else if (mindiff == dc)
			return c;
		else
			return a4;
	}
};

class OpRG12
{
public:
	typedef    ConvSigned    ConvSign;
	static __forceinline float rg(float cr, float a1, float a2, float a3, float a4, float c, float a5, float a6, float a7, float a8, const int chroma) {
		float                a[8] = { a1, a2, a3, a4, a5, a6, a7, a8 };

		std::sort(&a[0], (&a[0]) + 8);
		const float        mi = std::min(a[2 - 1], c);
		const float        ma = std::max(a[7 - 1], c);

		return limit(cr, mi, ma);
	}
};

class OpRG13
{
public:
	typedef    ConvSigned    ConvSign;
	static __forceinline float rg(float cr, float a1, float a2, float a3, float a4, float c, float a5, float a6, float a7, float a8, const int chroma) {
		float                a[8] = { a1, a2, a3, a4, a5, a6, a7, a8 };

		std::sort(&a[0], (&a[0]) + 8);
		const float        mi = std::min(a[3 - 1], c);
		const float        ma = std::max(a[6 - 1], c);

		return limit(cr, mi, ma);
	}
};

class OpRG14
{
public:
	typedef    ConvSigned    ConvSign;
	static __forceinline float rg(float cr, float a1, float a2, float a3, float a4, float c, float a5, float a6, float a7, float a8, const int chroma) {
		float                a[8] = { a1, a2, a3, a4, a5, a6, a7, a8 };

		std::sort(&a[0], (&a[0]) + 8);
		const float        mi = std::min(a[4 - 1], c);
		const float        ma = std::max(a[5 - 1], c);

		return limit(cr, mi, ma);
	}
};

class OpRG15 {
public:
	typedef    ConvSigned    ConvSign;
	static __forceinline float rg(float cr, float a1, float a2, float a3, float a4, float c, float a5, float a6, float a7, float a8, const int chroma) {
		AvsFilterRepair16_SORT_AXIS_CPP

			const double      c1 = std::abs(static_cast<double>(c) - limit(c, mi1, ma1));
		const double      c2 = std::abs(static_cast<double>(c) - limit(c, mi2, ma2));
		const double      c3 = std::abs(static_cast<double>(c) - limit(c, mi3, ma3));
		const double      c4 = std::abs(static_cast<double>(c) - limit(c, mi4, ma4));

		const double      mindiff = std::min(std::min(c1, c2), std::min(c3, c4));

		float            mi;
		float            ma;
		if (mindiff == c4) {
			mi = mi4;
			ma = ma4;
		}
		else if (mindiff == c2) {
			mi = mi2;
			ma = ma2;
		}
		else if (mindiff == c3) {
			mi = mi3;
			ma = ma3;
		}
		else {
			mi = mi1;
			ma = ma1;
		}

		mi = std::min(mi, c);
		ma = std::max(ma, c);

		return limit(cr, mi, ma);
	}
};

class OpRG16 {
public:
	typedef    ConvSigned    ConvSign;
	static __forceinline float rg(float cr, float a1, float a2, float a3, float a4, float c, float a5, float a6, float a7, float a8, const int chroma) {
		AvsFilterRepair16_SORT_AXIS_CPP

			double maximum, minimum;
		if (chroma) {
			maximum = 0.5;
			minimum = -0.5;
		}
		else {
			maximum = 1.;
			minimum = 0.;
		}

		const double      d1 = static_cast<double>(ma1) - mi1;
		const double      d2 = static_cast<double>(ma2) - mi2;
		const double      d3 = static_cast<double>(ma3) - mi3;
		const double      d4 = static_cast<double>(ma4) - mi4;

		const double      c1 = limit((std::abs(static_cast<double>(c) - limit(c, mi1, ma1)) * 2.) + d1, minimum, maximum);
		const double      c2 = limit((std::abs(static_cast<double>(c) - limit(c, mi2, ma2)) * 2.) + d2, minimum, maximum);
		const double      c3 = limit((std::abs(static_cast<double>(c) - limit(c, mi3, ma3)) * 2.) + d3, minimum, maximum);
		const double      c4 = limit((std::abs(static_cast<double>(c) - limit(c, mi4, ma4)) * 2.) + d4, minimum, maximum);

		const double      mindiff = std::min(std::min(c1, c2), std::min(c3, c4));

		float            mi;
		float            ma;
		if (mindiff == c4) {
			mi = mi4;
			ma = ma4;
		}
		else if (mindiff == c2) {
			mi = mi2;
			ma = ma2;
		}
		else if (mindiff == c3) {
			mi = mi3;
			ma = ma3;
		}
		else {
			mi = mi1;
			ma = ma1;
		}

		mi = std::min(mi, c);
		ma = std::max(ma, c);

		return limit(cr, mi, ma);
	}
};

class OpRG17 {
public:
	typedef    ConvSigned    ConvSign;
	static __forceinline float rg(float cr, float a1, float a2, float a3, float a4, float c, float a5, float a6, float a7, float a8, const int chroma) {
		AvsFilterRepair16_SORT_AXIS_CPP

			const float      l = std::max(std::max(mi1, mi2), std::max(mi3, mi4));
		const float      u = std::min(std::min(ma1, ma2), std::min(ma3, ma4));

		const float      mi = std::min(std::min(l, u), c);
		const float      ma = std::max(std::max(l, u), c);

		return limit(cr, mi, ma);
	}
};

class OpRG18 {
public:
	typedef    ConvSigned    ConvSign;
	static __forceinline float rg(float cr, float a1, float a2, float a3, float a4, float c, float a5, float a6, float a7, float a8, const int chroma) {
		const double      d1 = std::max(std::abs(static_cast<double>(c) - a1), std::abs(static_cast<double>(c) - a8));
		const double      d2 = std::max(std::abs(static_cast<double>(c) - a2), std::abs(static_cast<double>(c) - a7));
		const double      d3 = std::max(std::abs(static_cast<double>(c) - a3), std::abs(static_cast<double>(c) - a6));
		const double      d4 = std::max(std::abs(static_cast<double>(c) - a4), std::abs(static_cast<double>(c) - a5));

		const double      mindiff = std::min(std::min(d1, d2), std::min(d3, d4));

		float            mi;
		float            ma;
		if (mindiff == d4) {
			mi = std::min(a4, a5);
			ma = std::max(a4, a5);
		}
		else if (mindiff == d2) {
			mi = std::min(a2, a7);
			ma = std::max(a2, a7);
		}
		else if (mindiff == d3) {
			mi = std::min(a3, a6);
			ma = std::max(a3, a6);
		}
		else {
			mi = std::min(a1, a8);
			ma = std::max(a1, a8);
		}

		mi = std::min(mi, c);
		ma = std::max(ma, c);

		return limit(cr, mi, ma);
	}
};

class OpRG19 {
public:
	typedef    ConvUnsigned    ConvSign;
	static __forceinline float rg(float cr, float a1, float a2, float a3, float a4, float c, float a5, float a6, float a7, float a8, const int chroma) {
		double maximum, minimum;
		if (chroma) {
			maximum = 0.5;
			minimum = -0.5;
		}
		else {
			maximum = 1.;
			minimum = 0.;
		}

		const double d1 = std::abs(static_cast<double>(c) - a1);
		const double d2 = std::abs(static_cast<double>(c) - a2);
		const double d3 = std::abs(static_cast<double>(c) - a3);
		const double d4 = std::abs(static_cast<double>(c) - a4);
		const double d5 = std::abs(static_cast<double>(c) - a5);
		const double d6 = std::abs(static_cast<double>(c) - a6);
		const double d7 = std::abs(static_cast<double>(c) - a7);
		const double d8 = std::abs(static_cast<double>(c) - a8);

		const double mindiff = std::min(std::min(std::min(d1, d2), std::min(d3, d4)), std::min(std::min(d5, d6), std::min(d7, d8)));

		return static_cast<float>(limit(static_cast<double>(cr), limit(c - mindiff, minimum, maximum), limit(c + mindiff, minimum, maximum)));
	}
};

class OpRG20 {
public:
	typedef    ConvUnsigned    ConvSign;
	static __forceinline float rg(float cr, float a1, float a2, float a3, float a4, float c, float a5, float a6, float a7, float a8, const int chroma) {
		double maximum, minimum;
		if (chroma) {
			maximum = 0.5;
			minimum = -0.5;
		}
		else {
			maximum = 1.;
			minimum = 0.;
		}

		const double d1 = std::abs(static_cast<double>(c) - a1);
		const double d2 = std::abs(static_cast<double>(c) - a2);
		const double d3 = std::abs(static_cast<double>(c) - a3);
		const double d4 = std::abs(static_cast<double>(c) - a4);
		const double d5 = std::abs(static_cast<double>(c) - a5);
		const double d6 = std::abs(static_cast<double>(c) - a6);
		const double d7 = std::abs(static_cast<double>(c) - a7);
		const double d8 = std::abs(static_cast<double>(c) - a8);

		double mindiff = std::min(d1, d2);
		double maxdiff = std::max(d1, d2);

		maxdiff = limit(maxdiff, mindiff, d3);
		mindiff = std::min(mindiff, d3);

		maxdiff = limit(maxdiff, mindiff, d4);
		mindiff = std::min(mindiff, d4);

		maxdiff = limit(maxdiff, mindiff, d5);
		mindiff = std::min(mindiff, d5);

		maxdiff = limit(maxdiff, mindiff, d6);
		mindiff = std::min(mindiff, d6);

		maxdiff = limit(maxdiff, mindiff, d7);
		mindiff = std::min(mindiff, d7);

		maxdiff = limit(maxdiff, mindiff, d8);

		return static_cast<float>(limit(static_cast<double>(cr), limit(c - maxdiff, minimum, maximum), limit(c + maxdiff, minimum, maximum)));
	}
};

class OpRG21 {
public:
	typedef    ConvSigned    ConvSign;
	static __forceinline float rg(float cr, float a1, float a2, float a3, float a4, float c, float a5, float a6, float a7, float a8, const int chroma) {
		AvsFilterRepair16_SORT_AXIS_CPP

			double maximum, minimum;
		if (chroma) {
			maximum = 0.5;
			minimum = -0.5;
		}
		else {
			maximum = 1.;
			minimum = 0.;
		}
		const double d1 = limit(static_cast<double>(ma1) - c, minimum, maximum);
		const double d2 = limit(static_cast<double>(ma2) - c, minimum, maximum);
		const double d3 = limit(static_cast<double>(ma3) - c, minimum, maximum);
		const double d4 = limit(static_cast<double>(ma4) - c, minimum, maximum);

		const double rd1 = limit(static_cast<double>(c) - mi1, minimum, maximum);
		const double rd2 = limit(static_cast<double>(c) - mi2, minimum, maximum);
		const double rd3 = limit(static_cast<double>(c) - mi3, minimum, maximum);
		const double rd4 = limit(static_cast<double>(c) - mi4, minimum, maximum);

		const double u1 = std::max(d1, rd1);
		const double u2 = std::max(d2, rd2);
		const double u3 = std::max(d3, rd3);
		const double u4 = std::max(d4, rd4);

		const double u = std::min(std::min(u1, u2), std::min(u3, u4));

		return static_cast<float>(limit(static_cast<double>(cr), limit(c - u, minimum, maximum), limit(c + u, minimum, maximum)));
	}
};

class OpRG22 {
public:
	typedef    ConvUnsigned    ConvSign;
	static __forceinline float rg(float cr, float a1, float a2, float a3, float a4, float c, float a5, float a6, float a7, float a8, const int chroma) {
		double maximum, minimum;
		if (chroma) {
			maximum = 0.5;
			minimum = -0.5;
		}
		else {
			maximum = 1.;
			minimum = 0.;
		}

		const double d1 = std::abs(static_cast<double>(cr) - a1);
		const double d2 = std::abs(static_cast<double>(cr) - a2);
		const double d3 = std::abs(static_cast<double>(cr) - a3);
		const double d4 = std::abs(static_cast<double>(cr) - a4);
		const double d5 = std::abs(static_cast<double>(cr) - a5);
		const double d6 = std::abs(static_cast<double>(cr) - a6);
		const double d7 = std::abs(static_cast<double>(cr) - a7);
		const double d8 = std::abs(static_cast<double>(cr) - a8);

		const double mindiff = std::min(std::min(std::min(d1, d2), std::min(d3, d4)), std::min(std::min(d5, d6), std::min(d7, d8)));

		return static_cast<float>(limit(static_cast<double>(c), limit(cr - mindiff, minimum, maximum), limit(cr + mindiff, minimum, maximum)));
	}
};

class OpRG23 {
public:
	typedef    ConvUnsigned    ConvSign;
	static __forceinline float rg(float cr, float a1, float a2, float a3, float a4, float c, float a5, float a6, float a7, float a8, const int chroma) {
		double maximum, minimum;
		if (chroma) {
			maximum = 0.5;
			minimum = -0.5;
		}
		else {
			maximum = 1.;
			minimum = 0.;
		}

		const double d1 = std::abs(static_cast<double>(cr) - a1);
		const double d2 = std::abs(static_cast<double>(cr) - a2);
		const double d3 = std::abs(static_cast<double>(cr) - a3);
		const double d4 = std::abs(static_cast<double>(cr) - a4);
		const double d5 = std::abs(static_cast<double>(cr) - a5);
		const double d6 = std::abs(static_cast<double>(cr) - a6);
		const double d7 = std::abs(static_cast<double>(cr) - a7);
		const double d8 = std::abs(static_cast<double>(cr) - a8);

		double mindiff = std::min(d1, d2);
		double maxdiff = std::max(d1, d2);

		maxdiff = limit(maxdiff, mindiff, d3);
		mindiff = std::min(mindiff, d3);

		maxdiff = limit(maxdiff, mindiff, d4);
		mindiff = std::min(mindiff, d4);

		maxdiff = limit(maxdiff, mindiff, d5);
		mindiff = std::min(mindiff, d5);

		maxdiff = limit(maxdiff, mindiff, d6);
		mindiff = std::min(mindiff, d6);

		maxdiff = limit(maxdiff, mindiff, d7);
		mindiff = std::min(mindiff, d7);

		maxdiff = limit(maxdiff, mindiff, d8);

		return static_cast<float>(limit(static_cast<double>(c), limit(cr - maxdiff, minimum, maximum), limit(cr + maxdiff, minimum, maximum)));
	}
};

class OpRG24 {
public:
	typedef    ConvSigned    ConvSign;
	static __forceinline float rg(float cr, float a1, float a2, float a3, float a4, float c, float a5, float a6, float a7, float a8, const int chroma) {
		AvsFilterRepair16_SORT_AXIS_CPP

			double maximum, minimum;
		if (chroma) {
			maximum = 0.5;
			minimum = -0.5;
		}
		else {
			maximum = 1.;
			minimum = 0.;
		}

		const double d1 = limit(static_cast<double>(ma1) - cr, minimum, maximum);
		const double d2 = limit(static_cast<double>(ma2) - cr, minimum, maximum);
		const double d3 = limit(static_cast<double>(ma3) - cr, minimum, maximum);
		const double d4 = limit(static_cast<double>(ma4) - cr, minimum, maximum);

		const double rd1 = limit(static_cast<double>(cr) - mi1, minimum, maximum);
		const double rd2 = limit(static_cast<double>(cr) - mi2, minimum, maximum);
		const double rd3 = limit(static_cast<double>(cr) - mi3, minimum, maximum);
		const double rd4 = limit(static_cast<double>(cr) - mi4, minimum, maximum);

		const double u1 = std::max(d1, rd1);
		const double u2 = std::max(d2, rd2);
		const double u3 = std::max(d3, rd3);
		const double u4 = std::max(d4, rd4);

		const double u = std::min(std::min(u1, u2), std::min(u3, u4));

		return static_cast<float>(limit(static_cast<double>(c), limit(cr - u, minimum, maximum), limit(cr + u, minimum, maximum)));
	}
};

template <class OP, class T>
class PlaneProc {
public:

	static void process_subplane_cpp(const T *src1_ptr, const T *src2_ptr, T *dst_ptr, int stride, int width, int height, const int chroma)
	{
		const int        y_b = 1;
		const int        y_e = height - 1;

		dst_ptr += y_b * stride;
		src1_ptr += y_b * stride;
		src2_ptr += y_b * stride;

		const int        x_e = width - 1;

		for (int y = y_b; y < y_e; ++y)
		{
			dst_ptr[0] = src1_ptr[0];

			process_row_cpp(
				dst_ptr,
				src1_ptr,
				src2_ptr,
				stride,
				1,
				x_e,
				chroma
				);

			dst_ptr[x_e] = src1_ptr[x_e];

			dst_ptr += stride;
			src1_ptr += stride;
			src2_ptr += stride;
		}
	}

	static void process_row_cpp(T *dst_ptr, const T *src1_ptr, const T *src2_ptr, int stride_src, int x_beg, int x_end, const int chroma)
	{
		const int      om = stride_src - 1;
		const int      o0 = stride_src;
		const int      op = stride_src + 1;

		src1_ptr += x_beg;
		src2_ptr += x_beg;

		for (int x = x_beg; x < x_end; ++x)
		{
			const float        cr = src1_ptr[0];
			const float        a1 = src2_ptr[-op];
			const float        a2 = src2_ptr[-o0];
			const float        a3 = src2_ptr[-om];
			const float        a4 = src2_ptr[-1];
			const float        c = src2_ptr[0];
			const float        a5 = src2_ptr[1];
			const float        a6 = src2_ptr[om];
			const float        a7 = src2_ptr[o0];
			const float        a8 = src2_ptr[op];

			const float        res = OP::rg(cr, a1, a2, a3, a4, c, a5, a6, a7, a8, chroma);

			dst_ptr[x] = res;

			++src1_ptr;
			++src2_ptr;
		}
	}

	template <class OP1, class T1>
	static void do_process_plane_cpp(const VSFrameRef *src1_frame, const VSFrameRef *src2_frame, VSFrameRef *dst_frame, int plane_id, const VSAPI *vsapi, const int chroma)
	{
		const int        w = vsapi->getFrameWidth(src1_frame, plane_id);
		const int        h = vsapi->getFrameHeight(src1_frame, plane_id);
		T1 *            dst_ptr = reinterpret_cast<T1*>(vsapi->getWritePtr(dst_frame, plane_id));
		const int        stride = vsapi->getStride(src1_frame, plane_id);

		const T1*        src1_ptr = reinterpret_cast<const T1*>(vsapi->getReadPtr(src1_frame, plane_id));
		const T1*        src2_ptr = reinterpret_cast<const T1*>(vsapi->getReadPtr(src2_frame, plane_id));

		// First line
		memcpy(dst_ptr, src1_ptr, stride);

		// Main content
		PlaneProc<OP1, T1>::process_subplane_cpp(src1_ptr, src2_ptr, dst_ptr, stride / sizeof(T1), w, h, chroma);

		// Last line
		const int        lp = (h - 1) * stride / sizeof(T1);
		memcpy(dst_ptr + lp, src1_ptr + lp, stride);
	}

};

typedef struct {
	VSNodeRef *node1;
	VSNodeRef *node2;
	const VSVideoInfo *vi;
	int mode[3];
} RepairData;

static void VS_CC repairInit(VSMap *in, VSMap *out, void **instanceData, VSNode *node, VSCore *core, const VSAPI *vsapi) {
	RepairData *d = static_cast<RepairData *>(*instanceData);
	vsapi->setVideoInfo(d->vi, 1, node);
}

static const VSFrameRef *VS_CC repairGetFrame(int n, int activationReason, void **instanceData, void **frameData, VSFrameContext *frameCtx, VSCore *core, const VSAPI *vsapi) {
	RepairData *d = static_cast<RepairData *>(*instanceData);

	if (activationReason == arInitial) {
		vsapi->requestFrameFilter(n, d->node1, frameCtx);
		vsapi->requestFrameFilter(n, d->node2, frameCtx);
	}
	else if (activationReason == arAllFramesReady) {
		const VSFrameRef *src1_frame = vsapi->getFrameFilter(n, d->node1, frameCtx);
		const VSFrameRef *src2_frame = vsapi->getFrameFilter(n, d->node2, frameCtx);
		int planes[3] = { 0, 1, 2 };
		const VSFrameRef * cp_planes[3] = { d->mode[0] ? nullptr : src1_frame, d->mode[1] ? nullptr : src1_frame, d->mode[2] ? nullptr : src1_frame };
		VSFrameRef *dst_frame = vsapi->newVideoFrame2(vsapi->getFrameFormat(src1_frame), vsapi->getFrameWidth(src1_frame, 0), vsapi->getFrameHeight(src1_frame, 0), cp_planes, planes, src1_frame, core);


#define PROC_ARGS(op) PlaneProc <op, float>::do_process_plane_cpp<op, float>(src1_frame, src2_frame, dst_frame, i, vsapi, i && d->vi->format->colorFamily != cmRGB); break;

		for (int i = 0; i < d->vi->format->numPlanes; i++) {
			switch (d->mode[i])
			{
			case  1: PROC_ARGS(OpRG01)
			case  2: PROC_ARGS(OpRG02)
			case  3: PROC_ARGS(OpRG03)
			case  4: PROC_ARGS(OpRG04)
			case  5: PROC_ARGS(OpRG05)
			case  6: PROC_ARGS(OpRG06)
			case  7: PROC_ARGS(OpRG07)
			case  8: PROC_ARGS(OpRG08)
			case  9: PROC_ARGS(OpRG09)
			case 10: PROC_ARGS(OpRG10)
			case 11: PROC_ARGS(OpRG01)
			case 12: PROC_ARGS(OpRG12)
			case 13: PROC_ARGS(OpRG13)
			case 14: PROC_ARGS(OpRG14)
			case 15: PROC_ARGS(OpRG15)
			case 16: PROC_ARGS(OpRG16)
			case 17: PROC_ARGS(OpRG17)
			case 18: PROC_ARGS(OpRG18)
			case 19: PROC_ARGS(OpRG19)
			case 20: PROC_ARGS(OpRG20)
			case 21: PROC_ARGS(OpRG21)
			case 22: PROC_ARGS(OpRG22)
			case 23: PROC_ARGS(OpRG23)
			case 24: PROC_ARGS(OpRG24)
			default: break;
			}
		}

		vsapi->freeFrame(src1_frame);
		vsapi->freeFrame(src2_frame);
		return dst_frame;
	}

	return nullptr;
}

static void VS_CC repairFree(void *instanceData, VSCore *core, const VSAPI *vsapi) {
	RepairData *d = static_cast<RepairData *>(instanceData);
	vsapi->freeNode(d->node1);
	vsapi->freeNode(d->node2);
	delete d;
}

void VS_CC repairCreate(const VSMap *in, VSMap *out, void *userData, VSCore *core, const VSAPI *vsapi) {
	RepairData d;

	d.node1 = vsapi->propGetNode(in, "clip", 0, nullptr);
	d.vi = vsapi->getVideoInfo(d.node1);

	if (!isConstantFormat(d.vi)) {
		vsapi->freeNode(d.node1);
		vsapi->setError(out, "Repair: Only constant format input supported");
		return;
	}

	d.node2 = vsapi->propGetNode(in, "repairclip", 0, nullptr);

	if (!isSameFormat(d.vi, vsapi->getVideoInfo(d.node2))) {
		vsapi->freeNode(d.node1);
		vsapi->freeNode(d.node2);
		vsapi->setError(out, "Repair: Input clips must have the same format");
		return;
	}

	int n = d.vi->format->numPlanes;
	int m = vsapi->propNumElements(in, "mode");
	if (n < m) {
		vsapi->freeNode(d.node1);
		vsapi->freeNode(d.node2);
		vsapi->setError(out, "Repair: Number of modes specified must be equal or fewer than the number of input planes");
		return;
	}

	for (int i = 0; i < 3; i++) {
		if (i < m) {
			d.mode[i] = int64ToIntS(vsapi->propGetInt(in, "mode", i, nullptr));
			if (d.mode[i] < 0 || d.mode[i] > 24)
			{
				vsapi->freeNode(d.node1);
				vsapi->freeNode(d.node2);
				vsapi->setError(out, "Repair: Invalid mode specified, only 0-24 supported");
				return;
			}
		}
		else {
			d.mode[i] = d.mode[i - 1];
		}
	}

	RepairData *data = new RepairData(d);

	vsapi->createFilter(in, out, "Repair", repairInit, repairGetFrame, repairFree, fmParallel, 0, data, core);
}
