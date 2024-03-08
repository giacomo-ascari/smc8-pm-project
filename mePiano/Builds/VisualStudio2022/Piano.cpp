#pragma once

#include "Piano.h"

/*

stringCount(i) = stringCountWave,int(i+mapDelta) : rdtable;
stringCountWave = waveform{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
string0Pitch(i) = string0PitchWave,int(i+mapDelta) : rdtable;
string0PitchWave = waveform{25.9, 27.2, 29.6, 31.6, 33.2, 35.3, 36.1, 38.6, 42.2, 44.3, 45, 51.7, 54.7, 56.7, 60.6, 64, 67.8, 71.6, 75.8, 80.5, 85, 90.4, 95.6, 101.5, 107.6, 113.7, 120.6, 127.7, 136, 144.1, 149.5, 160, 170.2, 177.8, 190.4, 200.8, 213.3, 227.5, 240, 252.6, 271.3, 282.3, 298.1, 313.7, 342.9, 360.9, 381, 400, 422.9, 450.7, 480, 505.3, 530.5, 551.7, 600, 635.2, 671.2, 705.8, 755.9, 800, 847.1, 905.7, 947.4, 1021.2, 1066.8, 1133.9, 1200, 1288.6, 1361.7, 1454.6, 1536, 1590.5, 1690.3, 1802.7, 1913.5, 2009.1, 2152.6, 2288.2, 2428.8, 2541.8, 2733.1, 2868.1, 3062.9, 3259.6, 3454.1};
string1Pitch(i) = string1PitchWave,int(i+mapDelta) : rdtable;
string1PitchWave = waveform{440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 56.1, 60, 64.5, 68.2, 72.2, 76.4, 79.8, 85.6, 91.1, 96.2, 100.6, 106.4, 112.6, 119.3, 128.6, 136.8, 145.1, 147.8, 158.3, 171.3, 175.8, 188.4, 198.8, 215.1, 225.1, 237.4, 254.4, 273.5, 284.4, 300.2, 316.2, 345.8, 357.4, 377.4, 396.3, 426.1, 446.2, 483.5, 508.4, 525.3, 555.8, 604.9, 639.7, 675.6, 698.4, 760.7, 792.9, 853.4, 897.8, 955.4, 1011.9, 1055.7, 1140.6, 1210.4, 1299, 1372.2, 1466.3, 1547.7, 1575.4, 1675.7, 1817.3, 1893, 1990.4, 2170.6, 2266.1, 2407.1, 2561.6, 2706.6, 2837.3, 3028.7, 3223, 3474.1};
string2Pitch(i) = string2PitchWave,int(i+mapDelta) : rdtable;
string2PitchWave = waveform{440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 151.1, 157.9, 168, 179.8, 187.9, 203.1, 215.2, 229.5, 242.2, 249.5, 267.9, 278.4, 301.2, 317.2, 346, 356.5, 385.3, 394.3, 417.8, 454.7, 485.1, 510.1, 510.2, 544.8, 606, 641.2, 677.9, 696.5, 747, 790.3, 856.5, 914.5, 934.4, 1030.4, 1077.9, 1143.8, 1186.2, 1273, 1343.5, 1437.5, 1516.3, 1605.9, 1667.7, 1820, 1886.7, 1983.6, 2122, 2258.9, 2394.8, 2571.2, 2696.7, 2829, 3022, 3216.8, 3490.6};

*/

class Piano
{
public:
	Piano();
	~Piano();
private:

};