//
// RotaryEncoder.h
//
// This library manages a Keyes KY-040 Rotary Encoder by sampling the
// appropriate pins when requested. It is NOT interrupt driven, so you'll need
// to sample often.
//
// The encoder has five pins in a 0.1" row, so you can directly plug it into
// an Arduino's data-pin sockets. The sequence of pins matches the order of
// parameters in the constructor - use ascending (or descending) numbers for
// ease.
//
// If you pass in NoPin for gnd, vcc, or sw (the switch), then that parameter
// is ignored. You'll need to organise those pins yourself! Of course, NoPin is
// not valid for dt or clk.
//

#ifndef RotaryEncoder_h
#define RotaryEncoder_h

class RotaryEncoder
{

public: // Typedefs and enums

   typedef byte Pin;
   typedef signed char OptPin;

   enum Pins : OptPin
   {
      NoPin = -1
   }; // Pins

public: // Methods

   // Define which pins to use for the various functions.
   // If Optional Pins are "NoPin", then they won't be initialised
   RotaryEncoder(OptPin gnd, OptPin vcc, OptPin sw, Pin dt, Pin clk);

   // Start RotaryEncoder with optional maxBias (call Rebias() periodically)
   void Begin(byte maxBias=1);

   // Returns true if switch is currently pressed
   bool Switch() const;

   // Returns 0 for no change, +ve for clockwise, -ve for counter-clockwise
   int Get();

   // Call this periodically to reduce the bias
   void Rebias(int delta=1);

private: // Variables

   const OptPin gndPin;
   const OptPin vccPin;
   const OptPin swPin;
   const Pin dtPin;
   const Pin clkPin;
   byte maxBias;
   byte bias;
   bool clk;
   bool dt;

}; // RotaryEncoder

#endif // RotaryEncoder_h
