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

#define AvsFilterRemoveGrain16_SORT_AXIS_CPP \
    const float      ma1 = std::max(a1, a8);   \
    const float      mi1 = std::min(a1, a8);   \
    const float      ma2 = std::max(a2, a7);   \
    const float      mi2 = std::min(a2, a7);   \
    const float      ma3 = std::max(a3, a6);   \
    const float      mi3 = std::min(a3, a6);   \
    const float      ma4 = std::max(a4, a5);   \
    const float      mi4 = std::min(a4, a5);

class OpRG01 : public LineProcAll {
public:
	typedef    ConvSigned    ConvSign;
	static __forceinline float rg(float c, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, const int chroma) {
		const float        mi = std::min(
			std::min(std::min(a1, a2), std::min(a3, a4)),
			std::min(std::min(a5, a6), std::min(a7, a8))
			);
		const float        ma = std::max(
			std::max(std::max(a1, a2), std::max(a3, a4)),
			std::max(std::max(a5, a6), std::max(a7, a8))
			);

		return limit(c, mi, ma);
	}
};

class OpRG02 : public LineProcAll {
public:
	typedef    ConvSigned    ConvSign;
	static __forceinline float rg(float c, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, const int chroma) {
		float                a[8] = { a1, a2, a3, a4, a5, a6, a7, a8 };

		std::sort(&a[0], (&a[7]) + 1);

		return limit(c, a[2 - 1], a[7 - 1]);
	}
};

class OpRG03 : public LineProcAll {
public:
	typedef    ConvSigned    ConvSign;
	static __forceinline float rg(float c, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, const int chroma) {
		float                a[8] = { a1, a2, a3, a4, a5, a6, a7, a8 };

		std::sort(&a[0], (&a[7]) + 1);

		return limit(c, a[3 - 1], a[6 - 1]);
	}
};

class OpRG04 : public LineProcAll {
public:
	typedef    ConvSigned    ConvSign;
	static __forceinline float rg(float c, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, const int chroma) {
		float                a[8] = { a1, a2, a3, a4, a5, a6, a7, a8 };

		std::sort(&a[0], (&a[7]) + 1);

		return limit(c, a[4 - 1], a[5 - 1]);
	}
};

class OpRG05 : public LineProcAll {
public:
	typedef ConvSigned ConvSign;
	static __forceinline float
		rg(float c, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, const int chroma) {
		AvsFilterRemoveGrain16_SORT_AXIS_CPP

			const double      c1 = std::abs(static_cast<double>(c) - limit(c, mi1, ma1));
		const double      c2 = std::abs(static_cast<double>(c) - limit(c, mi2, ma2));
		const double      c3 = std::abs(static_cast<double>(c) - limit(c, mi3, ma3));
		const double      c4 = std::abs(static_cast<double>(c) - limit(c, mi4, ma4));

		const double      mindiff = std::min(std::min(c1, c2), std::min(c3, c4));

		if (mindiff == c4) {
			return limit(c, mi4, ma4);
		}
		else if (mindiff == c2) {
			return limit(c, mi2, ma2);
		}
		else if (mindiff == c3) {
			return limit(c, mi3, ma3);
		}

		return limit(c, mi1, ma1);
	}
};


class OpRG06 : public LineProcAll {
public:
	typedef ConvSigned ConvSign;
	static __forceinline float
		rg(float c, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, const int chroma) {
		AvsFilterRemoveGrain16_SORT_AXIS_CPP

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

		const float      cli1 = limit(c, mi1, ma1);
		const float      cli2 = limit(c, mi2, ma2);
		const float      cli3 = limit(c, mi3, ma3);
		const float      cli4 = limit(c, mi4, ma4);

		const double      c1 = limit((std::abs(static_cast<double>(c) - cli1) * 2.) + d1, minimum, maximum);
		const double      c2 = limit((std::abs(static_cast<double>(c) - cli2) * 2.) + d2, minimum, maximum);
		const double      c3 = limit((std::abs(static_cast<double>(c) - cli3) * 2.) + d3, minimum, maximum);
		const double      c4 = limit((std::abs(static_cast<double>(c) - cli4) * 2.) + d4, minimum, maximum);

		const double      mindiff = std::min(std::min(c1, c2), std::min(c3, c4));

		if (mindiff == c4) {
			return cli4;
		}
		else if (mindiff == c2) {
			return cli2;
		}
		else if (mindiff == c3) {
			return cli3;
		}

		return cli1;
	}
};

class OpRG07 : public LineProcAll {
public:
	typedef ConvSigned ConvSign;
	static __forceinline float
		rg(float c, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, const int chroma) {
		AvsFilterRemoveGrain16_SORT_AXIS_CPP

			const double      d1 = static_cast<double>(ma1) - mi1;
		const double      d2 = static_cast<double>(ma2) - mi2;
		const double      d3 = static_cast<double>(ma3) - mi3;
		const double      d4 = static_cast<double>(ma4) - mi4;

		const float      cli1 = limit(c, mi1, ma1);
		const float      cli2 = limit(c, mi2, ma2);
		const float      cli3 = limit(c, mi3, ma3);
		const float      cli4 = limit(c, mi4, ma4);

		const double      c1 = std::abs(static_cast<double>(c) - cli1) + d1;
		const double      c2 = std::abs(static_cast<double>(c) - cli2) + d2;
		const double      c3 = std::abs(static_cast<double>(c) - cli3) + d3;
		const double      c4 = std::abs(static_cast<double>(c) - cli4) + d4;

		const double      mindiff = std::min(std::min(c1, c2), std::min(c3, c4));

		if (mindiff == c4) {
			return cli4;
		}
		else if (mindiff == c2) {
			return cli2;
		}
		else if (mindiff == c3) {
			return cli3;
		}

		return cli1;
	}
};

class OpRG08 : public LineProcAll {
public:
	typedef ConvSigned ConvSign;
	static __forceinline float
		rg(float c, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, const int chroma) {
		AvsFilterRemoveGrain16_SORT_AXIS_CPP

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

		const float      cli1 = limit(c, mi1, ma1);
		const float      cli2 = limit(c, mi2, ma2);
		const float      cli3 = limit(c, mi3, ma3);
		const float      cli4 = limit(c, mi4, ma4);

		const double      c1 = limit(std::abs(static_cast<double>(c) - cli1) + (d1 * 2.), minimum, maximum);
		const double      c2 = limit(std::abs(static_cast<double>(c) - cli2) + (d2 * 2.), minimum, maximum);
		const double      c3 = limit(std::abs(static_cast<double>(c) - cli3) + (d3 * 2.), minimum, maximum);
		const double      c4 = limit(std::abs(static_cast<double>(c) - cli4) + (d4 * 2.), minimum, maximum);

		const double      mindiff = std::min(std::min(c1, c2), std::min(c3, c4));

		if (mindiff == c4) {
			return cli4;
		}
		else if (mindiff == c2) {
			return cli2;
		}
		else if (mindiff == c3) {
			return cli3;
		}

		return cli1;
	}
};

class OpRG09 : public LineProcAll {
public:
	typedef ConvSigned ConvSign;
	static __forceinline float
		rg(float c, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, const int chroma) {
		AvsFilterRemoveGrain16_SORT_AXIS_CPP

			const double      d1 = static_cast<double>(ma1) - mi1;
		const double      d2 = static_cast<double>(ma2) - mi2;
		const double      d3 = static_cast<double>(ma3) - mi3;
		const double      d4 = static_cast<double>(ma4) - mi4;

		const double      mindiff = std::min(std::min(d1, d2), std::min(d3, d4));

		if (mindiff == d4) {
			return limit(c, mi4, ma4);
		}
		else if (mindiff == d2) {
			return limit(c, mi2, ma2);
		}
		else if (mindiff == d3) {
			return limit(c, mi3, ma3);
		}

		return limit(c, mi1, ma1);
	}
};

class OpRG10 : public LineProcAll {
public:
	typedef ConvUnsigned ConvSign;
	static __forceinline float
		rg(float c, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, const int chroma) {
		const double      d1 = std::abs(static_cast<double>(c) - a1);
		const double      d2 = std::abs(static_cast<double>(c) - a2);
		const double      d3 = std::abs(static_cast<double>(c) - a3);
		const double      d4 = std::abs(static_cast<double>(c) - a4);
		const double      d5 = std::abs(static_cast<double>(c) - a5);
		const double      d6 = std::abs(static_cast<double>(c) - a6);
		const double      d7 = std::abs(static_cast<double>(c) - a7);
		const double      d8 = std::abs(static_cast<double>(c) - a8);

		const double      mindiff = std::min(
			std::min(std::min(d1, d2), std::min(d3, d4)),
			std::min(std::min(d5, d6), std::min(d7, d8))
			);

		if (mindiff == d7) { return (a7); }
		if (mindiff == d8) { return (a8); }
		if (mindiff == d6) { return (a6); }
		if (mindiff == d2) { return (a2); }
		if (mindiff == d3) { return (a3); }
		if (mindiff == d1) { return (a1); }
		if (mindiff == d5) { return (a5); }

		return (a4);
	}
};

class OpRG11 : public LineProcAll {
public:
	typedef    ConvUnsigned    ConvSign;
	static __forceinline float rg(float c, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, const int chroma) {
		const double        sum = 4. * static_cast<double>(c) + 2. * (static_cast<double>(a2) + a4 + a5 + a7) + a1 + a3 + a6 + a8;
		const double        val = sum / 16.;

		return static_cast<float>(val);
	}
};

class OpRG12 : public LineProcAll {
public:
	typedef    ConvUnsigned    ConvSign;
	static __forceinline float rg(float c, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, const int chroma) {
		return (OpRG11::rg(c, a1, a2, a3, a4, a5, a6, a7, a8, chroma));
	}
};

class OpRG1314 {
public:
	static __forceinline float
		rg(float c, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, const int chroma) {
		const double      d1 = std::abs(static_cast<double>(a1) - a8);
		const double      d2 = std::abs(static_cast<double>(a2) - a7);
		const double      d3 = std::abs(static_cast<double>(a3) - a6);

		const double      mindiff = std::min(std::min(d1, d2), d3);

		if (mindiff == d2) {
			return static_cast<float>((static_cast<double>(a2) + a7) / 2.);
		}
		if (mindiff == d3) {
			return static_cast<float>((static_cast<double>(a3) + a6) / 2.);
		}

		return static_cast<float>((static_cast<double>(a1) + a8) / 2.);
	}
};

class OpRG13 : public OpRG1314, public LineProcEven {};
class OpRG14 : public OpRG1314, public LineProcOdd {};

class OpRG1516 {
public:
	static __forceinline float
		rg(float c, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, const int chroma) {
		const double      d1 = std::abs(static_cast<double>(a1) - a8);
		const double      d2 = std::abs(static_cast<double>(a2) - a7);
		const double      d3 = std::abs(static_cast<double>(a3) - a6);

		const double      mindiff = std::min(std::min(d1, d2), d3);
		const float      average = static_cast<float>((2. * (static_cast<double>(a2) + a7) + a1 + a3 + a6 + a8) / 8.);

		if (mindiff == d2) {
			return limit(average, std::min(a2, a7), std::max(a2, a7));
		}
		if (mindiff == d3) {
			return limit(average, std::min(a3, a6), std::max(a3, a6));
		}

		return limit(average, std::min(a1, a8), std::max(a1, a8));
	}
};

class OpRG15 : public OpRG1516, public LineProcEven {};
class OpRG16 : public OpRG1516, public LineProcOdd {};

class OpRG17 : public LineProcAll {
public:
	typedef ConvSigned ConvSign;
	static __forceinline float
		rg(float c, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, const int chroma) {
		AvsFilterRemoveGrain16_SORT_AXIS_CPP

			const float      l = std::max(std::max(mi1, mi2), std::max(mi3, mi4));
		const float      u = std::min(std::min(ma1, ma2), std::min(ma3, ma4));

		return limit(c, std::min(l, u), std::max(l, u));
	}
};

class OpRG18 : public LineProcAll {
public:
	typedef ConvUnsigned ConvSign;
	static __forceinline float
		rg(float c, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, const int chroma) {
		const double      d1 = std::max(std::abs(static_cast<double>(c) - a1), std::abs(static_cast<double>(c) - a8));
		const double      d2 = std::max(std::abs(static_cast<double>(c) - a2), std::abs(static_cast<double>(c) - a7));
		const double      d3 = std::max(std::abs(static_cast<double>(c) - a3), std::abs(static_cast<double>(c) - a6));
		const double      d4 = std::max(std::abs(static_cast<double>(c) - a4), std::abs(static_cast<double>(c) - a5));

		const double      mindiff = std::min(std::min(d1, d2), std::min(d3, d4));

		if (mindiff == d4) {
			return limit(c, std::min(a4, a5), std::max(a4, a5));
		}
		if (mindiff == d2) {
			return limit(c, std::min(a2, a7), std::max(a2, a7));
		}
		if (mindiff == d3) {
			return limit(c, std::min(a3, a6), std::max(a3, a6));
		}

		return limit(c, std::min(a1, a8), std::max(a1, a8));
	}
};

class OpRG19 : public LineProcAll {
public:
	typedef    ConvUnsigned    ConvSign;
	static __forceinline float rg(float c, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, const int chroma) {
		const double        sum = static_cast<double>(a1) + a2 + a3 + a4 + a5 + a6 + a7 + a8;
		const double        val = sum / 8.;

		return static_cast<float>(val);
	}
};

class OpRG20 : public LineProcAll {
public:
	typedef    ConvUnsigned    ConvSign;
	static __forceinline float rg(float c, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, const int chroma) {
		const double        sum = static_cast<double>(a1) + a2 + a3 + a4 + c + a5 + a6 + a7 + a8;
		const double        val = sum / 9.;

		return static_cast<float>(val);
	}
};

class OpRG21 : public LineProcAll {
public:
	typedef ConvUnsigned ConvSign;
	static __forceinline float
		rg(float c, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, const int chroma) {
		const double      l1 = (static_cast<double>(a1) + a8) / 2.;
		const double      l2 = (static_cast<double>(a2) + a7) / 2.;
		const double      l3 = (static_cast<double>(a3) + a6) / 2.;
		const double      l4 = (static_cast<double>(a4) + a5) / 2.;

		const double      mi = std::min(std::min(l1, l2), std::min(l3, l4));
		const double      ma = std::max(std::max(l1, l2), std::max(l3, l4));

		return static_cast<float>(limit(static_cast<double>(c), mi, ma));
	}
};

class OpRG22 : public LineProcAll {
public:
	typedef    ConvUnsigned    ConvSign;
	static __forceinline float rg(float c, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, const int chroma) {
		return (OpRG21::rg(c, a1, a2, a3, a4, a5, a6, a7, a8, chroma));
	}
};

class OpRG23 : public LineProcAll {
public:
	static __forceinline float
		rg(float c, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, const int chroma) {
		AvsFilterRemoveGrain16_SORT_AXIS_CPP

			double minimum;
		if (chroma)
			minimum = -0.5;
		else
			minimum = 0.;

		const double      linediff1 = static_cast<double>(ma1) - mi1;
		const double      linediff2 = static_cast<double>(ma2) - mi2;
		const double      linediff3 = static_cast<double>(ma3) - mi3;
		const double      linediff4 = static_cast<double>(ma4) - mi4;

		const double      u1 = std::min(static_cast<double>(c) - ma1, linediff1);
		const double      u2 = std::min(static_cast<double>(c) - ma2, linediff2);
		const double      u3 = std::min(static_cast<double>(c) - ma3, linediff3);
		const double      u4 = std::min(static_cast<double>(c) - ma4, linediff4);
		const double      u = std::max(
			std::max(std::max(u1, u2), std::max(u3, u4)),
			minimum
			);

		const double      d1 = std::min(static_cast<double>(mi1) - c, linediff1);
		const double      d2 = std::min(static_cast<double>(mi2) - c, linediff2);
		const double      d3 = std::min(static_cast<double>(mi3) - c, linediff3);
		const double      d4 = std::min(static_cast<double>(mi4) - c, linediff4);
		const double      d = std::max(
			std::max(std::max(d1, d2), std::max(d3, d4)),
			minimum
			);

		return static_cast<float>(static_cast<double>(c) - u + d);  // This probably will never overflow.
	}
};

class OpRG24 : public LineProcAll {
public:
	static __forceinline float
		rg(float c, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, const int chroma) {
		AvsFilterRemoveGrain16_SORT_AXIS_CPP

			double minimum;
		if (chroma)
			minimum = -0.5;
		else
			minimum = 0.;

		const double      linediff1 = static_cast<double>(ma1) - mi1;
		const double      linediff2 = static_cast<double>(ma2) - mi2;
		const double      linediff3 = static_cast<double>(ma3) - mi3;
		const double      linediff4 = static_cast<double>(ma4) - mi4;

		const double      tu1 = static_cast<double>(c) - ma1;
		const double      tu2 = static_cast<double>(c) - ma2;
		const double      tu3 = static_cast<double>(c) - ma3;
		const double      tu4 = static_cast<double>(c) - ma4;

		const double      u1 = std::min(tu1, linediff1 - tu1);
		const double      u2 = std::min(tu2, linediff2 - tu2);
		const double      u3 = std::min(tu3, linediff3 - tu3);
		const double      u4 = std::min(tu4, linediff4 - tu4);
		const double      u = std::max(
			std::max(std::max(u1, u2), std::max(u3, u4)),
			minimum
			);

		const double      td1 = static_cast<double>(mi1) - c;
		const double      td2 = static_cast<double>(mi2) - c;
		const double      td3 = static_cast<double>(mi3) - c;
		const double      td4 = static_cast<double>(mi4) - c;

		const double      d1 = std::min(td1, linediff1 - td1);
		const double      d2 = std::min(td2, linediff2 - td2);
		const double      d3 = std::min(td3, linediff3 - td3);
		const double      d4 = std::min(td4, linediff4 - td4);
		const double      d = std::max(
			std::max(std::max(d1, d2), std::max(d3, d4)),
			minimum
			);

		return static_cast<float>(static_cast<double>(c) - u + d);  // This probably will never overflow.
	}
};

template <class OP, class T>
class PlaneProc {
public:

	static void process_subplane_cpp(const T *src_ptr, int stride_src, T *dst_ptr, int stride_dst, int width, int height, const int chroma)
	{
		const int        y_b = 1;
		const int        y_e = height - 1;

		dst_ptr += y_b * stride_dst;
		src_ptr += y_b * stride_src;

		const int        x_e = width - 1;

		for (int y = y_b; y < y_e; ++y)
		{
			if (OP::skip_line(y)) {
				memcpy(dst_ptr, src_ptr, width * sizeof(T));
			}
			else {

				dst_ptr[0] = src_ptr[0];

				process_row_cpp(
					dst_ptr,
					src_ptr,
					stride_src,
					1,
					x_e,
					chroma
					);

				dst_ptr[x_e] = src_ptr[x_e];
			}

			dst_ptr += stride_dst;
			src_ptr += stride_src;
		}
	}

	static void process_row_cpp(T *dst_ptr, const T *src_ptr, int stride_src, int x_beg, int x_end, const int chroma)
	{
		const int      om = stride_src - 1;
		const int      o0 = stride_src;
		const int      op = stride_src + 1;

		src_ptr += x_beg;

		for (int x = x_beg; x < x_end; ++x)
		{
			const float        a1 = src_ptr[-op];
			const float        a2 = src_ptr[-o0];
			const float        a3 = src_ptr[-om];
			const float        a4 = src_ptr[-1];
			const float        c = src_ptr[0];
			const float        a5 = src_ptr[1];
			const float        a6 = src_ptr[om];
			const float        a7 = src_ptr[o0];
			const float        a8 = src_ptr[op];

			const float        res = OP::rg(c, a1, a2, a3, a4, a5, a6, a7, a8, chroma);

			dst_ptr[x] = res;

			++src_ptr;
		}
	}


	template <class OP1, class T1>
	static void do_process_plane_cpp(const VSFrameRef *src_frame, VSFrameRef *dst_frame, int plane_id, const VSAPI *vsapi, const int chroma)
	{
		const int        w = vsapi->getFrameWidth(src_frame, plane_id);
		const int        h = vsapi->getFrameHeight(src_frame, plane_id);
		T1 *                dst_ptr = reinterpret_cast<T1*>(vsapi->getWritePtr(dst_frame, plane_id));
		const int        stride = vsapi->getStride(dst_frame, plane_id);

		const T1*        src_ptr = reinterpret_cast<const T1*>(vsapi->getReadPtr(src_frame, plane_id));

		// First line
		memcpy(dst_ptr, src_ptr, w * sizeof(T1));

		// Main content
		PlaneProc<OP1, T1>::process_subplane_cpp(src_ptr, stride / sizeof(T1), dst_ptr, stride / sizeof(T1), w, h, chroma);

		// Last line
		const int        lp = (h - 1) * stride / sizeof(T1);
		memcpy(dst_ptr + lp, src_ptr + lp, w * sizeof(T1));
	}

};

typedef struct {
	VSNodeRef *node;
	const VSVideoInfo *vi;
	int mode[3];
} RemoveGrainData;

static void VS_CC removeGrainInit(VSMap *in, VSMap *out, void **instanceData, VSNode *node, VSCore *core, const VSAPI *vsapi) {
	RemoveGrainData *d = static_cast<RemoveGrainData *>(*instanceData);
	vsapi->setVideoInfo(d->vi, 1, node);
}

static const VSFrameRef *VS_CC removeGrainGetFrame(int n, int activationReason, void **instanceData, void **frameData, VSFrameContext *frameCtx, VSCore *core, const VSAPI *vsapi) {
	RemoveGrainData *d = static_cast<RemoveGrainData *>(*instanceData);

	if (activationReason == arInitial) {
		vsapi->requestFrameFilter(n, d->node, frameCtx);
	}
	else if (activationReason == arAllFramesReady) {
		const VSFrameRef *src_frame = vsapi->getFrameFilter(n, d->node, frameCtx);
		int planes[3] = { 0, 1, 2 };
		const VSFrameRef * cp_planes[3] = { d->mode[0] ? nullptr : src_frame, d->mode[1] ? nullptr : src_frame, d->mode[2] ? nullptr : src_frame };
		VSFrameRef *dst_frame = vsapi->newVideoFrame2(vsapi->getFrameFormat(src_frame), vsapi->getFrameWidth(src_frame, 0), vsapi->getFrameHeight(src_frame, 0), cp_planes, planes, src_frame, core);


#define PROC_ARGS(op) PlaneProc <op, float>::do_process_plane_cpp<op, float>(src_frame, dst_frame, i, vsapi, i && d->vi->format->colorFamily != cmRGB); break;

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
			case 11: PROC_ARGS(OpRG11)
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


		vsapi->freeFrame(src_frame);

		return dst_frame;
	}

	return nullptr;
}

static void VS_CC removeGrainFree(void *instanceData, VSCore *core, const VSAPI *vsapi) {
	RemoveGrainData *d = static_cast<RemoveGrainData *>(instanceData);
	vsapi->freeNode(d->node);
	delete d;
}

void VS_CC removeGrainCreate(const VSMap *in, VSMap *out, void *userData, VSCore *core, const VSAPI *vsapi) {
	RemoveGrainData d;

	d.node = vsapi->propGetNode(in, "clip", 0, nullptr);
	d.vi = vsapi->getVideoInfo(d.node);

	if (!d.vi->format) {
		vsapi->freeNode(d.node);
		vsapi->setError(out, "RemoveGrain: Only constant format input supported");
		return;
	}

	int n = d.vi->format->numPlanes;
	int m = vsapi->propNumElements(in, "mode");
	if (n < m) {
		vsapi->freeNode(d.node);
		vsapi->setError(out, "RemoveGrain: Number of modes specified must be equal or fewer than the number of input planes");
		return;
	}

	for (int i = 0; i < 3; i++) {
		if (i < m) {
			d.mode[i] = int64ToIntS(vsapi->propGetInt(in, "mode", i, nullptr));
			if (d.mode[i] < 0 || d.mode[i] > 24)
			{
				vsapi->freeNode(d.node);
				vsapi->setError(out, "RemoveGrain: Invalid mode specified, only modes 0-24 supported");
				return;
			}
		}
		else {
			d.mode[i] = d.mode[i - 1];
		}
	}

	RemoveGrainData *data = new RemoveGrainData(d);

	vsapi->createFilter(in, out, "RemoveGrain", removeGrainInit, removeGrainGetFrame, removeGrainFree, fmParallel, 0, data, core);
}
