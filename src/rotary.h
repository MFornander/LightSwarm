#ifndef __SWARM_ROTARY_H__
#define __SWARM_ROTARY_H__

namespace LightSwarm {

// This library manages a Keyes KY-040 Rotary Encoder by sampling the
// appropriate pins when requested. It is NOT interrupt driven, so you'll need
// to sample often.
//
// The encoder has five pins in a 0.1" row, so you can directly plug it into
// an Arduino's data-pin sockets. The sequence of pins matches the order of
// parameters in the constructor - use ascending (or descending) numbers for
// ease.
class RotaryEncoder
{
public:
	// Define which pins to use for the various functions.
	RotaryEncoder(int dt, int clk, int sw = -1);

	// Start RotaryEncoder with optional maxBias (call Rebias() periodically)
	void Begin(byte maxBias = 1);

	// Returns true if switch is currently pressed
	bool Switch() const;

	// Returns 0 for no change, +ve for clockwise, -ve for counter-clockwise
	int Get();

	// Call this periodically to reduce the bias
	void Rebias(int delta = 1);

	private:
	const int	swPin;
	const int	dtPin;
	const int	clkPin;
	byte		maxBias;
	byte		bias;
	bool		clk;
	bool		dt;
};
}

#endif