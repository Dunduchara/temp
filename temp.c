#include "temp.h"

static struct
{
	double volt;
	double temp;
} temp_by_v [] =
{
	{.volt = 0, .temp = 0},
	{.volt = 397e-3, .temp = 10},
	{.volt = 798e-3, .temp = 20},
	{.volt = 1203e-3, .temp = 30},
	{.volt = 1612e-3, .temp = 40},
	{.volt = 2023e-3, .temp = 50},
	{.volt = 2436e-3, .temp = 60},
	{.volt = 2851e-3, .temp = 70},
	{.volt = 3267e-3, .temp = 80},
	{.volt = 3682e-3, .temp = 90},
	{.volt = 4096e-3, .temp = 100},
	{.volt = 4609e-3, .temp = 110},
	{.volt = 4920e-3, .temp = 120},
	{.volt = 5328e-3, .temp = 130},
	{.volt = 5735e-3, .temp = 140},
	{.volt = 6138e-3, .temp = 150},
	{.volt = 6540e-3, .temp = 160},
	{.volt = 6941e-3, .temp = 170},
	{.volt = 7340e-3, .temp = 180},
	{.volt = 7739e-3, .temp = 190},
	{.volt = 8138e-3, .temp = 200},
	{.volt = 8539e-3, .temp = 210},
	{.volt = 8940e-3, .temp = 220},
	{.volt = 9343e-3, .temp = 230},
	{.volt = 9747e-3, .temp = 240},
	{.volt = 10153e-3, .temp = 250},
	{.volt = 10561e-3, .temp = 260},
	{.volt = 10971e-3, .temp = 270},
	{.volt = 11382e-3, .temp = 280},
	{.volt = 11795e-3, .temp = 290},
	{.volt = 12209e-3, .temp = 300},
	{.volt = 12624e-3, .temp = 310},
	{.volt = 13040e-3, .temp = 320},
	{.volt = 13457e-3, .temp = 330},
	{.volt = 13874e-3, .temp = 340},
	{.volt = 14293e-3, .temp = 350},
	{.volt = 14713e-3, .temp = 360},
	{.volt = 15133e-3, .temp = 370},
	{.volt = 15554e-3, .temp = 380},
	{.volt = 15975e-3, .temp = 390},
	{.volt = 16397e-3, .temp = 400},
	{.volt = 16820e-3, .temp = 410},
	{.volt = 17243e-3, .temp = 420},
	{.volt = 17667e-3, .temp = 430},
	{.volt = 18091e-3, .temp = 440},
	{.volt = 18516e-3, .temp = 450},
	{.volt = 18941e-3, .temp = 460},
	{.volt = 19366e-3, .temp = 470},
	{.volt = 19792e-3, .temp = 480},
	{.volt = 20218e-3, .temp = 490},
	{.volt = 20644e-3, .temp = 500},
	{.volt = 21071e-3, .temp = 510},
	{.volt = 21497e-3, .temp = 520},
	{.volt = 21924e-3, .temp = 530},
	{.volt = 22350e-3, .temp = 540},
	{.volt = 22776e-3, .temp = 550},
	{.volt = 23203e-3, .temp = 560},
	{.volt = 23629e-3, .temp = 570},
	{.volt = 24055e-3, .temp = 580},
	{.volt = 24480e-3, .temp = 590},
	{.volt = 24905e-3, .temp = 600},
	{.volt = 25330e-3, .temp = 610},
	{.volt = 25755e-3, .temp = 620},
	{.volt = 26179e-3, .temp = 630},
	{.volt = 26602e-3, .temp = 640},
	{.volt = 27025e-3, .temp = 650},
	{.volt = 27447e-3, .temp = 660},
	{.volt = 27869e-3, .temp = 670},
	{.volt = 28289e-3, .temp = 680},
	{.volt = 28710e-3, .temp = 690},
	{.volt = 29129e-3, .temp = 700},
	{.volt = 29548e-3, .temp = 710},
	{.volt = 29965e-3, .temp = 720},
	{.volt = 30382e-3, .temp = 730},
	{.volt = 30798e-3, .temp = 740},
	{.volt = 31213e-3, .temp = 750},
	{.volt = 31628e-3, .temp = 760},
	{.volt = 32041e-3, .temp = 770},
	{.volt = 32453e-3, .temp = 780},
	{.volt = 32865e-3, .temp = 790},
	{.volt = 33275e-3, .temp = 800}
};

static uint8_t _find_lower_bound (double);

double get_temperature (double voltage)
{
	uint8_t lb = _find_lower_bound (voltage);
	uint8_t hb = lb + 1;

	double delta = temp_by_v[hb].temp - temp_by_v[lb].temp;
	double k = (voltage - temp_by_v[lb].volt) / (temp_by_v[hb].volt - temp_by_v[lb].volt);
	
	return temp_by_v[lb].temp + k * delta;
}

static uint8_t __bin_search_lower_bound (uint8_t, uint8_t, double);

static uint8_t _find_lower_bound (double v)
{
	return __bin_search_lower_bound (0, sizeof (temp_by_v) / sizeof (*temp_by_v), v);
}

static uint8_t __bin_search_lower_bound (uint8_t l, uint8_t r, double v)
{
	if (l >= r - 1) {
		return l;
	} else {
		uint8_t mid = l + ((r - l) >> 1);
		if (v < temp_by_v[mid].volt) {
			return __bin_search_lower_bound (l, mid, v);
		} else {
			return __bin_search_lower_bound (mid, r, v);
		}
	}
}

