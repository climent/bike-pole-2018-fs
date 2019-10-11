

Define the array of leds:
 There 2 sets of 3 buffers. For each set:
  * buffer 0 and 1 are for base and layer effects
    * effect->Animate animates the effects
    * effect->Render renders the effect in the buffers
  * buffer 2 is a buffer for layer composition
    * mixer->Mix renders the layering
  * finally, outputBuffer is used to transition from one effect to another
    * selector->ChangeEffect selects a new effect combination
    *
