// xll_fixed_income.cpp - Fixed income instruments and valuation.
#include "fre_fixed_income.h"
#include "xll_fre.h"

#undef CATEGORY
#define CATEGORY "FI"
#define PWFLAT "PWFLAT"

using namespace fre;
using namespace xll;

// !!! Implement FI.INSTRUMENT
AddIn xai_instrument_(
	Function(XLL_HANDLE, "xll_instrument_", CATEGORY ".INSTRUMENT")
	.Arguments({
		Arg(XLL_FPX, "Time", "is an array of positive increasing times."),
		Arg(XLL_FPX, "Cash", "is an array of cash flows."),
		})
		.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to a fixed income instrument.")
);
HANDLEX WINAPI xll_instrument_(const _FPX* pt, const _FPX* pc)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX; // default return value

	try {
		ensure(size(*pt) == size(*pc));

		if (size(*pt) == 1 and pt->array[0] == 0) {
			handle<fixed_income::instrument<>> i(new fixed_income::instrument<>());
			ensure(i);

			h = i.get();
		}
		else {
			handle<fixed_income::instrument<>> i(new fixed_income::instrument<>(size(*pt), pt->array, pc->array));
			ensure(i);

			h = i.get();
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

// !!! Implement FI.PRESEENT_VALUE
AddIn xai_present_value(
	Function(XLL_DOUBLE, "xll_present_value", CATEGORY ".PRESEENT_VALUE")
	.Arguments({
		Arg(XLL_HANDLEX, "Curve", "is handle returned by \\" PWFLAT  ".CURVE."),
		Arg(XLL_HANDLEX, "Instrument", "is handle returned by", CATEGORY ".INSTRUMENT"),
		})
		.Category(CATEGORY)
	.FunctionHelp("Return present value of fixed income instrument given the forward curve.")
);
double WINAPI xll_present_value(HANDLEX curve, HANDLEX instrument)
{
#pragma XLLEXPORT
	double result = 0;
	try {
		handle <pwflat::curve<>> c(curve);
		ensure(c);
		handle <fixed_income::instrument<>> i(instrument);
		ensure(i);
		for (int j = 0; j < i->size(); j++) {
			result += i->cash()[j] * c->discount(i->time()[j]);
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		return 0;
	}

	return result;
}