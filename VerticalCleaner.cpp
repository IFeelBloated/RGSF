#include "shared.h"

struct VerticalCleanerData {
	VSNodeRef * node;
	const VSVideoInfo * vi;
	int mode[3];
};

template<typename T>
static void verticalMedian(const T * VS_RESTRICT srcp, T * VS_RESTRICT dstp, const int width, const int height, const int stride) {
	memcpy(dstp, srcp, stride * sizeof(T));

	srcp += stride;
	dstp += stride;

	for (int y = 1; y < height - 1; y++) {
		for (int x = 0; x < width; x++) {
			const T up = srcp[x - stride];
			const T center = srcp[x];
			const T down = srcp[x + stride];
			dstp[x] = std::min(std::max(std::min(up, down), center), std::max(up, down));
		}

		srcp += stride;
		dstp += stride;
	}

	memcpy(dstp, srcp, stride * sizeof(T));
}

template<typename T>
static void relaxedVerticalMedian(const T * VS_RESTRICT srcp, T * VS_RESTRICT dstp, const int width, const int height, const int stride, const int chroma) {
	double maximum, minimum;
	if (chroma) {
		maximum = 0.5;
		minimum = -0.5;
	}
	else {
		maximum = 1.;
		minimum = 0.;
	}

	memcpy(dstp, srcp, stride * sizeof(T) * 2);

	srcp += stride * 2;
	dstp += stride * 2;

	for (int y = 2; y < height - 2; y++) {
		for (int x = 0; x < width; x++) {
			const double p2 = static_cast<double>(srcp[x - stride * 2]);
			const double p1 = static_cast<double>(srcp[x - stride]);
			const double c = static_cast<double>(srcp[x]);
			const double n1 = static_cast<double>(srcp[x + stride]);
			const double n2 = static_cast<double>(srcp[x + stride * 2]);

			const double upper = std::max(std::max(std::min(limit(limit(p1 - p2, minimum, maximum) + p1, minimum, maximum), limit(limit(n1 - n2, minimum, maximum) + n1, minimum, maximum)), p1), n1);
			const double lower = std::min(std::min(p1, n1), std::max(limit(p1 - limit(p2 - p1, minimum, maximum), minimum, maximum), limit(n1 - limit(n2 - n1, minimum, maximum), minimum, maximum)));

			dstp[x] = static_cast<T>(limit(c, lower, upper));
		}

		srcp += stride;
		dstp += stride;
	}

	memcpy(dstp, srcp, stride * sizeof(T) * 2);
}

static void VS_CC verticalCleanerInit(VSMap *in, VSMap *out, void **instanceData, VSNode *node, VSCore *core, const VSAPI *vsapi) {
	VerticalCleanerData * d = static_cast<VerticalCleanerData *>(*instanceData);
	vsapi->setVideoInfo(d->vi, 1, node);
}

static const VSFrameRef *VS_CC verticalCleanerGetFrame(int n, int activationReason, void **instanceData, void **frameData, VSFrameContext *frameCtx, VSCore *core, const VSAPI *vsapi) {
	const VerticalCleanerData * d = static_cast<const VerticalCleanerData *>(*instanceData);

	if (activationReason == arInitial) {
		vsapi->requestFrameFilter(n, d->node, frameCtx);
	}
	else if (activationReason == arAllFramesReady) {
		const VSFrameRef * src = vsapi->getFrameFilter(n, d->node, frameCtx);
		const VSFrameRef * fr[] = { d->mode[0] ? nullptr : src, d->mode[1] ? nullptr : src, d->mode[2] ? nullptr : src };
		const int pl[] = { 0, 1, 2 };
		VSFrameRef * dst = vsapi->newVideoFrame2(d->vi->format, d->vi->width, d->vi->height, fr, pl, src, core);

		for (int plane = 0; plane < d->vi->format->numPlanes; plane++) {
			const int width = vsapi->getFrameWidth(src, plane);
			const int height = vsapi->getFrameHeight(src, plane);
			const int stride = vsapi->getStride(src, plane);
			const uint8_t * srcp = vsapi->getReadPtr(src, plane);
			uint8_t * dstp = vsapi->getWritePtr(dst, plane);

			if (d->mode[plane] == 1) {
				verticalMedian<float>(reinterpret_cast<const float *>(srcp), reinterpret_cast<float *>(dstp), width, height, stride / 4);
			}
			else if (d->mode[plane] == 2) {
				relaxedVerticalMedian<float>(reinterpret_cast<const float *>(srcp), reinterpret_cast<float *>(dstp), width, height, stride / 4, plane && d->vi->format->colorFamily != cmRGB);
			}
		}

		vsapi->freeFrame(src);
		return dst;
	}

	return nullptr;
}

static void VS_CC verticalCleanerFree(void *instanceData, VSCore *core, const VSAPI *vsapi) {
	VerticalCleanerData * d = static_cast<VerticalCleanerData *>(instanceData);
	vsapi->freeNode(d->node);
	delete d;
}

void VS_CC verticalCleanerCreate(const VSMap *in, VSMap *out, void *userData, VSCore *core, const VSAPI *vsapi) {
	VerticalCleanerData d;

	d.node = vsapi->propGetNode(in, "clip", 0, nullptr);
	d.vi = vsapi->getVideoInfo(d.node);

	const int m = vsapi->propNumElements(in, "mode");

	if (m > d.vi->format->numPlanes) {
		vsapi->setError(out, "VerticalCleaner: number of modes specified must be equal to or fewer than the number of input planes");
		vsapi->freeNode(d.node);
		return;
	}

	for (int i = 0; i < 3; i++) {
		if (i < m) {
			d.mode[i] = int64ToIntS(vsapi->propGetInt(in, "mode", i, nullptr));
			if (d.mode[i] < 0 || d.mode[i] > 2) {
				vsapi->setError(out, "VerticalCleaner: invalid mode specified, only modes 0-2 supported");
				vsapi->freeNode(d.node);
				return;
			}
		}
		else {
			d.mode[i] = d.mode[i - 1];
		}

		const int height = d.vi->height >> (i ? d.vi->format->subSamplingH : 0);
		if (d.mode[i] == 1 && height < 3) {
			vsapi->setError(out, "VerticalCleaner: corresponding plane's height must be greater than or equal to 3 for mode 1");
			vsapi->freeNode(d.node);
			return;
		}
		else if (d.mode[i] == 2 && height < 5) {
			vsapi->setError(out, "VerticalCleaner: corresponding plane's height must be greater than or equal to 5 for mode 2");
			vsapi->freeNode(d.node);
			return;
		}
	}

	VerticalCleanerData * data = new VerticalCleanerData(d);

	vsapi->createFilter(in, out, "VerticalCleaner", verticalCleanerInit, verticalCleanerGetFrame, verticalCleanerFree, fmParallel, 0, data, core);
}
