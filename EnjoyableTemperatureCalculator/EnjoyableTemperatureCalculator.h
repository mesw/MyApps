#ifndef _EnjoyableTemperatureCalculator_EnjoyableTemperatureCalculator_h
#define _EnjoyableTemperatureCalculator_EnjoyableTemperatureCalculator_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <EnjoyableTemperatureCalculator/EnjoyableTemperatureCalculator.lay>
#include <CtrlCore/lay.h>

class EnjoyableTemperatureCalculator : public WithEnjoyableTemperatureCalculatorLayout<TopWindow> {
typedef EnjoyableTemperatureCalculator CLASSNAME;

public:
	EnjoyableTemperatureCalculator();
		void setupGui();

	void updateVessel();
	void updatePrimary();
	void updateMixer();
	void updateTarget();
	void updateCalculation();
	VectorMap<String, Tuple<double, String>> vessels;
	VectorMap<String, Tuple<double, String, double, String>> primaries;
	VectorMap<String, Tuple<double, String>> mixers;
	VectorMap<String, Tuple<double, String>> targets;
	Index<String> units;
	
};

#endif
