#include "Key.h"

Key::Key(float sr)
{
	state = IDLE;
	time = 0;
	pitch = 0;
	sampleRate = sr;
}

Key::~Key() { }

void Key::press(int midiNoteNumber, float v)
{
	// get data
	float pitch1 = 0, pitch2 = 0, pitch3 = 0;
	getPitches(midiNoteNumber, pitch1, pitch2, pitch3);
	int stringsCount = getStringsCount(midiNoteNumber);
	
	// tune
	float length = 1;
	if (stringsCount == 1)
	{
		length = sampleRate / pitch / 2.f
		p1 = pitch;
		p2 = 0;
		p3 = 0;
	}
	else if (stringsCount == 2)
	{
		length = sampleRate / pitch / 2.f
		p1 = pitch * std::powf(2.f, -0.1f / 12.f);
		p2 = pitch * std::powf(2.f, 0.1f / 12.f);
		p3 = 0;
	}
	else if (stringsCount == 3)
	{
		p1 = pitch;
		p2 = pitch * std::powf(2.f, 0.1f / 12.f);
		p3 = pitch * std::powf(2.f, -0.1f / 12.f);

	if s
	float length = sampleRate / 440.f / 2.f;
	frontTopSegment.setLength(length / 2);
	frontBotSegment.setLength(length / 2);
	backSegment.setLength(length);

	state = ATTACKING;
}

float Key::process()
{

	//testFilter = new Filter(sampleRate);
	//testFilter->configure(LPF, 500.f, 1.f, 1.f);

	frontTopSegment.pushSample(frontBotSegment.getSample() + input);
	backSegment.pushSample(frontTopSegment.getSample() * (-0.99f));
	frontBotSegment.pushSample(backSegment.getSample() * (-0.99f));
	;
	return (
		frontTopSegment.getSample() +
		backSegment.getSample()) / 2;
}
