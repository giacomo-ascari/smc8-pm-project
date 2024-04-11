/*
  ==============================================================================

    PitchTables.h
    Created: 28 Mar 2024 12:17:57pm
    Author:  Giacomo

  ==============================================================================
*/

#pragma once

#define LOWER_NOTE 33
#define HIGHER_NOTE 117

#define NOTE_COUNT 85

#define ONE_STRING_NOTES 13
#define TWO_STRING_NOTES 17
#define THREE_STRING_NOTES 55

#define MAX_PITCHES_COUNT 3

struct noteInfo {
	float pitches[MAX_PITCHES_COUNT];
	bool dampenWorks;
};

const noteInfo pitchTable[NOTE_COUNT] = { {{30.2937068939209,0,0},true},
	{{33.9893341064453,0,0},true},
	{{30.7800912857055,0,0},true},
	{{32.534324645996,0,0},true},
	{{32.7040252685546,0,0},true},
	{{36.5072040557861,0,0},true},
	{{38.6466941833496,0,0},true},
	{{40.72261428833,0,0},true},
	{{42.9021339416503,0,0},true},
	{{46.0251579284668,0,0},true},
	{{46.5161437988281,0,0},true},
	{{51.6283149719238,0,0},true},
	{{54.6830863952636,0,0},true},
	{{56.6124114990234,57.1212657254265,0},true},
	{{60.6531066894531,60.9814643297122,0},true},
	{{63.9025268554687,64.3588213968449,0},true},
	{{67.6918182373046,67.0240831810011,0},true},
	{{71.3297653198242,70.6589370856466,0},true},
	{{75.6064376831054,76.3227102219721,0},true},
	{{80.5254135131836,80.0824493989314,0},true},
	{{84.8227310180664,84.3065030435307,0},true},
	{{90.3798599243164,89.3531594845354,0},true},
	{{95.7601776123046,95.1019769390761,0},true},
	{{101.463134765625,102.595031247108,0},true},
	{{107.489639282226,108.430191341083,0},true},
	{{113.878990173339,112.680318344736,0},true},
	{{120.647888183593,120.145111993161,0},true},
	{{127.106117248535,125.771580537994,0},true},
	{{136.192443847656,135.034315025718,0},true},
	{{143.494117736816,144.21135787694,0},true},
	{{149.350601196289,150.543767644735,148.673215517794},true},
	{{159.629768371582,160.196820342078,158.659169432941},true},
	{{169.644348144531,170.18107316664,168.412296816476},true},
	{{177.673294067382,178.383847449274,175.945207361343},true},
	{{190.401901245117,191.455573681207,189.145465586859},true},
	{{200.577224731445,202.064514303512,198.449278868384},true},
	{{213.735321044921,214.008832443369,212.562110344202},true},
	{{226.460876464843,227.169003323526,225.977773138677},true},
	{{239.990730285644,241.365876245905,239.502575856232},true},
	{{251.443443298339,252.705937867821,251.135311630905},true},
	{{270.76040649414,271.540662489694,268.639948784211},true},
	{{282.017822265625,284.158412213832,279.276364489988},true},
	{{300.594772338867,301.517641666067,300.329978860033},true},
	{{312.967956542968,316.51880207504,312.809392815409},true},
	{{341.12612915039,343.533507570582,338.139289986459},true},
	{{359.423645019531,362.064836788065,359.267261785048},true},
	{{380.888488769531,381.72770048645,378.103597933755},true},
	{{401.343139648437,401.616748914492,399.721888198037},true},
	{{422.569091796875,424.293798544646,418.758701539122},true},
	{{450.223815917968,454.084871086168,447.974990756349},true},
	{{478.395965576171,481.82099544013,476.103299000522},true},
	{{505.330474853515,507.472171228346,504.153008557467},true},
	{{517.264617919921,523.247941111737,511.598338876241},true},
	{{548.1640625,552.859006305977,546.899936708633},true},
	{{594.329315185546,600.459527423868,588.60497166344},true},
	{{632.853576660156,634.113761793514,629.303441828165},true},
	{{667.360473632812,670.646829037308,664.30244442252},true},
	{{706.652893066406,710.259728321485,701.443207628949},true},
	{{748.640808105468,754.092415843224,747.646167462358},true},
	{{802.356292724609,810.964095613148,795.736708058881},true},
	{{849.851867675781,855.131855361981,845.569384462033},true},
	{{905.081390380859,908.063983687253,899.840601138885},true},
	{{942.766174316406,950.995884196017,940.56274378744},true},
	{{1016.83023071289,1017.14306072938,1007.53049302723},true},
	{{1064.78637695312,1073.73545211234,1057.29213648738},true},
	{{1128.65643310546,1133.24457133299,1117.87845425498},true},
	{{1200.79827880859,1205.41640016468,1197.87769952199},true},
	{{1283.14276123046,1288.30885093063,1270.93208997881},true},
	{{1341.02954101562,1342.05250638731,1332.75640570202},true},
	{{1442.12908935546,1454.3372207296,1439.61415874126},true},
	{{1532.4273071289,1548.51557285857,1519.46352793717},true},
	{{1609.13317871093,1615.14964614348,1590.97294623557},true},
	{{1755,1828.52,1594.56},false},
	{{1828.51544189453,1849.29550375294,1821.37490792193},true},
	{{1987.3062133789,1987.82504242258,1984.977660508},true},
	{{2019.15991210937,2035.91262468646,2007.91960953811},true},
	{{2194.42956542968,2211.3902080841,2176.61876056714},true},
	{{2305,2315.7042992418,2299.75905556307},true},
	{{2485,2509.15172548993,2462.46229599882},true},
	{{2615,2616.83327785651,2598.67657575489},true},
	{{2752,2779.81504546326,2739.901436806},true},
	{{2925,2952.14035885009,2914.01595632275},true},
	{{3075,3104.08845340062,3056.83412577441},true},
	{{3285,3292.50547095964,3283.55490797727},true},
	{{3455,3487.97875091632,3420.82427219835},true},
};




class PitchTable {
public:

	static int getStringsCount(int midiNoteNumber)
	{
		int index = midiNoteNumber - LOWER_NOTE;
		if (index < ONE_STRING_NOTES)
		{
			return 1;
		}
		else if (index < ONE_STRING_NOTES + TWO_STRING_NOTES)
		{
			return 2;
		}
		else
		{
			return 3;
		}
	}

	static void getPitches(int midiNoteNumber, float *p)
	{
		// assumes that the note number is playable by the piano
		// the check is a constraint belonging to the piano

		int index = midiNoteNumber - LOWER_NOTE;

		float pitch = 27.5f * std::powf(2.f, (float)(index + 1) / 12.f);

		int stringsCount = getStringsCount(midiNoteNumber);

		if (stringsCount == 1)
		{
			p[0] = pitch;
			p[1] = 0;
			p[2] = 0;
		}
		else if (stringsCount == 2)
		{
			p[0] = pitch * std::powf(2.f, -0.1f / 12.f);
			p[1] = pitch * std::powf(2.f, 0.1f / 12.f);
			p[2] = 0;
		}
		else if (stringsCount == 3)
		{
			p[0] = pitch;
			p[1] = pitch * std::powf(2.f, 0.2f / 12.f);
			p[2] = pitch * std::powf(2.f, -0.2f / 12.f);
		}
	}

	static void getMeasuredPitches(int midiNoteNumber, float* p, bool &dampenWorks)
	{
		// assumes that the note number is playable by the piano
		// the check is a constraint belonging to the piano

		int index = midiNoteNumber - LOWER_NOTE;

		p[0] = pitchTable[index].pitches[0];
		p[1] = pitchTable[index].pitches[1];
		p[2] = pitchTable[index].pitches[2];
		dampenWorks = pitchTable[index].dampenWorks;
	}
};


