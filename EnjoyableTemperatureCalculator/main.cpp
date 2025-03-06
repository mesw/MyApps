#include "EnjoyableTemperatureCalculator.h"

EnjoyableTemperatureCalculator::EnjoyableTemperatureCalculator()
{
	CtrlLayout(*this, "Enjoyable Temperature Calculator");

	setupGui();

	updateVessel();
	updatePrimary();
	updateMixer();
	updateTarget();

	updateCalculation();
}

void EnjoyableTemperatureCalculator::setupGui()
{

	vessels = {{"Espresso Cup", {60.0, "mL"}}, {"Coffee Cup", {200.0, "mL"}},
	           {"Coffee Mug", {300.0, "mL"}},  {"Big Glass", {350.0, "mL"}},
	           {"Beer Glass", {500.0, "mL"}},  {"Soup Bowl", {500.0, "mL"}},
	           {"Cooking pot", {5.0, "L"}},    {"Bath Tub", {250.0, "L"}}};

	primaries = {
		{"Hot Coffee", {150, "mL", 90., "°C"}}, {"Cold Coffee", {150, "mL", 20., "°C"}},
		{"Hot Tea", {200, "mL", 85., "°C"}},    {"Cold Tea", {200, "mL", 20., "°C"}},
		{"Hot Soup", {300, "mL", 95., "°C"}},   {"Cold Soup", {300, "mL", 20., "°C"}},
		{"Hot Tub", {100, "L", 50., "°C"}},     {"Cold Tub", {100, "L", 20., "°C"}},
		{"Boiling water", {1, "L", 99., "°C"}}};
	mixers = {{"Hot Coffee", {90., "°C"}},   {"Cold Coffee", {20., "°C"}},
	          {"Hot Tea", {85., "°C"}},      {"Cold Tea", {20., "°C"}},
	          {"Hot Soup", {95., "°C"}},     {"Cold Soup", {20., "°C"}},
	          {"Hot Water", {50., "°C"}},    {"Cold Water", {15., "°C"}},
	          {"Boiling water", {99., "°C"}}};

	targets = {
		{"Drinkable Hot", {45, "°C"}}, {"Luke warm", {25, "°C"}},  {"Bathing hot", {40, "°C"}},
		{"Bathing warm", {36, "°C"}},  {"Edible Hot", {50, "°C"}},
	};

	for(auto key : vessels.GetKeys()) {
		vesselPreset.Add(key);
	}
	for(auto key : primaries.GetKeys()) {
		primaryPreset.Add(key);
	}
	for(auto key : mixers.GetKeys()) {
		mixerPreset.Add(key);
	}
	for(auto key : targets.GetKeys()) {
		targetPreset.Add(key);
	}

	vesselPreset.WhenAction = THISFN(EnjoyableTemperatureCalculator::updateVessel);
	vesselVolume.WhenAction = THISFN(EnjoyableTemperatureCalculator::updateVessel);
	vesselUnit.WhenAction = THISFN(EnjoyableTemperatureCalculator::updateVessel);
	primaryPreset.WhenAction = THISFN(EnjoyableTemperatureCalculator::updatePrimary);
	primaryTemperature.WhenAction = THISFN(EnjoyableTemperatureCalculator::updatePrimary);
	primaryTemperatureUnit.WhenAction = THISFN(EnjoyableTemperatureCalculator::updatePrimary);
	primaryVolume.WhenAction = THISFN(EnjoyableTemperatureCalculator::updatePrimary);
	primaryVolumeUnit.WhenAction = THISFN(EnjoyableTemperatureCalculator::updatePrimary);
	mixerPreset.WhenAction = THISFN(EnjoyableTemperatureCalculator::updateMixer);
	mixerTemperature.WhenAction = THISFN(EnjoyableTemperatureCalculator::updateMixer);
	mixerTemperatureUnit.WhenAction = THISFN(EnjoyableTemperatureCalculator::updateMixer);
	targetPreset.WhenAction = THISFN(EnjoyableTemperatureCalculator::updateTarget);
	targetTemperature.WhenAction = THISFN(EnjoyableTemperatureCalculator::updateTarget);
	targetTemperatureUnit.WhenAction = THISFN(EnjoyableTemperatureCalculator::updateTarget);

	vesselPreset.SetIndex(2);
	vesselVolume.MinMax(1, 10000);
	vesselVolume.SetInc(0.1);
	vesselUnit.Add("mL");
	vesselUnit.Add("L");

	primaryPreset.SetIndex(1);
	primaryVolume.MinMax(1, 10000);
	primaryVolume.SetInc(0.1);
	primaryVolumeUnit.Add("mL");
	primaryVolumeUnit.Add("L");
	primaryTemperature.MinMax(1, 10000);
	primaryTemperature.SetInc(0.1);
	primaryTemperatureUnit.Add("°C");

	mixerPreset.SetIndex(0);
	mixerTemperature.MinMax(1, 10000);
	mixerTemperature.SetInc(0.1);
	mixerTemperatureUnit.Add("°C");

	targetPreset.SetIndex(0);
	targetTemperature.MinMax(1, 10000);
	targetTemperature.SetInc(0.1);
	targetTemperatureUnit.Add("°C");
}

void EnjoyableTemperatureCalculator::updateVessel()
{
	auto vessel = ~vesselPreset;
	double volume = vessels.Get(vessel).a;
	String unit = vessels.Get(vessel).b;
	vesselVolume <<= volume;
	vesselUnit <<= unit;

	updateCalculation();
}

void EnjoyableTemperatureCalculator::updatePrimary()
{
	auto primary = ~primaryPreset;
	auto primaryValue = primaries.Get(primary);
	double volume = primaryValue.a;
	String volumeUnit = primaryValue.b;
	double temperature = primaryValue.c;
	String temperatureUnit = primaryValue.d;
	primaryVolume <<= volume;
	primaryVolumeUnit <<= volumeUnit;
	primaryTemperature <<= temperature;
	primaryTemperatureUnit <<= temperatureUnit;

	updateCalculation();
}

void EnjoyableTemperatureCalculator::updateMixer()
{
	auto mixer = ~mixerPreset;
	auto mixerValue = mixers.Get(mixer);
	double temperature = mixerValue.a;
	String unit = mixerValue.b;
	mixerTemperature <<= temperature;
	mixerTemperatureUnit <<= unit;

	updateCalculation();
}

void EnjoyableTemperatureCalculator::updateTarget()
{
	auto target = ~targetPreset;
	auto targetValue = targets.Get(target);
	double temperature = targetValue.a;
	String unit = targetValue.b;
	targetTemperature <<= temperature;
	targetTemperatureUnit <<= unit;

	updateCalculation();
}

void EnjoyableTemperatureCalculator::updateCalculation()
{
	// calculation is done in L
	double pV = ~primaryVolume;
	if(~primaryVolumeUnit == "mL")
		pV = pV / 1000.;

	double pT = ~primaryTemperature;
	double mT = ~mixerTemperature;
	double eT = ~targetTemperature;

	// eT = (pV * pT) + (mV * mT) / (pV + mV)
	// eT * pV + eT * mV = pV * pT + mV * mT
	// eT * mV - mV * mT = pV * pT - eT * pV
	// mV = (pV * pT - eT * pV) / (eT - mT)
	auto mV = pV * (pT - eT) / (eT - mT);
	auto eV = pV + mV;

	String text;
	if(mV > 1.0)
		text = Format("Add %d L of MIXER liquid to achieve %d °C", mV, eT);
	else
		text = Format("Add %d mL of MIXER liquid to achieve %d °C", mV * 1000, eT);

	DUMP(text);
	mixtureText.SetText(text);
	mixtureText.Refresh();

	double vV = ~vesselVolume;
	if(~vesselUnit == "mL")
		vV = vV / 1000.;

	if(eV <= vV) {
		mixtureWarning.Hide();
				mixtureProgress.SetColor(Green());

	}
	else {
		mixtureWarning.Show(true);
		mixtureProgress.SetColor(Red());
	}

	mixtureProgress.Set(eV * 1000, vV * 1000);

	DUMP(vV);
	DUMP(pV);
	DUMP(pT);
	DUMP(mV);
	DUMP(mT);
	DUMP(eV);
	DUMP(eT);
}

GUI_APP_MAIN { EnjoyableTemperatureCalculator().Run(); }
